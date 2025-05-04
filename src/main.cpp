

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




void questionOne()
{
    // ARRAY
    // Load data from csv Array
    dataContainer2D ct_data = getData("cleaned_transactions.csv");
    dataContainer2D cr_data = getData("cleaned_reviews.csv");
    // Call avg result container for array
    avgSortResult BubbleResultArray, InsertionResultArray, SelectionResultArray, MergeResultArray;

    for (int i = 0; i < 10; i++) {
        long long memBefore = 0, memAfter = 0;
    
        // Array Bubble sort
        memoryUsage("Before Bubble Sort", memBefore);
        dataContainer2D bubbleData = cloneContainer2D(ct_data);
        SortResult bubbleResult = bubbleSortArray(bubbleData, 4, true);
        memoryUsage("After Bubble Sort", memAfter);
        bubbleResult.memoryKBUsed = memAfter - memBefore;
        BubbleResultArray.avgTime += bubbleResult.timeMicroseconds;
        BubbleResultArray.avgMemory += bubbleResult.memoryKBUsed;
        freeContainer(bubbleData);
    
        // Array Insertion sort
        memoryUsage("Before Insertion Sort", memBefore);
        dataContainer2D insertionData = cloneContainer2D(ct_data);
        SortResult insertionResult = insertionSortArray(insertionData, 4, true);
        memoryUsage("After Insertion Sort", memAfter);
        insertionResult.memoryKBUsed = memAfter - memBefore;
        InsertionResultArray.avgTime += insertionResult.timeMicroseconds;
        InsertionResultArray.avgMemory += insertionResult.memoryKBUsed;
        freeContainer(insertionData);
    
        // Array Selection sort
        memoryUsage("Before Selection Sort", memBefore);
        dataContainer2D selectionData = cloneContainer2D(ct_data);
        SortResult selectionResult = selectionSortArray(selectionData, 4, true);
        memoryUsage("After Selection Sort", memAfter);
        selectionResult.memoryKBUsed = memAfter - memBefore;
        SelectionResultArray.avgTime += selectionResult.timeMicroseconds;
        SelectionResultArray.avgMemory += selectionResult.memoryKBUsed;
        freeContainer(selectionData);
    
        // Array Merge sort
        memoryUsage("Before Merge Sort", memBefore);
        dataContainer2D mergeData = cloneContainer2D(ct_data);
        SortResult mergeResult = mergeSortArray(mergeData, 4, true);
        memoryUsage("After Merge Sort", memAfter);
        mergeResult.memoryKBUsed = memAfter - memBefore;
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

    for (int i = 0; i < 10; i++) {
        long long memBefore, memAfter;

    // Bubble Sort - Linked List
        memoryUsage("Before Bubble Sort LL", memBefore);
        LinkedList BubbleClone = cloneList(original);
        SortResult sortedBubbleLL = bubbleSortLinked(BubbleClone, "Date");
        memoryUsage("After Bubble Sort LL", memAfter);
        sortedBubbleLL.memoryKBUsed = memAfter - memBefore;
        BubbleResultLL.avgTime += sortedBubbleLL.timeMicroseconds;
        BubbleResultLL.avgMemory += sortedBubbleLL.memoryKBUsed;

    // Insertion Sort - Linked List
        memoryUsage("Before Insertion Sort LL", memBefore);
        LinkedList InsertionClone = cloneList(original);
        SortResult sortedInsertionLL = insertionSortLinked(InsertionClone, "Date");
        memoryUsage("After Insertion Sort LL", memAfter);
        sortedInsertionLL.memoryKBUsed = memAfter - memBefore;
        InsertionResultLL.avgTime += sortedInsertionLL.timeMicroseconds;
        InsertionResultLL.avgMemory += sortedInsertionLL.memoryKBUsed;

    // Selection Sort - Linked List
        memoryUsage("Before Selection Sort LL", memBefore);
        LinkedList SelectionClone = cloneList(original);
        SortResult sortedSelectionLL = selectionSortLinked(SelectionClone, "Date");
        memoryUsage("After Selection Sort LL", memAfter);
        sortedSelectionLL.memoryKBUsed = memAfter - memBefore;
        SelectionResultLL.avgTime += sortedSelectionLL.timeMicroseconds;
        SelectionResultLL.avgMemory += sortedSelectionLL.memoryKBUsed;

    // Merge Sort - Linked List
        memoryUsage("Before Merge Sort LL", memBefore);
        LinkedList MergeClone = cloneList(original);
        SortResult sortedMergeLL = mergeSortLinked(MergeClone, "Date");
        memoryUsage("After Merge Sort LL", memAfter);
        sortedMergeLL.memoryKBUsed = memAfter - memBefore;
        MergeResultLL.avgTime += sortedMergeLL.timeMicroseconds;
        MergeResultLL.avgMemory += sortedMergeLL.memoryKBUsed;
}

// Display Results
    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Average Bubble Sort Time: " << BubbleResultLL.avgTime / 10000 << " mili seconds" << endl;
    cout << "Average Insertion Sort Time: " << InsertionResultLL.avgTime / 10000 << " mili seconds" << endl;
    cout << "Average Selection Sort Time: " << SelectionResultLL.avgTime / 10000 << " mili seconds" << endl;
    cout << "Average Merge Sort Time: " << MergeResultLL.avgTime / 10000 << " mili seconds" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Average Bubble Sort Memory Usage: " << BubbleResultLL.avgMemory / 10 << " KB" << endl;
    cout << "Average Insertion Sort Memory Usage: " << InsertionResultLL.avgMemory / 10 << " KB" << endl;
    cout << "Average Selection Sort Memory Usage: " << SelectionResultLL.avgMemory / 10 << " KB" << endl;
    cout << "Average Merge Sort Memory Usage: " << MergeResultLL.avgMemory / 10 << " KB" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of transactions in cleaned_transactions.csv: " << original.y << endl;
    cout << "-----------------------------------------" << endl;
    LinkedList list;
    if (!list.loadFromCSV("cleaned_transactions.csv")) 
    {
        return;
    }
    cout << "------Proof of Sort by Printing First 10 Rows-------" << endl;
    bubbleSort(list, "Date");
    list.printForward(10);
    
}


void questionTwo() {
    // Array
    dataContainer2D ct_data = getData("cleaned_transactions.csv");
    dataContainer2D mergeData = cloneContainer2D(ct_data);
    mergeSortArray(mergeData, 2, true);

    SearchResult FirstLinerSearchResult, SecondLinearSearchResult, FirstBinarySearchResult, SecondBinarySearchResult;
    avgSearchResult arrayLinear = {}, arrayBinary = {};

    long long memBefore, memAfter;

    for (int i = 0; i < 10; i++) {
        // Linear Search Array
        memoryUsage("Before Array Linear Search 1", memBefore);
        dataContainer2D FirstLinerSearch = Array_LinearSearch(ct_data, 2, "Electronics", FirstLinerSearchResult);
        memoryUsage("After Array Linear Search 1", memAfter);
        FirstLinerSearchResult.memoryUsed = memAfter - memBefore;

        memoryUsage("Before Array Linear Search 2", memBefore);
        dataContainer2D SecondLinearSearch = Array_LinearSearch(FirstLinerSearch, 5, "Credit Card", SecondLinearSearchResult);
        memoryUsage("After Array Linear Search 2", memAfter);
        SecondLinearSearchResult.memoryUsed = memAfter - memBefore;

        arrayLinear.avgTime += FirstLinerSearchResult.timeMicroseconds + SecondLinearSearchResult.timeMicroseconds;
        arrayLinear.avgMemory += FirstLinerSearchResult.memoryUsed + SecondLinearSearchResult.memoryUsed;
        arrayLinear.avgResultCount += SecondLinearSearchResult.resultCount;
        arrayLinear.percentage = (SecondLinearSearch.y / (float)FirstLinerSearch.y) * 100;

        // Binary Search Array
        memoryUsage("Before Array Binary Search 1", memBefore);
        dataContainer2D FirstBinarySearch = Array_BinarySearch(mergeData, 2, "Electronics", FirstBinarySearchResult);
        memoryUsage("After Array Binary Search 1", memAfter);
        FirstBinarySearchResult.memoryUsed = memAfter - memBefore;

        mergeSortArray(FirstBinarySearch, 5, true);

        memoryUsage("Before Array Binary Search 2", memBefore);
        dataContainer2D SecondBinarySearch = Array_BinarySearch(FirstBinarySearch, 5, "Credit Card", SecondBinarySearchResult);
        memoryUsage("After Array Binary Search 2", memAfter);
        SecondBinarySearchResult.memoryUsed = memAfter - memBefore;

        arrayBinary.avgTime += FirstBinarySearchResult.timeMicroseconds + SecondBinarySearchResult.timeMicroseconds;
        arrayBinary.avgMemory += FirstBinarySearchResult.memoryUsed + SecondBinarySearchResult.memoryUsed;
        arrayBinary.avgResultCount += SecondBinarySearchResult.resultCount;
        arrayBinary.percentage = (SecondBinarySearch.y / (float)FirstBinarySearch.y) * 100;

        freeContainer(FirstLinerSearch);
        freeContainer(SecondLinearSearch);
        freeContainer(FirstBinarySearch);
        freeContainer(SecondBinarySearch);
    }

    // Results - Array
    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Total Transactions: " << ct_data.y << endl;
    cout << "Linear - Result Count: " << arrayLinear.avgResultCount / 10 << " | %: " << arrayLinear.percentage << endl;
    cout << "Binary - Result Count: " << arrayBinary.avgResultCount / 10 << " | %: " << arrayBinary.percentage << endl;
    cout << "Average Linear Time: " << arrayLinear.avgTime / 10 << " micro seconds | Memory: " << arrayLinear.avgMemory / 10 << " KB" << endl;
    cout << "Average Binary Time: " << arrayBinary.avgTime / 10 << " micro seconds | Memory: " << arrayBinary.avgMemory / 10 << " KB" << endl;
    cout << "-----------------------------------------" << endl;


    // Linked List
    LinkedList original;
    original.loadFromCSV("cleaned_transactions.csv");
    LinkedList mergeDataLL = cloneList(original);

    SearchResult FirstLinerSearchResultLL, SecondLinearSearchResultLL, FirstBinarySearchResultLL, SecondBinarySearchResultLL;
    avgSearchResult listLinear = {}, listBinary = {};

    for (int i = 0; i < 10; i++) {
        // Linear Search Linked List
        memoryUsage("Before LL Linear Search 1", memBefore);
        LinkedList FirstLinerSearchLL = LL_linearSearch(original, "Category", "Electronics", FirstLinerSearchResultLL);
        memoryUsage("After LL Linear Search 1", memAfter);
        FirstLinerSearchResultLL.memoryUsed = memAfter - memBefore;

        memoryUsage("Before LL Linear Search 2", memBefore);
        LinkedList SecondLinearSearchLL = LL_linearSearch(FirstLinerSearchLL, "Payment Method", "Credit Card", SecondLinearSearchResultLL);
        memoryUsage("After LL Linear Search 2", memAfter);
        SecondLinearSearchResultLL.memoryUsed = memAfter - memBefore;

        listLinear.avgTime += FirstLinerSearchResultLL.timeMicroseconds + SecondLinearSearchResultLL.timeMicroseconds;
        listLinear.avgMemory += FirstLinerSearchResultLL.memoryUsed + SecondLinearSearchResultLL.memoryUsed;
        listLinear.avgResultCount = SecondLinearSearchResultLL.resultCount;
        listLinear.percentage = (SecondLinearSearchLL.y / (float)FirstLinerSearchLL.y) * 100;

        // Binary Search Linked List
        LinkedList CloneLLBinary = cloneList(mergeDataLL);
        mergeSortLinked(CloneLLBinary, "Category");

        memoryUsage("Before LL Binary Search 1", memBefore);
        LinkedList FirstBinarySearchLL = LL_binarySearch(CloneLLBinary, "Category", "Electronics", FirstBinarySearchResultLL);
        memoryUsage("After LL Binary Search 1", memAfter);
        FirstBinarySearchResultLL.memoryUsed = memAfter - memBefore;

        mergeSortLinked(FirstBinarySearchLL, "Payment Method");

        memoryUsage("Before LL Binary Search 2", memBefore);
        LinkedList SecondBinarySearchLL = LL_binarySearch(FirstBinarySearchLL, "Payment Method", "Credit Card", SecondBinarySearchResultLL);
        memoryUsage("After LL Binary Search 2", memAfter);
        SecondBinarySearchResultLL.memoryUsed = memAfter - memBefore;

        //SecondBinarySearchLL.printForward(10);

        listBinary.avgTime += FirstBinarySearchResultLL.timeMicroseconds + SecondBinarySearchResultLL.timeMicroseconds;
        listBinary.avgMemory += FirstBinarySearchResultLL.memoryUsed + SecondBinarySearchResultLL.memoryUsed;
        listBinary.avgResultCount = SecondBinarySearchResultLL.resultCount;
        listBinary.percentage = (SecondBinarySearchLL.y / (float)FirstBinarySearchLL.y) * 100;
    }

    listBinary.avgResultCount /= 10;
    listLinear.avgResultCount /= 10;

    // Results - Linked List
    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Total Transactions: " << original.y << endl;
    cout << "Linear - Result Count: " << listLinear.avgResultCount << " | %: " << listLinear.percentage << endl;
    cout << "Binary - Result Count: " << listBinary.avgResultCount << " | %: " << listBinary.percentage << endl;
    cout << "Average Linear Time: " << listLinear.avgTime / 10 << " micro seconds | Memory: " << listLinear.avgMemory / 10 << " KB" << endl;
    cout << "Average Binary Time: " << listBinary.avgTime / 10 << " micro seconds | Memory: " << listBinary.avgMemory / 10 << " KB" << endl;
    cout << "-----------------------------------------" << endl;

    LinkedList list;
    if (!list.loadFromCSV("cleaned_transactions.csv")) 
    {
        return;
    }
   
    LinkedList onlyElectronics = list.linearKeepRows("Category", "Electronics");
    LinkedList onlyElectronicsNCreditCard = onlyElectronics.linearKeepRows("Payment Method", "Credit Card");
    cout << "--------Proof of Search by Printing First 10 Rows------------" << endl;
    onlyElectronicsNCreditCard.printForward(10);

}


void questionThree() {
    // ------------------- ARRAY SECTION -------------------
    dataContainer2D cr_data = getData("cleaned_reviews.csv");

    SearchResult OneStarResult;
    dataContainer2D OneStar = Array_LinearSearch(cr_data, 2, "1", OneStarResult); // Column 2 = Rating

    WordFrequency wf = getWordFrequencyArray(OneStar, 3); // Column 3 = Review Text

    long long memoryBeforeSortArray, memoryAfterSortArray;
    memoryUsage("Before Array Sort", memoryBeforeSortArray);
    WFsort(wf);
    memoryUsage("After Array Sort", memoryAfterSortArray);
    wf.memoryUsed = memoryAfterSortArray - memoryBeforeSortArray;

    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of reviews in cleaned_reviews.csv: " << cr_data.y << endl;
    cout << "Number of reviews with 1 star in cleaned_reviews.csv: " << OneStar.y << endl;
    cout << "Number of different words in cleaned_reviews.csv: " << wf.size << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Top 10 words:\n";
    for (int i = 0; i < 10 && i < wf.size; ++i) {
        cout << wf.words[i] << ": " << wf.counts[i] << endl;
    }

    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << wf.timeMicroseconds << " Micro Seconds\n";
    cout << "Estimated memory used for sorting: " << wf.memoryUsed << " KB\n";
    cout << "-----------------------------------------" << endl;

    freeContainer(OneStar);
    freeContainer(cr_data);
    freeFrequencyContainer(wf);

    // ---------------- LINKED LIST SECTION ----------------
    LinkedList original;
    original.loadFromCSV("cleaned_reviews.csv");

    LinkedList OneStarLL = original.linearKeepRows("Rating", "1");

    long long memoryBeforeSortLL, memoryAfterSortLL;
    memoryUsage("Before Linked List Sort", memoryBeforeSortLL);
    WordFrequency wfLL = countTopWordsFromLinkedList(OneStarLL, "Review Text", 10);
    memoryUsage("After Linked List Sort", memoryAfterSortLL);
    wfLL.memoryUsed = memoryAfterSortLL - memoryBeforeSortLL;

    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of reviews in cleaned_reviews.csv: " << original.y << endl;
    cout << "Number of reviews with 1 star in cleaned_reviews.csv: " << OneStarLL.y << endl;
    cout << "Number of different words in cleaned_reviews.csv: " << wfLL.size << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Top 10 words:\n";
    for (int i = 0; i < 10 && i < wfLL.size; ++i) {
        cout << wfLL.words[i] << ": " << wfLL.counts[i] << endl;
    }

    cout << "-----------------------------------------" << endl;
    cout << "Time taken: " << wfLL.timeMicroseconds << " Micro Seconds\n";
    cout << "Estimated memory used for sorting: " << wfLL.memoryUsed << " KB\n";
    cout << "-----------------------------------------" << endl;

    freeFrequencyContainer(wfLL);
}

void question_insert() 
{
    long long memBefore, memAfter;

    // ---------------------- ARRAY ----------------------
    dataContainer2D cr_data = getData("cleaned_reviews.csv");
    InsDelResult ArrayInsertResult;
    // -------------------- LINKED LIST ------------------
    LinkedList original;
    original.loadFromCSV("cleaned_reviews.csv");

    const char** newRecord = getValidatedNewRecord(original.fieldHead, original.x);
    //const char * newRecord[] = {"PROD441", "CUST0001", "1", "This is a new review."};
    int recordSize = 4;

    memoryUsage("Before Array Insert", memBefore);
    dataContainer2D UpdatedData = writeNewTopLines(cr_data, newRecord, recordSize, ArrayInsertResult);
    memoryUsage("After Array Insert", memAfter);
    ArrayInsertResult.memory = (memAfter - memBefore) * 1024; // in bytes

    
    InsDelResult LLInsertResult;

    memoryUsage("Before Linked List Insert", memBefore);
    original.insertNewTopRowArray(newRecord);
    memoryUsage("After Linked List Insert", memAfter);
    LLInsertResult.memory = (((memAfter - memBefore) * 1024)/10000000); // in mega bytes

    // Print the last 10 records to verify the insertion);
    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "New record added successfully.\n";
    cout << "Total records before insert: " << cr_data.y << "\n";
    cout << "Total records after insert: " << UpdatedData.y << "\n";
    cout << "Time taken: " << ArrayInsertResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << ArrayInsertResult.memory << " bytes\n";

    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "New record added successfully.\n";
    cout << "Total records before insert: " << original.y-1 << "\n";
    cout << "Total records after insert: " << original.y << "\n";
    cout << "Time taken: " << LLInsertResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << LLInsertResult.memory << " mega bytes\n";
    cout << "\n----Proof of Insert by Printing First 10 Rows (Array)-----\n" << endl;
    for (int i = 0; i < min(10, UpdatedData.y); ++i) {
        for (int j = 0; j < UpdatedData.x; ++j) {
            cout << UpdatedData.data[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n----Proof of Insert by Printing First 10 Rows (Linked List)-----\n" << endl;
    original.printForward(10);


    freeContainer(cr_data);
    freeContainer(UpdatedData);
}

void question_delete() {
    long long memBefore, memAfter;

    // ---------------------- ARRAY ----------------------
    dataContainer2D cr_data = getData("cleaned_reviews.csv");
    InsDelResult ArrayDeleteResult;

    memoryUsage("Before Array Delete", memBefore);
    dataContainer2D deletedData = deleteAllRecords(cr_data, "Customer ID", "CUST7329", ArrayDeleteResult);
    memoryUsage("After Array Delete", memAfter);
    ArrayDeleteResult.memory = (memAfter - memBefore) * 1024;

    // -------------------- LINKED LIST ------------------
    LinkedList originalList;
    originalList.loadFromCSV("cleaned_reviews.csv");
    InsDelResult LLDeleteResult;

    memoryUsage("Before Linked List Delete", memBefore);
    originalList.deleteRows("Customer ID", "CUST7329");
    memoryUsage("After Linked List Delete", memAfter);
    LLDeleteResult.memory = (memAfter - memBefore) * 1024;

    originalList.printForward(10);
    cout << "-----------------------------------------" << endl;
    cout << "                  Array                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "All records with Customer ID = CUST7329 deleted successfully.\n";
    cout << "Total records before delete: " << cr_data.y << "\n";
    cout << "Total records after delete: " << deletedData.y << "\n";
    cout << "Time taken: " << ArrayDeleteResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << ArrayDeleteResult.memory << " bytes\n";

    cout << "-----------------------------------------" << endl;
    cout << "               Linked List               " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "All records with Customer ID = CUST7329 deleted successfully.\n";
    cout << "Total records before delete: " << originalList.y+1 << "\n";
    cout << "Total records after delete: " << originalList.y << "\n";
    cout << "Time taken: " << LLDeleteResult.time << " Micro Seconds\n";
    cout << "Estimated memory: " << LLDeleteResult.memory << " bytes\n";

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