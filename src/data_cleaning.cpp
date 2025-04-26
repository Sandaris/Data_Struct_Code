/********************************************************************
 *  Simple CSV cleaner – four-pass algorithm (C1 → C2 → C3 → C4)    *
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>

/* ------------------ minimal dynamic array for rows -------------- */
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
    void erase(size_t idx) {                          // O(n) shift-left
        for (size_t i = idx + 1; i < size; ++i) data[i - 1] = std::move(data[i]);
        --size;
    }
};

/* --------------------------- helpers ---------------------------- */
static inline std::string lower(std::string s) {
    for (char& c : s) c = std::tolower(c);
    return s;
}
bool isBlankOrNull(const std::string& s) {
    std::string t = lower(s);
    return t.empty() || t == "nan" || t == "null";
}
bool isValidRating(const std::string& s) {            // C3 rule
    if (isBlankOrNull(s)) return false;
    for (char c : s) if (!std::isdigit(c)) return false;
    int r = std::stoi(s);
    return r >= 1 && r <= 5;
}

/* quick split (no quoted commas) */
void split(const std::string& line, std::string cols[], size_t nCols) {
    size_t start = 0, col = 0;
    for (size_t i = 0; i <= line.size() && col < nCols; ++i) {
        if (i == line.size() || line[i] == ',') {
            cols[col++] = line.substr(start, i - start);
            start = i + 1;
        }
    }
    while (col < nCols) cols[col++] = "";
}

/* return dirname + basename of a path */
void splitPath(const std::string& path, std::string& dir, std::string& base) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) { dir.clear(); base = path; }
    else { dir = path.substr(0, pos + 1); base = path.substr(pos + 1); }
}

/* ----------------------- main routine --------------------------- */
int main(int argc, char* argv[])
{
    std::string inFile = (argc > 1) ? argv[1] : "../data/reviews.csv";

    std::ifstream fin(inFile);
    if (!fin) { std::cerr << "Cannot open input file: " << inFile << '\n'; return 1; }

    RowArray rows;
    std::string header, line;
    std::getline(fin, header);
    while (std::getline(fin, line)) rows.push_back(line);
    fin.close();

    std::string cols[4];                // we need four columns now

    /* Pass-1 : C1 -------------------------------------------------- */
    for (size_t i = 0; i < rows.size; ) {
        split(rows.data[i], cols, 4);
        if (isBlankOrNull(cols[0])) rows.erase(i); else ++i;
    }
    /* Pass-2 : C2 -------------------------------------------------- */
    for (size_t i = 0; i < rows.size; ) {
        split(rows.data[i], cols, 4);
        if (isBlankOrNull(cols[1])) rows.erase(i); else ++i;
    }
    /* Pass-3 : C3 (rating rule) ----------------------------------- */
    for (size_t i = 0; i < rows.size; ) {
        split(rows.data[i], cols, 4);
        if (!isValidRating(cols[2])) rows.erase(i); else ++i;
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

    std::cout << "Finished. Kept " << rows.size
              << " rows  →  " << outFile << '\n';
    return 0;
}
