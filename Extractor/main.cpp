/*
	Title: main.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program extracts data from a merged CSV file. This is the main function file.
*/

#include "extractor.h"

int main() {
    //variables
    string input;
    Files File;

    //prints starting message to screen
    cout << "\n\n***********  ATTENDEE INFORMATION MERGER  ***********";
    cout << "\nThis program will extract info from \".csv\" reports created";
    cout << "\nusing AttendeeReportCreator.exe or ReportMerger.exe.";
    cout << "\nPlease note that this program will not work if any";
    cout << "\nreports used are already open in other programs.";
    cout << "\n*****************************************************\n";

    cout << "\nCreated by Benjamin Clark\n";

    //asks the user for file paths and attempts to open them
    do {
        cout << "\nPlease enter or paste the path of the report.\nThe path to the file can be found by shift+right-clicking the file and selecting \"Copy as Path\".\n\t>> ";
        getline(cin, input);

        if (input[0] == '\"' && input.size() > 2) {
            input = input.substr(1);
            input.pop_back();
        }

        if (input.size() > 11) {
            if (input.substr(input.size()-11) == ".MERGED.csv" || input.substr(input.size()-11) == ".REPORT.csv") {
                File.file.open(input, ios::in);

                if (File.file.is_open()) {
                    cout << "\nFile opened successfully.\n";
                } else {
                    cout << "\nFile could not be opened. Please try again.\n";
                }
            } else {
                cout << "\nThe name of this file does not end in \".REPORT.csv\". Ensure it is a properly formatted report,\nthen rename the file to end in \".REPORT.csv\"\n";
            }
        } else {
            cout << "\nThe name of this file is less than 11 characters long. Ensure it is a properly formatted report,\nand that the name ends in \".REPORT.csv\"\n";
        }

    } while (!(File.file.is_open()));

    //extracts data from the files
    getCells(File);

    //processes the data and writes to the new file
    outputCSV(File);

    //program ends, displays exit message
    cout << "\nProgram complete. This window will automatically close in 10 seconds.\n";
    delay(10);
    return 0;
}