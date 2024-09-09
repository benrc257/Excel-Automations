/*
	Title: merger.h
	Author: Benjamin Clark
	Date: 8/24/24
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
	vector<int> degreeCount;
	vector<int> degreeAttendance;
	int count;
	int totalAttendance;
};

//function prototypes
void delay(int);
void findCells(int&, int&, fstream&);
void getCells(Files&);
void outputCSV(Files&);

#endif