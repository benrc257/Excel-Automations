/*
	Title: main.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program merges existing reports into a single CSV file. This is the main function file.
*/

#include "merger.h"

int main() {
    //variables
    int total;
    Files *File = NULL;

    //prints starting message to screen
    cout << "\n\n***********  ATTENDEE INFORMATION MERGER  ***********";
    cout << "\nThis program will take any \".csv\" reports created";
    cout << "\nusing AttendeeReportCreator.exe and combine them.";
    cout << "\nPlease note that this program will not work if any";
    cout << "\nreports being merged are open while it is running.";      
    cout << "\n*****************************************************\n";

    cout << "\nCreated by Benjamin Clark\n";

    //ask the user for the paths of the CSV input files
    cout << "\nPlease enter the total number of reports to be combined.\n\t>> ";
    while (!(cin >> total)) {
        cout << "\nInvalid input.";
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\nPlease enter the total number of reports to be combined.\n\t>> ";
    }
    cin.ignore();

    //dynamically allocates Files structure
    File = new Files[total];

    for (int i = 0; i < total; i++) {
        do {    
            cout << "\nPlease enter or paste the path of file " << i+1 << ".\nThe path to the file can be found by right-clicking the file and selecting \"Copy as Path\".\n\t>> ";
            getline(cin, File[i].filename);
            if (!(File[i].filename.size() >= 4 && File[i].filename.substr(File[i].filename.size() - 5) == ".csv")) {
                File[i].file.open(File[i].filename, ios::in);
                if (!File[i].file.is_open()) {
                    cout << "\nFile " << i+1 << " could not be opened. Please try again.\n";
                }
            } else {
                cout << "\nFilename must end in \".csv\". If the file is in another format,\nopen it in Excel and export it as a \".csv\" file.\n";
            }
        } while (!File[i].file.is_open());
    }

    //extracts data from the files
    getCells(total, File);

    //processes the data and writes to the new file
    outputCSV(total, File);
    for (int i = 0; i < total; i++) {
        File[i].file.close();
    }

    //frees the dynamically allocated memory
    cout << "\nFreeing memory...\n";
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < File[i].rows; j++) {
           delete [] File[i].Cell[j];
        }
    }
    delete [] File;

    //program ends, displays exit message
    cout << "\nProgram complete. This window will automatically close in 10 seconds.\n";
    delay(10);
    return 0;
}