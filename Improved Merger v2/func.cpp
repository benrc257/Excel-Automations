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
    Name: findCells()
    Inputs: int&, int&, fstream
    Purpose: Count the number of rows and columns in the given file.
*/
void findCells(int& rows, int& columns, fstream& file) {
    //variables
    string line;
    size_t output = 0;

    //set rows and columns to 0
    rows = 0;
    columns = 0;

    //for each line, increase rows
    while (getline(file, line)) {
        rows++;
        if (rows == 1) {
            do {    //check for the number of columns with .find(",") starting at the index of the last found comma +1
                columns++;
                output = line.find(",", output+1);
            } while(output != string::npos);
        }
    }

    //return to the start of the file
    file.clear();
    file.seekg(0, ios::beg);

    return;
}

/*
    Name: getCells()
    Inputs: int, Files
    Purpose: Retrieve the value of each cell in the file and enter it into the structure array.
*/
void getCells(int total, vector<Files> File) {
    //variables
    string line;
    size_t quote1, quote2;

    //for each file, find the rows and columns using findCells, then retrieve the data from each cell
    for (int i = 0; i < total; i++) { //repeats for each file
        findCells(File[i].rows, File[i].columns, File[i].file);
        for (int j = 0; j < File[i].rows; j++) {
            getline(File[i].file, line);
            for (int k = 0; k < File[i].columns; k++) {
                

                File[i].cell[j][k]
            }
        }
    }

    return;
}

/*
    Name: outputCSV()
    Inputs: int, Files
    Purpose: Retrieve the value of each cell and print it to the output file, accounting for duplicates.
*/
void outputCSV(int total, Files File) {
    //variables
    size_t start, pos, end;
    fstream outfile;
    string outfilename, outfolder, fileContents = "", line, name, tempfileContents, repeats;
    int last = 0, first = 1, college = 2, major = 3, year = 4;

    do {
        cout << "\nPlease enter the path of the output folder: ";
        getline(cin, outfolder);
        cout << "\nPlease enter the name of the output file (without .csv): ";
        getline(cin, outfilename);
        outfilename = outfolder+"\\"+outfilename+".csv";
        cout << endl << outfilename << endl;
        cout << "\nCreating output file...\n";
        outfile.open(outfilename, ios::out);
        if (!outfile.is_open()) {
            cout << "\nFile could not be created or opened. Please try again.\n";
        } else {
            cout << "\nOutput file successfully created and opened.\n";
        }
    } while (!outfile.is_open());

    cout << "\nAnalyzing formatting...\n";
    

    //
    //HEY BEN THIS IS WHERE YOU CHANGE THE FORMATTING OF THE OUTPUT!!!!!
    //

    cout << "\nPrinting to file...\n";
    outfile << "Last Name,First Name,College,Major,Year,Attended\n";
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < File[i].columns; j++) {
            if (File[i].cell[0][j] == "First Name") {
                first = j;
            } else if (File[i].cell[0][j] == "Last Name") {
                last = j;
            } else if (File[i].cell[0][j] == "College") {
                college = j;
            } else if (File[i].cell[0][j] == "Major") {
                major = j;
            } else if (File[i].cell[0][j] == "Year") {
                year = j;
            }
        }
        for (int j = 1; j < File[i].rows; j++) {
            name = File[i].cell[j][last]+","+File[i].cell[j][first];
            if (fileContents.find(name) == string::npos) { //if the name is not already in the file
                repeats = "1";
                outfile << File[i].cell[j][last] << ","
                    << File[i].cell[j][first] << ","
                    << File[i].cell[j][college] << ","
                    << File[i].cell[j][major] << ","
                    << File[i].cell[j][year] << ","
                    << repeats << "\n";
                    fileContents.append(name);
            } else { //if the name is already in the file, increment that lines repeat count
                outfile.close();
                outfile.open(outfilename, ios::in);

                while (getline(outfile, line)) {
                    tempfileContents += line;
                }

                outfile.close();
                outfile.open(outfilename, ios::out);

                start = tempfileContents.find(name);
                end = tempfileContents.find('\n', start);
                pos = tempfileContents.rfind(',', end);
                pos++;
                line = tempfileContents.substr(pos, (end-pos));
                cout << endl << line << endl << start << endl << end << endl << pos << endl << line << endl << tempfileContents << endl;
                repeats = ""+(stoi(line)+1);

                tempfileContents.replace((end-pos), line.length(), repeats);
                outfile << repeats << "\n";

                outfile.close();
                outfile.open(outfilename, ios::out | ios::app);
            }
        }
        last = 0, first = 1, college = 2, major = 3, year = 4;
    }
    cout << "\nPrinting complete. Output can be found at: " << outfilename << "\n";

    cout << "\nClosing file...\n";
    for (int i = 0; i < total; i++) {
        (*(File[i].file)).close();
    }
    outfile.close();
    return;
}