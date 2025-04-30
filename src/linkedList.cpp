#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "common_function.hpp"
#include "linkedList.hpp"

int main() 
{
    LinkedList list;
    if (!list.loadFromCSV("cleaned_reviews.csv")) 
    {
        return 1;
    }

    int val1 = bubbleSort(list, "Product ID");

    int val2 = mergeSortList(list, "Product ID");

    int val3 = insertionSort(list, "Product ID");

    int val4 = selectionSort(list, "Product ID");
    
    //list.printForward();

    cout << "Bubble Sort: " << val1 << "\n";
    cout << "Merge Sort: " << val2 << "\n";
    cout << "Insertion Sort: " << val3 << "\n";
    cout << "Selection Sort: " << val4 << "\n";
    cout << "Sorting completed.\n";

}
