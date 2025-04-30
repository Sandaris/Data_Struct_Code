#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>
#include "linkedList.hpp"


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
    while (start <= len) {
        size_t comma = line.find(',', start);
        if (comma == string::npos) comma = len;
        if (isBadCell({ line.data() + start, comma - start }))
            return false;
        start = comma + 1;
    }
    // Reject if line ends with a comma → empty last cell
    return !(len > 0 && line.back() == ',');
}

// --- free function to drop any row containing a bad cell ---
void dropCells(LinkedList& list) 
{
    Node* cur = list.head;
    while (cur) {
        bool bad = false;
        for (int i = 0; i < list.x; ++i) {
            if (isBadCell(cur->data[i])) {
                bad = true;
                break;
            }
        }

        Node* nxt = cur->next;
        if (bad) {
            // unlink
            if (cur->prev) cur->prev->next = cur->next;
            else            list.head = cur->next;

            if (cur->next) cur->next->prev = cur->prev;
            else            list.tail = cur->prev;

            delete cur;
            --list.y;
        }
        cur = nxt;
    }
}

void writeCleanCSV(const LinkedList& list, const string& origFilename) {
    // build the output filename
    string outName = "cleaned_" + origFilename;
    fs::path outPath = fs::current_path().parent_path() / "data" / outName;

    ofstream out(outPath);
    if (!out.is_open()) {
        cerr << "Error: Cannot write to " << outPath << "\n";
        return;
    }

    // helper to escape a single field:
    auto escapeField = [&](const string& field) -> string {
        bool needQuotes = false;
        string buf;
        buf.reserve(field.size() * 2);
        for (char c : field) {
            if (c == '"' ) {
                buf += "\"\"";      // double the quote
                needQuotes = true;
            } else {
                buf += c;
                if (c == ',' || c == '\n' || c == '\r')
                    needQuotes = true;
            }
        }
        if (needQuotes) {
            return "\"" + buf + "\"";
        } else {
            return buf;
        }
    };

    // 1) write header row
    for (int col = 0; col < list.x; ++col) {
        out << escapeField(list.fieldHead[col]);
        if (col + 1 < list.x) out << ',';
    }
    out << '\n';

    // 2) write each data row
    Node* cur = list.head;
    while (cur) {
        for (int col = 0; col < list.x; ++col) {
            out << escapeField(cur->data[col]);
            if (col + 1 < list.x) out << ',';
        }
        out << '\n';
        cur = cur->next;
    }

    cout << "Wrote cleaned CSV to: " << outPath << "\n";
}


int main()
{
    string filename = "reviews.csv";
    
    LinkedList list;
    if (!list.loadFromCSV(filename)) {
        cerr << "Error: Cannot load CSV file.\n";
        return 1;
    }
   
    cout << list.y << " rows read\n";
    // clean out rows with any bad cell
    dropCells(list);

    //write into CSV
    writeCleanCSV(list, filename);

    cout << "Cleaned list has " << list.y << " valid rows.\n";
    
    // … now you can sort/search the clean list …

    return 0;


}


// int main(int argc, char* argv[]) 
// {
//     // Set input path
//     fs::path inPath = fs::current_path().parent_path() / "data" / "cleaned_transactions.csv";

//     // Open input
//     ifstream fin(inPath);
//     if (!fin) {
//         cerr << "Cannot open input file: " << inPath << "\n";
//         return 1;
//     }

//     // Prepare output next to source
//     path outPath = inPath.parent_path()
//                      / ("cleaned_" + inPath.filename().string());
//     ofstream fout(outPath);
//     if (!fout) {
//         cerr << "Cannot create output file: " << outPath << "\n";
//         return 1;
//     }

//     // Copy header
//     string line;
//     if (getline(fin, line)) {
//         fout << line << '\n';
//     }

//     // Process and write valid rows in one pass
//     size_t kept = 0;
//     while (getline(fin, line)) {
//         if (isValidLine(line)) {
//             fout << line << '\n';
//             ++kept;
//         }
//     }

//     cout << "Finished. Kept " << kept
//               << " rows → " << outPath << "\n";
//     return 0;
// }
