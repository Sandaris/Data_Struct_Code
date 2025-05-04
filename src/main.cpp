

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "linkedList.hpp"
#include "Array.hpp"
#include "common_function.hpp"

namespace fs = std::filesystem;
using fs::path;

using namespace std;

long long memoryUsage(const string& sortType, long long& memoryUsed) {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        memoryUsed = pmc.WorkingSetSize / 1024;
        return memoryUsed;
    }
    else {
        cout << "Unable to determine memory usage." << endl;
        memoryUsed = -1;
        cout << memoryUsed;
        return memoryUsed;
    }
};


/////////////////////////////CLEAR THE SCREEN ///////////////////////////////////////////////////////////////
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void questionOne()
{
    // ARRAY
    // Load data from csv Array
    dataContainer2D ct_data = getData("cleaned_transactions.csv");
    dataContainer2D cr_data = getData("cleaned_reviews.csv");
    // Call avg result container for array
    avgSortResult BubbleResultArray, InsertionResultArray, SelectionResultArray, MergeResultArray;

    for(int i = 0; i < 10; i++){
        // Array Bubble sort
        dataContainer2D bubbleData = cloneContainer2D(ct_data);
        SortResult bubbleResult = bubbleSortArray(bubbleData, 4, true);
        BubbleResultArray.avgTime += bubbleResult.timeMicroseconds;
        BubbleResultArray.avgMemory += bubbleResult.memoryKBUsed;
        freeContainer(bubbleData);

        // Array Insertion sort
        dataContainer2D insertionData = cloneContainer2D(ct_data);
        SortResult insertionResult = insertionSortArray(insertionData, 4, true);
        InsertionResultArray.avgTime += insertionResult.timeMicroseconds;
        InsertionResultArray.avgMemory += insertionResult.memoryKBUsed;
        freeContainer(insertionData);

        // Array Selection sort
        dataContainer2D selectionData = cloneContainer2D(ct_data);
        SortResult selectionResult = selectionSortArray(selectionData, 4, true);
        SelectionResultArray.avgTime += selectionResult.timeMicroseconds;
        SelectionResultArray.avgMemory += selectionResult.memoryKBUsed;
        freeContainer(selectionData);

        // Array Merge sort
        dataContainer2D mergeData = cloneContainer2D(ct_data);
        SortResult mergeResult = mergeSortArray(mergeData, 4, true);
        MergeResultArray.avgTime += mergeResult.timeMicroseconds;
        MergeResultArray.avgMemory += mergeResult.memoryKBUsed;
        freeContainer(mergeData);
    }

    // The time store in microseconds but convert to mili seconds
    // /1000 is conversion and /10 is average
    cout << "-----------------------------------------" << endl;
    cout << "                   Array                 " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Average Bubble Sort Time: " << BubbleResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Insertion Sort Time: " << InsertionResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Selection Sort Time: " << SelectionResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Merge Sort Time: " << MergeResultArray.avgTime/10000 << " mili seconds" << endl;
    cout << "-----------------------------------------" << endl;
    // The memory is in Bytes
    cout << "Average Bubble Sort Memory Usage: " << BubbleResultArray.avgMemory/10 << endl;
    cout << "Average Insertion Sort Memory Usage: " << InsertionResultArray.avgMemory/10 << endl;
    cout << "Average Selection Sort Memory Usage: " << SelectionResultArray.avgMemory/10 << endl;
    cout << "Average Merge Sort Memory Usage: " << MergeResultArray.avgMemory/10 << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of transactions in cleaned_transactions.csv: " << ct_data.y << endl;
    cout << "Number of reviews in cleaned_reveiws.csv: " << cr_data.y << endl;
    cout << "-----------------------------------------" << endl;

    freeContainer(ct_data);
    freeContainer(cr_data);

    // LINKED LIST

    LinkedList original;
    original.loadFromCSV("cleaned_transactions.csv");

    avgSortResult BubbleResultLL, InsertionResultLL, SelectionResultLL, MergeResultLL;
    for(int i = 0; i < 10; i++){

        LinkedList BubbleClone = cloneList(original);
        SortResult sortedBubbleLL = bubbleSortLinked(BubbleClone, "Date");
        BubbleResultLL.avgTime += sortedBubbleLL.timeMicroseconds;
        BubbleResultLL.avgMemory += sortedBubbleLL.memoryKBUsed;


        LinkedList InsertionClone = cloneList(original);
        SortResult sortedInsertionLL = insertionSortLinked(InsertionClone, "Date");
        InsertionResultLL.avgTime += sortedInsertionLL.timeMicroseconds;
        InsertionResultLL.avgMemory += sortedInsertionLL.memoryKBUsed;


        LinkedList SelectionClone = cloneList(original);
        SortResult sortedSelectionLL = selectionSortLinked(SelectionClone, "Date");
        SelectionResultLL.avgTime += sortedSelectionLL.timeMicroseconds;
        SelectionResultLL.avgMemory += sortedSelectionLL.memoryKBUsed;


        LinkedList MergeClone = cloneList(original);
        SortResult sortedMergeLL = mergeSortLinked(MergeClone, "Date");
        MergeResultLL.avgTime += sortedMergeLL.timeMicroseconds;
        MergeResultLL.avgMemory += sortedMergeLL.memoryKBUsed;

    }

    // The time store in microseconds but convert to mili seconds
    // /1000 is conversion and /10 is average
    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Average Bubble Sort Time: " << BubbleResultLL.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Insertion Sort Time: " << InsertionResultLL.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Selection Sort Time: " << SelectionResultLL.avgTime/10000 << " mili seconds" << endl;
    cout << "Average Merge Sort Time: " << MergeResultLL.avgTime/10000 << " mili seconds" << endl;
    cout << "-----------------------------------------" << endl;
    // The memory is in Bytes
    cout << "Average Bubble Sort Memory Usage: " << BubbleResultLL.avgMemory/10 << endl;
    cout << "Average Insertion Sort Memory Usage: " << InsertionResultLL.avgMemory/10 << endl;
    cout << "Average Selection Sort Memory Usage: " << SelectionResultLL.avgMemory/10 << endl;
    cout << "Average Merge Sort Memory Usage: " << MergeResultLL.avgMemory/10 << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of transactions in cleaned_transactions.csv: " << original.y<< endl;
    cout << "-----------------------------------------" << endl;
    
    //      // shorten to four algorithm names
    //      const char* names[4] = 
    //      {
    //         "Bubble Sort",
    //         "Selection Sort",
    //         "Insertion Sort",
    //         "Merge Sort"
    //     };
    
    //     // split LL vs Array for easy looping
    //     avgSortResult resultsLL[4]    = { BubbleResultLL, InsertionResultLL, SelectionResultLL, MergeResultLL };
    //     avgSortResult resultsArr[4]   = { BubbleResultArray, InsertionResultArray, SelectionResultArray, MergeResultArray };
    
    //     // column widths
    //     constexpr int COL1 = 15;  // Algorithm
    //     constexpr int COL2 =  7;  // each numeric column
    //     // total inner width = COL1 + 4 cols of COL2 + 5 separators ('|')
    //     int innerW = COL1 + COL2*4 + 5;
    //     std::string sep(innerW, '-');
    
    //     // 1) top border
    //     std::cout << sep << "\n";
    
    //     // 2) centered title
    //     std::cout << center("Average of 10 Simulations", innerW) << "\n";
    
    //     // 3) underline
    //     std::cout << sep << "\n";
    
    //     // 4) two-group header: blank, [ LL ], [ Array ]
    //     int grpW = COL2*2 + 1;  // spans two sub-columns plus the inner '|'
    //     std::cout 
    //         << std::setw(COL1) << "" 
    //         << "|" << center("LL",    grpW)
    //         << "|" << center("Array", grpW)
    //         << "|\n";
       
    //    std::cout << std::setw(COL1);
    
    //     // 5) sub-headers
    //     std::cout
    //         << std::setw(COL1) << std::left   << "Algorithm"
    //         << "|" << std::setw(COL2) << std::right << "Time"
    //         << "|" << std::setw(COL2)              << "Memory"
    //         << "|" << std::setw(COL2)              << "Time"
    //         << "|" << std::setw(COL2)              << "Memory"
    //         << "|\n";
    
    //     // 6) separator
    //     std::cout << sep << "\n";
    
    //     // 7) data rows
    //     for (int i = 0; i < 4; ++i) {
    //         std::cout
    //             << std::setw(COL1) << std::left   << names[i]
    //             << "|" << std::setw(COL2) << std::right << resultsLL[i].avgTime/10000
    //             << "|" << std::setw(COL2)              << resultsLL[i].avgMemory
    //             << "|" << std::setw(COL2)              << resultsArr[i].avgTime/10000
    //             << "|" << std::setw(COL2)              << resultsArr[i].avgMemory
    //             << "|\n";
    //     }
    
    //     // 8) bottom border
    //     std::cout << sep << "\n";
}


void questionTwo(){
    // Array
    dataContainer2D ct_data = getData("cleaned_transactions.csv");

    dataContainer2D mergeData = cloneContainer2D(ct_data);
    mergeSortArray(mergeData, 2, true);
    
    SearchResult FirstLinerSearchResult, SecondLinearSearchResult, FirstBinarySearchResult, SecondBinarySearchResult;
    avgSearchResult arrayLinear, arrayBinary;

    for(int i = 0; i < 10; i++) {
        dataContainer2D FirstLinerSearch = Array_LinearSearch(ct_data, 2, "Electronics", FirstLinerSearchResult);
        dataContainer2D SecondLinearSearch = Array_LinearSearch(FirstLinerSearch, 5, "Credit Card", SecondLinearSearchResult);
    
        arrayLinear.avgTime += FirstLinerSearchResult.timeMicroseconds + SecondLinearSearchResult.timeMicroseconds;
        arrayLinear.avgMemory += FirstLinerSearchResult.memoryUsed + SecondLinearSearchResult.memoryUsed;
        arrayLinear.avgResultCount += SecondLinearSearchResult.resultCount;
        arrayLinear.percentage = (SecondLinearSearch.y / (float)FirstLinerSearch.y) * 100;
    
        dataContainer2D FirstBinarySearch = Array_BinarySearch(mergeData, 2, "Electronics", FirstBinarySearchResult);
        mergeSortArray(FirstBinarySearch, 5, true);
        dataContainer2D SecondBinarySearch = Array_BinarySearch(FirstBinarySearch, 5, "Credit Card", SecondBinarySearchResult);
    
        arrayBinary.avgTime += FirstBinarySearchResult.timeMicroseconds + SecondBinarySearchResult.timeMicroseconds;
        arrayBinary.avgMemory += FirstBinarySearchResult.memoryUsed + SecondBinarySearchResult.memoryUsed;
        arrayBinary.avgResultCount += SecondBinarySearchResult.resultCount;
        arrayBinary.percentage = (SecondBinarySearch.y / (float)FirstBinarySearch.y) * 100;
    
        freeContainer(FirstLinerSearch);
        freeContainer(SecondLinearSearch);
        freeContainer(FirstBinarySearch);
        freeContainer(SecondBinarySearch);
    }
    
    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Total Transactions: " << ct_data.y << endl;
    cout << "Linear - Result Count: " << arrayLinear.avgResultCount / 10 << " | %: " << arrayLinear.percentage << endl;
    cout << "Binary - Result Count: " << arrayBinary.avgResultCount / 10 << " | %: " << arrayBinary.percentage << endl;
    cout << "Average Linear Time: " << arrayLinear.avgTime / 10 << " Micro Seconds | Memory: " << arrayLinear.avgMemory / 10 << " bytes" << endl;
    cout << "Average Binary Time: " << arrayBinary.avgTime / 10 << " Micro Seconds | Memory: " << arrayBinary.avgMemory / 10 << " bytes" << endl;
    cout << "-----------------------------------------" << endl;    

    // Linked List
    LinkedList original;
    original.loadFromCSV("cleaned_transactions.csv");
    LinkedList mergeDataLL = cloneList(original);
    
    // create storing container
    SearchResult FirstLinerSearchResultLL, SecondLinearSearchResultLL, FirstBinarySearchResultLL, SecondBinarySearchResultLL;
    avgSearchResult listLinear, listBinary;

    // 10 iteration to get average
    for(int i = 0; i < 10; i++) {
        // linear search LL
        LinkedList FirstLinerSearchLL = LL_linearSearch(original, "Category", "Electronics", FirstLinerSearchResultLL);
        LinkedList SecondLinearSearchLL = LL_linearSearch(FirstLinerSearchLL, "Payment Method", "Credit Card", SecondLinearSearchResultLL);
        // Linear storing
        listLinear.avgTime += FirstLinerSearchResultLL.timeMicroseconds + SecondLinearSearchResultLL.timeMicroseconds;
        listLinear.avgMemory += FirstLinerSearchResultLL.memoryUsed + SecondLinearSearchResultLL.memoryUsed;
        listLinear.percentage = (SecondLinearSearchLL.y / (float)FirstLinerSearchLL.y) * 100;
        
        // binary search LL
        LinkedList CloneLLBinary = cloneList(mergeDataLL);
        mergeSortLinked(CloneLLBinary, "Category");
        LinkedList FirstBinarySearchLL = LL_binarySearch(CloneLLBinary, "Category", "Electronics", FirstBinarySearchResultLL);
        mergeSortLinked(FirstBinarySearchLL, "Payment Method");
        LinkedList SecondBinarySearchLL = LL_binarySearch(FirstBinarySearchLL, "Payment Method", "Credit Card", SecondBinarySearchResultLL);
        // Binary storing
        listBinary.avgTime += FirstBinarySearchResultLL.timeMicroseconds + SecondBinarySearchResultLL.timeMicroseconds;
        listBinary.avgMemory += FirstBinarySearchResultLL.memoryUsed + SecondBinarySearchResultLL.memoryUsed;
        listBinary.percentage = (SecondBinarySearchLL.y / (float)FirstBinarySearchLL.y) * 100;
    }
    listBinary.avgResultCount = SecondBinarySearchResultLL.resultCount;
    listLinear.avgResultCount = SecondLinearSearchResultLL.resultCount;
    
    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Total Transactions: " << original.y << endl;
    cout << "Linear - Result Count: " << listLinear.avgResultCount / 10 << " | %: " << listLinear.percentage << endl;
    cout << "Binary - Result Count: " << listBinary.avgResultCount / 10 << " | %: " << listBinary.percentage << endl;
    cout << "Average Linear Time: " << listLinear.avgTime / 10 << " Micro Seconds | Memory: " << listLinear.avgMemory / 10 << " bytes" << endl;
    cout << "Average Binary Time: " << listBinary.avgTime / 10 << " Micro Seconds | Memory: " << listBinary.avgMemory / 10 << " bytes" << endl;
    cout << "-----------------------------------------" << endl;

}

void questionThree(){

    // Array
    // Load data from csv Array
    dataContainer2D cr_data = getData("cleaned_reviews.csv");

    // create filtering container
    SearchResult OneStarResult;
    dataContainer2D OneStar = Array_LinearSearch(cr_data, 2, "1", OneStarResult);
    // find most common words
    WordFrequency wf = getWordFrequencyArray(OneStar, 3);
    // sort highest count to lowest
    WFsort(wf);

    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of reviews in cleaned_reviews.csv: " << cr_data.y << endl;
    cout << "Number of reviews with 1 star in cleaned_reviews.csv: " << OneStar.y << endl;
    cout << "Number of different words in cleaned_reviews.csv: " << wf.size << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Top 10 words:\n";
    for (int i = 0; i < 10; ++i) {
        cout << wf.words[i] << ": " << wf.counts[i] << endl;
    }
    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << wf.timeMicroseconds << " Micro Seconds\n";
    cout << "Estimated memory: " << wf.memoryUsed << " bytes\n";
    cout << "-----------------------------------------" << endl;
    // free container
    freeContainer(OneStar);
    freeContainer(cr_data);
    freeFrequencyContainer(wf);

    // Linked List
    // Load data from csv Linked List
    LinkedList original;
    original.loadFromCSV("cleaned_reviews.csv");

    // create filtering container
    LinkedList OneStarLL = original.linearKeepRows("Rating", "1");

    // find most common words
    WordFrequency wfLL = countTopWordsFromLinkedList(OneStarLL, "Review Text", 10);

    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of reviews in cleaned_reviews.csv: " << original.y << endl;
    cout << "Number of reviews with 1 star in cleaned_reviews.csv: " << OneStarLL.y << endl;
    cout << "Number of different words in cleaned_reviews.csv: " << wf.size << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Top 10 words:\n";
    for (int i = 0; i < 10; ++i) {
        cout << wfLL.words[i] << ": " << wfLL.counts[i] << endl;
    }
    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << wfLL.timeMicroseconds << " Micro Seconds\n";
    cout << "Estimated memory: " << wfLL.memoryUsed << " bytes\n";
    cout << "-----------------------------------------" << endl;
    // free container
    freeFrequencyContainer(wfLL);

}

void question_insert(){
    // Array
    dataContainer2D cr_data = getData("cleaned_reviews.csv");
    InsDelResult ArrayInsertResult;

    cout << "Total records: " << cr_data.y << "\n";

    const char * newRecord[] = {"PROD441", "CUST0001","1", "This is a new review."};
    int recordSize = 4; // Number of fields in the new record

    dataContainer2D UpdatedData = writeNewLines(cr_data, newRecord, recordSize, ArrayInsertResult);

    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "New record added successfully.\n";
    cout << "-----------------------------------------" << endl;
    cout << "Total records before insert: " << cr_data.y << "\n";
    cout << "Total records after insert: " << UpdatedData.y << "\n";
    cout << "-----------------------------------------" << endl;
        cout << "3373th row: ";
        for (int j = 0; j < UpdatedData.x; ++j) 
        {
            cout << " | " <<UpdatedData.data[3373-1][j] << " | ";
        }
        cout << "\n";
    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << ArrayInsertResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << ArrayInsertResult.memory << " bytes\n";
    cout << "-----------------------------------------" << endl;

    // Linked List    
    LinkedList original;
    original.loadFromCSV("cleaned_reviews.csv");
    InsDelResult LLInsertResult;

    const char * newRow[] = {"PROD424", "CUST0001","3", "This is a new review."};

    LinkedList updatedLL = insertNewRowLinkedList(original,newRow,4,LLInsertResult);

    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "New record added successfully.\n";
    cout << "-----------------------------------------" << endl;
    cout << "Total records before insert: " << original.y << "\n";
    cout << "Total records after insert: " << updatedLL.y << "\n";
    cout << "-----------------------------------------" << endl;
    //cout << " HOW TO PRINT LINE FOR LINK LIST ? " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << LLInsertResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << LLInsertResult.memory << " bytes\n";
    cout << "-----------------------------------------" << endl;
    //updatedLL.printForward(3373-1); // Print the 3373th row


freeContainer(cr_data);
freeContainer(UpdatedData);
}

void question_delete(){

    // Array
    dataContainer2D cr_data = getData("cleaned_reviews.csv");
    InsDelResult ArrayDeleteResult;

    dataContainer2D deletedData = deleteAllRecords(cr_data, "Customer ID", "CUST5045", ArrayDeleteResult);

    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "ALl records with Customer ID = CUST5045 deleted successfully.\n";
    cout << "-----------------------------------------" << endl;
    cout << "Total records before delete: " << cr_data.y << "\n";
    cout << "Total records after delete: " << deletedData.y << "\n";
    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << ArrayDeleteResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << ArrayDeleteResult.memory << " bytes\n";
    cout << "-----------------------------------------" << endl;

    // Link List
    LinkedList originalList;
    originalList.loadFromCSV("cleaned_reviews.csv");
    InsDelResult LLDeleteResult;

    LinkedList deletedLL = deleteRows(originalList, "Customer ID", "CUST7710", LLDeleteResult);

    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "ALl records with Customer ID = CUST7710 deleted successfully.\n";
    cout << "-----------------------------------------" << endl;
    cout << "Total records before delete: " << originalList.y << "\n";
    cout << "Total records after delete: " << deletedLL.y << "\n";
    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << LLDeleteResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << LLDeleteResult.memory << " bytes\n";
    cout << "-----------------------------------------" << endl;

    freeContainer(cr_data);
    freeContainer(deletedData);
}

void menu()
{
    while (true) {
        clearScreen();

        cout << "1. Sort Customer by Date\n"
             << "2. Percentage of Electronics that paid using Credit Card\n"
             << "3. Most Frequent Word in 1 Star Rating\n"
             << "4. Insert new row\n"
             << "5. Delete a row\n"
             << "6. Exit\n"
             << "Enter your choice (1-6): ";

        int choice;
        // validate numeric input
        while (!(cin >> choice) || choice < 1 || choice > 6) {
            cin.clear();
            clearScreen();
            cout << "Invalid choice. Please enter a number between 1 and 6.\n"
                 << "Enter your choice (1-6): ";
        }

        clearScreen();
        // dispatch
        switch (choice) {
            case 1:
                cout << "Sorting Customer by Date...\n\n";
                questionOne();
                break;
            case 2:
                cout << "Calculating percentage of Electronics that paid using Credit Card...\n\n";
                questionTwo();
                break;
            case 3:
                cout << "Finding most frequent word in 1 Star Rating...\n\n";
                questionThree();
                break;
            case 4:
                cout << "Inserting new row...\n\n";
                question_insert();
                break;
            case 5:
                cout << "Deleting a row...\n\n";
                question_delete();
                break;
            case 6:
                cout << "Exiting...\n";
                return;
        }

        // ask if we should loop again
        char again;
        do {
            cout << "\nReturn to menu? (Y/N): ";
            cin >> again;
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
            again = tolower(again);
        } while (again != 'y' && again != 'n');

        if (again == 'n') {
            cout << "Goodbye!\n";
            break;
        }
        // else continue; next iteration will clearScreen() and redraw menu
    }
}

int main() 
{
    menu();
}  