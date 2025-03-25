#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <filesystem>


class Helper {
public:
    static void ensure_directory_exists(const std::string& dir);
    static std::string get_current_time();
};

#endif
