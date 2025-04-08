#include "Helper.h"
#include <filesystem>
#include <iostream>
#include <ctime>
#include <ncurses.h>

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

void Helper::print_progress_bar(int current, int total, int row, int col) {
    int width = 30;
    float ratio = total > 0 ? (float)current / total : 0;
    int filled = static_cast<int>(ratio * width);

    std::string bar = "[";
    for (int i = 0; i < width; ++i) {
        bar += (i < filled) ? '#' : '-';
    }
    bar += "] ";

    bar += std::to_string(current) + " / " + std::to_string(total);

    mvprintw(row, col, "%s", bar.c_str());
    refresh();
}