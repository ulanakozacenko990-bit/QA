#ifndef ARRAY_H
#define ARRAY_H

#include <string>
#include <iostream>

using namespace std;

template <typename Type>
class Array {
private:
	int tempSize = 0;
	int size = 2;
	Type* arr = new Type[size];

public:
	Array() = default;
	Array(int size);

	void resize();
	void add(Type value);
	void addIndex(int index, Type value);
	void remove(int index);
	Type get(int index);
	void changeValue(int index, Type newValue);

	int getSize() const;
	bool isEmpty() const;

	~Array();
};

template <typename Type> Array<Type>::Array(int size) {
	if (size > 0) {
		this->size = size;
	}

	arr = new string[size];
}

template <typename Type> void Array<Type>::resize() {
	size *= 2;

	string* newArr = new string[size];

	for (int i = 0; i < tempSize; i++) {
		newArr[i] = arr[i];
	}

	delete[] arr;
	arr = newArr;
}

template <typename Type> void Array<Type>::add(Type value) {
	if (tempSize >= size) {
		resize();
	}

	arr[tempSize] = value;
	tempSize++;
}

template <typename Type> void Array<Type>::addIndex(int index, Type value) {
	if (index < 0 || index > tempSize) {
		cout << "Incorrect index.";
	}

	if (tempSize >= size) {
		resize();
	}

	for (int i = tempSize; i > index; i--) {
		arr[i] = arr[i - 1];
	}

	arr[index] = value;
	tempSize++;
}

template <typename Type> void Array<Type>::remove(int index) {
	if (index < 0 || index > tempSize) {
		cout << "Incorrect index.";
	}

	for (int i = index; i < tempSize - 1; i++) {
		arr[i] = arr[i + 1];
	}

	tempSize--;
}

template <typename Type> Type Array<Type>::get(int index) {
	if (index < 0 || index > tempSize) {
		cout << "Incorrect index.";
	}

	return arr[index];
}

template <typename Type> void Array<Type>::changeValue(int index, Type newValue) {
	if (index < 0 || index > tempSize) {
		cout << "Incorrect index.";
	}

	arr[index] = newValue;
}

template <typename Type> int Array<Type>::getSize() const {
	return tempSize;
}

template <typename Type> bool Array<Type>::isEmpty() const {
	return tempSize == 0;
}

template <typename Type> Array<Type>::~Array() {
	delete[] arr;
	arr = nullptr;
}

#endif // ARRAY_H
