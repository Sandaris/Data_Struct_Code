#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "common_function.hpp"
#include "linkedList.hpp"
#include "Array.hpp"

void array_questionOne(){
    dataContainer2D ct_data = getData("cleaned_transactions.csv");
    dataContainer2D cr_data = getData("cleaned_reveiws.csv");

    dataContainer2D bubbleData = cloneContainer(ct_data);
    dataContainer2D insertionData = cloneContainer(ct_data);
    dataContainer2D selectionData = cloneContainer(ct_data);
    dataContainer2D mergeData = cloneContainer(ct_data);

    SortResult bubbleResult = bubbleSortArray(bubbleData, 4, true);
    SortResult insertionResult = insertionSortArray(insertionData, 4, true);
    SortResult selectionResult = selectionSortArray(selectionData, 4, true);
    SortResult mergeResult = mergeSortArray(mergeData, 4, true);
    
    cout << "-----------------------------------------" << endl;
    cout << "Bubble Sort Time: " << bubbleResult.timeMicroseconds << endl;
    cout << "Insertion Sort Time: " << insertionResult.timeMicroseconds << endl;
    cout << "Selection Sort Time: " << selectionResult.timeMicroseconds << endl;
    cout << "Merge Sort Time: " << mergeResult.timeMicroseconds << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Bubble Sort Comparisons: " << bubbleResult.memoryKBUsed << endl;
    cout << "Insertion Sort Comparisons: " << insertionResult.memoryKBUsed << endl;
    cout << "Selection Sort Comparisons: " << selectionResult.memoryKBUsed << endl;
    cout << "Merge Sort Comparisons: " << mergeResult.memoryKBUsed << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of transactions in cleaned_transactions.csv: " << ct_data.y << endl;
    cout << "Number of reviews in cleaned_reveiws.csv: " << cr_data.y << endl;
    cout << "-----------------------------------------" << endl;

    freeContainer(bubbleData);
    freeContainer(insertionData);
    freeContainer(selectionData);
    freeContainer(mergeData);
    freeContainer(ct_data);
    freeContainer(cr_data);

}

void array_questionTwo(){
    dataContainer2D ct_data = getData("cleaned_transactions.csv");

    SearchResult FirstLinerSearchResult, SecondLinearSearchResult, FirstBinarySearchResult, SecondBinarySearchResult;
    
    dataContainer2D FirstLinerSearch = linearSearchOneField(ct_data, 2, "Electronics", FirstLinerSearchResult);
    dataContainer2D SecondLinearSearch = linearSearchOneField(FirstLinerSearch, 5, "Credit Card", SecondLinearSearchResult);

    bubbleSortArray(ct_data, 2, true);
    dataContainer2D FirstBinarySearch = binarySearchOneField(ct_data, 2, "Electronics", FirstBinarySearchResult);
    bubbleSortArray(FirstBinarySearch, 5, true);
    dataContainer2D SecondBinarySearch = binarySearchOneField(FirstBinarySearch, 5, "Credit Card", SecondBinarySearchResult);

    float LinearPercentage = (SecondLinearSearch.y / (float)FirstLinerSearch.y)*100;
    int totalLineartime = FirstLinerSearchResult.timeMicroseconds + SecondLinearSearchResult.timeMicroseconds;
    
    float BinaryPercentage = (SecondBinarySearch.y / (float)FirstBinarySearch.y)*100;
    int totalBinarytime = FirstBinarySearchResult.timeMicroseconds + SecondBinarySearchResult.timeMicroseconds;

    cout << "-----------------------------------------" << endl;
    cout << "Number of transactions in cleaned_transactions.csv: " << ct_data.y << endl;
    cout << "Number of transactions with Electronics and Credit Card in cleaned_transactions.csv: " << SecondLinearSearch.y << endl;
    cout << "Percentage of transactions with Electronics and Credit Card in cleaned_transactions.csv: " << LinearPercentage << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Number of transactions in cleaned_transactions.csv: " << ct_data.y << endl;
    cout << "Number of transactions with Electronics and Credit Card in cleaned_transactions.csv: " << SecondBinarySearch.y << endl;
    cout << "Percentage of transactions with Electronics and Credit Card in cleaned_transactions.csv: " << BinaryPercentage << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Linear Search Time: " << totalLineartime << endl;
    cout << "Binary Search Time: " << totalBinarytime << endl;
    cout << "-----------------------------------------" << endl;

    freeContainer(FirstLinerSearch);
    freeContainer(SecondLinearSearch);
    freeContainer(FirstBinarySearch);
    freeContainer(SecondBinarySearch);
    freeContainer(ct_data);
}

void array_questionThree(){
    dataContainer2D cr_data = getData("cleaned_reviews.csv");

    SearchResult OneStarResult;
    dataContainer2D OneStar = linearSearchOneField(cr_data, 2, "1", OneStarResult);

    WordFrequency wf = countWordsFromField(OneStar, 3);

    cout << "-----------------------------------------" << endl;
    cout << "Number of reviews in cleaned_reviews.csv: " << cr_data.y << endl;
    cout << "Number of reviews with 1 star in cleaned_reviews.csv: " << OneStar.y << endl;
    cout << "Number of words in cleaned_reviews.csv: " << wf.size << endl;
    cout << "-----------------------------------------" << endl;

    for (int i = 0; i < wf.size; ++i) {
        cout << wf.words[i] << ": " << wf.counts[i] << endl;
    }

    freeContainer(OneStar);
    freeContainer(cr_data);
    freeFrequencyContainer(wf);
}

int main(){
    int i = 0;
    cout << "Enter question number: " << endl;
    cin >> i;
    switch(i){
        case 1:
            array_questionOne();
            break;
        case 2:
            array_questionTwo();
            break;
        case 3:
            array_questionThree();
            break;
        default:
            cout << "Invalid input" << endl;
            break;
    }
    return 0;
}

