#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <filesystem>


class Helper {
public:
    static void print_progress_bar(int current, int total, int row, int col = 0);
    static void ensure_directory_exists(const std::string& dir);
    static std::string get_current_time();
};

#endif
