#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "common_function.hpp"
//#include "linkedList.hpp"

namespace fs = std::filesystem;
using fs::path;

using namespace std;

// Node: holds one CSV row of x columns
struct Node {
    std::string* data;  // array of length x
    Node* prev;
    Node* next;

    Node(int x)
      : data(new std::string[x]), prev(nullptr), next(nullptr)
    {}

    ~Node() {
        delete[] data;
    }
};

// LinkedList: container for header + doubly‐linked chain of Nodes
struct LinkedList {
    std::string* fieldHead;  // array of column names, length x
    Node* head;              // first data node
    Node* tail;              // last data node
    int x;                   // # columns
    int y;                   // # data rows

    // ctor: copy headers[0..numFields-1]
    LinkedList(const std::string headers[], int numFields)
      : head(nullptr), tail(nullptr), x(numFields), y(0)
    {
        fieldHead = new std::string[x];
        for (int i = 0; i < x; ++i) {
            fieldHead[i] = headers[i];
        }
    }

    // dtor: free all nodes + header array
    ~LinkedList() {
        delete[] fieldHead;
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    // append a new row (rowData[0..x-1]) at the tail
    void appendRow(const std::string rowData[]) {
        Node* node = new Node(x);
        for (int i = 0; i < x; ++i) {
            node->data[i] = rowData[i];
        }

        if (!head) {
            head = tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
        ++y;
    }

    // print headers, then all rows forward
    void printForward() const {
        // headers
        for (int i = 0; i < x; ++i) {
            std::cout << fieldHead[i]
                      << (i+1 < x ? " | " : "\n");
        }
        std::cout << std::string(50, '-') << "\n";

        // rows
        for (Node* cur = head; cur; cur = cur->next) {
            for (int i = 0; i < x; ++i) {
                std::cout << cur->data[i]
                          << (i+1 < x ? " | " : "\n");
            }
        }
    }

    // print all rows backward (from tail to head)
    void printBackward() const {
        for (Node* cur = tail; cur; cur = cur->prev) {
            for (int i = 0; i < x; ++i) {
                std::cout << cur->data[i]
                          << (i+1 < x ? " | " : "\n");
            }
        }
    }
};

int main() {
    // 1) Set up headers
    std::string headers[2] = {
        "Header in column 1",
        "Header in column 2"
    };
    LinkedList list(headers, 2);

    // 2) Append two rows
    std::string row1[2] = { "row 1 value1", "row 1 value2" };
    std::string row2[2] = { "row 2 value1", "row 2 value2" };
    list.appendRow(row1);
    list.appendRow(row2);

    // 3) Show stats + forward/backward print
    std::cout << "Columns: " << list.x
              << " | Rows: "   << list.y << "\n\n";

    std::cout << ">> Forward:\n";
    list.printForward();

    std::cout << "\n>> Backward:\n";
    list.printBackward();

    return 0;
}
