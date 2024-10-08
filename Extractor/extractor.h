/*
	Title: merger.h
	Author: Benjamin Clark
	Date: 9/9/24
	Purpose: This program extracts data from a merged CSV file.
*/

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

//libraries
#include <iostream>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstring>
#include <vector>
using namespace std;

//structures
struct Files {
    int rows;
    int columns;
	fstream file;
	vector<vector<string>> cell;
};

struct Colleges {
	string name;
	vector<string> degrees;
	vector<int> degreeCount; //unique count
	vector<int> degreeAttendance; //total
	int count = 0; //unique count
	int totalAttendance = 0; //total
};

//function prototypes
void delay(int);
void findCells(int&, int&, fstream&);
void getCells(Files&);
void outputCSV(Files&);

#endif