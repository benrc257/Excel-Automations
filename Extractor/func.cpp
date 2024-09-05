/*
	Title: func.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program extracts data from a merged CSV file. This file contains function definitions.
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
void getCells(Files File) {
    //variables
    string line;
    size_t start, end;

    //for each file, find the rows and columns using findCells, then retrieve the data from each cell
    for (int i = 0; i < total; i++) { //repeats for each file
        findCells(File[i].rows, File[i].columns, File[i].file);
        File[i].cell.resize(File[i].rows);

        if (File[i].rows > 1) {
            for (int j = 0; j < File[i].rows; j++) {
                start = 0;
                end = 0;
                getline(File[i].file, line);
                File[i].cell[j].resize(File[i].columns);

                for (int k = 0; k < File[i].columns; k++) {
                    end = line.find(',', start);

                    if (end != string::npos) {
                        File[i].cell[j][k] = line.substr(start, (end - start));
                    } else {
                        File[i].cell[j][k] = line.substr(start);
                    }

                    start = end+1;
                }
            }
        } else {
            cout << "\033[31m" << "\nFile " << i+1 << " is empty. Skipping retrieval...\n" << "\033[0m";
        }
    }

    return;
}

/*
    Name: outputCSV()
    Inputs: int, Files
    Purpose: Retrieve the value of each cell and print it to the output file, accounting for duplicates.
*/
void outputCSV(Files File) {
    //variables
    fstream outfile;
    vector<string> printqueue = {"Last Name,First Name,College,Major,Year,Attended\n"};
    size_t inqueue = 0, pos, num;
    bool found = false;
    string outfilename, outfolder, tag;
    int last = 0, first = 1, college = 2, major = 3, year = 4, attended;

    do {
        cout << "\nPlease enter the path of the output folder: ";
        getline(cin, outfolder);

        if (outfolder[0] == '\"' && outfolder.size() > 2) {
            outfolder = outfolder.substr(1);
            outfolder.pop_back();
        }

        cout << "\nPlease enter the name of the output file (without .csv): ";
        getline(cin, outfilename);
        outfilename = outfolder+"\\"+outfilename+".MERGED.csv";
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
    for (int i = 0; i < total; i++) {
        if (File[i].rows > 1) {
            attended = -1;
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
                } else if (File[i].cell[0][j] == "Attended") {
                    attended = j;
                }
            }

            for (int j = 1; j < File[i].rows; j++) {
                found = false;
                tag = File[i].cell[j][last]+","+File[i].cell[j][first]+","+File[i].cell[j][college]+","+File[i].cell[j][major]+","+File[i].cell[j][year]+",";

                if (attended != -1) { //handles merging .MERGED files
                    for (int i = 1; i < printqueue.size(); i++) {
                        if (printqueue[i].find(tag) != string::npos) {
                            found = true;
                            num = stoi(File[i].cell[j][attended]);
                            tag = printqueue[i].substr(0, printqueue[i].find_last_of(",")+1);
                            tag = tag + to_string(num) + "\n";
                            printqueue[i] = tag;
                            break;
                        }
                    }
                } else { //handles merging duplicates
                    for (int i = 1; i < printqueue.size(); i++) {
                        if (printqueue[i].find(tag) != string::npos) {
                            found = true;
                            pos = (printqueue[i].find("\n") - printqueue[i].find_last_of(",")) - 1;
                            tag = printqueue[i].substr(printqueue[i].find_last_of(",") + 1, pos);
                            num = stoi(tag)+1;
                            tag = printqueue[i].substr(0, printqueue[i].find_last_of(",")+1);
                            tag = tag + to_string(num) + "\n";
                            printqueue[i] = tag;
                            break;
                        }
                    }
                }

                if (!found && attended != -1) {
                    tag = tag + File[i].cell[j][attended] + "\n";
                    printqueue.push_back(tag);
                    inqueue++;
                } else if (!found) {
                    tag = tag + "1\n";
                    printqueue.push_back(tag);
                    inqueue++;
                }
            }
        
            last = 0, first = 1, college = 2, major = 3, year = 4;
        }
    }

    for (int i = 0; i < printqueue.size(); i++) {
        outfile << printqueue[i];
    }

    cout << "\nPrinting complete. Output can be found at: " << outfilename << "\n";

    cout << "\nClosing file...\n";
    for (int i = 0; i < total; i++) {
        File[i].file.close();
    }
    outfile.close();
    return;
}