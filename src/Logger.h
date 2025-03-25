#ifndef LOGGER_H
#define LOGGER_H

#include <string>
using namespace std;

class Logger {
    public:
        static void log(const string& playbook_path, const string& output);
};

#endif