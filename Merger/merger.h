/*
	Title: merger.h
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program merges existing reports into a single CSV file.
*/

#ifndef MERGER_H
#define MERGER_H

//libraries
#include <iostream>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstring>
using namespace std;

//structures
struct Cells {
    string value;
};

struct Files {
	string filename;
    fstream file;
    int size;
    int rows;
    int columns;
	Cells **Cell;
};

//function prototypes
void delay(int);
int findCells(int&, int&, int&, fstream&);
void getCells(int, Files*);
void outputCSV(int, Files*);

#endif