#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <vector>
#include <string>

using namespace std;

class Main_menu {
public:
    static void print_menu(int choice, vector<string> options, int current_page);
    static int main_menu();
    static int handle_choice_navigation(int choice, int max_options, int key, int &current_page);
};

#endif