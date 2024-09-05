/*
	Title: merger.h
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program extracts data from a merged CSV file.
*/

#ifndef MERGER_H
#define MERGER_H

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

	Files() : rows(0), columns(0), file(), cell() {}
};

//function prototypes
void delay(int);
void findCells(int&, int&, fstream&);
void getCells(Files);
void outputCSV(Files);

#endif