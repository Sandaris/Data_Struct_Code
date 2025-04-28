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
    string fileContent = readCSVFile("cleaned_reviews.csv");

    stringstream inputFile(fileContent);

    string line;
    while (getline(inputFile, line)) 
    {
        cout << line << endl;
    }

    return 0;
}