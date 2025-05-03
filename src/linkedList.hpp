#pragma once

#include <functional>
#include <filesystem>
#include <unordered_map>
#include <regex>

#include "common_function.hpp"

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
    void printForward() const 
    {
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

    void printForward(int index) const {
        // sanity check
        if (index < 0 || index >= y) {
            std::cerr << "Error: index " << index << " out of range (0–" << (y-1) << ")\n";
            return;
        }

        cout << "test";
        // walk to the desired node
        Node* cur = head;
        for (int i = 0; i < index; ++i) {
            cur = cur->next;
        }

        cout << "test";
        // print only that row
        for (int i = 0; i < x; ++i) {
            std::cout << cur->data[i]
                      << (i+1 < x ? " | " : "\n");
        }
    }




    /*
    call in this way:

    LinkedList filtered_result = list.linearSearch1Field("Product ID", "PROD956");

    then filter to check in: filtered_result.printForward();

    */
    LinkedList linearKeepRows(const string& columnName, const string& searchValue) const
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

        return result;  
    }

};

LinkedList insertNewRowLinkedList(LinkedList list, const char* newValues[], int recordLen, InsDelResult& result) {
    result.memory = 0;
    result.time = 0;

    Timer timer;
    timer.begin();
    size_t memStart = getUsedMemoryKB();

    // Validate column count
    if (recordLen != list.x || list.fieldHead == nullptr || list.x == 0) {
        cerr << "Error: Field count mismatch or header not initialized.\n";
        return list;
    }

    // Create new node and assign values
    Node* newNode = new Node(list.x);
    for (int i = 0; i < list.x; ++i) {
        newNode->data[i] = string(newValues[i]);
    }

    // Append node to the list
    if (!list.head) {
        list.head = list.tail = newNode;
    } else {
        list.tail->next = newNode;
        newNode->prev = list.tail;
        list.tail = newNode;
    }

    list.y++;  // Increase row count

    result.memory = getUsedMemoryKB() - memStart;
    timer.finish();
    result.time = timer.getDurationMicroseconds();

    return list;
}


LinkedList deleteRows(LinkedList list, const string& columnName, const string& value, InsDelResult& result) {
    result.memory = 0;
    result.time = 0;

    Timer timer;
    timer.begin();
    size_t memStart = getUsedMemoryKB();

    // Find the index of the column
    int colIdx = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            colIdx = i;
            break;
        }
    }
    if (colIdx < 0) {
        cerr << "Error: column '" << columnName << "' not found\n";
        return list;
    }

    // Delete all matching nodes
    Node* cur = list.head;
    while (cur) {
        Node* nxt = cur->next;
        if (cur->data[colIdx] == value) {
            // If current node matches, delete it
            if (cur->prev) {
                cur->prev->next = cur->next;
            } else {
                list.head = cur->next;  // If first node, update head
            }

            if (cur->next) {
                cur->next->prev = cur->prev;
            } else {
                list.tail = cur->prev;  // If last node, update tail
            }

            delete cur;  // Free memory
            --list.y;  // Decrease row count
        }
        cur = nxt;  // Move to the next node
    }

    result.memory = getUsedMemoryKB() - memStart;
    timer.finish();
    result.time = timer.getDurationMicroseconds();

    return list;
}
// ——————————————————
// Bubble‐sort as a free function
/* 
    use the function in this way:
    bubbleSort(list, "columnName");

    print to verify:
    list.printForward();

*/
////////////////////////////////////////////////////////////// CLONING FUNCTION ////////////////////////////////////////////////////////////
LinkedList cloneList(const LinkedList& original) {
    LinkedList copy;
    copy.x = original.x;
    copy.y = 0;

    // Step 1: Copy header fields
    copy.fieldHead = new string[original.x];
    for (int i = 0; i < original.x; ++i) {
        copy.fieldHead[i] = original.fieldHead[i];
    }

    // Step 2: Copy each row (Node)
    Node* current = original.head;
    while (current) {
        Node* newNode = new Node(original.x);
        for (int i = 0; i < original.x; ++i) {
            newNode->data[i] = current->data[i];
        }

        // Link node to new list
        if (!copy.head) {
            copy.head = copy.tail = newNode;
        } else {
            copy.tail->next = newNode;
            newNode->prev = copy.tail;
            copy.tail = newNode;
        }

        current = current->next;
        copy.y++;
    }

    return copy;
}
// ——————————————————
// LinkedList original;
// original.loadFromCSV("cleaned_transactions.csv");
// LinkedList clone = cloneList(original);

///////////////////////////////////////////////////////////// SORT ALGORITHM ////////////////////////////////////////////////////////////
// ——————————————————
SortResult bubbleSortLinked(LinkedList& list, const string& columnName) {
    SortResult result;

    int col = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0) return result;

    Timer timer;
    timer.begin();
    size_t memStart = getUsedMemoryKB();

    bool swapped;
    do {
        swapped = false;
        for (Node* cur = list.head; cur && cur->next; cur = cur->next) {
            if (cur->data[col] > cur->next->data[col]) {
                swap(cur->data, cur->next->data);
                swapped = true;
            }
        }
    } while (swapped);

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;

    return result;
}

SortResult selectionSortLinked(LinkedList& list, const string& columnName) {
    SortResult result;

    int col = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0 || !list.head || !list.head->next) return result;

    Timer timer;
    timer.begin();
    size_t memStart = getUsedMemoryKB();

    for (Node* i = list.head; i; i = i->next) {
        Node* minNode = i;
        for (Node* j = i->next; j; j = j->next) {
            if (j->data[col] < minNode->data[col]) {
                minNode = j;
            }
        }
        if (minNode != i) {
            swap(i->data, minNode->data);
        }
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;

    return result;
}

SortResult insertionSortLinked(LinkedList& list, const string& columnName) {
    SortResult result;

    int col = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0 || !list.head || !list.head->next) return result;

    Timer timer;
    timer.begin();
    size_t memStart = getUsedMemoryKB();

    Node* sorted = list.head;
    Node* curr = sorted->next;
    sorted->prev = sorted->next = nullptr;

    while (curr) {
        Node* next = curr->next;
        curr->prev = curr->next = nullptr;

        if (curr->data[col] < sorted->data[col]) {
            curr->next = sorted;
            sorted->prev = curr;
            sorted = curr;
        } else {
            Node* p = sorted;
            while (p->next && p->next->data[col] <= curr->data[col]) {
                p = p->next;
            }
            curr->next = p->next;
            if (p->next) p->next->prev = curr;
            p->next = curr;
            curr->prev = p;
        }

        curr = next;
    }

    list.head = sorted;
    list.tail = sorted;
    while (list.tail->next) {
        list.tail = list.tail->next;
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;

    return result;
}

SortResult mergeSortLinked(LinkedList& list, const string& columnName) {
    SortResult result;

    int col = -1;
    for (int i = 0; i < list.x; ++i) {
        if (list.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0 || !list.head) return result;

    Timer timer;
    timer.begin();
    size_t memStart = getUsedMemoryKB();

    auto split = [](Node* head) -> Node* {
        Node* slow = head, *fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node* second = slow->next;
        slow->next = nullptr;
        if (second) second->prev = nullptr;
        return second;
    };

    function<Node*(Node*, Node*)> sortedMerge = [&](Node* a, Node* b) -> Node* {
        if (!a) return b;
        if (!b) return a;
        if (a->data[col] <= b->data[col]) {
            a->next = sortedMerge(a->next, b);
            if (a->next) a->next->prev = a;
            return a;
        } else {
            b->next = sortedMerge(a, b->next);
            if (b->next) b->next->prev = b;
            return b;
        }
    };

    function<Node*(Node*)> mergeSort = [&](Node* head) -> Node* {
        if (!head || !head->next) return head;
        Node* second = split(head);
        head = mergeSort(head);
        second = mergeSort(second);
        return sortedMerge(head, second);
    };

    list.head = mergeSort(list.head);

    Node* cur = list.head;
    Node* prev = nullptr;
    int count = 0;
    while (cur) {
        prev = cur;
        cur = cur->next;
        count++;
    }
    list.tail = prev;
    list.y = count;

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;

    return result;
}

//////////////////////////////////////////////////////////////////////// SEARCH ALGORITHM ////////////////////////////////////////////////////////////////////////

LinkedList LL_linearSearch(
    const LinkedList& input,
    const string& columnName,
    const string& searchValue,
    SearchResult& meta
) {
    auto start = chrono::high_resolution_clock::now();
    meta.memoryUsed = 0;
    size_t memStart = getUsedMemoryKB();

    LinkedList result;
    result.x = input.x;
    result.y = 0;

    // Copy field headers
    result.fieldHead = new string[result.x];
    for (int i = 0; i < result.x; ++i)
        result.fieldHead[i] = input.fieldHead[i];

    // Find the column index
    int col = -1;
    for (int i = 0; i < input.x; ++i) {
        if (input.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0) {
        meta.resultCount = 0;
        meta.timeMicroseconds = 0;
        return result;
    }

    // Search and copy matching nodes
    for (Node* cur = input.head; cur; cur = cur->next) {
        if (cur->data[col] == searchValue) {
            Node* newNode = new Node(result.x);
            for (int j = 0; j < result.x; ++j)
                newNode->data[j] = cur->data[j];

            if (!result.head) {
                result.head = result.tail = newNode;
            } else {
                result.tail->next = newNode;
                newNode->prev = result.tail;
                result.tail = newNode;
            }
            result.y++;
            meta.resultCount++;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    meta.timeMicroseconds = chrono::duration_cast<chrono::microseconds>(end - start).count();
    meta.memoryUsed = getUsedMemoryKB() - memStart;
    return result;
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
LinkedList LL_binarySearch(
    const LinkedList& input,
    const string& columnName,
    const string& searchValue,
    SearchResult& meta
) {
    auto start = chrono::high_resolution_clock::now();
    meta.memoryUsed = 0;
    size_t memStart = getUsedMemoryKB();

    LinkedList result;
    result.x = input.x;
    result.y = 0;

    // Copy headers
    result.fieldHead = new string[result.x];
    for (int i = 0; i < result.x; ++i)
        result.fieldHead[i] = input.fieldHead[i];

    // Get column index
    int col = -1;
    for (int i = 0; i < input.x; ++i) {
        if (input.fieldHead[i] == columnName) {
            col = i;
            break;
        }
    }
    if (col < 0) {
        meta.resultCount = 0;
        meta.timeMicroseconds = 0;
        return result;
    }

    // Middle node logic
    auto getMiddle = [&](Node* low, Node* high) {
        Node* slow = low;
        Node* fast = low;
        while (fast != high && fast->next != high) {
            fast = fast->next->next;
            slow = slow->next;
        }
        return slow;
    };

    Node* low = input.head;
    Node* high = input.tail;
    Node* found = nullptr;

    while (low && high) {
        // Ensure low ≤ high by checking reachability
        bool valid = false;
        for (Node* p = low; p; p = p->next) {
            if (p == high) { valid = true; break; }
        }
        if (!valid) break;

        Node* mid = getMiddle(low, high);
        if (!mid) break;

        if (mid->data[col] == searchValue) {
            found = mid;
            break;
        } else if (mid->data[col] < searchValue) {
            low = mid->next;
        } else {
            high = mid->prev;
        }
    }

    // Copy all matching nodes
    if (found) {
        Node* start = found;
        while (start->prev && start->prev->data[col] == searchValue)
            start = start->prev;

        for (Node* p = start; p && p->data[col] == searchValue; p = p->next) {
            Node* newNode = new Node(result.x);
            for (int j = 0; j < result.x; ++j)
                newNode->data[j] = p->data[j];

            if (!result.head) {
                result.head = result.tail = newNode;
            } else {
                result.tail->next = newNode;
                newNode->prev = result.tail;
                result.tail = newNode;
            }
            result.y++;
            meta.resultCount++;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    meta.timeMicroseconds = chrono::duration_cast<chrono::microseconds>(end - start).count();
    meta.memoryUsed = getUsedMemoryKB() - memStart;
    return result;
}

//////////////////////////////////////////////////////////////////////// Word Frequency Function ////////////////////////////////////////////////////////////////////////

/*
   SearchResult linearMeta;
LinkedList filteredLinear = LL_linearSearch(list, "Product ID", "PROD1234", linearMeta);
std::cout << "Linear search took " << linearMeta.timeMicroseconds << " µs and found "
          << linearMeta.resultCount << " rows.\n";
*/
WordFrequency countTopWordsFromLinkedList(const LinkedList& list,
    const string& columnName,
    int topN = 10) {
    WordFrequency wf;
    wf.size = 0;
    wf.capacity = topN;
    wf.words = new char*[topN];
    wf.counts = new int[topN];

    for (int i = 0; i < topN; ++i) {
    wf.words[i] = nullptr;
    wf.counts[i] = 0;
    }

    size_t memStart = getUsedMemoryKB() * 1024;
    auto start = chrono::high_resolution_clock::now();

    // Step 1: Find column index
    int col = -1;
    for (int i = 0; i < list.x; ++i) {
    if (list.fieldHead[i] == columnName) {
    col = i;
    break;
    }
    }
    if (col < 0) {
    wf.timeMicroseconds = 0;
    wf.memoryUsed = 0;
    return wf;
    }

    // Step 2: Count word frequencies
    unordered_map<string, int> freq;
    for (Node* cur = list.head; cur; cur = cur->next) {
    const string& s = cur->data[col];
    string token;
    for (unsigned char c : s) {
    if (isalnum(c)) token += tolower(c);
    else if (!token.empty()) {
    ++freq[token];
    token.clear();
    }
    }
    if (!token.empty()) ++freq[token];
    }

    // Step 3: Pick top N using repeated max scan
    struct Pair { string w; int c; };
    vector<Pair> picks;
    picks.reserve(topN);

    for (int count = 0; count < topN; ++count) 
    {
        string best;
        int bestC = 0;
        for (auto& kv : freq) 
        {
            if (kv.second > bestC) 
            {
                best = kv.first;
                bestC = kv.second;
            }
        }
    if (bestC == 0) break;
        picks.push_back({best, bestC});
        freq[best] = -1;
    }

    // Step 4: Copy into WordFrequency container
    wf.size = picks.size();
    for (int i = 0; i < wf.size; ++i) {
    wf.words[i] = strdup(picks[i].w.c_str());
    wf.counts[i] = picks[i].c;
    }

    auto end = chrono::high_resolution_clock::now();
    size_t memEnd = getUsedMemoryKB() * 1024;

    wf.timeMicroseconds = chrono::duration_cast<chrono::microseconds>(end - start).count();
    wf.memoryUsed = memEnd - memStart;

    return wf;
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