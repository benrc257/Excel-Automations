/*
	Title: main.cpp
	Author: Benjamin Clark
	Date: 8/24/24
	Purpose: This program sorts attendee info and outputs a CSV file. This is the main function file.
*/

#include "sorter.h"

int main() {
    //variables
    int size = 0, rows = 0, columns = 0;
    string filename;
    fstream file;
    Cells **Cell = NULL;

    //prints starting message to screen
    cout << "\n\n**********  ATTENDEE INFORMATION FORMATTER  **********";
    cout << "\nThis program will take any \".csv\" file downloaded from";
    cout << "\nhandshake and format it for use in our data reports.";
    cout << "\nPlease note that this program will override previous";
    cout << "\nreports if they share the same name as the input file.";      
    cout << "\n******************************************************\n";

    cout << "\nCreated by Benjamin Clark\n";

    //ask the user for the path of the CSV input file until the file opens
    do {
        cout << "\nPlease enter or paste the path of the .CSV file with the attendee info.\nThe path to the file can be found by shift + right-clicking the file and selecting \"Copy as Path\".\n\t>> ";
        getline(cin, filename);
        if (filename[0] == '\"' && filename.size() > 2) {
            filename = filename.substr(1);
            filename.pop_back();
        }
        if (filename.size() > 4 && filename.substr(filename.size() - 5) == ".csv") {
            file.open(filename, ios::in);
            if (!file.is_open()) {
                cout << "\nFile could not be opened. Please try again.\n";
            }
        } else {
            cout << "\nFilename must end in \".csv\". If the file is in another format,\nopen it in Excel and export it as a \".csv\" file.\n";
        }
    } while (!file.is_open());

    cout << "\nFile opened successfully.\n";

    //counts cells, rows, and columns in the csv file
    cout << "\nCounting cells...\n";

    if (!getCells(size, rows, columns, file)) {
        cout << "\nCounting failed. Closing program in 5...\n";
        delay(5);
        return 1;
    } else if (rows == 0 || columns == 0) {
        cout << "\nGiven \".csv\" file is empty. Closing program in 5...\n";
        delay(5);
        return 1;
    } else if (rows <= 1) {
        cout << "\nGiven \".csv\" file has no attendee information. Closing program in 5...\n";
        delay(5);
        return 1;
    } else {
        cout << "\nCounting complete. " << size << " cells, with " << rows << " rows and " << columns << " columns found.\n";
    }

    //initializes the Cell pointer to be a dynamically allocated 2D array of the cells from the source CSV file.
    Cell = new Cells*[rows];
    for (int i = 0; i < rows; i++) {
        Cell[i] = new Cells[columns];
    }

    //fills the 2D Cell array with values from the CSV file
    getCells(size, rows, columns, file, Cell);
    file.close();

    //processes the data and writes to the new file
    outputCSV(size, rows, columns, filename, Cell);

    //frees the dynamically allocated memory
    cout << "\nFreeing memory...\n";
    for (int i = 0; i < rows; i++) {
       delete [] Cell[i];
    }
    delete [] Cell;

    //program ends, displays exit message
    cout << "\nProgram complete. This window will automatically close in 10 seconds.\n";
    delay(10);
    return 0;
}