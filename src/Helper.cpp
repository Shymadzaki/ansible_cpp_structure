#include "Helper.h"
#include <filesystem>
#include <iostream>
#include <ctime>

namespace fs = std::filesystem;

void Helper::ensure_directory_exists(const std::string& dir) {
    if (!fs::exists(dir)) {
        fs::create_directory(dir);
    }
}

std::string Helper::get_current_time() {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char buffer[9];
    strftime(buffer, sizeof(buffer), "%H-%M-%S", localtm);
    return std::string(buffer);
}