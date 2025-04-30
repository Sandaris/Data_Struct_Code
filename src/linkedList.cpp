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

    bubbleSort(list, "Product ID");

    //list.printForward();


    mergeSortList(list, "Product ID");

    insertionSort(list, "Product ID");

    selectionSort(list, "Product ID");
    
    //list.printForward();

}
