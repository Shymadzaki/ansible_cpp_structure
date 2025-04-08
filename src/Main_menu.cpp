#include "Main_menu.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <ncurses.h>
#include <cstdlib>
#include <termios.h> 
#include <ctime>
#include <unistd.h>

using namespace std;

void Main_menu::print_menu(int choice, vector<string> options, int current_page) {
    
    int items_per_page = 10;
    int start_index = current_page * items_per_page;
    int end_index = min(start_index + items_per_page, (int)options.size());

    for (int i = start_index; i < end_index; i++) {
        if (i == choice) attron(A_REVERSE);
        printw("%s\n", options[i].c_str());
        if (i == choice) attroff(A_REVERSE);
    }
}

int Main_menu::handle_choice_navigation(int choice, int max_options, int key, int &current_page) {
    int items_per_page = 10;
    int total_pages = (max_options + items_per_page - 1) / items_per_page;


    int start_index = current_page * items_per_page;
    int end_index = min(start_index + items_per_page, max_options);

    if (key == KEY_UP) {
        if (choice > start_index) {
            return choice - 1;
        } else {
            return end_index -1;
        }
    } 
    else if (key == KEY_DOWN) {
        if (choice < end_index - 1) {
            return choice + 1;
        } else if (choice < items_per_page){
            return start_index;
        }
    }
    else if (key == KEY_LEFT) {
        current_page--;
        if (current_page <= 0) {current_page = total_pages - 1;}
        return current_page * items_per_page;
        
    }
    else if (key == KEY_RIGHT) {
        current_page++;
        if (current_page > total_pages - 1) {current_page = 0;}
        return current_page * items_per_page;
        
    }
    return choice;
}

int Main_menu::main_menu() {
    vector<string> options = {
        " = White list =", 
        " = Choose your poison =", 
        " = Create your poison =", 
        " = Map of Empire =",
        " = Exterminatus =",
        " = Suicide ="
    };

    int current_page = 0;
    int choice = 0;
    int key;
        
        while (true) {
        clear();
        printw(" === Main Menu === \n");

        Main_menu::print_menu(choice, options, current_page);
        key = getch();
        choice = Main_menu::handle_choice_navigation(choice, options.size(), key, current_page);

        if (key == '\n') {return choice;}
        }
    }
