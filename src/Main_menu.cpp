#include "Main_menu.h"
#include <iostream>
#include <ncurses.h>

using namespace std;

int Main_menu::handle_choice_navigation(int choice, int max_options, int key) {
    if (key == KEY_UP) {
        if (choice > 0) {
            return choice - 1;
        } else {
            return max_options - 1;
        }
    } 
    else if (key == KEY_DOWN) {
        if (choice < max_options - 1) {
            return choice + 1;
        } else {
            return 0;
        }
    }
    return choice;
}

int Main_menu::main_menu() {
    vector<string> options = {
        " = White list =", 
        " = Choose your poison =", 
        " = Create your poison =", 
        " = Exterminatus =",
        " = Suicide ="
    };

    int choice = 0;
    int key;

    while (true) {
        clear();
        printw(" === Main Menu === \n");

        for (size_t i = 0; i < options.size(); i++) {
            if (i == choice) attron(A_REVERSE);
            printw("%s\n", options[i].c_str());
            if (i == choice) attroff(A_REVERSE);
        }

        key = getch();
        choice = Main_menu::handle_choice_navigation(choice, options.size(), key);

        if (key == '\n') return choice;
    }
}
