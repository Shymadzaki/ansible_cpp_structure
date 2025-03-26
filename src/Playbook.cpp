#include "Playbook.h"
#include "Logger.h"
#include "Helper.h"
#include "Main_menu.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <ncurses.h>
#include <cstdlib>
#include <termios.h> 
#include <ctime>
#include <unistd.h>

using namespace std;
namespace fs = filesystem;

const string Playbook::PLAYBOOK_DIR = "/home/shymas/Desktop/ansible_cpp_structure/playbooks";
const string Playbook::ANSIBLE_CONFIG = "/home/shymas/Desktop/Hosts/ansible.cfg";
const string Playbook::HOSTS = "/home/shymas/Desktop/Hosts/hosts.yaml";

void Playbook::clear_ssh_fingerprints() {
    cout << "Clearing old SSH fingerprints and adding new ones...\n";

    ifstream hosts_file(HOSTS);
    if (!hosts_file) {
        cerr << "Error: can't open " << HOSTS << "\n";
        return;
    }

    string line;
    while (getline(hosts_file, line)) {
        // Шукаємо IP або хост
        if (line.find("ansible_host") != string::npos) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string host_ip = line.substr(pos + 1);
                host_ip.erase(0, host_ip.find_first_not_of(" \t")); // Видаляємо пробіли
                
                cout << "Removing old fingerprint: " << host_ip << "\n";
                string remove_command = "ssh-keygen -f ~/.ssh/known_hosts -R " + host_ip;
                system(remove_command.c_str()); // Видалення старого fingerprint

                cout << "Adding new fingerprint: " << host_ip << "\n";
                string add_command = "ssh-keyscan -H " + host_ip + " >> ~/.ssh/known_hosts";
                system(add_command.c_str()); // Додавання нового fingerprint
            }
        }
    }
    hosts_file.close();
}

vector<string> Playbook::get_playbooks(const string& directory) {
    vector<string> playbooks;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".yaml") {
            playbooks.push_back(entry.path().filename().string());
        }
    }
    return playbooks;
}

void Playbook::preview(const string& filepath) {
    clear();
    printw("=== Preview ===\n");

    ifstream file(filepath);
    if (!file) {
        printw("Error: Poison not found!\n");
        refresh();
        getch();
        return;
    }

    string line;
    while (getline(file, line)) {
        printw("%s\n", line.c_str());
    }

    file.close();
    printw("\n ==== Press F ==== ");
    refresh();
    getch();
}

void Playbook::edit(const string& filepath) {
    endwin();
    string command = "nano " + filepath;
    system(command.c_str());
    initscr();
}

void Playbook::create() {

	endwin();
	string filename;

	cout << "new poison?";
	getline(cin, filename);

	if (filename.empty()) {
		cout << "no name for poison? U will regret for this! \n";
		return;
	}

	string filepath = PLAYBOOK_DIR + "/" + filename + ".yaml";

	ofstream file(filepath);
	if (!file) {
		cerr << "Error u cant create smth like " << filepath << "!\n";
	}

	file	<< "---\n"
			<< "- name: \n"
			<< "hosts: \n"
			<< "tasks: \n"
			<< "	- name: \n"
			<< "	  debug:\n"
			<< "		msg: \n";
	file.close();

	cout << "Nice poison with name: " << filepath << " was created or no?\n";
	Playbook::edit(filepath);
}

void Playbook::run(const string& playbook) {
    endwin();

    string password;
    cout << "Enter sudo password for Ansible: ";
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getline(cin, password);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;

    string command = "expect -c '"
                     "spawn ansible-playbook " + playbook + " -i " + HOSTS + " --ask-become-pass; "
                     "expect \"BECOME password:\"; "
                     "send \"" + password + "\\r\"; "
                     "interact' 2>&1";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cerr << "Error: cannot start playbook!\n";
        initscr();
        return;
    }

    string output;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
        cout << buffer;  // Виводимо на екран для реального часу
    }

    int exit_code = pclose(pipe);
    if (exit_code != 0) {
        cerr << "Error: playbook failed with exit code " << exit_code << endl;
    }

    Logger::log(playbook, output);
    
    initscr();
}

int Playbook::select_playbook(const vector<string>& playbooks, const string& playbook_dir) {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

    int current_page = 0;
	int choice = 0;
	int key;

	while (true) {
		clear();
		printw("choose your poison:\n");
        
        Main_menu::print_menu(choice, playbooks, current_page);
        
        key = getch ();
		
        choice = Main_menu::handle_choice_navigation(choice, playbooks.size(), key, current_page);
        
        if (key == 'v') {
			string filepath = playbook_dir + "/" + playbooks[choice];
			Playbook::preview(filepath);
		}
		else if (key == 'e') {
			string filepath = playbook_dir + "/" + playbooks[choice];
			Playbook::edit(filepath);
		}
		else if (key == 'q') {
            return -1;
		}
		else if (key == '\n') {
			break;
		}
	}
	endwin();
	return choice;
}
