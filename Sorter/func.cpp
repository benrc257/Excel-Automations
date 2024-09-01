/*
	Title: func.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program sorts attendee info and outputs a CSV file. This file contains function definitions.
*/

#include "sorter.h"

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
int getCells(int& cells, int& rows, int& columns, fstream& file) {
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
    Inputs: int&, int&, int&, fstream&, Cells**
    Purpose: Retrieve the value of each cell in the file and enter it into the structure array.
*/
int getCells(int& cells, int& rows, int& columns, fstream& file, Cells** Cell) {
    //variables
    string line, quoteLine1, quoteLine2, inquotes;
    char * output;
    size_t start, end, quote1, quote2, comma;

    file.clear();
    file.seekg(0, ios::beg);

    for (int i = 0; i < rows; i++) {
        start = 0;
        end = 0;
        getline(file, line);

        quote2 = 0; //do some work to detect quotes, detect commas inside, and replace them with a pipe
        do {
            quote1 = line.find("\"", quote2);
            if (quote1 != string::npos) {
                quote2 = line.find("\"", quote1+1);
                if (quote2 != string::npos && (quote1+1) != quote2) {
                    inquotes = line.substr(quote1, (quote2-quote1)+1);
                    comma = inquotes.find(",");
                    if (comma != string::npos) {
                        quoteLine1 = inquotes.substr(0, comma);
                        quoteLine2 = inquotes.substr(comma+1);
                        inquotes = quoteLine1 + " |" + quoteLine2;
                        quoteLine1 = line.substr(0, quote1);
                        quoteLine2 = line.substr(quote2+1);
                        line = quoteLine1 + inquotes + quoteLine2;
                    }
                    quote2 = quote2+1;
                } else if (quote2 == string::npos) {
                    break;
                } else if ((quote1+1) == quote2) {
                    quote2 = quote1+2;
                }
            }
        } while (quote1 != string::npos);

        for (int j = 0; j < columns; j++) {
            end = line.find(',', start);
            if (end != string::npos) {
                Cell[i][j].value = line.substr(start, (end - start));
            } else {
                Cell[i][j].value = line.substr(start);
            }
            start = end+1;
        }
    }

    return 1;
}

/*
    Name: outputCSV()
    Inputs: int&, int&, int&, string, Cells**
    Purpose: Retrieve the value of each cell in the file and enter it into the structure array.
*/
void outputCSV(int& cells, int& rows, int& columns, string filename, Cells** Cell) {
    //variables
    size_t start;
    ofstream file;
    int first = -1, last = -1, checked = -1, college = -1, major = -1, year = -1;
    bool checkedIn[rows];
    string formattedCollege[rows];

    filename.erase(filename.size() - 4);
    filename.append(".REPORT.csv");

    cout << "\nCreating output file...\n";
    file.open(filename);
    if (!file.is_open()) {
        cout << "\nFile could not be created or opened. Aborting...\n";
        return;
    }
    cout << "\nOutput file successfully created and opened.\n";

    //
    //HEY BEN THIS IS WHERE YOU CHANGE THE FORMATTING OF THE DATA AND OUTPUT!!!!!
    //

    cout << "\nAnalyzing formatting...\n";
    for (int j = 0; j < columns; j++) {
        if (Cell[0][j].value == "First Name") {
            first = j;
        } else if (Cell[0][j].value == "Last Name") {
            last = j;
        } else if (Cell[0][j].value == "Checked In") {
            checked = j;
        } else if (Cell[0][j].value == "College") {
            college = j;
        } else if (Cell[0][j].value == "Majors") {
            major = j;
        } else if (Cell[0][j].value == "School Year") {
            year = j;
        }
    }

    if (first == -1 || last == -1 || checked == -1 || college == -1 || major == -1 || year == -1) {
        cout << "\nERROR: THE FORMATTING IN THE INPUT DOCUMENT HAS CHANGED.\nHANDSHAKE MAY HAVE UPDATED THEIR CSV FORMAT.\nAborting...\n";
        return;
    }

    cout << "\nFormatting data...\n";
    for (int i = 1; i < rows; i++) {
        if (Cell[i][checked].value == "\"\"" || Cell[i][checked].value == "") {
            checkedIn[i] = false;
        } else {
            checkedIn[i] = true;
        }
        if (Cell[i][college].value.find("Agriculture") != string::npos) {
            formattedCollege[i] = "Agriculture and Human Ecology";
        } else if (Cell[i][college].value.find("Sciences") != string::npos) {
            formattedCollege[i] = "Arts and Sciences";
        } else if (Cell[i][college].value.find("Business") != string::npos) {
            formattedCollege[i] = "Business";
        } else if (Cell[i][college].value.find("Education") != string::npos) {
            formattedCollege[i] = "Education";
        } else if (Cell[i][college].value.find("Engineering") != string::npos || Cell[i][major].value.find("Engineering") != string::npos) {
            formattedCollege[i] = "Engineering";
        } else if (Cell[i][college].value.find("Fine") != string::npos) {
            formattedCollege[i] = "Fine Arts";
        } else if (Cell[i][college].value.find("Interdisciplinary") != string::npos) {
            formattedCollege[i] = "Interdisciplinary Studies";
        } else if (Cell[i][college].value.find("Nurs") != string::npos) {
            formattedCollege[i] = "Nursing";
        } else {
            formattedCollege[i] = "Other or Unlisted";
        }
    }


    cout << "\nPrinting to file...\n";
    file << "Last Name,First Name,College,Major,Year\n";
    for (int i = 1; i < rows; i++) {
        if (checkedIn[i]) {
            file << Cell[i][last].value << ","
                << Cell[i][first].value << ","
                << formattedCollege[i] << ","
                << Cell[i][major].value << ","
                << Cell[i][year].value << "\n";
        }
    }
    cout << "\nPrinting complete. Output can be found at: " << filename << "\n";

    cout << "\nClosing file...\n";
    file.close();
    return;
}