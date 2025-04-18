#include <iostream>
#include <vector>
#include <ncurses.h>
#include "Main_menu.h"
#include "Playbook.h"
#include "MachineMonitor.h"

using namespace std;

int main() {

	string playbook_dir = "/home/shymas/Desktop/Hosts";
	vector<string> playbooks = Playbook::get_playbooks(playbook_dir);

	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	while (true) {
		int choice = Main_menu::main_menu();

		if (choice == 0) {
			Playbook::edit(Playbook::HOSTS);
		}
		else if (choice == 1) {
			if (playbooks.empty()) {
				cerr << "error, no poison " << playbook_dir << " ,no poison \n";
				refresh();
				getch();
				continue;
			}
		

		int pb_choice = Playbook::select_playbook(playbooks, playbook_dir);
			if (pb_choice == -1) {
			continue;	
			}
			else {
			string selected_playbook = playbook_dir + "/" + playbooks[pb_choice];
			cout << "drink poison: " << selected_playbook << "...\n";
			Playbook::run(selected_playbook);
			}
			break;
		}
		else if (choice == 2) {
			Playbook::create();
		}
		else if (choice == 3) {
			MachineMonitor monitor("/home/shymas/Desktop/Hosts/hosts.yaml");
			monitor.start_monitoring();
		}
		else if (choice == 4) {
			Playbook::clear_ssh_fingerprints();
		}
		else if (choice == 5) { break; }
	}
	
	
	endwin();
	return 0;
}