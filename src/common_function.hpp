#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <limits>
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
    long long timeMicroseconds = 0;
    long long memoryKBUsed = 0;
};

///////////////////////////////////// frequency Struct /////////////////////////////////////
struct WordFrequency {
    char** words;                  // array of unique words
    int* counts;                   // corresponding frequency for each word
    int size;                      // current number of unique words stored
    int capacity;                  // current max capacity before resizing
    long long timeMicroseconds;   // time taken to compute
    size_t memoryUsed;  // manually tracked memory usage
};

///////////////////////////////////// avgSortResult Struct /////////////////////////////////////
struct avgSortResult
{
    long long avgTime = 0;
    long long avgMemory = 0;
};

///////////////////////////////////// avgSearchResult Struct /////////////////////////////////////
struct avgSearchResult {
    long long avgTime = 0;               // time taken
    long long avgMemory = 0;             // memory used                 
    int avgResultCount = 0;              // number of row found
    float percentage = 0.0f;             // percentage    
};

///////////////////////////////////// Insert & Delete Struct /////////////////////////////////////
struct InsDelResult {
    long long time = 0;
    size_t memory = 0;
};
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

size_t GetCurrentMemory() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.PrivateUsage;  // More stable memory usage measure in bytes
    }
    return 0;
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

void menu()
{
    int choice;
    do {
        // Clear the terminal
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << "1. Sort Customer by Date\n"
             << "2. Percentage of Electronics that paid using Credit Card\n"
             << "3. Most Frequent Word in 1 Star Rating\n"
             << "4. Insert new row\n"
             << "5. Delete a row\n"
             << "6. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 6) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            system("cls");
            cout << "Invalid choice. Please enter a number between 1 and 6. Press Enter to Continue...\n";
            cin.get(); // wait for user to press Enter before clearing the screen
        } else {
            break; // valid input
        }
    } while (true);

    // Perform action based on choice
    if (choice == 1) 
    {
        // Call sorting function here
        cout << "Sorting Customer by Date...\n";
    }
    else if (choice == 2) 
    {
        // Call percentage calculation function here
        cout << "Calculating percentage of Electronics that paid using Credit Card...\n";
    } 
    else if (choice == 3) 
    {
        // Call most frequent word function here
        cout << "Finding most frequent word in 1 Star Rating...\n";
    } 
    else if (choice == 4) 
    {
        // Call insert new row function here
        cout << "Inserting new row...\n";
    } 
    else if (choice == 5) 
    {
        // Call delete a row function here
        cout << "Deleting a row...\n";
    } 
    else if (choice == 6) 
    {
        cout << "Exiting...\n";
        return;
    }
}


#endif