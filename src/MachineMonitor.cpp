#include "MachineMonitor.h"
#include "Main_menu.h"
#include "Helper.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include <ncurses.h>
#include <set>

using namespace std;

MachineMonitor::MachineMonitor(const string& filename) {
    load_hosts(filename);
}

void MachineMonitor::load_hosts(const string& filename) {
    ifstream infile(filename);
    string line;
    set<string> included_groups;
    map<string, vector<pair<string, string>>> group_hosts;

    string current_group;
    string current_host;

    // Collect Blyzenko
    bool in_blyzenko = false;
    while (getline(infile, line)) {
        string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

        if (trimmed == "Blyzenko:") {
            in_blyzenko = true;
            continue;
        }

        if (in_blyzenko && trimmed == "children:") {
            continue;
        }

        if (in_blyzenko && trimmed.find("BZ") != string::npos) {
            string group_name = trimmed;
            group_name.erase(group_name.find(":"), string::npos);
            included_groups.insert(group_name);
        }

        if (in_blyzenko && trimmed.find(":") != string::npos &&
            trimmed.find("BZ") == string::npos &&
            trimmed != "children:") {
            break;
        }
    }

    infile.clear();
    infile.seekg(0); 

    while (getline(infile, line)) {
        string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

        if (trimmed.find("BZ") == 0 && trimmed.find(":") != string::npos) {
            string group = trimmed.substr(0, trimmed.find(":"));
            current_group = included_groups.count(group) ? group : "";
        }

        if (!current_group.empty() && trimmed == "hosts:") {
            continue;
        }

        if (!current_group.empty() && trimmed.find(":") != string::npos && trimmed.find("ansible_host") == string::npos) {
            current_host = trimmed.substr(0, trimmed.find(":"));
        }

        if (!current_group.empty() && trimmed.find("ansible_host:") != string::npos) {
            string ip = trimmed.substr(trimmed.find(":") + 1);
            ip.erase(0, ip.find_first_not_of(" \t"));
            ip.erase(ip.find_last_not_of(" \t\r\n") + 1);
            group_hosts[current_group].emplace_back(current_host, ip);
        }
    }

    for (const auto& [group, entries] : group_hosts) {
        for (const auto& [name, ip] : entries) {
            hosts.emplace_back(name, ip); 
            statuses[ip] = {false, false}; 
        }
    }    
}


bool MachineMonitor::ping_host(const string& ip) {
    string cmd = "ping -c 1 -W 1 " + ip + " > /dev/null 2>&1";
    return system(cmd.c_str()) == 0;
}

bool MachineMonitor::ssh_check(const string& ip) {
    string cmd = "timeout 2 ssh -o BatchMode=yes -o ConnectTimeout=2 " + ip + " exit > /dev/null 2>&1";
    return system(cmd.c_str()) == 0;
}

void MachineMonitor::update_all_statuses() {
    for (const auto& [name, ip] : hosts) {
        update_single_status(ip);
    }
}

void MachineMonitor::update_single_status(const string& ip) {
    bool is_online = ping_host(ip);
    bool has_ssh = is_online ? ssh_check(ip) : false;
    statuses[ip] = {is_online, has_ssh};
}

void MachineMonitor::show_host_info(const string& name, const string& ip) {
    update_single_status(ip);
    auto [ping, ssh] = statuses[ip];

    clear();
    mvprintw(2, 5, "=== Host Details ===");
    mvprintw(4, 5, "Name: %s", name.c_str());
    mvprintw(5, 5, "IP: %s", ip.c_str());
    mvprintw(6, 5, "Ping Status: %s", ping ? "Online" : "Offline");
    mvprintw(7, 5, "SSH Access: %s", ssh ? "Available" : "No Access");
    mvprintw(9, 5, "Press any key to return...");

    refresh();
    getch();
}

void MachineMonitor::full_refresh_display() {
    clear();
    int total = hosts.size();

    for (size_t i = 0; i < hosts.size(); ++i) {
        const auto& [name, ip] = hosts[i];
        update_single_status(ip);

        Helper::print_progress_bar(i + 1, total, 0, 0);
    }

    clear();
    mvprintw(0, 0, "=== All Hosts Status ===");

    auto display = generate_full_display_list();

    for (size_t i = 0; i < display.size(); ++i) {
        mvprintw(2 + i, 2, "%s", display[i].c_str());
    }

    mvprintw(3 + display.size(), 2, "Press any key to return...");
    refresh();
    getch();
}

vector<string> MachineMonitor::generate_display_list() {
    vector<string> display;
    for (const auto& [name, ip] : hosts) {
        auto [ping, ssh] = statuses[ip];
        string row = name + " | IP: " + ip;
        display.push_back(row);
    }
    return display;
}

vector<string> MachineMonitor::generate_full_display_list() {
    vector<string> display;
    for (const auto& [name, ip] : hosts) {
        auto [ping, ssh] = statuses[ip];
        string row = name + " | IP: " + ip + " | Active: " + (ping ? "Yes" : "No") + " | SSH: " + (ssh ? "Available" : "No Access");
        display.push_back(row);
    }
    return display;
}

void MachineMonitor::start_monitoring() {
    timeout(-1);

    int choice = 0;
    int page = 0;

    while (true) {
        clear();
        vector<string> display = generate_display_list();

        mvprintw(0, 0, "=== Map of Empire === \n(Press Enter for details, 'r' to refresh, 'q' to quit)\n");

        Main_menu::print_menu(choice, display, page);

        int ch = getch();

        if (ch == 'q') break;
        if (ch == 'r') {
            full_refresh_display();
            continue;
        }

        if (ch == '\n' || ch == KEY_ENTER) {
            const auto& [name, ip] = hosts[choice];
            show_host_info(name, ip);
            continue;
        }

        choice = Main_menu::handle_choice_navigation(choice, display.size(), ch, page);
    }
}
