#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>

namespace fs = std::filesystem;
using fs::path;

using namespace std;

void print_helo()
{
    std::cout << "hello world\n";
}

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
