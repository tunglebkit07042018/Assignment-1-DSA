/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Spring 2018
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "requestLib.h"

void loadRequests(char* fName, L1List<VRequest> &rList) {
	ifstream inFile(fName);
	if (inFile) {
		string line;
		while (getline(inFile , line)) {
			if (line[line.length() - 1] == '\r') line.erase(line.length() - 1);
			if (line == "") continue;
			istringstream iss(line);
			while (iss) {
				string sub;
				iss >> sub;
				if (sub.length()) {
					VRequest __r(sub);
					rList.insertHead(__r);
				}
			}
		}
		rList.reverse();
		inFile.close();
	}
	else {
		cout << "The file is not found!";
	}
}
