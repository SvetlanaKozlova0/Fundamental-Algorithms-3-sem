#include <iostream>

class vector {
   private:
	double* values = nullptr;
	size_t vector_length = 0;
	size_t vector_capacity = 0;
   public:
	double* at(size_t index) const;
	double* front() const;
	double* back() const;
	double* data() const;
	bool empty() const {
		return vector_length == 0;
	}
	size_t size() const;
	size_t capacity() const;
	int reserve(size_t num);
	int shrink_to_fit();
	void clear();
	int insert(size_t index, double elem);
	void erase(size_t index);
	int push_back(double elem);
	void pop_back(double elem);
	int resize(size_t size, double elem);
	int operator == (const vector& right) const;
	int operator <=>(const vector& right) const;
};

double* vector::at(size_t index) const {
	if (index >= vector_length) return nullptr;
	return &values[index];
}
double* vector::front() const {
	if (vector_length == 0) return nullptr;
	return &values[0];
}
double* vector::back() const {
	if (vector_length == 0) return nullptr;
	return &values[vector_length - 1];
}
double* vector::data() const {
	return values;
}
size_t vector::size() const {
	return vector_length;
}
size_t vector::capacity() const {
	return vector_capacity;
}
int vector::operator == (const vector& right) const {
	if (vector_length != right.size()) {
		return 0;
	}
	for (size_t i = 0; i < vector_length; i++) {
		if (values[i] != *(right.at(i))) {
			return 0;
		}
	}
	return 1;
}
int vector::reserve(size_t num) {
	if (vector_length >= num) return 1;
	double* temp = (double*)realloc(values, sizeof(double) * num);
	if (temp == nullptr) {
		return 0;
	}
	values = temp;
	return 1;
}
int vector::shrink_to_fit() {
	vector_capacity = vector_length;
}
void vector::clear() {
	free(values);
	vector_capacity = 0;
	vector_length = 0;
}
void vector::pop_back(double elem) {
	vector_length--;
}
int vector::operator <=> (const vector& right) const{
	if (vector_length > right.size()) return 1;
	if (vector_length < right.size()) return -1;
	for (int i = 0; i < vector_length; i++) {
		if (values[i] > *(right.at(i))) return 1;
		else if (values[i] < *(right.at(i))) return -1;
	}
	return 0;
}
int vector::push_back(double elem) {
	if (vector_length == vector_capacity) {
		vector_capacity *= 2;
		double* temp = (double*)realloc(values, vector_capacity);
		if (temp == nullptr) {
			return 0;
		}
		values = temp;
	}
	values[vector_length] = elem;
	vector_length++;
}
//change
int vector::resize(size_t size, double elem) {
	if (size == vector_length) return 1;
	if (size > vector_length) {
		double* temp = (double*)realloc(values, size);
		if (temp == nullptr) {
			return 0;
		}
		values = temp;
		for (size_t i = vector_length - 1; i < size; i++) values[i] = elem;
		vector_length = size;
		vector_capacity = size;
		return 1;
	} else {
		double* temp = (double*)realloc(values, size);
		if (temp == nullptr) {
			return 0;
		}
		values = temp;
		return 1;
	}
}
int vector::insert(size_t index, double elem) {
	int result = reserve(vector_length + 1);
	if (result == 0) return 0;
	for (size_t i = index; i < vector_length - 1; i++) {
		values[i] = values[i + 1];
	}
	values[index] = elem;
	return 1;
}
void vector::erase(size_t index){
	for (size_t i = index; i < vector_length - 1; i++) {
		values[i] = values[i + 1];
	}
	vector_length--;
}
