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
    
    // cout << "----------------------------------------" << endl;

    // dataContainer2D cleaned_review_data = getData("cleaned_transactions.csv");

    // cout << "----------------------------------------" << endl;

    // dataContainer2D bubble_Container = cloneContainer(cleaned_review_data);

    // cout << "----------------------------------------" << endl;

    // SortResult bubble_result = insertionSortArray(bubble_Container, 4, true);

    // cout << "----------------------------------------" << endl;

    // std::cout << "Bubble Sort took " << bubble_result.timeMicroseconds << " μs"<< " and made " << bubble_result.swapCount << " swaps.\n";

    // cout << "----------------------------------------" << endl;\
    
    // cout << cleaned_review_data.y << " rows read\n";

    // cout << "----------------------------------------" << endl;

    // cout << "RAM used: " << bubble_result.memoryKBUsed << " KB\n";
    
    // cout << "----------------------------------------" << endl;

    // cout << "Manual estimated memory: " << bubble_result.manualEstimatedBytes << " bytes\n";

    // cout << "----------------------------------------" << endl;

//     SearchResult meta;
// dataContainer2D filtered = linearSearchTwoFields(cleaned_review_data, 2, "Books", 5, "Debit Card", meta);

// int i, j = 0;

// for (i = 0; i < filtered.y; i++) {
//     for (j = 0; j < filtered.x; j++) {
//         cout << filtered.data[i][j] << " ";
//     }
//     cout << endl;
// }

// std::cout << "Found " << meta.resultCount << " rows in " << meta.timeMicroseconds << " μs "
//           << "with " << meta.comparisons << " comparisons.\n";

//     freeContainer(cleaned_review_data);

//     return 0;

        // ───── Dummy values for testing ─────
        avgSortResult bubbleRes    { 250, 1024 };
        avgSortResult selectionRes { 220,  980 };
        avgSortResult insertionRes { 200,  950 };
        avgSortResult mergeRes     {  50, 2048 };
    
        avgSortResult linearRes    {  30,  512 };
        avgSortResult binaryRes    {   5,  256 };
    
        avgSortResult deleteRes    {  10,  128 };
        avgSortResult insertRes    {  15,  128 };
        // ───────────────────────────────────
    
        printAll(
            bubbleRes, selectionRes, insertionRes, mergeRes,
            linearRes, binaryRes,   deleteRes,    insertRes
        );
        return 0;

}