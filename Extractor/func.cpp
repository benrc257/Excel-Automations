/*
	Title: func.cpp
	Author: Benjamin Clark
	Date: 9/9/24
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
    Inputs: Files
    Purpose: Retrieve the value of each cell in the file and enter it into the File vector.
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
    Inputs: Files
    Purpose: Organizes each degree and college then prints them to the output file.
*/
void outputCSV(Files& File) {
    //variables
    fstream outfile;
    vector<string> majors, unsorted;
    vector<int> attendance;
    vector<Colleges> College;
    size_t start, end;
    string outfilename, outfolder, line;
    int college = 2, major = 3, attended;

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
        for (int i = 0; i < File.columns; i++) {
            if (File.cell[0][i] == "College") {
                college = i;
            } else if (File.cell[0][i] == "Major") {
                major = i;
            } else if (File.cell[0][i] == "Attended") {
                attended = i;
            }
        }

        for (int i = 1; i < File.rows; i++) {
            if (File.cell[i][college] != "") { //if the college is in the data
                if (File.cell[i][major].find("|") == string::npos) { //if there is only one degree
                    for (int j = 0; j < College.size(); j++) { //search through each college
                        if (College[i].name == File.cell[i][college]) { //when college matches
                            College[i].count++; //unique attendance
                            if (attended == -1) { //if there isn't an attendance count
                                College[i].totalAttendance++;
                            } else { //if there is
                                College[i].totalAttendance+=stoi(File.cell[i][attended]);
                            }

                            for (int k = 0; k < College[i].degrees.size(); k++) { //searches through each degree
                                if (College[i].degrees[k] == File.cell[i][major]) { //if degrees match
                                    College[i].degreeCount[k]++; //unique attendance
                                    if (attended == -1) { //if there isn't an attendance count
                                        College[i].degreeAttendance[k]++;
                                    } else { //if there is
                                        College[i].degreeAttendance[k]+=stoi(File.cell[i][attended]);
                                    }

                                    break;
                                }
                                if (k == (College[i].degrees.size()-1)) { //if degree not found and on last element
                                    College[i].degrees.push_back(File.cell[i][major]);
                                    College[i].degreeCount.push_back(1);
                                    if (attended = -1) { //no attendance
                                        College[i].degreeAttendance.push_back(1);
                                    } else { //attendance
                                        College[i].degreeAttendance.push_back(stoi(File.cell[i][attended]));
                                    }
                                }
                            }

                            break;
                        }
                        if (j == (College.size()-1)) { //if college not found and the last element is reached
                            College.push_back({});
                            College[i+1].name = File.cell[i][college];
                            College[i+1].count++;
                            if (attended = -1) { //no attendance
                                College[i+1].totalAttendance++;
                            } else { //attendance
                                College[i+1].totalAttendance+=stoi(File.cell[i][attended]);
                            }
                        }
                    }

                } else { //if there are multiple degrees
                    for (int j = 0; j < College.size(); j++) { //search through each college
                        if (College[i].name == File.cell[i][college]) { //when college matches
                            College[i].count++; //unique attendance
                            if (attended == -1) { //if there isn't an attendance count
                                College[i].totalAttendance++;
                            } else { //if there is
                                College[i].totalAttendance+=stoi(File.cell[i][attended]);
                            }

                            start = 0;
                            end = 0;
                            line = File.cell[i][major].find("|");
                            do {
                                end = line.find("|");
                                majors.push_back(line.substr(start, end));
                                start = end+1;
                                line = line.substr(start);
                            } while (line.find("|") != string::npos);
                            majors.push_back(line);

                            for (int l = 0; l < majors.size(); l++) {
                                for (int k = 0; k < College[i].degrees.size(); k++) { //searches through each degree
                                    if (College[i].degrees[k] == majors[l]) { //if degrees match
                                        College[i].degreeCount[k]++; //unique attendance
                                        if (attended == -1) { //if there isn't an attendance count
                                            College[i].degreeAttendance[k]++;
                                        } else { //if there is
                                            College[i].degreeAttendance[k]+=stoi(File.cell[i][attended]);
                                        }

                                        break;
                                    }
                                    if (k == (College[i].degrees.size()-1)) { //if degree not found and on last element
                                        College[i].degrees.push_back(majors[l]);
                                        College[i].degreeCount.push_back(1);
                                        if (attended = -1) { //no attendance
                                            College[i].degreeAttendance.push_back(1);
                                        } else { //attendance
                                            College[i].degreeAttendance.push_back(stoi(File.cell[i][attended]));
                                        }
                                    }
                                }
                            }

                            break;
                        }
                        if (j == (College.size()-1)) { //if college not found and the last element is reached
                            College.push_back({});
                            College[i+1].name = File.cell[i][college];
                            College[i+1].count++;
                            if (attended = -1) { //no attendance
                                College[i+1].totalAttendance++;
                            } else { //attendance
                                College[i+1].totalAttendance+=stoi(File.cell[i][attended]);
                            }
                        }
                    }
                }
            } else { //if the college is missing
                if (File.cell[i][major].find("|") != string::npos) {
                    do {
                        end = line.find("|");
                        majors.push_back(line.substr(start, end));
                        start = end+1;
                        line = line.substr(start);
                    } while (line.find("|") != string::npos);

                    for (int k = 0; k < majors.size(); k++) {
                        unsorted.push_back(majors[k]);
                        if (attended == -1) {
                            attendance.push_back(1);
                        } else {
                            attendance.push_back(stoi(File.cell[i][attended]));
                        }
                    }
                } else {
                    unsorted.push_back(File.cell[i][major]);
                }
            }
        }
    }

    College.push_back({});
    College[College.size()-1].name = "Unsorted";

    for (int i = 0; i < unsorted.size(); i++) {
        for (int j = 0; j < College.size(); j++) {
            for (int k = 0; College[i].degrees.size(); k++) {
                if (College[j].degrees[k] == unsorted[i]) {
                    College[j].count++;
                    College[j].degreeCount[k]++;
                    College[j].degreeAttendance[k]+=attendance[i];
                    j = College.size();
                    break;
                }
                if (j == (College.size()-1) && k == (College[i].degrees.size()-1)) {
                    College[j].degrees.push_back(unsorted[i]);
                    College[j].degreeAttendance.push_back(attendance[i]);
                }
            }
        }
    }

    for (int i = 0; i < College.size(); i++) {
        outfile << College[i].name
                << ",Unique Attendance: " << to_string(College[i].count)
                << ",Total Attendance: " << to_string(College[i].totalAttendance) << "\n"
                << "------,-----------------,----------------\n"
                << "Degree,Unique Attendance,Total Attendance\n";
        
        for (int j = 0; j < College[i].degrees.size(); j++) {
            outfile << College[i].degrees[j] << ","
                    << College[i].degreeCount[j] << ","
                    << College[i].degreeAttendance[j] << "\n";
        }

        outfile << ",,\n,,\n";
    }

    cout << "\nPrinting complete. Output can be found at: " << outfilename << "\n";

    cout << "\nClosing file...\n";
    File.file.close();
    outfile.close();
    return;
}