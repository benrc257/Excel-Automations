/*
	Title: sorter.h
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program sorts attendee info and outputs a CSV file.
*/

#ifndef SORTER_H
#define SORTER_H

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

//function prototypes
void delay(int);
int getCells(int&, int&, int&, fstream&);
int getCells(int&, int&, int&, fstream&, Cells**);
void outputCSV(int&, int&, int&, string, Cells**);

#endif