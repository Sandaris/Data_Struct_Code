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

///////////////////////////////////// Array Data Structure /////////////////////////////////////
struct dataContainer2D {
    int error = 0;
    char** fields = nullptr;     // 1D array: fields
    char*** data = nullptr;      // 2D array: data (rows and columns)
    int x = 0;                   // number of columns
    int y = 0;                   // number of rows
};

struct dataContainer1D {
    int error = 0;
    char** fields = nullptr; // 1D array: fields
    char** data = nullptr;   // 1D array: data
    int x = 0;
};


///////////////////////////////////// Reading CSV /////////////////////////////////////////////
char** splitCustom(const std::string& line, int expectedFields, int& count) {
    char** tokens = new char*[expectedFields];
    for (int i = 0; i < expectedFields; ++i) tokens[i] = nullptr;

    std::string current;
    bool insideQuotes = false;
    int tokenIndex = 0;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes) {
            tokens[tokenIndex++] = strdup(current.c_str());
            current.clear();
            if (tokenIndex >= expectedFields - 1) {
                // After expected fields-1 commas, put the rest into last field
                tokens[tokenIndex] = strdup(line.substr(i + 1).c_str());
                count = expectedFields;
                return tokens;
            }
        } else {
            current += c;
        }
    }

    tokens[tokenIndex] = strdup(current.c_str());
    count = tokenIndex + 1;
    return tokens;
}

dataContainer2D getData(const std::string& filename) {
    
    dataContainer2D container;
    fs::path filePath = fs::current_path().parent_path() / "data" / filename;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        container.error = 1;
        return container;
    }

    std::string buffer;
    bool firstLine = true;
    int rowCount = 0;
    int rowCapacity = 10;
    int expectedFields = 0;

    container.data = new char**[rowCapacity];

    while (std::getline(file, buffer)) {
        if (buffer.empty()) continue;

        int count = 0;

        if (firstLine) {
            // Count how many commas in header
            expectedFields = 1; // at least one field
            for (char c : buffer) {
                if (c == ',') expectedFields++;
            }
            container.fields = splitCustom(buffer, expectedFields, count);
            container.x = count;
            firstLine = false;
        } else {
            if (rowCount >= rowCapacity) {
                int newCapacity = rowCapacity * 2;
                char*** newData = new char**[newCapacity];
                for (int i = 0; i < rowCount; ++i) {
                    newData[i] = container.data[i];
                }
                delete[] container.data;
                container.data = newData;
                rowCapacity = newCapacity;
            }
            container.data[rowCount++] = splitCustom(buffer, expectedFields, count);
        }
    }

    container.y = rowCount;
    file.close();

    return container;
}

///////////////////////////////////// Free Memory /////////////////////////////////////////////
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


