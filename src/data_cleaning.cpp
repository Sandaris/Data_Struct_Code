#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;
using fs::path;

using namespace std;


// Check a single cell (no allocations)
bool isBadCell(string_view cell) 
{
    return cell.empty() ||
           cell == "NaN" ||
           cell == "Invalid Date" ||
           cell == "Invalid Rating";
}

// Validate a whole CSV line by scanning for commas
bool isValidLine(const string& line) 
{
    size_t start = 0, len = line.size();
    while (start <= len) 
    {
        size_t comma = line.find(',', start);
        if (comma == string::npos) comma = len;
        if (isBadCell({ line.data() + start, comma - start }))
            return false;
        start = comma + 1;
    }
    // Reject if line ends with a comma → empty last cell
    return !(len > 0 && line.back() == ',');
}

int main(int argc, char* argv[]) 
{
    // Set input path
    fs::path inPath = fs::current_path().parent_path() / "data" / "cleaned_transactions.csv";

    // Open input
    ifstream fin(inPath);
    if (!fin) 
    {
        cerr << "Cannot open input file: " << inPath << "\n";
        return 1;
    }

    // Prepare output next to source
    path outPath = inPath.parent_path()
                     / ("cleaned_" + inPath.filename().string());
    ofstream fout(outPath);
    if (!fout) 
    {
        cerr << "Cannot create output file: " << outPath << "\n";
        return 1;
    }

    // Copy header
    string line;
    if (getline(fin, line)) 
    {
        fout << line << '\n';
    }
    /* Pass-4 : C4 (blank / null / NaN) ---------------------------- */
    for (size_t i = 0; i < rows.size; ) {
        split(rows.data[i], cols, 4);
        if (isBlankOrNull(cols[3])) rows.erase(i); else ++i;
    }

    /* build output path */
    std::string dir, base;
    splitPath(inFile, dir, base);
    std::string outFile = dir + "cleaned_" + base;

    std::ofstream fout(outFile);
    if (!fout) { std::cerr << "Cannot create output file: " << outFile << '\n'; return 1; }

    fout << header << '\n';
    for (size_t i = 0; i < rows.size; ++i) fout << rows.data[i] << '\n';
    fout.close();

    std::cout << "Finished. Kept " << rows.size << " rows  →  " << outFile << '\n';

    // Process and write valid rows in one pass
    size_t kept = 0;
    while (getline(fin, line)) 
    {
        if (isValidLine(line)) 
        {
            fout << line << '\n';
            ++kept;
        }
    }

    cout << "Finished. Kept " << kept
              << " rows → " << outPath << "\n";
    return 0;
}
