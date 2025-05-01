#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <filesystem>
#include <unordered_map>
//#include "common_function.hpp"
#include "Array.hpp"

namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono;

// Node: holds one CSV row of x columns
struct Node 
{
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
struct LinkedList 
{

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

    Return true on success (or false if the file couldn’t be opened or was empty).
    
    - call the function in this way:

        linkedList list;
        if (!list.loadFromCSV("cleaned_transactions.csv")) 
        {
            return 1;
        }

    */
   bool loadFromCSV(const string& filename) 
   {
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
       x = 1;
       for (char c : line) if (c == ',') ++x;
       fieldHead = new string[x];
   
       // --- 2) Split header, respecting quoted fields ---
       {
           size_t pos = 0;
           for (int col = 0; col < x; ++col) {
               if (pos < line.size() && line[pos] == '"') {
                   // quoted field
                   size_t end = line.find('"', pos + 1);
                   fieldHead[col] = line.substr(pos + 1, end - pos - 1);
                   pos = end + 1;                // move past closing quote
                   if (pos < line.size() && line[pos] == ',') ++pos;
               } else {
                   // unquoted
                   size_t comma = line.find(',', pos);
                   if (comma == string::npos) comma = line.size();
                   fieldHead[col] = line.substr(pos, comma - pos);
                   pos = comma;
                   if (pos < line.size() && line[pos] == ',') ++pos;
               }
           }
       }
   
       // --- 3) Read each subsequent row, same logic ---
       while (getline(in, line)) {
           if (line.empty()) continue;
   
           Node* node = new Node(x);
           size_t pos = 0;
           for (int col = 0; col < x; ++col) {
               if (pos < line.size() && line[pos] == '"') {
                   size_t end = line.find('"', pos + 1);
                   node->data[col] = line.substr(pos + 1, end - pos - 1);
                   pos = end + 1;
                   if (pos < line.size() && line[pos] == ',') ++pos;
               } else {
                   size_t comma = line.find(',', pos);
                   if (comma == string::npos) comma = line.size();
                   node->data[col] = line.substr(pos, comma - pos);
                   pos = comma;
                   if (pos < line.size() && line[pos] == ',') ++pos;
               }
           }
   
           // link into the doubly linked list
           if (!head) {
               head = tail = node;
           } else {
               node->prev = tail;
               tail->next = node;
               tail       = node;
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

    int deleteRows(const string& columnName,
                   const string& value)
    {
        using namespace chrono;
        auto start = high_resolution_clock::now();

        // find the index of the column
        int colIdx = -1;
        for (int i = 0; i < x; ++i) {
            if (fieldHead[i] == columnName) {
                colIdx = i;
                break;
            }
        }
        if (colIdx < 0) {
            cerr << "Error: column '" << columnName << "' not found\n";
            return 0;
        }

        // now delete matching nodes
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            if (cur->data[colIdx] == value) {
                if (cur->prev) cur->prev->next = cur->next;
                else           head = cur->next;

                if (cur->next) cur->next->prev = cur->prev;
                else           tail = cur->prev;

                delete cur;
                --y;
            }
            cur = nxt;
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        cout << "Time taken for deletion: " << duration << " ms\n";
        
        return static_cast<int>(duration);
    }

    /*
    call in this way:

    LinkedList filtered_result = list.linearSearch1Field("Product ID", "PROD956");

    then filter to check in: filtered_result.printForward();

    */
    LinkedList linearSearch1Field(const string& columnName, const string& searchValue) const
    {
        auto start = high_resolution_clock::now();

        // 1) Prepare the result list (copy header + column count)
        LinkedList result;
        result.x = x;
        result.y = 0;
        result.fieldHead = new string[x];
        for (int i = 0; i < x; ++i)
            result.fieldHead[i] = fieldHead[i];

        // 2) Find the index of the search column
        int colIdx = -1;
        for (int i = 0; i < x; ++i) {
            if (fieldHead[i] == columnName) {
                colIdx = i;
                break;
            }
        }
        if (colIdx < 0) {
            cerr << "Error: Column '" << columnName << "' not found.\n";
            return result;    // empty result
        }

        // 3) Walk through original list, copying matches
        for (Node* cur = head; cur; cur = cur->next) {
            if (cur->data[colIdx] == searchValue) {
                // deep‐copy this row
                Node* node = new Node(x);
                for (int j = 0; j < x; ++j)
                    node->data[j] = cur->data[j];

                // link into result
                if (!result.head) {
                    result.head = result.tail = node;
                } else {
                    node->prev       = result.tail;
                    result.tail->next = node;
                    result.tail       = node;
                }
                ++result.y;
            }
        }

        // 4) (optional) measure and print elapsed time
        auto end      = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        printf("\nSearched \"%s = %s\" in %lld microseconds; found %d rows.\n",
               columnName.c_str(), searchValue.c_str(), duration, result.y);

        return result;  
    }
};

// ——————————————————
// Bubble‐sort as a free function
/* 
    use the function in this way:
    bubbleSort(list, "columnName");

    print to verify:
    list.printForward();

*/

// ——————————————————
int bubbleSort(LinkedList& list, const string& columnName) 
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
        cerr << "Error: column \"" << columnName << "\" not found.\n";
        return 1;
    }

    // 2) start timer & memory tracking
    auto t0 = high_resolution_clock::now();

    // 3) bubble‐sort by swapping data pointers
    bool swapped;
    do {
        swapped = false;
        for (Node* cur = list.head; cur && cur->next; cur = cur->next) {
            if (cur->data[colIndex] > cur->next->data[colIndex]) {
                swap(cur->data, cur->next->data);
                swapped = true;
            }
        }
    } while (swapped);

    // 4) Stop timer
    auto t1 = high_resolution_clock::now();
    auto ms = duration_cast<milliseconds>(t1 - t0).count();

    // 5) Report elapsed time
    cout << "Selection sort on \"" << columnName
              << "\" took " << ms << " ms\n";

    return static_cast<int>(ms);
}



int mergeSortList(LinkedList& list, const string& columnName) {
    if (!list.head) return 1; // nothing to sort

    // 1) find the column index
    int colIndex = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            colIndex = i;
            break;
        }
    }
    if (colIndex < 0) {
        cerr << "Error: column \"" << columnName << "\" not found.\n";
        return 1;
    }

    using NodePtr = Node*;

    // 2) recursive lambdas, capturing colIndex
    function<NodePtr(NodePtr)> split = [&](NodePtr head) -> NodePtr {
        NodePtr slow = head, fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        NodePtr second = slow->next;
        slow->next = nullptr;
        if (second) second->prev = nullptr;
        return second;
    };

    function<NodePtr(NodePtr,NodePtr)> sortedMerge =
      [&](NodePtr a, NodePtr b) -> NodePtr
    {
        if (!a) return b;
        if (!b) return a;
        if (a->data[colIndex] <= b->data[colIndex]) {
            a->next = sortedMerge(a->next, b);
            if (a->next) a->next->prev = a;
            a->prev = nullptr;
            return a;
        } else {
            b->next = sortedMerge(a, b->next);
            if (b->next) b->next->prev = b;
            b->prev = nullptr;
            return b;
        }
    };

    function<NodePtr(NodePtr)> mergeSort =
      [&](NodePtr head) -> NodePtr
    {
        if (!head || !head->next) return head;
        NodePtr second = split(head);
        head   = mergeSort(head);
        second = mergeSort(second);
        return sortedMerge(head, second);
    };

    // 3) START TIMER
    auto t0 = high_resolution_clock::now();

    // 4) sort
    list.head = mergeSort(list.head);

    // 5) STOP TIMER & REPORT
    // 4) Stop timer
    auto t1 = high_resolution_clock::now();
    auto ms = duration_cast<milliseconds>(t1 - t0).count();

    // 6) Report elapsed time
    cout << "Merge sort on \"" << columnName
              << "\" took " << ms << " ms\n";

    // 6) repair tail & update row count
    NodePtr cur = list.head, prev = nullptr;
    int count = 0;
    while (cur) {
        prev = cur;
        cur  = cur->next;
        ++count;
    }
    list.tail = prev;
    list.y    = count;

    return static_cast<int>(ms);
}

int insertionSort(LinkedList& list, const string& columnName) {
    
    // 1) Find column index
    int col = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0) {
        cerr << "Error: column \"" << columnName << "\" not found.\n";
        return 1;
    }

    // nothing to sort if empty or single node
    if (!list.head || !list.head->next) return 1 ;

    // 2) Start timer
    auto t0 = high_resolution_clock::now();

    // 3) Perform insertion sort on the doubly-linked list
    Node* sorted = list.head;
    Node* curr   = sorted->next;
    sorted->prev = sorted->next = nullptr;

    while (curr) {
        Node* next = curr->next;
        curr->prev = curr->next = nullptr;

        // insert at front?
        if (curr->data[col] < sorted->data[col]) {
            curr->next   = sorted;
            sorted->prev = curr;
            sorted       = curr;
        } else {
            // locate insertion point
            Node* p = sorted;
            while (p->next && p->next->data[col] <= curr->data[col]) {
                p = p->next;
            }
            // insert after p
            curr->next        = p->next;
            if (p->next) p->next->prev = curr;
            p->next           = curr;
            curr->prev        = p;
        }

        curr = next;
    }

    // 4) Stop timer
    auto t1 = high_resolution_clock::now();
    auto ms = duration_cast<milliseconds>(t1 - t0).count();

    // 5) Update list head & tail
    list.head = sorted;
    list.tail = sorted;
    while (list.tail->next) {
        list.tail = list.tail->next;
    }

    // 6) Report elapsed time
    cout << "Insertion sort on \"" << columnName
              << "\" took " << ms << " ms\n";

    return static_cast<int>(ms);
}

int selectionSort(LinkedList& list, const string& columnName) 
{
    // 1) Find column index
    int col = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0) {
        cerr << "Error: column \"" << columnName << "\" not found.\n";
        return 1;
    }

    // nothing to do on empty or single‐node list
    if (!list.head || !list.head->next) return 1;

    // 2) Start timer
    auto t0 = high_resolution_clock::now();

    // 3) Selection‐sort by swapping each node's data pointer
    for (Node* i = list.head; i; i = i->next) {
        // find min in [i … end)
        Node* minNode = i;
        for (Node* j = i->next; j; j = j->next) {
            if (j->data[col] < minNode->data[col]) {
                minNode = j;
            }
        }
        // swap row‐buffers if needed
        if (minNode != i) {
            swap(i->data, minNode->data);
        }
    }

    // 4) Stop timer
    auto t1 = high_resolution_clock::now();
    auto ms = duration_cast<milliseconds>(t1 - t0).count();

    // 5) Report elapsed time
    cout << "Selection sort on \"" << columnName
              << "\" took " << ms << " ms\n";

    return static_cast<int>(ms);
}

int linearSearch1Field(const LinkedList& list, const string& columnName, const string& searchValue) 
{

    auto start = high_resolution_clock::now();

    int columnIndex = -1;

    // Find the index of the column
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            columnIndex = i;
            break;
        }
    }

    if (columnIndex == -1) {
        cerr << "Error: Column '" << columnName << "' not found.\n";
        return 1;
    }

    cout << "Results for " << columnName << " = " << searchValue << ":\n";
    cout << string(50, '-') << "\n";

    int matchCount = 0;
    for (Node* cur = list.head; cur; cur = cur->next) {
        if (cur->data[columnIndex] == searchValue) {
            for (int i = 0; i < list.x; ++i) {
                cout << cur->data[i] << (i + 1 < list.x ? " | " : "\n");
            }
            ++matchCount;
        }
    }

    if (matchCount == 0) {
        cout << "No matching results found.\n";
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "\nTime taken for linear search: " << duration << " ms\n";

    return static_cast<int>(duration);
}


void linearSearchByTwoColumns(const LinkedList& list,
                               const string& columnName1, const string& value1,
                               const string& columnName2, const string& value2) 
{
    auto start = high_resolution_clock::now();

    int col1Index = -1;
    int col2Index = -1;

    // Find index of both columns
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName1) col1Index = i;
        if (list.fieldHead[i] == columnName2) col2Index = i;
    }

    if (col1Index == -1 || col2Index == -1) {
        cerr << "Error: One or both column names not found.\n";
        return;
    }

    cout << "Results for (" << columnName1 << " = " << value1
         << ") AND (" << columnName2 << " = " << value2 << "):\n";
    cout << string(50, '-') << "\n";

    int matchCount = 0;
    for (Node* cur = list.head; cur; cur = cur->next) {
        if (cur->data[col1Index] == value1 && cur->data[col2Index] == value2) {
            for (int i = 0; i < list.x; ++i) {
                cout << cur->data[i] << (i + 1 < list.x ? " | " : "\n");
            }
            ++matchCount;
        }
    }

    if (matchCount == 0) {
        cout << "No matching results found.\n";
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    cout << "\nTime taken for linear search: " << duration << " microseconds\n";
}



/*you will need to sort it first before u do the search

    - call any of the sorting function first, e.g.:
        insertionSort(list, "Product ID");
        bubbleSort(list, "Product ID");
        mergeSortList(list, "Product ID");
        selectionSort(list, "Product ID");

    - then call the binary search function in this way:
        binarySearch(list, "Product ID", "PROD956");
*/
int binarySearch(LinkedList& list,
                         const string& columnName,
                         const string& key)
{
    // 1) find column index
    int idx = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        cerr << "Error: Column \"" << columnName << "\" not found.\n";
        return -1;
    }

    auto t0 = chrono::high_resolution_clock::now();

    // 2) helper to find middle between start..end (inclusive)
    auto getMiddle = [&](Node* start, Node* end) {
        Node* slow = start;
        Node* fast = start;
        while (fast != end && fast->next != end) {
            fast = fast->next->next;
            slow = slow->next;
        }
        return slow;
    };

    // 3) binary-search for any one match
    Node* low   = list.head;
    Node* high  = list.tail;
    Node* found = nullptr;


    while (low && high) {
        // ensure low ≤ high by walking from low to high
        bool valid = false;
        for (Node* p = low; p; p = p->next) {
            if (p == high) { valid = true; break; }
        }
        if (!valid) break;

        Node* mid = getMiddle(low, high);
        if (mid->data[idx] == key) {
            found = mid;
            break;
        }
        else if (mid->data[idx] < key) {
            low = mid->next;
        }
        else {
            high = mid->prev;
        }
    }


    // 5) Print results
    if (!found) {
        cout << "No results for " << columnName
                  << " = " << key << "\n";
        return 1;
    }

    // Walk back to first matching node
    Node* first = found;
    while (first->prev && first->prev->data[idx] == key) {
        first = first->prev;
    }

    // Print header once
    cout << "Results for " << columnName
              << " = " << key << ":\n";
    cout << string(50, '-') << "\n";

    // Walk forward and print all matches
    for (Node* p = first; p && p->data[idx] == key; p = p->next) {
        for (int c = 0; c < list.x; ++c) {
            cout << p->data[c]
                      << (c + 1 < list.x ? " | " : "\n");
        }
    }

    // 4) Stop timer
    auto t1 = chrono::high_resolution_clock::now();
    int ms = static_cast<int>(
        chrono::duration_cast<chrono::milliseconds>(t1 - t0)
        .count());

    cout << "Binary search on \"" << columnName
              << "\" took " << ms << " ms\n";

    return static_cast<int>(ms);
}

static char* make_cstr(const string& s) {
    char* buf = new char[s.size()+1];
    memcpy(buf, s.c_str(), s.size()+1);
    return buf;
}


/*
    call in this way:

    - first load the data into the linked list:
        LinkedList list;
        if (!list.loadFromCSV("cleaned_transactions.csv")) {
            return 1;
        }
    - Filter the data by rating:
        LinkedList oneStar = list.linearSearch1Field("Rating","1");

    - call the function to get the top N frequent words:    
        dataContainer2D dc = getTopFrequentWordsDC(list, "Product ID", 10);

    - print to verify:
        printDataContainer(dc);

*/
dataContainer2D getTopFrequentWordsDC(const LinkedList& list,
                                      const string& columnName,
                                      int topN = 10)
{
    dataContainer2D dc;
    // 1) find column index
    int col = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            col = i; break;
        }
    }
    if (col < 0) {
        dc.error = 1;
        return dc;
    }

    // 2) count frequencies
    unordered_map<string,int> freq;
    for (Node* cur = list.head; cur; cur = cur->next) {
        const string &s = cur->data[col];
        string token;
        for (unsigned char c : s) {
            if (c >= 'A' && c <= 'Z')           token.push_back(c - 'A' + 'a');
            else if ((c >= 'a' && c <= 'z') ||
                     (c >= '0' && c <= '9'))   token.push_back(c);
            else {
                if (!token.empty()) {
                    ++freq[token];
                    token.clear();
                }
            }
        }
        if (!token.empty()) {
            ++freq[token];
        }
    }

    // 3) pick topN via repeated max-scan
    struct Pair { string w; int c; };
    Pair picks[100];            // assume topN <= 100
    int actual = 0;
    for (; actual < topN; ++actual) {
        string best; int bestC = 0;
        for (auto &kv : freq) {
            if (kv.second > bestC) {
                bestC = kv.second;
                best  = kv.first;
            }
        }
        if (bestC == 0) break;
        picks[actual] = {best, bestC};
        freq[best]    = -1;
    }

    // 4) allocate dataContainer2D
    dc.x = 2;
    dc.y = actual;
    // fields
    dc.fields = new char*[2];
    dc.fields[0] = make_cstr("Word");
    dc.fields[1] = make_cstr("Frequency");
    // data
    dc.data = new char**[dc.y];
    for (int i = 0; i < dc.y; ++i) {
        dc.data[i] = new char*[2];
        dc.data[i][0] = make_cstr(picks[i].w);
        dc.data[i][1] = make_cstr(to_string(picks[i].c));
    }

    return dc;
}


void printDataContainer(dataContainer2D& dc) 
{
    // 1) Print header
    std::cout 
        << dc.fields[0] 
        << " | " 
        << dc.fields[1] 
        << "\n";
    std::cout << std::string(30, '-') << "\n";

    // 2) Print each row
    for (int i = 0; i < dc.y; ++i) {
        std::cout
            << dc.data[i][0]
            << " | "
            << dc.data[i][1]
            << "\n";
    }

    // 3) Free all allocated memory
    freeContainer(dc);
}


/*    
    You Basically just copy paste the code into the main function and run : 

    LinkedList list;
    if (!list.loadFromCSV("cleaned_reviews.csv")) 
    {
        return 1;
    }

    LinkedList filtered_result = list.linearSearch1Field("Rating", "1");

    insertionSort(filtered_result, "Product ID");

    LinkedList topWords = getTopFrequentWords(filtered_result, "Review Text", 10);

    topWords.printForward();
*/
// top-N most frequent ASCII words in columnName
// LinkedList getTopFrequentWords(const LinkedList& list,
//                                const string& columnName,
//                                int topN = 10)
// {
//     // 1) Find column index
//     int col = -1;
//     for (int i = 0; i < list.x; ++i) {
//         if (list.fieldHead[i] == columnName) {
//             col = i;
//             break;
//         }
//     }
//     if (col < 0) return LinkedList();  // column not found

//     // 2) Count frequencies
//     unordered_map<string,int> freq;
//     for (Node* cur = list.head; cur; cur = cur->next) {
//         const string &s = cur->data[col];
//         string token;
//         for (unsigned char c : s) {
//             if (c >= 'A' && c <= 'Z') {
//                 token.push_back(c - 'A' + 'a');
//             }
//             else if ((c >= 'a' && c <= 'z') ||
//                      (c >= '0' && c <= '9'))
//             {
//                 token.push_back(c);
//             }
//             else {
//                 // delimiter: flush token if any
//                 if (!token.empty()) {
//                     ++freq[token];
//                     token.clear();
//                 }
//             }
//         }
//         // flush last token
//         if (!token.empty()) {
//             ++freq[token];
//         }
//     }

//     // 3) Build result LinkedList
//     LinkedList result;
//     result.x = 2;
//     // **two-step header init** (guaranteed)
//     result.fieldHead = new string[2];
//     result.fieldHead[0] = "Word";
//     result.fieldHead[1] = "Frequency";
//     result.head = result.tail = nullptr;
//     result.y = 0;

//     // 4) Extract topN by repeated max-scan
//     for (int k = 0; k < topN; ++k) {
//         string bestWord;
//         int bestCount = 0;
//         for (auto &kv : freq) {
//             if (kv.second > bestCount) {
//                 bestCount = kv.second;
//                 bestWord  = kv.first;
//             }
//         }
//         if (bestCount == 0) break;  // no more words

//         // append node
//         Node* node = new Node(result.x);
//         node->data[0] = bestWord;
//         node->data[1] = to_string(bestCount);
//         node->prev    = result.tail;
//         node->next    = nullptr;
//         if (result.tail) result.tail->next = node;
//         else             result.head        = node;
//         result.tail = node;
//         ++result.y;

//         // mark used
//         freq[bestWord] = -1;
//     }

//     return result;
// }