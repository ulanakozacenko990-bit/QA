#pragma once
#include "DoubleLinkedList.h"
#include "DynamicArray.h"
#include "Stack.h"
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
class DoubleLinkedList;

template <typename T>
class TimSort {
private:
    DoubleLinkedList<T>& list;
    DynamicArray<T> tempArray;
    const int MIN_MERGE = 32;
    const int GALLOP_THRESHOLD = 7;
    const int MAX_SIZE = 50000;

    struct Run {
        int start;
        int end;
        int length;

        Run(int s = 0, int e = 0, int len = 0)
            : start(s), end(e), length(len) {
        }
    };

    Stack<Run> runStack;

    int computeMinRun(int n);
    void insertionSort(int start, int end);
    void reverseSegment(int start, int end);
    Run findNextRun(int startIndex);
    void mergeRuns(Run& left, Run& right);
    void mergeRunsWithGallop(Run& left, Run& right);
    int gallopLeft(int start, int end, T key);
    int gallopRight(int start, int end, T key);
    bool shouldMerge();
    void forceMerge();

public:
    TimSort(DoubleLinkedList<T>& lst) : list(lst) {}

    bool isSizeValid(int proposedSize) const {
        return proposedSize <= MAX_SIZE;
    }

    int getMaxSize() const {
        return MAX_SIZE;
    }

    void sort();
};

template <typename T>
int TimSort<T>::computeMinRun(int n) {
    int r = 0;
    while (n >= MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

template <typename T>
void TimSort<T>::insertionSort(int start, int end) {
    if (start >= end) return;

    for (int i = start + 1; i <= end; i++) {
        T key = list.at(i);  
        int j = i - 1;

        while (j >= start && list.at(j) > key) {
            
            list.setAt(j + 1, list.at(j));
            j--;
        }
        list.setAt(j + 1, key);
    }
}

template <typename T>
void TimSort<T>::reverseSegment(int start, int end) {
    int left = start;
    int right = end;

    while (left < right) {
        
        list.swap(left, right);
        left++;
        right--;
    }
}

template <typename T>
typename TimSort<T>::Run TimSort<T>::findNextRun(int startIndex) {
    int listSize = list.getSize();
    if (startIndex >= listSize - 1) {
        return Run(startIndex, listSize - 1, 1);
    }

    int current = startIndex;
    int runLength = 1;

    
    if (list.at(current) > list.at(current + 1)) {
        
        while (current < listSize - 1 && list.at(current) > list.at(current + 1)) {
            current++;
            runLength++;
        }
        reverseSegment(startIndex, current);
    }
    else {
        
        while (current < listSize - 1 && list.at(current) <= list.at(current + 1)) {
            current++;
            runLength++;
        }
    }

    
    int minRun = computeMinRun(listSize);
    if (runLength < minRun) {
        int extendTo = min(startIndex + minRun - 1, listSize - 1);
        runLength = extendTo - startIndex + 1;
        insertionSort(startIndex, extendTo);
        current = extendTo;
    }

    return Run(startIndex, current, runLength);
}

template <typename T>
int TimSort<T>::gallopLeft(int start, int end, T key) {
    int count = 0;
    int i = start;

    while (i <= end && list.at(i) < key) {
        count++;
        i++;
    }
    return count;
}

template <typename T>
int TimSort<T>::gallopRight(int start, int end, T key) {
    int count = 0;
    int i = start;

    while (i <= end && list.at(i) <= key) {
        count++;
        i++;
    }
    return count;
}

template <typename T>
void TimSort<T>::mergeRunsWithGallop(Run& left, Run& right) {
    tempArray.clear();

    int leftIdx = left.start;
    int rightIdx = right.start;
    int leftEnd = left.end;
    int rightEnd = right.end;

    int leftGallopCount = 0;
    int rightGallopCount = 0;

    while (leftIdx <= leftEnd && rightIdx <= rightEnd) {
        if (leftGallopCount >= GALLOP_THRESHOLD) {
            int gallopCount = gallopRight(rightIdx, rightEnd, list.at(leftIdx));

            for (int i = 0; i < gallopCount; i++) {
                tempArray.push_back(list.at(rightIdx));
                rightIdx++;
            }

            tempArray.push_back(list.at(leftIdx));
            leftIdx++;
            leftGallopCount = 0;
            rightGallopCount = 0;
        }
        else if (rightGallopCount >= GALLOP_THRESHOLD) {
            int gallopCount = gallopLeft(leftIdx, leftEnd, list.at(rightIdx));

            for (int i = 0; i < gallopCount; i++) {
                tempArray.push_back(list.at(leftIdx));
                leftIdx++;
            }

            tempArray.push_back(list.at(rightIdx));
            rightIdx++;
            leftGallopCount = 0;
            rightGallopCount = 0;
        }
        else {
            if (list.at(leftIdx) <= list.at(rightIdx)) {
                tempArray.push_back(list.at(leftIdx));
                leftIdx++;
                leftGallopCount++;
                rightGallopCount = 0;
            }
            else {
                tempArray.push_back(list.at(rightIdx));
                rightIdx++;
                rightGallopCount++;
                leftGallopCount = 0;
            }
        }
    }

    
    while (leftIdx <= leftEnd) {
        tempArray.push_back(list.at(leftIdx));
        leftIdx++;
    }

    while (rightIdx <= rightEnd) {
        tempArray.push_back(list.at(rightIdx));
        rightIdx++;
    }

    
    int destIdx = left.start;
    for (int i = 0; i < tempArray.getSize(); i++) {
        list.setAt(destIdx, tempArray[i]);
        destIdx++;
    }
}

template <typename T>
void TimSort<T>::mergeRuns(Run& left, Run& right) {
    if (left.length > MIN_MERGE || right.length > MIN_MERGE) {
        mergeRunsWithGallop(left, right);
    }
    else {
        tempArray.clear();

        int leftIdx = left.start;
        int rightIdx = right.start;
        int leftEnd = left.end;
        int rightEnd = right.end;

        while (leftIdx <= leftEnd && rightIdx <= rightEnd) {
            if (list.at(leftIdx) <= list.at(rightIdx)) {
                tempArray.push_back(list.at(leftIdx));
                leftIdx++;
            }
            else {
                tempArray.push_back(list.at(rightIdx));
                rightIdx++;
            }
        }

        while (leftIdx <= leftEnd) {
            tempArray.push_back(list.at(leftIdx));
            leftIdx++;
        }

        while (rightIdx <= rightEnd) {
            tempArray.push_back(list.at(rightIdx));
            rightIdx++;
        }

        
        int destIdx = left.start;
        for (int i = 0; i < tempArray.getSize(); i++) {
            list.setAt(destIdx, tempArray[i]);
            destIdx++;
        }
    }
}

template <typename T>
bool TimSort<T>::shouldMerge() {
    if (runStack.size() < 3) return false;

    Run Z = runStack.top(); runStack.pop();
    Run Y = runStack.top(); runStack.pop();
    Run X = runStack.top(); runStack.pop();

    bool result = (X.length <= Y.length + Z.length) || (Y.length <= Z.length);

    runStack.push(X);
    runStack.push(Y);
    runStack.push(Z);

    return result;
}

template <typename T>
void TimSort<T>::forceMerge() {
    while (runStack.size() > 1) {
        Run right = runStack.top(); runStack.pop();
        Run left = runStack.top(); runStack.pop();

        mergeRuns(left, right);

        Run mergedRun(left.start, right.end, left.length + right.length);
        runStack.push(mergedRun);
    }
}

template <typename T>
void TimSort<T>::sort() {
    if (list.empty() || list.getSize() <= 1) return;

    if (list.getSize() > MAX_SIZE) {
        cout << "Ошибка: невозможно отсортировать " << list.getSize()
            << " элементов. Превышен лимит в " << MAX_SIZE << " элементов." << endl;
        return;
    }

    while (!runStack.empty()) {
        runStack.pop();
    }

    int currentIndex = 0;
    int listSize = list.getSize();

    while (currentIndex < listSize) {
        Run newRun = findNextRun(currentIndex);
        runStack.push(newRun);

        while (shouldMerge()) {
            Run Z = runStack.top(); runStack.pop();
            Run Y = runStack.top(); runStack.pop();
            Run X = runStack.top(); runStack.pop();

            if (X.length <= Z.length) {
                mergeRuns(X, Y);
                Run mergedXY(X.start, Y.end, X.length + Y.length);
                runStack.push(mergedXY);
                runStack.push(Z);
            }
            else if (Y.length <= Z.length) {
                mergeRuns(Y, Z);
                Run mergedYZ(Y.start, Z.end, Y.length + Z.length);
                runStack.push(X);
                runStack.push(mergedYZ);
            }
            else {
                runStack.push(X);
                runStack.push(Y);
                runStack.push(Z);
                break;
            }
        }

        currentIndex = newRun.end + 1;
    }

    forceMerge();

    cout << "Тимсорт сортировка завершена. Отсортировано " << list.getSize()
        << " элементов." << endl;
}