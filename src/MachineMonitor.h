#ifndef MACHINE_MONITOR_H
#define MACHINE_MONITOR_H

#include <string>
#include <vector>
#include <map>

class MachineMonitor {
public:
    MachineMonitor(const std::string& filename);
    void start_monitoring();

private:
    std::vector<std::pair<std::string, std::string>> hosts; 
    std::map<std::string, std::pair<bool, bool>> statuses; 

    void update_single_status(const std::string& ip);
    void update_all_statuses();
    void show_host_info(const std::string& name, const std::string& ip);
    void load_hosts(const std::string& filename);
    void update_statuses();
    void full_refresh_display();
    bool ping_host(const std::string& ip);
    bool ssh_check(const std::string& ip);
    std::vector<std::string> generate_display_list(); 
    std::vector<std::string> generate_full_display_list();
};

#endif
