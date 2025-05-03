 ///////////////////////////////////// Array file /////////////////////////////////////////////////
#include "common_function.hpp"

///////////////////////////////////// Date Helper Functions ///////////////////////////////////// 
bool isDateFormat(const char* str) {
    return strlen(str) == 10 && str[2] == '/' && str[5] == '/';
}
int parseDate(const char* str) {
    int day, month, year;
    sscanf(str, "%d/%d/%d", &day, &month, &year);
    return year * 10000 + month * 100 + day;
}

dataContainer2D cloneContainer2D(const dataContainer2D& original) {
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

void WFsort(WordFrequency& freq) {
    // Simple bubble sort or you can use std::vector with std::sort for efficiency
    for (int i = 0; i < freq.size - 1; ++i) {
        for (int j = 0; j < freq.size - i - 1; ++j) {
            if (freq.counts[j] < freq.counts[j + 1]) {
                // Swap counts
                std::swap(freq.counts[j], freq.counts[j + 1]);
                // Swap corresponding words
                std::swap(freq.words[j], freq.words[j + 1]);
            }
        }
    }
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

///////////////////////////////////// Sort Algorithm /////////////////////////////////////

 // Bubble Algorithm
 SortResult bubbleSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.memoryKBUsed = 0;

    size_t memStart = getUsedMemoryKB();
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
            }
        }
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;
    return result;
}

  // Selection Algorithm
  SortResult selectionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.memoryKBUsed = 0;

    size_t memStart = getUsedMemoryKB();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 0; i < container.y - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < container.y; ++j) {
            bool shouldSelect = false;

            if (isDate) {
                int d1 = parseDate(container.data[j][fieldIndex]);
                int d2 = parseDate(container.data[minIdx][fieldIndex]);
                shouldSelect = (ascending && d1 < d2) || (!ascending && d1 > d2);
            } else {
                int cmp = strcmp(container.data[j][fieldIndex], container.data[minIdx][fieldIndex]);
                shouldSelect = (ascending && cmp < 0) || (!ascending && cmp > 0);
            }

            if (shouldSelect) {
                minIdx = j;
            }
        }

        if (minIdx != i) {
            std::swap(container.data[i], container.data[minIdx]);
        }
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;
    return result;
}
 // Insertion Algorithm
 SortResult insertionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.memoryKBUsed = 0;

    size_t memStart = getUsedMemoryKB();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 1; i < container.y; ++i) {
        char** key = container.data[i];
        int j = i - 1;

        while (j >= 0) {
            bool move = false;

            if (isDate) {
                int d1 = parseDate(container.data[j][fieldIndex]);
                int d2 = parseDate(key[fieldIndex]);
                move = (ascending && d1 > d2) || (!ascending && d1 < d2);
            } else {
                int cmp = strcmp(container.data[j][fieldIndex], key[fieldIndex]);
                move = (ascending && cmp > 0) || (!ascending && cmp < 0);
            }

            if (move) {
                container.data[j + 1] = container.data[j];
                --j;
            } else {
                break;
            }
        }
        container.data[j + 1] = key;
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;
    return result;
}
 // Merge Algorithm
 void merge(char*** data, int left, int mid, int right, int fieldIndex, bool ascending, bool isDate) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    char*** L = new char**[n1];
    char*** R = new char**[n2];

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
    }

    while (i < n1) data[k++] = L[i++];
    while (j < n2) data[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSortHelper(char*** data, int left, int right, int fieldIndex, bool ascending, bool isDate) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(data, left, mid, fieldIndex, ascending, isDate);
        mergeSortHelper(data, mid + 1, right, fieldIndex, ascending, isDate);
        merge(data, left, mid, right, fieldIndex, ascending, isDate);
    }
}

SortResult mergeSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.memoryKBUsed = 0;

    size_t memStart = getUsedMemoryKB();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);
    mergeSortHelper(container.data, 0, container.y - 1, fieldIndex, ascending, isDate);

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryKB() - memStart;
    return result;
}

 ///////////////////////////////////// Search Algorithm /////////////////////////////////////


 dataContainer2D Array_LinearSearch(
    const dataContainer2D& input,
    int fieldX, const char* valueX,
    SearchResult& meta
) {
    auto start = std::chrono::high_resolution_clock::now();
    meta.memoryUsed = 0;
    size_t memStart = getUsedMemoryKB();


    dataContainer2D result;
    result.x = input.x;
    result.y = 0;
    result.fields = new char*[input.x];
    for (int i = 0; i < input.x; ++i) {
        result.fields[i] = strdup(input.fields[i]);
    }
    result.data = new char**[input.y];

    for (int i = 0; i < input.y; ++i) {
        if (strcmp(input.data[i][fieldX], valueX) == 0) {
            char** row = new char*[input.x];
            for (int j = 0; j < input.x; ++j) {
                row[j] = strdup(input.data[i][j]);
            }
            result.data[result.y++] = row;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    meta.timeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    meta.resultCount = result.y;
    meta.memoryUsed = getUsedMemoryKB() - memStart;

    return result;
}

// Binary search (sorted field only)
// Assumes the field is sorted in ascending order
// Returns all exact matches
dataContainer2D Array_BinarySearch(
    const dataContainer2D& input,
    int fieldX, const char* valueX,
    SearchResult& meta
) {
    auto start = std::chrono::high_resolution_clock::now();
    meta.memoryUsed = 0;
    size_t memStart = getUsedMemoryKB();

    dataContainer2D result;
    result.x = input.x;
    result.y = 0;
    result.fields = new char*[input.x];
    for (int i = 0; i < input.x; ++i) {
        result.fields[i] = strdup(input.fields[i]);
    }
    result.data = new char**[input.y];

    int low = 0, high = input.y - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(input.data[mid][fieldX], valueX);

        if (cmp == 0) {
            // Walk backward to first match
            int i = mid;
            while (i >= 0 && strcmp(input.data[i][fieldX], valueX) == 0) {
                i--;
            }
            i++;  // Move to the first matching row

            // Walk forward and collect all matches
            while (i < input.y && strcmp(input.data[i][fieldX], valueX) == 0) {
                char** row = new char*[input.x];
                for (int j = 0; j < input.x; ++j) {
                    row[j] = strdup(input.data[i][j]);
                }
                result.data[result.y++] = row;
                i++;
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
    meta.resultCount = result.y;
    meta.memoryUsed = getUsedMemoryKB() - memStart;

    return result;
}

// SearchResult meta;
// dataContainer2D filtered = linearSearchOneField(container, 2, "Credit Card", meta);

// std::cout << "Time: " << meta.timeMicroseconds << " µs\n";
// std::cout << "Matches found: " << meta.resultCount << "\n";
// std::cout << "Rows in result: " << filtered.y << "\n";


///////////////////////////////////// frequency function /////////////////////////////////////
WordFrequency getWordFrequencyArray(const dataContainer2D& container, int fieldIndex) {
    WordFrequency freq;
    freq.size = 0;
    freq.capacity = 100;
    freq.words = new char*[freq.capacity];
    freq.counts = new int[freq.capacity];

    for (int i = 0; i < freq.capacity; ++i) {
        freq.words[i] = nullptr;
        freq.counts[i] = 0;
    }

    size_t memoryStart = getUsedMemoryKB();  // convert to bytes
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < container.y; ++i) {
        const char* line = container.data[i][fieldIndex];
        std::string word;

        for (int j = 0; line[j] != '\0'; ++j) {
            if (isalnum(line[j])) {
                word += tolower(line[j]);
            } else if (!word.empty()) {
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

        // Handle trailing word
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

    auto end = std::chrono::high_resolution_clock::now();
    size_t memoryEnd = getUsedMemoryKB();

    freq.timeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    freq.memoryUsed = memoryEnd - memoryStart;

    return freq;
}

// WordFrequency wf = countWordsFromField(container, 5);  // 5 = index of review/comment field
// cout << "Top 10 words:\n";
// for (int i = 0; i < std::min(10, wf.size); ++i) {
//     cout << wf.words[i] << ": " << wf.counts[i] << endl;
// }
// cout << "Time taken: " << wf.timeMicroseconds << " μs\n";
// cout << "Estimated memory: " << wf.manualEstimatedBytes << " bytes\n";

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















