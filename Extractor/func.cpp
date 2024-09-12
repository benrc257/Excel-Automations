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
    bool found = false;

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

            //handling multiple majors in one cell
            start = 0;
            end = 0;
            line = File.cell[i][major];

            if (line.find("\"") != string::npos) { //if majors are in quotes
                line = line.substr(1, line.size()-2);
            }
            
            if (line.find("|") != string::npos) { //if pipe is found

                while (line.find("|") != string::npos) { //breaks down cell by pipe

                    end = line.find("|");
                    line = line.substr(start, end-1);
                    majors.push_back(line);
                    start = end+2;

                }

            } else { //if no pipe
                majors.push_back(File.cell[i][major]);
            }

            if (File.cell[i][college] != "" && File.cell[i][college] != "Other or Unlisted") { //if college cell is not empty
                
                if (College.size() != 0) { //if college is not empty vector

                    for (int j = 0; j < College.size(); j++) { //check if college exists yet
                        if (File.cell[i][college] == College[j].name) { //if college matches
                            
                            College[j].count++;
                            if (attended != -1) { //if attendance is present
                                College[j].totalAttendance+=stoi(File.cell[i][attended]);
                            } else { //if attendance is missing
                                College[j].totalAttendance++;
                            }

                            if (College[j].degrees.size() != 0) { //if degree is not empty vector

                                for (int l = 0; l < majors.size(); l++) {

                                    for (int k = 0; k < College[j].degrees.size(); k++) { //check if degree exists

                                        if (majors[l] == College[j].degrees[k]) { //if degree exists

                                            College[j].degreeCount[k]++;

                                            if (attended != -1) { //if attendance is present
                                                College[j].degreeAttendance[k]+=stoi(File.cell[i][attended]);
                                            } else { //if attendance is missing
                                                College[j].degreeAttendance[k]++;
                                            }

                                            break;

                                        } else if (k == College.size()-1) { //if degree is missing

                                            College[j].degrees.push_back(majors[l]);
                                            College[j].degreeCount.push_back(1);

                                            if (attended != -1) { //if attendance is present
                                                College[j].degreeAttendance.push_back(stoi(File.cell[i][attended]));
                                            } else { //if attendance is missing
                                                College[j].degreeAttendance.push_back(1);
                                            }
                                            break;

                                        }

                                    }

                                }

                            } else { //if degree is an empty vector

                                for (int l = 0; l < majors.size(); l++) {

                                    College[j].degrees.push_back(majors[l]);
                                    College[j].degreeCount.push_back(1);

                                    if (attended != -1) { //if attendance is present
                                        College[j].degreeAttendance.push_back(stoi(File.cell[i][attended]));
                                    } else { //if attendance is missing
                                        College[j].degreeAttendance.push_back(1);
                                    }

                                }

                            }

                            break;

                        } else if (j == College.size()-1) { //if college isn't found

                            College.push_back({});
                            College.back().name = File.cell[i][college];
                            College.back().count = 1;

                            if (attended != -1) { //if attendance is present
                                College.back().totalAttendance+=stoi(File.cell[i][attended]);
                            } else { //if attendance is missing
                                College.back().totalAttendance++;
                            }

                            for (int l = 0; l < majors.size(); l++) {

                                College.back().degrees.push_back(majors[l]);
                                College.back().degreeCount.push_back(1);

                                if (attended != -1) { //if attendance is present
                                    College.back().degreeAttendance.push_back(stoi(File.cell[i][attended]));
                                } else { //if attendance is missing
                                    College.back().degreeAttendance.push_back(1);
                                }

                            }
                            break;

                        }
                    }

                } else { //if college is empty vector

                    College.push_back({});
                    College.back().name = File.cell[i][college];
                    College.back().count = 1;

                    if (attended != -1) { //if attendance is present
                        College.back().totalAttendance+=stoi(File.cell[i][attended]);
                    } else { //if attendance is missing
                        College.back().totalAttendance++;
                    }

                    for (int l = 0; l < majors.size(); l++) {

                        College.back().degrees.push_back(majors[l]);
                        College.back().degreeCount.push_back(1);

                        if (attended != -1) { //if attendance is present
                            College.back().degreeAttendance.push_back(stoi(File.cell[i][attended]));
                            College.back().totalAttendance+=stoi(File.cell[i][attended]);
                        } else { //if attendance is missing
                            College.back().degreeAttendance.push_back(1);
                            College.back().totalAttendance++;
                        }

                    }

                }

            } else { //if college cell is empty

                for (int l = 0; l < majors.size(); l++) {

                    unsorted.push_back(majors[l]);

                    if (attended != -1) { //if attendance is present
                        attendance.push_back(stoi(File.cell[i][attended]));
                    } else { //if attendance is missing
                        attendance.push_back(1);
                    }

                }

            }
        }
    }

    //handling unsorted degrees
    College.push_back({});
    College.back().name = "Unsorted";

    for (int i = 0; i < unsorted.size(); i++) {

        for (int j = 0; j < College.size(); j++) {

            found = false;

            for (int k = 0; k < College[j].degrees.size(); k++) {
            
                if (unsorted[i] == College[j].degrees[k]) {
                    
                    College[j].degreeCount[k]++;
                    College[j].degreeAttendance[k]+=attendance[i];

                    found = true;
                    break;

                } else if (j == College.size()-1 && k == College[j].degrees.size()-1) {
                    
                    College.back().degrees.push_back(unsorted[i]);
                    College.back().degreeCount.push_back(1);
                    College.back().degreeAttendance.push_back(attendance[i]);
                    break;

                }

            }

            if (found) {
                break;
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