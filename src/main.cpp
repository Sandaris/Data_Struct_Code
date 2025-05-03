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


}















int main() 
{
    
    questionOne();
        

}