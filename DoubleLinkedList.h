#pragma once
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

template <typename T>
class DoubleLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoubleLinkedList() {
        clear();
    }

    // Добавляем недостающие методы для TimSort

    // Быстрая установка значения по индексу
    void setAt(int index, const T& value) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }

        Node* node = getNodeAt(index);
        node->data = value;
    }

    // Быстрый обмен элементов по индексам
    void swap(int index1, int index2) {
        if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) {
            throw out_of_range("Index out of range");
        }

        if (index1 == index2) return;

        Node* node1 = getNodeAt(index1);
        Node* node2 = getNodeAt(index2);

        T temp = node1->data;
        node1->data = node2->data;
        node2->data = temp;
    }

    // Вспомогательный метод для получения узла по индексу
    Node* getNodeAt(int index) const {
        if (index < 0 || index >= size) {
            return nullptr;
        }

        Node* current;
        if (index < size / 2) {
            current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
        }
        else {
            current = tail;
            for (int i = size - 1; i > index; i--) {
                current = current->prev;
            }
        }
        return current;
    }

    // Существующие методы
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void pop_back() {
        if (empty()) return;

        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node* temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
        }
        size--;
    }

    void pop_front() {
        if (empty()) return;

        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node* temp = head;
            head = head->next;
            head->prev = nullptr;
            delete temp;
        }
        size--;
    }

    T& at(int index) {
        Node* node = getNodeAt(index);
        if (node == nullptr) {
            throw out_of_range("Index out of range");
        }
        return node->data;
    }

    const T& at(int index) const {
        Node* node = getNodeAt(index);
        if (node == nullptr) {
            throw out_of_range("Index out of range");
        }
        return node->data;
    }

    bool empty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    void printForward(int maxDisplay = 50) const {
        if (empty()) {
            cout << "Список пуст" << endl;
            return;
        }

        Node* current = head;
        int count = 0;
        const int DISPLAY_LIMIT = 500;

        cout << "Элементы: ";

        int showCount = min(min(maxDisplay, DISPLAY_LIMIT), size);

        while (current != nullptr && count < showCount) {
            cout << current->data;
            current = current->next;
            count++;

            if (current != nullptr && count < showCount) {
                cout << " ";
            }
        }

        if (size > showCount) {
            cout << " ... (еще " << (size - showCount) << " элементов)";
        }
        cout << endl;
    }

    void printBackward() const {
        if (empty()) {
            cout << "Список пуст" << endl;
            return;
        }

        Node* current = tail;
        int count = 0;
        const int DISPLAY_LIMIT = 200;

        cout << "В обратном порядке: ";

        int showCount = min(DISPLAY_LIMIT, size);

        while (current != nullptr && count < showCount) {
            cout << current->data << " ";
            current = current->prev;
            count++;
        }

        if (size > showCount) {
            cout << "... (еще " << (size - showCount) << " элементов)";
        }
        cout << endl;
    }

   
    T& front() {
        if (empty()) {
            throw out_of_range("List is empty");
        }
        return head->data;
    }

    T& back() {
        if (empty()) {
            throw out_of_range("List is empty");
        }
        return tail->data;
    }
    void insert(int index, const T& value) {
        if (index < 0 || index > size) {
            throw out_of_range("Index out of range");
        }

        if (index == 0) {
            push_front(value);
        }
        else if (index == size) {
            push_back(value);
        }
        else {
            Node* currentNode = getNodeAt(index);
            Node* newNode = new Node(value);

            newNode->prev = currentNode->prev;
            newNode->next = currentNode;
            currentNode->prev->next = newNode;
            currentNode->prev = newNode;

            size++;
        }
    }
    void erase(int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }

        if (index == 0) {
            pop_front();
        }
        else if (index == size - 1) {
            pop_back();
        }
        else {
            Node* nodeToDelete = getNodeAt(index);
            nodeToDelete->prev->next = nodeToDelete->next;
            nodeToDelete->next->prev = nodeToDelete->prev;
            delete nodeToDelete;
            size--;
        }
    }
};