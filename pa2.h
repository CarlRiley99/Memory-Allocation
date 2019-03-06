/*
 * pa2.h
 *
 *  Created on: Mar 4, 2018
 *      Author: CarlRiley
 */

#ifndef PA2_H_
#define PA2_H_

#include <string>
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;

class LinkedList {
private:
	struct node {
		string data;
		node* next;
	};

	typedef struct node* nodePtr;

	nodePtr head;
	nodePtr tail;
	nodePtr tracker;	//tracker points the position where the program gets placed

public:
	LinkedList();
	void createMemorySpace(string addSpace);
	void addProgram(string addData, double programSize, int memorySize, int numOfPages, int mode);
	void deleteProgram(string deleteData);
	void printList();
	int checkFragments();
	int findFragment(int programSize, int mode);

};

LinkedList::LinkedList() {
	head = NULL;
	tail = NULL;
	tracker = NULL;
}

void LinkedList::deleteProgram(string deleteData) {
	nodePtr z; //points to the node to be deleted
	int counter = 0;
	bool foundData = false;
	z = head;
	while ((z != NULL) && (z->data.compare(deleteData) != 0)) {
		z = z->next;
	}
	while ((z != NULL) && z->data.compare(deleteData) == 0) {
		z->data.replace(0, deleteData.length(), "FREE");
		z = z->next;
		foundData = true;
		counter++;
	}
	if (!foundData) {		//if data NOT found
		cout << "data not found!" << endl;
	}
	if (foundData) {
		cout << "Program " << deleteData << " successfully killed, " << counter << " page(s) reclaimed" << endl;
	}
}

void LinkedList::printList() {
	int counter = 0;
	nodePtr currentPointer;	//creates a pointer to NULL
	currentPointer = head;	//creates a pointer and sets it to head
	//(head is a pointer as well, either it's pointing to the head node or null)
	while (currentPointer) {	//while the pointer is not pointing to null
		cout << currentPointer -> data << "   ";
		currentPointer = currentPointer -> next;
		counter++;
		if (counter % 8 == 0) {
			cout << endl;
		}
	}
}

void LinkedList::addProgram(string addData, double programSize, int memorySize, int numOfPages, int mode) {
	nodePtr deleteProgram;
	double pagesNeeded = 0;
	int space = 0;
	tracker = head;
	while (tracker) {
		if (tracker->data.compare(addData) == 0) {
			cout << "Error, Program " << addData << " already running." << endl;
			return;
		}
		tracker = tracker->next;
	}
	space = findFragment(programSize, mode);
	if (programSize > space) {
		cout << "Error, Not enough memory for Program " << addData << endl;
		return;
	}
	deleteProgram = tracker;
	pagesNeeded = ceil(programSize / 4.0);	//Inputs need to be doubles in order to prevent integer division before ceiling function gets executed
	//cout << pagesNeeded << endl;
	for (int i = 0; i < pagesNeeded; i++) {
		deleteProgram->data.replace(0, 4, addData);
		deleteProgram = deleteProgram->next;
	}
	cout << "Program " << addData << " added successfully: " << pagesNeeded << " page(s) used." << endl;
}

void LinkedList::createMemorySpace(string addSpace) {	//adds a node to the end of linked list
	nodePtr newTail = new node;	//creates another pointer to a newly created object
	newTail -> next = NULL;	//sets the node to point to NULL (since I am inserting it at the end)
	newTail -> data = addSpace;
	if (head != NULL) {	//if there are nodes in the linked list
		tail -> next = newTail;
		tail = newTail;
	}
	else {	//if linked list contains nothing (this starts the linked list)
		head = newTail;
		tail = newTail;
	}
}

int LinkedList::checkFragments() {
	int fragmentCounter = 0;
	nodePtr p = head;	//finds the beginning of the fragment
	nodePtr q = head;	//finds the end of the fragment
	while (q) {
		while ((q) && (p->data.compare("FREE") != 0 || q->data.compare("FREE") == 0)) {
			if ((q) && (p->data.compare("FREE") == 0 && q->data.compare("FREE") == 0)) {
				q = q->next;
			}
			if ((q) && (p->data.compare("FREE") != 0 && q->data.compare("FREE") != 0)) {
				p = p->next;
				q = q->next;
			}
			if ((q) && (p->data.compare("FREE") == 0 && q->data.compare("FREE") != 0)) {
				fragmentCounter++;
				break;
			}
			if ((!q) && p->data.compare("FREE") == 0) {
				fragmentCounter++;
				break;
			}
			if ((q) && (p->data.compare("FREE") != 0 && q->data.compare("FREE") == 0)){
				break;
			}
		}
		if (q) {	//Moves pointers "p" and "q" to the first "FREE" spot.
			p = p->next;
		}
	}
	return fragmentCounter;
}

//Returns the largest possible fragment for the program to and sets the tracker pointer
//to the start of the largest fragment.
int LinkedList::findFragment(int programSize, int mode) {
	int fragmentSize;
	if (mode == 0) {
		fragmentSize = 0;
	}
	else {
		fragmentSize = 129;	//This has to be larger than the max fragment size possible (which is 128)
	}
	int fragmentLength = 0;
	nodePtr p = head;	//finds the beginning of the fragment
	nodePtr q = head;	//finds the end of the fragment
	while (q) {
		fragmentLength = 0;
		while ((q) && (p->data.compare("FREE") != 0 || q->data.compare("FREE") == 0)) {
			if ((q) && (p->data.compare("FREE") == 0 && q->data.compare("FREE") == 0)) { //Moves q across the length of the fragment
				q = q->next;
				fragmentLength += 4;
			}
			if ((q) && (p->data.compare("FREE") != 0 && q->data.compare("FREE") != 0)) { //Moves p and q to the first free spot
				p = p->next;
				q = q->next;
			}
			if ((q) && (p->data.compare("FREE") == 0 && q->data.compare("FREE") != 0)) { //When p is at the beginning of the fragment and when q is at the end of it
				break;
			}
			if ((!q) && p->data.compare("FREE") == 0) {	//When q reaches NULL at the end of the linked list.
				break;
			}
			if ((q) && (p->data.compare("FREE") != 0 && q->data.compare("FREE") == 0)){
				break;
			}
		}
		//cout << "fragment length is " << fragmentLength << endl;
		if ((fragmentLength > fragmentSize) && ((fragmentLength * 4) >= programSize) && (mode == 0)) {	//If worst algorithm is selected
			fragmentSize = fragmentLength;
			//cout << "Max fragment size is " << fragmentSize << endl;
			tracker = p;
		}
		if ((fragmentLength < fragmentSize) && ((fragmentLength * 4) >= programSize) && (mode == 1)) {	//If best algorithm is selected
			fragmentSize = fragmentLength;
			//cout << "Smallest fragment size is " << fragmentSize << endl;
			tracker = p;
		}
		if (q) {	//Moves "p" to where "q" is.
			p = p->next;
		}
	}
	return fragmentSize;
}

#endif /* PA2_H_ */
