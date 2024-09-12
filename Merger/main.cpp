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
    vector<string> input;
    vector<Files> File;

    //prints starting message to screen
    cout << "\n\n***********  ATTENDEE INFORMATION MERGER  ***********";
    cout << "\nThis program will take any \".csv\" reports created";
    cout << "\nusing AttendeeReportCreator.exe and combine them.";
    cout << "\nPlease note that this program will not work if any";
    cout << "\nreports used are already open in other programs.";
    cout << "\n*****************************************************\n";

    cout << "\nCreated by Benjamin Clark\n";

    //asks the user for file paths and attempts to open them
    do {
        do {
            cout << "\nPlease enter or paste the path of file " << total+1 << ". If you are done entering files, enter \"N\".\nThe path to the file can be found by shift+right-clicking the file and selecting \"Copy as Path\".\n\t>> ";
            input.push_back("");
            getline(cin, input[total]);

            if (input[total][0] == '\"' && input[total].size() > 2) {
                input[total] = input[total].substr(1);
                input[total].pop_back();
            }

            for (int i = 0; i < total; i++) {
                if (input[i] == input[total]) {
                    duplicate = true;
                    break;
                } else {
                    duplicate = false;
                }
            }

            if (duplicate) {
                input.pop_back();
                cout << "\033[31m" << "\nThis file has already been entered.\n" << "\033[0m";
                break;
            }

            if (input[total] == "n" || input[total] == "N") {
                if (input.size() > 2) {
                    break;
                } else {
                    input.pop_back();
                    cout << "\033[31m" << "\nYou must select at least two reports to merge.\n" << "\033[0m";
                }
            } else if (input[total].size() > 11) {
                if (input[total].substr(input[total].size()-11) == ".REPORT.csv" || input[total].substr(input[total].size()-11) == ".MERGED.csv") {
                    File.push_back({});
                    File[total].file.open(input[total], ios::in);

                    if (File[total].file.is_open()) {
                        cout << "\033[32m" << "\nFile opened successfully.\n" << "\033[0m";
                        total++;
                    } else {
                        cout << "\033[31m" << "\nFile could not be opened. Please try again.\n" << "\033[0m";
                        File.pop_back();
                    }
                } else {
                    input.pop_back();
                    cout << "\033[31m" << "\nThe name of this file does not end in \".REPORT.csv\". Ensure it is a properly formatted report,\nthen rename the file to end in \".REPORT.csv\"\n" << "\033[0m";
                }
            } else {
                input.pop_back();
                cout << "\033[31m" << "\nThe name of this file is less than 11 characters long. Ensure it is a properly formatted report,\nand that the name ends in \".REPORT.csv\"\n" << "\033[0m";
            }

        } while (!(File[total].file.is_open()));
    } while (input[total] != "n" && input[total] != "N");

    //extracts data from the files
    getCells(total, File);

    //processes the data and writes to the new file
    outputCSV(total, File);

    //program ends, displays exit message
    cout << "\nProgram complete. This window will automatically close in 10 seconds.\n";
    delay(10);
    return 0;
}