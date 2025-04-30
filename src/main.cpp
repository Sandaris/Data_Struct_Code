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

//testinggggggggggggg

int main() 
{
    // string fileContent = readCSVFile("cleaned_reviews.csv");

    // stringstream inputFile(fileContent);

    // string line;
    // while (getline(inputFile, line)) 
    // {
    //     cout << line << endl;
    // }
    
    cout << "----------------------------------------" << endl;

    dataContainer2D cleaned_review_data = getData("cleaned_transactions.csv");

    cout << "----------------------------------------" << endl;

    dataContainer2D bubble_Container = cloneContainer(cleaned_review_data);

    cout << "----------------------------------------" << endl;
    
    SortResult bubble_result = bubbleSortArray(bubble_Container, 4, true);

    cout << "----------------------------------------" << endl;

    std::cout << "Bubble Sort took " << bubble_result.timeMicroseconds << " μs"<< " and made " << bubble_result.swapCount << " swaps.\n";

    cout << "----------------------------------------" << endl;\
    int i =0;
    while(i <cleaned_review_data.y){
        cout << bubble_Container.data[i][4] << endl;
        i++;
    }
    cout << cleaned_review_data.y << " rows read\n";
    cout << "----------------------------------------" << endl;
    cout << "----------------------------------------" << endl;


    freeContainer(bubble_Container);
    freeContainer(cleaned_review_data);

    return 0;
}