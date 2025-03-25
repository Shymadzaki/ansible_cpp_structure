#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <vector>
#include <string>

class Main_menu {
public:
    static int main_menu();
    static int handle_choice_navigation(int choice, int max_options, int key);
};

#endif