/*
	Title: func.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program merges existing reports into a single CSV file. This file contains function definitions.
*/

#include "merger.h"

/*
    Name: delay()
    Inputs: int
    Purpose: Create a timed delay.
*/
void delay(int delay) {
    //variables
    time_t timer;

    //obtain start time and the end time
    timer = time(0)+delay;

    while (time(0) < timer);
}

/*
    Name: getCells()
    Inputs: int&, int&, int&, fstream&
    Purpose: Count the number of cells, rows, and columns in the given file.
*/
int findCells(int& cells, int& rows, int& columns, fstream& file) {
    //variables
    string line;
    char * output;

    if (getline(file, line)) {
        columns++;
        rows++;
    }
    output = strpbrk(line.c_str(), ",");
    while (output != NULL) {
        output = strpbrk(output+1, ",");
        columns++;
    }

    while (getline(file, line)) {
        rows++;
    }

    cells = (rows)*(columns);
    return 1;
}

/*
    Name: getCells()
    Inputs: int, Files*
    Purpose: Retrieve the value of each cell in the file and enter it into the structure array.
*/
void getCells(int total, Files *File) {
    //variables
    string line;
    char * output;
    size_t start, end;

    for (int i = 0; i < total; i++) {
        findCells(File[i].size, File[i].rows, File[i].columns, File[i].file);
        //dynamically allocates Cells structures
        File[i].Cell = new Cells*[File[i].rows];
        for (int j = 0; j < File[i].rows; j++) {
            File[i].Cell[j] = new Cells[File[i].columns];
        }
        File[i].file.clear();
        File[i].file.seekg(0, ios::beg);
        for (int j = 0; j < File[i].rows; j++) {
            start = 0;
            end = 0;
            getline(File[i].file, line);
            for (int k = 0; k < File[i].columns; k++) {
                end = line.find(',', start);
                if (end != string::npos) {
                    File[i].Cell[j][k].value = line.substr(start, (end - start));
                } else {
                    File[i].Cell[j][k].value = line.substr(start);
                }
                start = end+1;
            }
        }
    }

    return;
}

/*
    Name: outputCSV()
    Inputs: int&, int&, int&, string, Cells**
    Purpose: Retrieve the value of each cell in the file and enter it into the structure array.
*/
void outputCSV(int total, Files *File) {
    //variables
    size_t start;
    fstream outfile;
    string outfilename, outfolder, fileContents = "", line, name;
    int last = 0, first = 1, college = 2, major = 3, year = 4;

    cout << "\nPlease enter the path of the output folder: ";
    getline(cin, outfolder);
    cout << "\nPlease enter the name of the output file (without .csv): ";
    getline(cin, outfilename);
    outfilename = outfolder+"\\"+outfilename+".csv";
    cout << endl << outfilename << endl;
    cout << "\nCreating output file...\n";
    outfile.open(outfilename, ios::out);
    if (!outfile.is_open()) {
        cout << "\nFile could not be created or opened. Aborting...\n";
        return;
    }
    cout << "\nOutput file successfully created and opened.\n";

    //
    //HEY BEN THIS IS WHERE YOU CHANGE THE FORMATTING OF THE DATA AND OUTPUT!!!!!
    //

    cout << "\nPrinting to file...\n";
    outfile << "Last Name, First Name, College, Major, Year\n";
    for (int i = 0; i < total; i++) {
        for (int j = 1; j < File[i].rows; j++) {
            name = File[i].Cell[j][last].value+","+File[i].Cell[j][first].value;
            if (fileContents.find(name) == string::npos) {
                outfile << File[i].Cell[j][last].value << ","
                    << File[i].Cell[j][first].value << ","
                    << File[i].Cell[j][college].value << ","
                    << File[i].Cell[j][major].value << ","
                    << File[i].Cell[j][year].value << "\n";
            }
            fileContents.append(name);
        }
    }
    cout << "\nPrinting complete. Output can be found at: " << outfilename << "\n";

    cout << "\nClosing file...\n";
    outfile.close();
    return;
}