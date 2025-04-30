 ///////////////////////////////////// Array file for sort algorithm and search algorithm /////////////////////////////////////

 #include <chrono>      // For timing
#include <cstring>     // For strcmp
#include <iostream>    // For input/output
#include <iomanip>     // Optional: pretty printing

struct SortResult {
    long long timeMicroseconds; // Time taken
    int swapCount;              // Swaps performed
};

struct Timer {
    std::chrono::steady_clock::time_point start, end;

    void begin() { start = std::chrono::steady_clock::now(); }
    void finish() { end = std::chrono::steady_clock::now(); }

    long long getDurationMicroseconds() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
};

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

 ///////////////////////////////////// Sort Algorithm /////////////////////////////////////

 // Sort Algorithm
SortResult bubbleSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.swapCount = 0;

    Timer timer;
    timer.begin();

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
    result.timeMicroseconds = timer.getDurationMicroseconds();
    return result;
}

  // Selection Algorithm
SortResult selectionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.swapCount = 0;

    Timer timer;
    timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    for (int i = 0; i < container.y - 1; ++i) {
        int selected = i;
        for (int j = i + 1; j < container.y; ++j) {
            bool isBetter = false;

            if (isDate) {
                int d1 = parseDate(container.data[j][fieldIndex]);
                int d2 = parseDate(container.data[selected][fieldIndex]);
                isBetter = (ascending && d1 < d2) || (!ascending && d1 > d2);
            } else {
                int cmp = strcmp(container.data[j][fieldIndex], container.data[selected][fieldIndex]);
                isBetter = (ascending && cmp < 0) || (!ascending && cmp > 0);
            }

            if (isBetter) selected = j;
        }

        if (selected != i) {
            std::swap(container.data[i], container.data[selected]);
            result.swapCount++;
        }
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    return result;
}

 // Insertion Algorithm
SortResult insertionSortArray(dataContainer2D& container, int fieldIndex, bool ascending) {
    SortResult result;
    result.swapCount = 0;

    Timer timer;
    timer.begin();

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
                --j;
            } else {
                break;
            }
        }
        container.data[j + 1] = keyRow;
    }

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
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
    Timer timer;
    timer.begin();

    bool isDate = isDateFormat(container.data[0][fieldIndex]);

    mergeSortHelper(container.data, 0, container.y - 1, fieldIndex, ascending, isDate);

    timer.finish();
    result.timeMicroseconds = timer.getDurationMicroseconds();
    result.swapCount = -1; // Merge sort doesn't count swaps easily
    return result;
}

 ///////////////////////////////////// Search Algorithm /////////////////////////////////////





















