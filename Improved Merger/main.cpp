/*
	Title: main.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program merges existing reports into a single CSV file. This is the main function file.
*/

#include "merger.h"

int main() {
    //variables
    int total = 0;
    bool duplicate = false;
    vector<string> input(1);
    vector<fstream> files(1);
    Files *File = NULL;

    //prints starting message to screen
    cout << "\n\n***********  ATTENDEE INFORMATION MERGER  ***********";
    cout << "\nThis program will take any \".csv\" reports created";
    cout << "\nusing AttendeeReportCreator.exe and combine them.";
    cout << "\nPlease note that this program will not work if any";
    cout << "\nreports being merged are open while it is running.";      
    cout << "\n*****************************************************\n";

    cout << "\nCreated by Benjamin Clark\n";

    //asks the user for file paths and attempts to open them
    do {
        do {    
            cout << "\nPlease enter or paste the path of file " << total+1 << ". If you are done entering files, enter \"N\".\nThe path to the file can be found by right-clicking the file and selecting \"Copy as Path\".\n\t>> ";
            getline(cin, input.at(total));
            if (total > 1 && (input.at(total) == "n" || input.at(total) == "N")) { //if there are two total files and the user enters n or N
                break; //breaks the loop of attempting to open the file
            } else if (total <= 1 && (input.at(total) == "n" || input.at(total) == "N")){ //if there are less than two total files and the user enters n or N
                cout << "\nYou must enter at least 2 files to continue.\n";
            } else if (input.at(total).size() > 4 && input.at(total).find(".csv", (input.at(total).size()-5)) != string::npos) { //if the file ends in .csv
                for (int i = 0; i < total; i++) {
                    if (input.at(total) == input.at(i)) {
                        cout << "\nThis file has already been entered.\n";
                        duplicate = true;
                        break;
                    } else {
                        duplicate = false;
                    }
                }
                files.at(total).open(input.at(total), ios::in);
                if (duplicate || !files.at(total).is_open()) { //if the file can't be opened or is a duplicate
                    cout << "\nFile " << total+1 << " could not be opened. Please try again.\n";
                } else if (!duplicate) { //if the file opens successfully and isn't a duplicate
                    total++;
                    input.push_back("");
                    files.push_back(fstream());
                }
            } else { //if the file does not end in .csv
                cout << "\nFilename must end in \".csv\". If the file is in another format,\nopen it in Excel and export it as a \".csv\" file.\n";
            }
        } while (!files.at(total).is_open()); //loops until the file is successfully opened
    } while (input.at(total) != "n" && input.at(total) != "N"); //loops until n or N is entered

    //dynamically allocates Files structure array
    File = new Files[total];

    //Assigns the file memory addresses to File structure
    for (int i = 0; i < total; i++) {
        File[i].file = &files[i];
    }

    //extracts data from the files
    getCells(total, File);

    //processes the data and writes to the new file
    outputCSV(total, File);
    

    //frees the dynamically allocated memory
    cout << "\nFreeing memory...\n";
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < File[i].rows; j++) {
           delete [] File[i].cell[j];
        }
        delete [] File[i].cell;
    }
    delete [] File;

    //program ends, displays exit message
    cout << "\nProgram complete. This window will automatically close in 10 seconds.\n";
    delay(10);
    return 0;
}