#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstring>

namespace fs = std::filesystem;
using fs::path;

using namespace std;

void print_helo()
{
    std::cout << "hello world\n";
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

char** splitCustom(const string& line, int& count) {
    char** tokens = new char*[4];
    for (int i = 0; i < 4; ++i) tokens[i] = nullptr;

    string current;
    bool insideQuotes = false;
    int commaCount = 0;
    int tokenIndex = 0;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes && commaCount < 3) {
            tokens[tokenIndex++] = strdup(current.c_str());
            current.clear();
            commaCount++;
        } else {
            current += c;
        }
    }

    tokens[tokenIndex] = strdup(current.c_str());
    count = tokenIndex + 1;
    return tokens;
}

dataContainer2D getData(const string& filename) {
    
    dataContainer2D container;

    fs::path filePath = fs::current_path().parent_path() / "data" / filename;

    ifstream file(filePath);

    if (!file.is_open()) {
        container.error = 1;
        return container;
    }

    string buffer;
    bool firstLine = true;
    int rowCount = 0;
    int rowCapacity = 10; // start small, grow dynamically

    container.data = new char**[rowCapacity];

    while (getline(file, buffer)) {
        if (buffer.empty()) continue;

        int count = 0;
        char** row = splitCustom(buffer, count);

        if (firstLine) {
            container.fields = row;
            container.x = count;
            firstLine = false;
        } else {
            if (rowCount >= rowCapacity) {
                // Expand memory
                int newCapacity = rowCapacity * 2;
                char*** newData = new char**[newCapacity];
                for (int i = 0; i < rowCount; ++i) {
                    newData[i] = container.data[i];
                }
                delete[] container.data;
                container.data = newData;
                rowCapacity = newCapacity;
            }
            container.data[rowCount++] = row;
        }
    }

    container.y = rowCount;
    file.close();

    return container;
}

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