#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstring>

#include "linkedList.hpp"


namespace fs = std::filesystem;
using fs::path;

using namespace std;

void print_helo()
{
    std::cout << "hello world\n";
}

void display_result(int val1, int val2, int val3)
{
    //code starts here
//     Time ( average of 10 simulation)	
//     linked	array
// Sort	bubble	value1	value2
// merge	value3	value4
// search	linear	value5	value6
// binary	value7	value8

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


