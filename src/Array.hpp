 ///////////////////////////////////// Array file for sort algorithm and search algorithm /////////////////////////////////////

 #include <chrono>      // For timing
#include <cstring>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>     // For strcmp
#include <iostream>    // For input/output
#include <iomanip>     // Optional: pretty printing
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define byte win_byte_override
#include <windows.h>
#undef byte
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

///////////////////////////////////// Array Data Structure /////////////////////////////////////
struct dataContainer2D {
    int error = 0;
    char** fields = nullptr;     // 1D array: fields
    char*** data = nullptr;      // 2D array: data (rows and columns)
    int x = 0;                   // number of columns
    int y = 0;                   // number of rows
};

struct dataContainer1D {
    int error = 0;
    char** fields = nullptr; // 1D array: fields
    char** data = nullptr;   // 1D array: data
    int x = 0;
};

///////////////////////////////////// SearchResult Struct /////////////////////////////////////
struct SearchResult {
    long long timeMicroseconds = 0;
    int comparisons = 0;
    int resultCount = 0;
};
// 1-field linear search
// Matches fieldX == valueX
// Returns a new container with matching rows

///////////////////////////////////// SortResult Struct /////////////////////////////////////
struct SortResult {
    long long timeMicroseconds;
    int swapCount;
    size_t manualEstimatedBytes = 0;
    size_t memoryKBStart = 0;
    size_t memoryKBEnd = 0;
    size_t memoryKBUsed = 0;
};

///////////////////////////////////// frequency Struct /////////////////////////////////////
struct WordFrequency {
    char** words;    // array of unique words
    int* counts;     // corresponding frequency for each word
    int size;        // current number of unique words stored
    int capacity;    // current max capacity before resizing
};
///////////////////////////////////// Windows Memory Tracker /////////////////////////////////////
size_t getUsedMemoryKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

///////////////////////////////////// Timer Function /////////////////////////////////////
struct Timer {
    std::chrono::steady_clock::time_point start, end;

    void begin() { start = std::chrono::steady_clock::now(); }
    void finish() { end = std::chrono::steady_clock::now(); }

    long long getDurationMicroseconds() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
};
///////////////////////////////////// Date Helper Functions ///////////////////////////////////// 
bool isDateFormat(const char* str) {
    return strlen(str) == 10 && str[2] == '/' && str[5] == '/';
}
int parseDate(const char* str) {
    int day, month, year;
    sscanf(str, "%d/%d/%d", &day, &month, &year);
    return year * 10000 + month * 100 + day;
}

dataContainer2D cloneContainer(const dataContainer2D& original) {
    dataContainer2D copy;
    copy.x = original.x;
    copy.y = original.y;
    copy.error = original.error;

    // Copy fields
    copy.fields = new char*[copy.x];
    for (int i = 0; i < copy.x; ++i) {
        copy.fields[i] = strdup(original.fields[i]);  // duplicate string
    }

    // Copy data (deep copy)
    copy.data = new char**[copy.y];
    for (int i = 0; i < copy.y; ++i) {
        copy.data[i] = new char*[copy.x];
        for (int j = 0; j < copy.x; ++j) {
            copy.data[i][j] = strdup(original.data[i][j]);  // duplicate each string
        }
    }

    return copy;
}

void freeFrequencyContainer(WordFrequency& wf) {
    for (int i = 0; i < wf.size; ++i) {
        free(wf.words[i]); // free each word string
    }
    delete[] wf.words;
    delete[] wf.counts;
    wf.words = nullptr;
    wf.counts = nullptr;
    wf.size = 0;
    wf.capacity = 0;
}
///////////////////////////////////// Read function /////////////////////////////////////////////
char** splitCustom(const std::string& line, int expectedFields, int& count) {
    char** tokens = new char*[expectedFields];
    for (int i = 0; i < expectedFields; ++i) tokens[i] = nullptr;

    std::string current;
    bool insideQuotes = false;
    int tokenIndex = 0;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes) {
            tokens[tokenIndex++] = strdup(current.c_str());
            current.clear();
            if (tokenIndex >= expectedFields - 1) {
                // After expected fields-1 commas, put the rest into last field
                tokens[tokenIndex] = strdup(line.substr(i + 1).c_str());
                count = expectedFields;
                return tokens;
            }
        } else {
            current += c;
        }
    }

    tokens[tokenIndex] = strdup(current.c_str());
    count = tokenIndex + 1;
    return tokens;
}

dataContainer2D getData(const std::string& filename) {
    
    dataContainer2D container;
    fs::path filePath = fs::current_path().parent_path() / "data" / filename;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        container.error = 1;
        return container;
    }

    std::string buffer;
    bool firstLine = true;
    int rowCount = 0;
    int rowCapacity = 10;
    int expectedFields = 0;

    container.data = new char**[rowCapacity];

    while (std::getline(file, buffer)) {
        if (buffer.empty()) continue;

        int count = 0;

        if (firstLine) {
            // Count how many commas in header
            expectedFields = 1; // at least one field
            for (char c : buffer) {
                if (c == ',') expectedFields++;
            }
            container.fields = splitCustom(buffer, expectedFields, count);
            container.x = count;
            firstLine = false;
        } else {
            if (rowCount >= rowCapacity) {
                int newCapacity = rowCapacity * 2;
                char*** newData = new char**[newCapacity];
                for (int i = 0; i < rowCount; ++i) {
                    newData[i] = container.data[i];
                }
                delete[] container.data;
                container.data = newData;
                rowCapacity = newCapacity;
            }
            container.data[rowCount++] = splitCustom(buffer, expectedFields, count);
        }
    }

    container.y = rowCount;
    file.close();

    return container;
}

///////////////////////////////////// Free Memory /////////////////////////////////////////////
void freeContainer(dataContainer2D& container) {
    if (container.fields != nullptr) {
        for (int i = 0; i < container.x; ++i) {
            if (container.fields[i] != nullptr) {
                free(container.fields[i]);
            }
        }
        delete[] container.fields;
        container.fields = nullptr;
    }

    if (container.data != nullptr) {
        for (int i = 0; i < container.y; ++i) {
            if (container.data[i] != nullptr) {
                for (int j = 0; j < container.x; ++j) {
                    if (container.data[i][j] != nullptr) {
                        free(container.data[i][j]);
                    }
                }
                delete[] container.data[i];
            }
        }
        delete[] container.data;
        container.data = nullptr;
    }

    container.x = 0;
    container.y = 0;
}
/**
 * string filename = "filepath";
 * 
 * dataContainer2D container = getData(filename);
 * container.error = 0; // 0: no error, 1: error
 * container.x = number of columns
 * container.y = number of rows
 * container.fields[n] = array of column names
 * container.data[n][n] = array of rows
 * 
 * 
 * freeContainer(container);
 */

///////////////////////////////////// Sort Algorithm /////////////////////////////////////

 // Sort Algorithm
SortResult bubbleSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.swapCount = 0;

    result.memoryKBStart = getUsedMemoryKB();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 0; i < container.y - 1; ++i) {
        for (int j = 0; j < container.y - i - 1; ++j) {
            bool shouldSwap = false;

            if (isDate) {
                int d1 = parseDate(container.data[j][fieldIndex]);
                int d2 = parseDate(container.data[j + 1][fieldIndex]);
                shouldSwap = (ascending && d1 > d2) || (!ascending && d1 < d2);
            } else {
                int cmp = strcmp(container.data[j][fieldIndex], container.data[j + 1][fieldIndex]);
                shouldSwap = (ascending && cmp > 0) || (!ascending && cmp < 0);
            }

            if (shouldSwap) {
                std::swap(container.data[j], container.data[j + 1]);
                result.swapCount++;
            }
        }
    }

    timer.finish();
    result.memoryKBEnd = getUsedMemoryKB();
    result.memoryKBUsed = result.memoryKBEnd - result.memoryKBStart;
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.manualEstimatedBytes = 0;

    return result;
}
  // Selection Algorithm
SortResult selectionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.swapCount = 0;

    result.memoryKBStart = getUsedMemoryKB();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 0; i < container.y - 1; ++i) {
        int selected = i;
        for (int j = i + 1; j < container.y; ++j) {
            bool better = false;

            if (isDate) {
                int d1 = parseDate(container.data[j][fieldIndex]);
                int d2 = parseDate(container.data[selected][fieldIndex]);
                better = (ascending && d1 < d2) || (!ascending && d1 > d2);
            } else {
                int cmp = strcmp(container.data[j][fieldIndex], container.data[selected][fieldIndex]);
                better = (ascending && cmp < 0) || (!ascending && cmp > 0);
            }

            if (better) selected = j;
        }

        if (selected != i) {
            std::swap(container.data[i], container.data[selected]);
            result.swapCount++;
        }
    }

    timer.finish();
    result.memoryKBEnd = getUsedMemoryKB();
    result.memoryKBUsed = result.memoryKBEnd - result.memoryKBStart;
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.manualEstimatedBytes = 0;

    return result;
}
 // Insertion Algorithm
SortResult insertionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.swapCount = 0;

    result.memoryKBStart = getUsedMemoryKB();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 1; i < container.y; ++i) {
        char** keyRow = container.data[i];
        int j = i - 1;

        while (j >= 0) {
            bool move = false;

            if (isDate) {
                int d1 = parseDate(container.data[j][fieldIndex]);
                int d2 = parseDate(keyRow[fieldIndex]);
                move = (ascending && d1 > d2) || (!ascending && d1 < d2);
            } else {
                int cmp = strcmp(container.data[j][fieldIndex], keyRow[fieldIndex]);
                move = (ascending && cmp > 0) || (!ascending && cmp < 0);
            }

            if (move) {
                container.data[j + 1] = container.data[j];
                result.swapCount++;
                j--;
            } else {
                break;
            }
        }
        container.data[j + 1] = keyRow;
    }

    timer.finish();
    result.memoryKBEnd = getUsedMemoryKB();
    result.memoryKBUsed = result.memoryKBEnd - result.memoryKBStart;
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.manualEstimatedBytes = 0;

    return result;
}
 // Merge Algorithm
void merge(char*** data, int left, int mid, int right, int fieldIndex, bool ascending, bool isDate, int x, int& swapCount, size_t& manualMemory) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    char*** L = new char**[n1];
    char*** R = new char**[n2];
    manualMemory += sizeof(char**) * (n1 + n2);

    for (int i = 0; i < n1; ++i) L[i] = data[left + i];
    for (int i = 0; i < n2; ++i) R[i] = data[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        bool takeLeft;
        if (isDate) {
            int d1 = parseDate(L[i][fieldIndex]);
            int d2 = parseDate(R[j][fieldIndex]);
            takeLeft = (ascending && d1 <= d2) || (!ascending && d1 >= d2);
        } else {
            int cmp = strcmp(L[i][fieldIndex], R[j][fieldIndex]);
            takeLeft = (ascending && cmp <= 0) || (!ascending && cmp >= 0);
        }

        data[k++] = takeLeft ? L[i++] : R[j++];
        swapCount++;
    }

    while (i < n1) data[k++] = L[i++];
    while (j < n2) data[k++] = R[j++];

    delete[] L;
    delete[] R;
}
void mergeSortHelper(char*** data, int left, int right, int fieldIndex, bool ascending, bool isDate, int x, int& swapCount, size_t& manualMemory) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(data, left, mid, fieldIndex, ascending, isDate, x, swapCount, manualMemory);
        mergeSortHelper(data, mid + 1, right, fieldIndex, ascending, isDate, x, swapCount, manualMemory);
        merge(data, left, mid, right, fieldIndex, ascending, isDate, x, swapCount, manualMemory);
    }
}
SortResult mergeSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.swapCount = 0;
    result.manualEstimatedBytes = 0;

    result.memoryKBStart = getUsedMemoryKB();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);
    mergeSortHelper(container.data, 0, container.y - 1, fieldIndex, ascending, isDate, container.x, result.swapCount, result.manualEstimatedBytes);

    timer.finish();
    result.memoryKBEnd = getUsedMemoryKB();
    result.memoryKBUsed = result.memoryKBEnd - result.memoryKBStart;
    result.timeMicroseconds = timer.getDurationMicroseconds();

    return result;
}

 ///////////////////////////////////// Search Algorithm /////////////////////////////////////


dataContainer2D linearSearchOneField(
    const dataContainer2D& input,
    int fieldX, const char* valueX,
    SearchResult& meta
) {
    auto start = std::chrono::high_resolution_clock::now();

    dataContainer2D result;
    result.fields = new char*[input.x];
    for (int i = 0; i < input.x; ++i) {
        result.fields[i] = strdup(input.fields[i]);
    }
    result.x = input.x;
    result.y = 0;
    result.data = new char**[input.y];

    for (int i = 0; i < input.y; ++i) {
        meta.comparisons++;
        if (strcmp(input.data[i][fieldX], valueX) == 0) {
            char** row = new char*[input.x];
            for (int j = 0; j < input.x; ++j) {
                row[j] = strdup(input.data[i][j]);
            }
            result.data[result.y++] = row;
            meta.resultCount++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    meta.timeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return result;
}

// 2-field linear search
// Matches field1 == value1 && field2 == value2

dataContainer2D linearSearchTwoFields(
    const dataContainer2D& input,
    int field1, const char* value1,
    int field2, const char* value2,
    SearchResult& meta
) {
    auto start = std::chrono::high_resolution_clock::now();

    dataContainer2D result;
    result.fields = new char*[input.x];
    for (int i = 0; i < input.x; ++i) {
        result.fields[i] = strdup(input.fields[i]);
    }
    result.x = input.x;
    result.y = 0;
    result.data = new char**[input.y];

    for (int i = 0; i < input.y; ++i) {
        meta.comparisons++;
        if (strcmp(input.data[i][field1], value1) == 0 && strcmp(input.data[i][field2], value2) == 0) {
            char** row = new char*[input.x];
            for (int j = 0; j < input.x; ++j) {
                row[j] = strdup(input.data[i][j]);
            }
            result.data[result.y++] = row;
            meta.resultCount++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    meta.timeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return result;
}

// Binary search (sorted field only)
// Assumes the field is sorted in ascending order
// Returns all exact matches

dataContainer2D binarySearchOneField(
    const dataContainer2D& input,
    int fieldX, const char* valueX,
    SearchResult& meta
) {
    auto start = std::chrono::high_resolution_clock::now();

    dataContainer2D result;
    result.fields = new char*[input.x];
    for (int i = 0; i < input.x; ++i) {
        result.fields[i] = strdup(input.fields[i]);
    }
    result.x = input.x;
    result.y = 0;
    result.data = new char**[input.y];

    int low = 0;
    int high = input.y - 1;
    bool found = false;

    while (low <= high) {
        meta.comparisons++;
        int mid = (low + high) / 2;
        int cmp = strcmp(input.data[mid][fieldX], valueX);

        if (cmp == 0) {
            found = true;
            // Collect all matching rows (check neighbors)
            int i = mid;
            while (i >= 0 && strcmp(input.data[i][fieldX], valueX) == 0) {
                --i;
            }
            ++i;
            while (i < input.y && strcmp(input.data[i][fieldX], valueX) == 0) {
                char** row = new char*[input.x];
                for (int j = 0; j < input.x; ++j) {
                    row[j] = strdup(input.data[i][j]);
                }
                result.data[result.y++] = row;
                meta.resultCount++;
                ++i;
            }
            break;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    meta.timeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return result;
}

///////////////////////////////////// frequency function /////////////////////////////////////
WordFrequency countWordsFromField(const dataContainer2D& container, int fieldIndex) {
    WordFrequency freq;
    freq.size = 0;
    freq.capacity = 100; // initial space
    freq.words = new char*[freq.capacity];
    freq.counts = new int[freq.capacity];

    for (int i = 0; i < freq.capacity; ++i) {
        freq.words[i] = nullptr;
        freq.counts[i] = 0;
    }

    for (int i = 0; i < container.y; ++i) {
        const char* line = container.data[i][fieldIndex];
        std::string word;
        
        for (int j = 0; line[j] != '\0'; ++j) {
            if (isalnum(line[j])) {
                word += tolower(line[j]);
            } else if (!word.empty()) {
                // Add the word to the frequency list
                bool found = false;
                for (int k = 0; k < freq.size; ++k) {
                    if (strcmp(freq.words[k], word.c_str()) == 0) {
                        freq.counts[k]++;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    // Resize if needed
                    if (freq.size >= freq.capacity) {
                        int newCapacity = freq.capacity * 2;
                        char** newWords = new char*[newCapacity];
                        int* newCounts = new int[newCapacity];

                        for (int k = 0; k < freq.size; ++k) {
                            newWords[k] = freq.words[k];
                            newCounts[k] = freq.counts[k];
                        }
                        delete[] freq.words;
                        delete[] freq.counts;

                        freq.words = newWords;
                        freq.counts = newCounts;
                        freq.capacity = newCapacity;
                    }

                    freq.words[freq.size] = strdup(word.c_str());
                    freq.counts[freq.size] = 1;
                    freq.size++;
                }

                word.clear();
            }
        }

        // Catch last word if line doesn't end in punctuation
        if (!word.empty()) {
            bool found = false;
            for (int k = 0; k < freq.size; ++k) {
                if (strcmp(freq.words[k], word.c_str()) == 0) {
                    freq.counts[k]++;
                    found = true;
                    break;
                }
            }

            if (!found) {
                if (freq.size >= freq.capacity) {
                    int newCapacity = freq.capacity * 2;
                    char** newWords = new char*[newCapacity];
                    int* newCounts = new int[newCapacity];

                    for (int k = 0; k < freq.size; ++k) {
                        newWords[k] = freq.words[k];
                        newCounts[k] = freq.counts[k];
                    }
                    delete[] freq.words;
                    delete[] freq.counts;

                    freq.words = newWords;
                    freq.counts = newCounts;
                    freq.capacity = newCapacity;
                }

                freq.words[freq.size] = strdup(word.c_str());
                freq.counts[freq.size] = 1;
                freq.size++;
            }
        }
    }

    return freq;
}

///////////////////////////////////// Insert function /////////////////////////////////////
void insertRow(dataContainer2D& container, char** newRow) {
    if (!newRow) return;

    // Allocate new data array with +1 row
    char*** newData = new char**[container.y + 1];

    // Copy old data
    for (int i = 0; i < container.y; ++i) {
        newData[i] = container.data[i];
    }

    // Assign the new row
    newData[container.y] = new char*[container.x];
    for (int j = 0; j < container.x; ++j) {
        newData[container.y][j] = strdup(newRow[j]); // deep copy
    }

    // Clean up old data pointer (not individual rows)
    delete[] container.data;

    // Update container
    container.data = newData;
    container.y++;
}

///////////////////////////////////// Delete function /////////////////////////////////////
bool deleteRowByValue(dataContainer2D& container, int fieldIndex, const char* value) {
    if (fieldIndex < 0 || fieldIndex >= container.x) return false;

    for (int i = 0; i < container.y; ++i) {
        if (strcmp(container.data[i][fieldIndex], value) == 0) {
            // Free memory for the row
            for (int j = 0; j < container.x; ++j) {
                free(container.data[i][j]);
            }
            delete[] container.data[i];

            // Shift remaining rows up
            for (int k = i + 1; k < container.y; ++k) {
                container.data[k - 1] = container.data[k];
            }

            container.y--;
            return true; // only delete first match
        }
    }
    return false; // not found
}

/*
// 1. Create a new row
char** newRow = new char*[cleaned_review_data.x];
newRow[0] = strdup("CUST0000");
newRow[1] = strdup("Mouse");
newRow[2] = strdup("Books");
newRow[3] = strdup("999.99");
newRow[4] = strdup("30/04/2024");
newRow[5] = strdup("Credit Card");

// 2. Insert into container
insertRow(cleaned_review_data, newRow);

// 3. Delete row by value
deleteRowByValue(cleaned_review_data, 0, "CUST4434");

// Don't forget to free newRow to avoid memory leak
for (int i = 0; i < cleaned_review_data.x; ++i) {
    free(newRow[i]);
}
delete[] newRow;
*/















