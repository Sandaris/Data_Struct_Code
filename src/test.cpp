

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

int main() {

    // LinkedList list;
    // if (!list.loadFromCSV("cleaned_transactions.csv")) 
    // {
    //     return 1;
    // }
    // bubbleSort(list, "Date");
    // list.printForward(10);
    
    // LinkedList list;
    // LinkedList list2;

    // LinkedList list3;
    // LinkedList list4;
    // if (!list.loadFromCSV("cleaned_transactions.csv")) 
    // {
    //     return 1;
    // }
    // if (!list2.loadFromCSV("cleaned_transactions.csv")) 
    // {
    //     return 1;
    // }
    // if (!list3.loadFromCSV("cleaned_transactions.csv")) 
    // {
    //     return 1;
    // }
    // if (!list4.loadFromCSV("cleaned_transactions.csv")) 
    // {
    //     return 1;
    // }

    // int val1 = bubbleSort(list, "Date");

    // int val2 = mergeSortList(list2, "Date");

    // int val3 = insertionSort(list3, "Date");

    // int val4 = selectionSort(list4, "Date");
    
    // list.printForward();

    // cout << "Bubble Sort: " << val1 << "ms\n";
    // cout << "Merge Sort: " << val2 << "ms\n";
    // cout << "Insertion Sort: " << val3 << "ms\n";
    // cout << "Selection Sort: " << val4 << "ms\n";
    // cout << "Sorting completed.\n";

    // cout << "Total number of transactions: " << list.y << "\n";


    // //////////////////////////  Q2   //////////////////////////////////////////////

    

    // LinkedList list;
    // if (!list.loadFromCSV("cleaned_transactions.csv")) 
    // {
    //     return 1;
    // }
   
    // LinkedList onlyElectronics = list.linearKeepRows("Category", "Electronics");
    // LinkedList onlyElectronicsNCreditCard = onlyElectronics.linearKeepRows("Payment Method", "Credit Card");
    
    // onlyElectronicsNCreditCard.printForward(10);

    // //////////////////////////  Q3   //////////////////////////////////////////////

    // LinkedList list;
    // if (!list.loadFromCSV("cleaned_reviews.csv")) 
    // {
    //     return 1;
    // }

    // LinkedList oneStar = list.linearKeepRows("Rating","1");
    // dataContainer2D dc = getTopFrequentWordsDC(oneStar, "Review Text", 10);

    // printDataContainer(dc);

    // return 0;

    // //////////////////////////  Insert   //////////////////////////////////////////////

    LinkedList list;
    if (!list.loadFromCSV("cleaned_reviews.csv")) 
    {
        return 1;
    }
    list.keepFirstRows(10);

    //list.insertNewRowFromInput();
    const char * newRecord[] = {"PROD441", "CUST0001", "1", "This is a new review."};
    list.insertNewRowAtTop(newRecord);
    list.printForward(11);
    
}
