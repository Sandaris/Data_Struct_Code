#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "common_function.hpp"
#include "Array.hpp"

namespace fs = std::filesystem;
using fs::path;

using namespace std;

//testinggggggggggggg

int main() 
{
    
        // // ───── Dummy values for testing ─────
        // avgSortResult bubbleRes    { 250, 1024 };
        // avgSortResult selectionRes { 220,  980 };
        // avgSortResult insertionRes { 200,  950 };
        // avgSortResult mergeRes     {  50, 2048 };
    
        // avgSortResult linearRes    {  30,  512 };
        // avgSortResult binaryRes    {   5,  256 };
    
        // avgSortResult deleteRes    {  10,  128 };
        // avgSortResult insertRes    {  15,  128 };
        // // ───────────────────────────────────
    
        // printAll(
        //     bubbleRes, selectionRes, insertionRes, mergeRes,
        //     linearRes, binaryRes,   deleteRes,    insertRes
        // );
        // return 0;

        //menu();

        //////////////// NEW LINES TESTING ////////////////////////

        dataContainer2D container = getData("cleaned_reviews.csv");
        if (container.error != 0) {
            cout << "Error loading data\n";
            return 1;
        }
        cout << "Total records: " << container.y << "\n";

        const char * newRecord[] = {
            "PROD441",
            "CUST0001",
            "1",
            "This is a new review."
        };
        int recordSize = 4; // Number of fields in the new record

        dataContainer2D dc = writeNewLines(container, newRecord, recordSize);

        if (dc.error != 0) {
            cout << "Error writing new lines\n";
            return 1;
        }
        cout << "New record added successfully.\n";
        cout << "Total records: " << dc.y << "\n";


        
            cout << "3373th row: ";
            for (int j = 0; j < dc.x; ++j) 
            {
                cout << dc.data[3373-1][j] << " ";
            }
            cout << "\n";
            
        dataContainer2D dc2 = deleteAllRecords(dc, "Customer ID", "CUST5045");
        if (dc2.error != 0) {
            cout << "Error deleting record\n";
            return 1;
        }

        cout << "New record deleted successfully.\n";
        cout << "Total records: " << dc2.y << "\n";


    
}   