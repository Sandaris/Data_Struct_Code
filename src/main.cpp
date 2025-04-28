#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "common_function.hpp"

namespace fs = std::filesystem;
using fs::path;

using namespace std;

int main() 
{
    // string fileContent = readCSVFile("cleaned_reviews.csv");

    // stringstream inputFile(fileContent);

    // string line;
    // while (getline(inputFile, line)) 
    // {
    //     cout << line << endl;
    // }
    
    string filepath = "cleaned_transactions.csv";
    dataContainer2D cleaned_transactions_data = getData(filepath);

    cout << cleaned_transactions_data.error << endl;
    cout << "Fields: ";    
    cout << cleaned_transactions_data.fields[5] << endl;
    cout << cleaned_transactions_data.data[0][1] << endl;
    cout << cleaned_transactions_data.x << endl;
    cout << cleaned_transactions_data.y << endl;

    cout << "----------------------------------------" << endl;


    freeContainer(cleaned_transactions_data);

    return 0;
}