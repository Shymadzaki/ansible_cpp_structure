#ifndef PLAYBOOK_H
#define PLAYBOOK_H

#include <string>
#include <vector>

using namespace std;
class Playbook {
    public:
        static vector<string> get_playbooks(const string& directory);
        static void preview(const string& filepath);
        static void edit(const string& filepath);
        static void create();
        static void run(const string& playbook);
        static void clear_ssh_fingerprints();
        static int select_playbook(const vector<string>& playbooks, const string& playbook_dir);
        
        static const string HOSTS;
        static const string PLAYBOOK_DIR;
        static const string ANSIBLE_CONFIG;
};

#endif