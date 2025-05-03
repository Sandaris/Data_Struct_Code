#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <limits>
#include <iostream>
#include <iomanip>
#ifndef COMMON_FUNCTION_HPP
#define COMMON_FUNCTION_HPP

#include <chrono>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")


//#include "linkedList.hpp"


namespace fs = std::filesystem;
using fs::path;

using namespace std;

///////////////////////////////////// Array Data Structure /////////////////////////////////////
struct dataContainer2D {
    int error = 0;
    char** fields = nullptr;     // 1D array: fields
    char*** data = nullptr;      // 2D array: data (rows and columns)
    int x = 0;                   // number of columns
    int y = 0;                   // number of rows
};

///////////////////////////////////// SearchResult Struct /////////////////////////////////////
struct SearchResult {
    long long timeMicroseconds = 0;
    int resultCount = 0;
    size_t memoryUsed = 0;
};
// 1-field linear search
// Matches fieldX == valueX
// Returns a new container with matching rows

///////////////////////////////////// SortResult Struct /////////////////////////////////////
struct SortResult {
    long long timeMicroseconds;
    size_t memoryKBUsed = 0;
};

///////////////////////////////////// frequency Struct /////////////////////////////////////
struct WordFrequency {
    char** words;                  // array of unique words
    int* counts;                   // corresponding frequency for each word
    int size;                      // .x of data container
    int capacity;                  // current max capacity before resizing
    long long timeMicroseconds;   // time taken to compute
    size_t memoryUsed;  // manually tracked memory usage
};

///////////////////////////////////// avgSortResult Struct /////////////////////////////////////
struct avgSortResult
{
    int avgTime = 0;
    int avgMemory = 0;
};

/////////////////////////////CLEAR THE SCREEN ///////////////////////////////////////////////////////////////
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

///////////////////////////////////// Free Array Container Memory /////////////////////////////////////////////
void freeContainer(dataContainer2D& container) {
    if (container.fields != nullptr) {
        for (int i = 0; i < container.x; ++i) {
            if (container.fields[i] != nullptr) {
                free(container.fields[i]);
            }
        }
        delete[] container.fields;
        container.fields = nullptr;
    }

    if (container.data != nullptr) {
        for (int i = 0; i < container.y; ++i) {
            if (container.data[i] != nullptr) {
                for (int j = 0; j < container.x; ++j) {
                    if (container.data[i][j] != nullptr) {
                        free(container.data[i][j]);
                    }
                }
                delete[] container.data[i];
            }
        }
        delete[] container.data;
        container.data = nullptr;
    }

    container.x = 0;
    container.y = 0;
}
/**
 * string filename = "filepath";
 * 
 * dataContainer2D container = getData(filename);
 * container.error = 0; // 0: no error, 1: error
 * container.x = number of columns
 * container.y = number of rows
 * container.fields[n] = array of column names
 * container.data[n][n] = array of rows
 * 
 * 
 * freeContainer(container);
 */

 ///////////////////////////////////// Free Frequency Container Memory /////////////////////////////////////////////
 void freeFrequencyContainer(WordFrequency& wf) {
    for (int i = 0; i < wf.size; ++i) {
        free(wf.words[i]); // free each word string
    }
    delete[] wf.words;
    delete[] wf.counts;
    wf.words = nullptr;
    wf.counts = nullptr;
    wf.size = 0;
    wf.capacity = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void print_helo()
{
    std::cout << "hello world\n";
}

void printAll(
    const avgSortResult &bubbleRes,
    const avgSortResult &selectionRes,
    const avgSortResult &insertionRes,
    const avgSortResult &mergeRes,
    const avgSortResult &linearRes,
    const avgSortResult &binaryRes,
    const avgSortResult &deleteRes,
    const avgSortResult &insertRes
) {
    // 1) human‐readable labels
    const char* names[] = {
        "Bubble Sort", "Selection Sort", "Insertion Sort", "Merge Sort",
        "Linear Search","Binary Search",  "Delete",          "Insert"
    };

    // 2) parallel results array
    const avgSortResult results[] = {
        bubbleRes,    selectionRes, insertionRes, mergeRes,
        linearRes,    binaryRes,    deleteRes,    insertRes
    };

    constexpr int COL1 = 18;
    constexpr int COL2 = 12;
    // total width = sum of column-widths + one '|' per column boundary (4 of them)
    const int TOTAL_W = COL1 + COL2 + COL2 + 4;

    // one-time left-align
    std::cout << std::left;

    // Header
    std::cout << "| " << std::string(TOTAL_W, '-') << "|\n";
    std::cout
      << "| " << std::setw(COL1) << "Algorithm"
      << "| " << std::setw(COL2) << "Time (ms)"
      << "| " << std::setw(COL2) << "Memory (KB)"
      << "|\n"
      << "| " << std::string(TOTAL_W, '-') << "|\n";

    // 4 sorts
    for (int i = 0; i < 4; ++i) {
        std::cout
          << "| " << std::setw(COL1) << names[i]
          << "| " << std::setw(COL2) << results[i].avgTime
          << "| " << std::setw(COL2) << results[i].avgMemory
          << "|\n";
    }
    std::cout << "| " << std::string(TOTAL_W, '-') << "|\n";

    // 2 searches
    for (int i = 4; i < 6; ++i) {
        std::cout
          << "| " << std::setw(COL1) << names[i]
          << "| " << std::setw(COL2) << results[i].avgTime
          << "| " << std::setw(COL2) << results[i].avgMemory
          << "|\n";
    }
    std::cout << "| " << std::string(TOTAL_W, '-') << "|\n";

    // delete & insert
    for (int i = 6; i < 8; ++i) {
        std::cout
          << "| " << std::setw(COL1) << names[i]
          << "| " << std::setw(COL2) << results[i].avgTime
          << "| " << std::setw(COL2) << results[i].avgMemory
          << "|\n";
    }
    std::cout << "| " << std::string(TOTAL_W, '-') << "|\n";
}


/**
 * \brief Read a CSV file and return its content as a string
 * 
 * juz call the function in this way: string fileContent = readCSVFile("cleaned_reviews.csv");
 * 
 * \param filename The name of the CSV file to read example : ("cleaned_reviews.csv") 
 * \return The content of the CSV file as a string
 * \throws runtime_error If the file cannot be opened
 */
string readCSVFile(const string& filename) 
{
    fs::path filePath = fs::current_path().parent_path() / "data" / filename;

    ifstream inputFile(filePath);

    if (!inputFile.is_open()) 
    {
        cerr << "Error: Could not open the file " << filePath << endl;
        return "";
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();

    return buffer.str();
}

///////////////////////////////////// Windows Memory Tracker /////////////////////////////////////
size_t getUsedMemoryBytes() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize;
}

size_t getUsedMemoryKB() { 
    return getUsedMemoryBytes() / 1024; 
}

///////////////////////////////////// Timer Function /////////////////////////////////////
struct Timer {
    std::chrono::steady_clock::time_point start, end;

    void begin() { start = std::chrono::steady_clock::now(); }
    void finish() { end = std::chrono::steady_clock::now(); }

    long long getDurationMicroseconds() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
};

std::string center(const std::string &s, int width) {
    if ((int)s.size() >= width) return s.substr(0, width);
    int pad = width - s.size();
    int left = pad/2;
    int right = pad - left;
    return std::string(left, ' ') + s + std::string(right, ' ');
}

void Question1()
{
     // your sample data
     avgSortResult bubbleResLL       {220,  980};
     avgSortResult bubbleResArray    {220,  980};
     avgSortResult selectionResLL    {220,  980};
     avgSortResult selectionResArray {220,  980};
     avgSortResult insertionResLL    {200,  950};
     avgSortResult insertionResArray {200,  950};
     avgSortResult mergeResLL        { 50, 2048};
     avgSortResult mergeResArray     { 50, 2048};
 
     // shorten to four algorithm names
     const char* names[4] = {
         "Bubble Sort",
         "Selection Sort",
         "Insertion Sort",
         "Merge Sort"
     };
 
     // split LL vs Array for easy looping
     avgSortResult resultsLL[4]    = { bubbleResLL,    selectionResLL,
                                       insertionResLL, mergeResLL };
     avgSortResult resultsArr[4]   = { bubbleResArray, selectionResArray,
                                       insertionResArray, mergeResArray };
 
     // column widths
     constexpr int COL1 = 15;  // Algorithm
     constexpr int COL2 =  7;  // each numeric column
     // total inner width = COL1 + 4 cols of COL2 + 5 separators ('|')
     int innerW = COL1 + COL2*4 + 5;
     std::string sep(innerW, '-');
 
     // 1) top border
     std::cout << sep << "\n";
 
     // 2) centered title
     std::cout << center("Average of 10 Simulations", innerW) << "\n";
 
     // 3) underline
     std::cout << sep << "\n";
 
     // 4) two-group header: blank, [ LL ], [ Array ]
     int grpW = COL2*2 + 1;  // spans two sub-columns plus the inner '|'
     std::cout 
         << std::setw(COL1) << "" 
         << "|" << center("LL",    grpW)
         << "|" << center("Array", grpW)
         << "|\n";
    
    std::cout << std::setw(COL1);
 
     // 5) sub-headers
     std::cout
         << std::setw(COL1) << std::left   << "Algorithm"
         << "|" << std::setw(COL2) << std::right << "Time"
         << "|" << std::setw(COL2)              << "Memory"
         << "|" << std::setw(COL2)              << "Time"
         << "|" << std::setw(COL2)              << "Memory"
         << "|\n";
 
     // 6) separator
     std::cout << sep << "\n";
 
     // 7) data rows
     for (int i = 0; i < 4; ++i) {
         std::cout
             << std::setw(COL1) << std::left   << names[i]
             << "|" << std::setw(COL2) << std::right << resultsLL[i].avgTime
             << "|" << std::setw(COL2)              << resultsLL[i].avgMemory
             << "|" << std::setw(COL2)              << resultsArr[i].avgTime
             << "|" << std::setw(COL2)              << resultsArr[i].avgMemory
             << "|\n";
     }
 
     // 8) bottom border
     std::cout << sep << "\n";
}

void Question2()
{
    avgSortResult linearResLL    {  30,  512 };
    avgSortResult linearResArray    {  30,  512 };
    avgSortResult binaryResLL    {   5,  256 };
    avgSortResult binaryResArray    {  30,  512 };

    // shorten to four algorithm names
    const char* names[2] = {
        "Linear Search",
        "Binary Search",
    };

    // split LL vs Array for easy looping
    avgSortResult resultsLL[2]    = { linearResLL,  binaryResLL};
    avgSortResult resultsArr[2]   = { linearResArray, binaryResArray };

    // column widths
    constexpr int COL1 = 15;  // Algorithm
    constexpr int COL2 =  7;  // each numeric column
    // total inner width = COL1 + 4 cols of COL2 + 5 separators ('|')
    int innerW = COL1 + COL2*4 + 5;
    std::string sep(innerW, '-');

    // 1) top border
    std::cout << sep << "\n";

    // 2) centered title
    std::cout << center("Average of 10 Simulations", innerW) << "\n";

    // 3) underline
    std::cout << sep << "\n";

    // 4) two-group header: blank, [ LL ], [ Array ]
    int grpW = COL2*2 + 1;  // spans two sub-columns plus the inner '|'
    std::cout 
        << std::setw(COL1) << "" 
        << "|" << center("LL",    grpW)
        << "|" << center("Array", grpW)
        << "|\n";
   
   std::cout << std::setw(COL1);

    // 5) sub-headers
    std::cout
        << std::setw(COL1) << std::left   << "Algorithm"
        << "|" << std::setw(COL2) << std::right << "Time"
        << "|" << std::setw(COL2)              << "Memory"
        << "|" << std::setw(COL2)              << "Time"
        << "|" << std::setw(COL2)              << "Memory"
        << "|\n";

    // 6) separator
    std::cout << sep << "\n";

    // 7) data rows
    for (int i = 0; i < 2; ++i) {
        std::cout
            << std::setw(COL1) << std::left   << names[i]
            << "|" << std::setw(COL2) << std::right << resultsLL[i].avgTime
            << "|" << std::setw(COL2)              << resultsLL[i].avgMemory
            << "|" << std::setw(COL2)              << resultsArr[i].avgTime
            << "|" << std::setw(COL2)              << resultsArr[i].avgMemory
            << "|\n";
    }

    // 8) bottom border
    std::cout << sep << "\n";
}
void Question3()
{
   //2 avgResult
}

void Question_Insert()
{
   //8 sort result
}

void Question_Delete()
{
   //8 sort result
}

#endif
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
                Question1();
                break;
            case 2:
                cout << "Calculating percentage of Electronics that paid using Credit Card...\n\n";
                Question2();
                break;
            case 3:
                cout << "Finding most frequent word in 1 Star Rating...\n\n";
                Question3();
                break;
            case 4:
                cout << "Inserting new row...\n\n";
                // your insertRowFunction();
                break;
            case 5:
                cout << "Deleting a row...\n\n";
                // your deleteRowFunction();
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            again = tolower(again);
        } while (again != 'y' && again != 'n');

        if (again == 'n') {
            cout << "Goodbye!\n";
            break;
        }
        // else continue; next iteration will clearScreen() and redraw menu
    }
}
