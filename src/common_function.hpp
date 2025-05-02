#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstring>

//#include "linkedList.hpp"


namespace fs = std::filesystem;
using fs::path;

using namespace std;

struct avgSortResult
{
    int avgTime = 0;
    int avgMemory = 0;
};

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
