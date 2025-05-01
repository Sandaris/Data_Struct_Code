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
    // LinkedList filtered_result = list.linearSearch1Field("Rating", "1");

    // insertionSort(filtered_result, "Product ID");

    // LinkedList topWords = getTopFrequentWords(filtered_result, "Review Text", 10);

    // topWords.printForward();

    // return 0;

    LinkedList oneStar = list.linearSearch1Field("Rating","1");
    dataContainer2D dc = getTopFrequentWordsDC(oneStar, "Review Text", 1);

    printDataContainer(dc);

    return 0;
}

    // int val1 = bubbleSort(list, "Product ID");

    // int val2 = mergeSortList(list, "Product ID");

    // int val3 = insertionSort(list, "Product ID");

    // int val4 = selectionSort(list, "Product ID");
    
    // //list.printForward();

    // cout << "Bubble Sort: " << val1 << "\n";
    // cout << "Merge Sort: " << val2 << "\n";
    // cout << "Insertion Sort: " << val3 << "\n";
    // cout << "Selection Sort: " << val4 << "\n";
    // cout << "Sorting completed.\n";

    // insertionSort(list, "Product ID");
    
    // linearSearch1Field(list, "Product ID", "PROD956");

    // //binarySearch(list, "Product ID", "PROD956");

    // list.deleteRows("Product ID", "PROD956");

    // //std::cout << "Time taken for deletion: " << ms << " ms\n";

    // linearSearch1Field(list, "Product ID", "PROD956");