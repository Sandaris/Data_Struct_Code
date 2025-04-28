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

int main() 
{
    LinkedList list;
    if (!list.loadFromCSV("cleaned_transactions.csv")) 
    {
        return 1;
    }

    cout << "Columns: " << list.x
         << " | Rows: "   << list.y << "\n\n";

    list.printForward();
    return 0;
}
