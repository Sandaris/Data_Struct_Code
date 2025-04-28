#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "common_function.hpp"

namespace fs = std::filesystem;
using fs::path;

using namespace std;

//test
int main() 
{
    string fileContent = readCSVFile("cleaned_reviews.csv");

    stringstream inputFile(fileContent);

    string line;
    while (getline(inputFile, line)) 
    {
        cout << line << endl;
    }
    
    string cleaned_review = "filepath";
    dataContainer2D cleaned_review_data = getData(cleaned_review);

    cout << cleaned_review_data.error << endl;
    cout << "Fields: ";    
    cout << cleaned_review_data.fields[1] << endl;
    cout << cleaned_review_data.data[0][0] << endl;
    cout << cleaned_review_data.x << endl;
    cout << cleaned_review_data.y << endl;

    freeContainer(cleaned_review_data);

    return 0;
}