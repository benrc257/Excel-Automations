/*
	Title: merger.h
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program extracts data from a merged CSV file.
*/

#ifndef EXTRACTOR_H
<<<<<<< HEAD:Extractor/extractor.h
#define EXTRACTOR_H
=======
#define MERGER_H
>>>>>>> b2c2792 (wip):Extractor/merger.h

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
<<<<<<< HEAD:Extractor/extractor.h
};

struct Colleges {
	string name;
	vector<string> degrees;
	int count;
=======
>>>>>>> b2c2792 (wip):Extractor/merger.h
};

//function prototypes
void delay(int);
void findCells(int&, int&, fstream&);
void getCells(Files&);
void outputCSV(Files&);

#endif