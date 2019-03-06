/*
 * pa2.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: CarlRiley
 */

#include <iostream>
#include "pa2.h"

using namespace std;

int mainMenu(int mode) {
	int userOption = 1;
	bool printMenu = true;
	while (printMenu) {
		if (mode == 1) {
			cout << "Using best fit algorithm" << endl;
			cout << endl;
		}
		else {
			cout << "Using worst fit algorithm" << endl;
			cout << endl;
		}
		cout << "1. Add program" << endl;
		cout << "2. Kill program" << endl;
		cout << "3. Fragmentation" << endl;
		cout << "4. Print memory" << endl;
		cout << "5. Exit" << endl;
		cout << "Enter Menu Option: ";

		cin >> userOption;
		cout << endl;
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			cout << "Invalid input. Please enter a number between 1 - 5" << endl;
			printMenu = true;
		}
		else {
			printMenu = false;
		}
	}
	return userOption;
}

void lowerCase(string& algorithmMode) {
	for (unsigned int i = 0; i < algorithmMode.length(); i++) {
		algorithmMode[i] = tolower(algorithmMode[i]);
	}
}

int main(int argc, char** argv) {
	string algorithmMode = argv[1] ;
	lowerCase(algorithmMode);
	int mode = 0;
	int userOption = 1;
	double programSize = 0.0;
	int memorySize = 128;
	int numOfPages = 32;
	int fragmentCounter = 0;
	string program = "";
	if (algorithmMode.compare("best") == 0) {
		mode = 1;
	}
	if (mode == 1) {
		cout << "Best mode" << endl;
	}
	else {
		cout << "Worst Mode" << endl;
	}
	LinkedList memory;
	//Fills the linked list with 32 pages of "FREE" space.
	for (int i = 0; i < numOfPages; i++) {
		memory.createMemorySpace("FREE");
	}

	while (userOption != 5) {
		userOption = mainMenu(mode);
		switch (userOption) {
		case 1:
			cout << "Program name: ";
			cin >> program;
			cout << endl;
			while (true) {
				cout << "Program size (KB)";
				cin >> programSize;
				if (cin.fail()) {
					cin.clear();
					cin.ignore();
					cout << "Please input an integer" << endl;
				}
				else {
					break;
				}
			}
			memory.addProgram(program, programSize, memorySize, numOfPages, mode);
			break;
		case 2:
			cout << "Name of program to be deleted: " << endl;
			cin >> program;
			memory.deleteProgram(program);
			break;
		case 3:
			fragmentCounter = memory.checkFragments();
			cout << "There are " << fragmentCounter << " fragment(s)" << endl;
			break;
		case 4:
			memory.printList();
			break;
		case 5:
			cout << "You have shutdown the program!" << endl;
			break;
		default:
			cout << "Invalid Input" << endl;
		}
	}
	return 0;
}

