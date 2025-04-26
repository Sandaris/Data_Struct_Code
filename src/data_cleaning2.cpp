/********************************************************************
 *  Simple CSV cleaner – single-pass using isBadCell()              *
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/* -------- tiny dynamic array to hold the rows ------------------- */
struct RowArray {
    std::string* data = nullptr;
    size_t size = 0, capacity = 0;
    ~RowArray() { delete[] data; }
    void reserve(size_t cap) {
        std::string* n = new std::string[cap];
        for (size_t i = 0; i < size; ++i) n[i] = std::move(data[i]);
        delete[] data; data = n; capacity = cap;
    }
    void push_back(const std::string& row) {
        if (size == capacity) reserve(capacity ? capacity * 2 : 1024);
        data[size++] = row;
    }
    void erase(size_t idx) {                  // O(n) shift-left
        for (size_t i = idx + 1; i < size; ++i) data[i - 1] = std::move(data[i]);
        --size;
    }
};

/* -------- your validation routine --------------------------------*/
bool isBadCell(const std::string& line)
{
    std::stringstream ss(line);
    std::string cell;

    while (std::getline(ss, cell, ',')) {
        if (cell.empty() || cell == "NaN" ||
            cell == "Invalid Date" || cell == "Invalid Rating") {
            return true;
        }
    }
    /* Edge case: line ends with a comma → empty last cell */
    return !line.empty() && line.back() == ',';
}

/* ----------------------------- helpers ---------------------------*/
void splitPath(const std::string& path, std::string& dir, std::string& base)
{
    size_t p = path.find_last_of("/\\");
    if (p == std::string::npos) { dir.clear(); base = path; }
    else { dir = path.substr(0, p + 1); base = path.substr(p + 1); }
}

/* ------------------------------- main ----------------------------*/
int main(int argc, char* argv[])
{
    std::string inFile = (argc > 1) ? argv[1] : "../data/transactions.csv";

    std::ifstream fin(inFile);
    if (!fin) { std::cerr << "Cannot open input file: " << inFile << '\n'; return 1; }

    std::string header, line;
    std::getline(fin, header);

    RowArray rows;
    while (std::getline(fin, line)) rows.push_back(line);
    fin.close();

    /* ---- single clean-up pass ----------------------------------- */
    for (size_t r = 0; r < rows.size; ) {
        if (isBadCell(rows.data[r])) rows.erase(r);
        else                         ++r;
    }

    /* ---- write cleaned file next to source ---------------------- */
    std::string dir, base; splitPath(inFile, dir, base);
    std::string outFile = dir + "cleaned_" + base;

    std::ofstream fout(outFile);
    if (!fout) { std::cerr << "Cannot create output file: " << outFile << '\n'; return 1; }

    fout << header << '\n';
    for (size_t i = 0; i < rows.size; ++i) fout << rows.data[i] << '\n';
    fout.close();

    std::cout << "Finished. Kept " << rows.size
              << " rows  →  " << outFile << '\n';
    return 0;
}
