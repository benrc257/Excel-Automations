/*
	Title: func.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program extracts data from a merged CSV file. This file contains function definitions.
*/

#include "extractor.h"

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
void getCells(Files& File) {
    //variables
    string line;
    size_t start, end;

    //find the rows and columns using findCells, then retrieve the data from each cell
    findCells(File.rows, File.columns, File.file);
    File.cell.resize(File.rows);

    if (File.rows > 1) {
        for (int j = 0; j < File.rows; j++) {
            start = 0;
            end = 0;
            getline(File.file, line);
            File.cell[j].resize(File.columns);

            for (int k = 0; k < File.columns; k++) {
                end = line.find(',', start);

                if (end != string::npos) {
                    File.cell[j][k] = line.substr(start, (end - start));
                } else {
                    File.cell[j][k] = line.substr(start);
                }

                start = end+1;
            }
        }
    } else {
        cout << "\nFile is empty. Skipping retrieval...\n";
    }

    return;
}

/*
    Name: outputCSV()
    Inputs: int, Files
    Purpose: Retrieve the value of each cell and print it to the output file, accounting for duplicates.
*/
void outputCSV(Files& File) {
    //variables
    fstream outfile;
    vector<string> majors;
    vector<Colleges> College;
    size_t inqueue = 0, pos, num;
    string outfilename, outfolder;
    int college = 2, major = 3, attended, foundcol, founddeg;

    do {
        cout << "\nPlease enter the path of the output folder: ";
        getline(cin, outfolder);

        if (outfolder[0] == '\"' && outfolder.size() > 2) {
            outfolder = outfolder.substr(1);
            outfolder.pop_back();
        }

        cout << "\nPlease enter the name of the output file (without .csv): ";
        getline(cin, outfilename);
        outfilename = outfolder+"\\"+outfilename+".EXTRACT.csv";
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

    if (File.rows > 1) {
        attended = -1;
        for (int j = 0; j < File.columns; j++) {
            if (File.cell[0][j] == "College") {
                college = j;
            } else if (File.cell[0][j] == "Major") {
                major = j;
            } else if (File.cell[0][j] == "Attended") {
                attended = j;
            }
        }

        for (int j = 1; j < File.rows; j++) {
            foundcol = 0;
            founddeg = 0;

            for (int i = 0; i < College.size(); i++) {
                if (College[i].name == File.cell[j][college]) {
                    foundcol = i;
                    College[i].count++;
                    if (attended != -1) {
                        College[i].totalAttendance+=stoi(File.cell[j][attended]);
                    } else {
                        College[i].totalAttendance++;
                    }
                    for (int k = 0; k < College[i].degrees.size(); k++) {
                        if (College[i].degrees[k] == File.cell[j][major]) {
                            founddeg = k;
                            College[i].degreeCount[k]++;
                            if (attended != -1) {
                                College[i].degreeAttendance[k]+=(stoi(File.cell[j][attended]));
                            } else {
                                College[i].degreeAttendance[k]++;
                            }
                            break;
                        }
                    }
                    break;
                }
            }

            if (foundcol == 0) {
                College.push_back({});
                College[0].name = File.cell[j][college];
                College[0].count = 1;
                if (attended != -1) {
                    College[0].totalAttendance = stoi(File.cell[j][attended]);
                } else {
                    College[0].totalAttendance = 1;
                }
            }
            
        }
    }

    for (int i = 0; i < printqueue.size(); i++) {
        outfile << printqueue[i];
    }

    cout << "\nPrinting complete. Output can be found at: " << outfilename << "\n";

    cout << "\nClosing file...\n";
    for (int i = 0; i < total; i++) {
        File.file.close();
    }
    outfile.close();
    return;
}