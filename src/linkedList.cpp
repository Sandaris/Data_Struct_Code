#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "common_function.hpp"

namespace fs = std::filesystem;
using namespace std;

// Node: holds one CSV row of x columns
struct Node {
    string* data;   // array of length x
    Node* prev;
    Node* next;

    Node(int x)
      : data(new string[x]), prev(nullptr), next(nullptr)
    {}

    ~Node() {
        delete[] data;
    }
};

// Doubly-linked list that also knows its header row
struct LinkedList {
    string* fieldHead;  // column names, length = x
    Node*   head;       // first data node
    Node*   tail;       // last data node
    int     x;          // # of columns
    int     y;          // # of data rows

    LinkedList()
      : fieldHead(nullptr), head(nullptr),
        tail(nullptr), x(0), y(0)
    {}

    ~LinkedList() {
        delete[] fieldHead;
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    // Load a CSV from ../data/filename into this list
/*Step-by-step:
Locate and open the file in ../data/filename.

Read the first line (the header):

Count commas (',') to figure out how many columns (x = commas + 1).

Allocate fieldHead[x].

Split that line at each comma and store each column name into fieldHead[0]…fieldHead[x-1].

For each remaining line (each CSV data row):

Skip if it’s blank.

Create a new Node(x)—an empty box for one row.

Split the line by commas again, putting each piece into node->data[0]…data[x-1].

Link the new node onto the end of the list:

If it’s the very first one, head = tail = node.

Otherwise, hook node->prev to the old tail, and tail->next to this new node, then update tail.

Increment the row count y.

Return true on success (or false if the file couldn’t be opened or was empty).*/
    bool loadFromCSV(const string& filename) {
        fs::path filePath = fs::current_path().parent_path()
                            / "data" / filename;
        ifstream in(filePath);
        if (!in.is_open()) {
            cerr << "Error: Cannot open " << filePath << "\n";
            return false;
        }

        string line;
        // --- 1) Read header line and count columns ---
        if (!getline(in, line)) return false;
        // count commas → columns = commas+1
        x = 1;
        for (char c : line) if (c == ',') ++x;
        // allocate header array
        fieldHead = new string[x];

        // split header line into fieldHead[0..x-1]
        {
            int start = 0, col = 0;
            for (int i = 0; i <= (int)line.size(); ++i) {
                if (i == (int)line.size() || line[i] == ',') {
                    fieldHead[col++] = line.substr(start, i - start);
                    start = i + 1;
                }
            }
        }

        // --- 2) Read each subsequent row ---
        while (getline(in, line)) {
            if (line.empty()) continue;  // skip blank lines

            Node* node = new Node(x);
            int start = 0, col = 0;
            for (int i = 0; i <= (int)line.size(); ++i) {
                if (i == (int)line.size() || line[i] == ',') {
                    // assign row value into node->data[col]
                    node->data[col++] = line.substr(start, i - start);
                    start = i + 1;
                }
            }

            // link into the doubly linked list
            if (!head) {
                head = tail = node;
            } else {
                node->prev     = tail;
                tail->next     = node;
                tail           = node;
            }
            ++y;
        }

        return true;
    }

    // Print header + all rows in forward order
    void printForward() const {
        // headers
        for (int i = 0; i < x; ++i) {
            cout << fieldHead[i]
                 << (i+1 < x ? " | " : "\n");
        }
        cout << string(50, '-') << "\n";
        // rows
        for (Node* cur = head; cur; cur = cur->next) {
            for (int i = 0; i < x; ++i) {
                cout << cur->data[i]
                     << (i+1 < x ? " | " : "\n");
            }
        }
    }
};


// ——————————————————
// Bubble‐sort as a free function
// ——————————————————
void bubbleSort(LinkedList& list, const std::string& columnName) 
{
    // 1) find the column index
    int colIndex = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            colIndex = i;
            break;
        }
    }
    if (colIndex < 0) {
        std::cerr << "Error: column \"" << columnName << "\" not found.\n";
        return;
    }

    // 2) start timer
    auto t0 = std::chrono::steady_clock::now();

    // 3) bubble‐sort by swapping data pointers
    bool swapped;
    do {
        swapped = false;
        for (Node* cur = list.head; cur && cur->next; cur = cur->next) {
            if (cur->data[colIndex] > cur->next->data[colIndex]) {
                std::swap(cur->data, cur->next->data);
                swapped = true;
            }
        }
    } while (swapped);

    // 4) stop timer & report
    auto t1 = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
    std::cout << "Bubble sort on \"" << columnName << "\" took "
              << elapsed.count() << " μs\n";
}


int main() 
{
    LinkedList list;
    if (!list.loadFromCSV("cleaned_transactions.csv")) 
    {
        return 1;
    }

    // sort externally
    bubbleSort(list, "Price");

    // print to verify
    list.printForward();

    // print fields
    std::cout << "Fields: ";
    for (int i = 0; i < list.x; ++i) {
        std::cout << list.fieldHead[i] << " ";
    }
    std::cout << "\n";

    return 0;
    //      << " | Rows: "   << list.y << "\n\n";

    // list.printForward();
    // return 0;
}
