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

    size_t memStart = getUsedMemoryBytes();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 0; i < container.y - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < container.y - i - 1; ++j) {
            char** a = container.data[j];
            char** b = container.data[j + 1];
            bool shouldSwap = false;

            if (isDate) {
                int d1 = parseDate(a[fieldIndex]);
                int d2 = parseDate(b[fieldIndex]);
                shouldSwap = (ascending && d1 > d2) || (!ascending && d1 < d2);
            } else {
                int cmp = strcmp(a[fieldIndex], b[fieldIndex]);
                shouldSwap = (ascending && cmp > 0) || (!ascending && cmp < 0);
            }

            if (shouldSwap) {
                std::swap(container.data[j], container.data[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Early exit if sorted
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryBytes() - memStart;
    return result;
}
  // Selection Algorithm
  SortResult selectionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.memoryKBUsed = 0;

    size_t memStart = getUsedMemoryBytes();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 0; i < container.y - 1; ++i) {
        int selectedIdx = i;
        for (int j = i + 1; j < container.y; ++j) {
            bool shouldSelect = false;
            char** current = container.data[j];
            char** selected = container.data[selectedIdx];

            if (isDate) {
                int d1 = parseDate(current[fieldIndex]);
                int d2 = parseDate(selected[fieldIndex]);
                shouldSelect = (ascending && d1 < d2) || (!ascending && d1 > d2);
            } else {
                int cmp = strcmp(current[fieldIndex], selected[fieldIndex]);
                shouldSelect = (ascending && cmp < 0) || (!ascending && cmp > 0);
            }

            if (shouldSelect) selectedIdx = j;
        }

        if (selectedIdx != i) std::swap(container.data[i], container.data[selectedIdx]);
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryBytes() - memStart;
    return result;
}
// Insertion Algorithm
SortResult insertionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.memoryKBUsed = 0;

    size_t memStart = getUsedMemoryBytes();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 1; i < container.y; ++i) {
        char** key = container.data[i];
        int j = i - 1;

        while (j >= 0) {
            bool move = false;
            char** current = container.data[j];

            if (isDate) {
                int d1 = parseDate(current[fieldIndex]);
                int d2 = parseDate(key[fieldIndex]);
                move = (ascending && d1 > d2) || (!ascending && d1 < d2);
            } else {
                int cmp = strcmp(current[fieldIndex], key[fieldIndex]);
                move = (ascending && cmp > 0) || (!ascending && cmp < 0);
            }

            if (!move) break;

            container.data[j + 1] = container.data[j];
            --j;
        }

        container.data[j + 1] = key;
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryBytes() - memStart;
    return result;
} 
// Merge Algorithm
void merge(char*** data, int left, int mid, int right, int fieldIndex, bool ascending, bool isDate, char*** temp) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        bool takeLeft = false;
        if (isDate) {
            int d1 = parseDate(data[i][fieldIndex]);
            int d2 = parseDate(data[j][fieldIndex]);
            takeLeft = (ascending && d1 <= d2) || (!ascending && d1 >= d2);
        } else {
            int cmp = strcmp(data[i][fieldIndex], data[j][fieldIndex]);
            takeLeft = (ascending && cmp <= 0) || (!ascending && cmp >= 0);
        }
        temp[k++] = takeLeft ? data[i++] : data[j++];
    }

    while (i <= mid) temp[k++] = data[i++];
    while (j <= right) temp[k++] = data[j++];

    for (int idx = left; idx <= right; ++idx) {
        data[idx] = temp[idx];
    }
}

void mergeSortHelper(char*** data, int left, int right, int fieldIndex, bool ascending, bool isDate, char*** temp) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortHelper(data, left, mid, fieldIndex, ascending, isDate, temp);
    mergeSortHelper(data, mid + 1, right, fieldIndex, ascending, isDate, temp);
    merge(data, left, mid, right, fieldIndex, ascending, isDate, temp);
}

SortResult mergeSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.memoryKBUsed = 0;

    size_t memStart = getUsedMemoryBytes();
    Timer timer; timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);
    char*** temp = new char**[container.y];
    mergeSortHelper(container.data, 0, container.y - 1, fieldIndex, ascending, isDate, temp);
    delete[] temp;

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.memoryKBUsed = getUsedMemoryBytes()- memStart;
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

dataContainer2D writeNewLines(dataContainer2D dc, const char* newValues[], int recordLen, InsDelResult& result) {

    result.memory = 0;
    result.time = 0;

    Timer timer; timer.begin();
    size_t memStart = getUsedMemoryKB();

    // 1) Validation: must match number of columns
    if (recordLen != dc.x) {
        std::cerr << "Error: expected " << dc.x
                  << " values but got " << recordLen << "\n";
        dc.error = -1;
        return dc;
    }

    // 2) Allocate new array of row-pointers, size y+1
    char*** newData = new char**[dc.y + 1];

    // 3) Copy existing row pointers
    for (int i = 0; i < dc.y; ++i) {
        newData[i] = dc.data[i];
    }

    // 4) Allocate space for the new row
    char** newRow = new char*[dc.x];
    for (int j = 0; j < dc.x; ++j) {
        // duplicate each string
        newRow[j] = strdup(newValues[j]);
    }
    newData[dc.y] = newRow;

    // 5) Clean up old “data” array (but not the row contents)
    delete[] dc.data;

    // 6) Update container and return
    dc.data = newData;
    dc.y  += 1;

    result.memory = getUsedMemoryKB() - memStart;
    timer.finish();
    result.time = timer.getDurationMicroseconds();

    return dc;
}

dataContainer2D writeNewTopLines(dataContainer2D dc,
                              const char* newValues[],
                              int recordLen,
                              InsDelResult& result)
{
    result.memory = 0;
    result.time   = 0;

    Timer timer; timer.begin();
    size_t memStart = getUsedMemoryKB();

    // 1) Validation: must match number of columns
    if (recordLen != dc.x) {
        std::cerr << "Error: expected " << dc.x
                  << " values but got " << recordLen << "\n";
        dc.error = -1;
        return dc;
    }

    // 2) Allocate new array of row‑pointers, size = old rows + 1
    char*** newData = new char**[dc.y + 1];

    // 3) Allocate & fill the new row
    char** newRow = new char*[dc.x];
    for (int j = 0; j < dc.x; ++j) {
        newRow[j] = strdup(newValues[j]);  // duplicate each C‑string
    }
    // 4) Insert the new row at the top
    newData[0] = newRow;

    // 5) Shift existing rows down by one
    for (int i = 0; i < dc.y; ++i) {
        newData[i + 1] = dc.data[i];
    }

    // 6) Clean up old “data” array (but not the row contents)
    delete[] dc.data;

    // 7) Update container
    dc.data = newData;
    dc.y   += 1;

    result.memory = getUsedMemoryKB() - memStart;
    timer.finish();
    result.time = timer.getDurationMicroseconds();

    return dc;
}

///////////////////////////////////// Delete function /////////////////////////////////////
dataContainer2D deleteRecord(dataContainer2D dc, const char* columnName, const char* key) {

    // 1) find column index
    int colIdx = -1;
    for (int j = 0; j < dc.x; ++j) {
        if (std::strcmp(dc.fields[j], columnName) == 0) {
            colIdx = j;
            break;
        }
    }
    if (colIdx < 0) {
        std::cerr << "Error: column \"" << columnName << "\" not found.\n";
        dc.error = -1;
        return dc;
    }

    // 2) locate the row to delete
    int delRow = -1;
    for (int i = 0; i < dc.y; ++i) {
        if (std::strcmp(dc.data[i][colIdx], key) == 0) {
            delRow = i;
            break;
        }
    }
    if (delRow < 0) {
        std::cerr << "Error: no record with " << columnName << " == \"" << key << "\".\n";
        dc.error = 1;
        return dc;
    }

    // 3) free the contents of that row
    for (int j = 0; j < dc.x; ++j) {
        free(dc.data[delRow][j]);   // each cell was strdup'd
    }
    delete[] dc.data[delRow];

    // 4) build a new data-pointer-array of size y-1
    char*** newData = new char**[dc.y - 1];
    for (int i = 0, k = 0; i < dc.y; ++i) {
        if (i == delRow) continue;
        newData[k++] = dc.data[i];
    }

    // 5) delete old pointer array and update container
    delete[] dc.data;
    dc.data = newData;
    dc.y  -= 1;

    return dc;
}

dataContainer2D deleteAllRecords(dataContainer2D dc, const char* columnName, const char* key, InsDelResult& result) {

    result.memory = 0;
    result.time = 0;

    Timer timer; timer.begin();
    size_t memStart = getUsedMemoryKB();

    // 1) find column index
    int colIdx = -1;
    for (int j = 0; j < dc.x; ++j) {
        if (std::strcmp(dc.fields[j], columnName) == 0) {
            colIdx = j;
            break;
        }
    }
    if (colIdx < 0) {
        std::cerr << "Error: column \"" << columnName << "\" not found.\n";
        dc.error = -1;
        return dc;
    }

    // 2) count how many rows match
    int matchCount = 0;
    for (int i = 0; i < dc.y; ++i) {
        if (std::strcmp(dc.data[i][colIdx], key) == 0) {
            ++matchCount;
        }
    }
    if (matchCount == 0) {
        std::cerr << "Error: no records with " << columnName 
                  << " == \"" << key << "\" found.\n";
        dc.error = 1;
        return dc;
    }

    // 3) build a new data-pointer-array of size y - matchCount
    int newY = dc.y - matchCount;
    char*** newData = new char**[newY];

    // 4) copy over non-matching rows, free matching ones
    for (int i = 0, k = 0; i < dc.y; ++i) {
        if (std::strcmp(dc.data[i][colIdx], key) == 0) {
            // free this row
            for (int j = 0; j < dc.x; ++j) {
                free(dc.data[i][j]); 
            }
            delete[] dc.data[i];
        } else {
            newData[k++] = dc.data[i];
        }
    }

    // 5) delete old pointer array, update container
    delete[] dc.data;
    dc.data = newData;
    dc.y    = newY;

    result.memory = getUsedMemoryKB() - memStart;
    timer.finish();
    result.time = timer.getDurationMicroseconds();
    
    return dc;
}