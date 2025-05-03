#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "LinkedList.hpp"
#include "Array.hpp"

namespace fs = std::filesystem;
using fs::path;

using namespace std;

void questionOne()
{
    // ARRAY
    dataContainer2D ct_data = getData("cleaned_transactions.csv");
    dataContainer2D cr_data = getData("cleaned_reviews.csv");

    avgSortResult avgBubbleResultArray, avgInsertionResultArray, avgSelectionResultArray, avgMergeResultArray;
    int b,i,s,m = 0;
    for(int i = 0; i < 10; i++){

        dataContainer2D bubbleData = cloneContainer2D(ct_data);
        SortResult bubbleResult = bubbleSortArray(bubbleData, 4, true);

        avgBubbleResultArray.avgTime += bubbleResult.timeMicroseconds;
        cout << avgBubbleResultArray.avgTime << endl;
        avgBubbleResultArray.avgMemory += bubbleResult.memoryKBUsed;
        cout << avgBubbleResultArray.avgMemory << endl;
        freeContainer(bubbleData);

        dataContainer2D insertionData = cloneContainer2D(ct_data);
        SortResult insertionResult = insertionSortArray(insertionData, 4, true);

        avgInsertionResultArray.avgTime += insertionResult.timeMicroseconds;
        cout << avgInsertionResultArray.avgTime << endl;
        avgInsertionResultArray.avgMemory += insertionResult.memoryKBUsed;
        cout << avgInsertionResultArray.avgMemory << endl;
        freeContainer(insertionData);

        dataContainer2D selectionData = cloneContainer2D(ct_data);
        SortResult selectionResult = selectionSortArray(selectionData, 4, true);

        avgSelectionResultArray.avgTime += selectionResult.timeMicroseconds;
        cout << avgSelectionResultArray.avgTime << endl;
        avgSelectionResultArray.avgMemory += selectionResult.memoryKBUsed;
        cout << avgSelectionResultArray.avgMemory << endl;
        freeContainer(selectionData);

        dataContainer2D mergeData = cloneContainer2D(ct_data);
        SortResult mergeResult = mergeSortArray(mergeData, 4, true);

        avgMergeResultArray.avgTime += mergeResult.timeMicroseconds;
        cout << avgMergeResultArray.avgTime << endl;
        avgMergeResultArray.avgMemory += mergeResult.memoryKBUsed;
        cout << avgMergeResultArray.avgMemory << endl;
        freeContainer(mergeData);
        cout << "-----------------------------------------" << endl;
    }

    // The time store in microseconds but convert to mili seconds
    cout << "-----------------------------------------" << endl;
    cout << "Average Bubble Sort Time: " << avgBubbleResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Insertion Sort Time: " << avgInsertionResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Selection Sort Time: " << avgSelectionResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Merge Sort Time: " << avgMergeResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "-----------------------------------------" << endl;
    // The memory is in Bytes
    cout << "Average Bubble Sort Memory Usage: " << avgBubbleResultArray.avgMemory/10 << endl;
    cout << "Average Insertion Sort Memory Usage: " << avgInsertionResultArray.avgMemory/10 << endl;
    cout << "Average Selection Sort Memory Usage: " << avgSelectionResultArray.avgMemory/10 << endl;
    cout << "Average Merge Sort Memory Usage: " << avgMergeResultArray.avgMemory/10 << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of transactions in cleaned_transactions.csv: " << ct_data.y << endl;
    cout << "Number of reviews in cleaned_reveiws.csv: " << cr_data.y << endl;
    cout << "-----------------------------------------" << endl;

    freeContainer(ct_data);
    freeContainer(cr_data);

    // LINKED LIST

    LinkedList original;
    original.loadFromCSV("cleaned_transactions.csv");
    LinkedList clone = cloneList(original);
    LinkedList sorted = mergeSortLinked(clone, "A", true);

}















int main() 
{
    questionOne();
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