#ifndef MACHINE_MONITOR_H
#define MACHINE_MONITOR_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class MachineMonitor {
public:
    MachineMonitor(const string& filename);
    void start_monitoring();

private:
    vector<pair<string, string>> hosts; 
    map<string, pair<bool, bool>> statuses; 

    void update_single_status(const string& ip);
    void update_all_statuses();
    void show_host_info(const string& name, const string& ip);
    void load_hosts(const string& filename);
    void update_statuses();
    void full_refresh_display();
    bool ping_host(const string& ip);
    bool ssh_check(const string& ip);
    vector<string> generate_display_list(); 
    vector<string> generate_full_display_list();
    map<string, string> parse_system_report(const string& ip);
};

#endif
