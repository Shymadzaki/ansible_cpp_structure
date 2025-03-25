#include "Logger.h"
#include "Helper.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

const string LOGS_DIR = "/home/shymas/Desktop/ansible_cpp_structure/logs";

void Logger::log(const string& playbook_path, const string& output) {
    Helper::ensure_directory_exists(LOGS_DIR);


    string playbook_name = fs::path(playbook_path).filename().string(); 
    string timestamp = Helper::get_current_time();
    string log_filename = LOGS_DIR + "/" + playbook_name + "_" + timestamp + ".log";

    ofstream log_file(log_filename);
    if (!log_file) {
        cerr << "error cant see your death in " << log_filename << "!\n";
        return;
    }

    log_file << output;
    log_file.close();

    cout << "Now u can see your death in " << log_filename << " i hope u happy?\n";
}