#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
//#include "common_function.hpp"
#include "linkedList.hpp"

int main() 
{

    ////////////////////////////  Q1   //////////////////////////////////////////////

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


    ////////////////////////////  Q2   //////////////////////////////////////////////


    // LinkedList list;
    // if (!list.loadFromCSV("cleaned_transactions.csv")) 
    // {
    //     return 1;
    // }

    // LinkedList onlyElectronics = list.linearKeepRows("Category", "Electronics");

    // LinkedList onlyElectronicsNCreditCard = onlyElectronics.linearKeepRows("Payment Method", "Credit Card");

    // onlyElectronicsNCreditCard.printForward();

    // cout << "Number of rows for creditcard in electronic: " << onlyElectronicsNCreditCard.y << "\n";
    // cout << "Number of rows for electronics category : " << onlyElectronics.y << "\n";

    // cout << "Percentage of Credit Card in Electronics: " 
    //      << (static_cast<double>(onlyElectronicsNCreditCard.y) / onlyElectronics.y) * 100 << "%\n";

    ////////////////////////////  Q3   //////////////////////////////////////////////

    // LinkedList list;
    // if (!list.loadFromCSV("cleaned_reviews.csv")) 
    // {
    //     return 1;
    // }

    // LinkedList oneStar = list.linearKeepRows("Rating","1");
    // dataContainer2D dc = getTopFrequentWordsDC(oneStar, "Review Text", 10);

    // printDataContainer(dc);

    // return 0;

}