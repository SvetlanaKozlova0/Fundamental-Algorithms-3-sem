#include <algorithm>
#include <compare>
#include <iostream>
#define START_AMOUNT 10
#define EPSILON 0.000000001
class Vector {
   private:
	double* array = nullptr;
	size_t length = 0;
	size_t max_amount = 0;

   public:
	Vector() {
		array = new double[START_AMOUNT]();
		length = START_AMOUNT;
		max_amount = START_AMOUNT;
	}
	Vector(size_t size) {
		array = new double[size]();
		length = size;
		max_amount = size;
	}
	template <typename Iterator>
	Vector(Iterator first, Iterator second) {
		if (first > second) {
			throw std::out_of_range("Invalid boundaries");
		}
		size_t amount = std::distance(first, second);
		array = new double[amount]();
		length = amount;
		max_amount = amount;
		std::copy(first, second, array);
	}
	Vector(std::initializer_list<double> list) {
		array = new double[list.size()]();
		length = list.size();
		max_amount = list.size();
		std::copy(list.begin(), list.end(), array);
	}
	Vector& operator=(const Vector& right) {
		if (&right == this) return *this;
		delete[] array;
		array = new double[right.size()];
		for (int i = 0; i < right.size(); i++) {
			array[i] = right.at(i);
		}
		length = right.size();
		max_amount = right.size();
		return *this;
	}
	Vector(const Vector& right) {
		array = new double[right.size()];
		for (int i = 0; i < right.size(); i++) {
			array[i] = right.at(i);
		}
		length = right.size();
		max_amount = right.size();
	}
	~Vector() { delete[] array; }
	double& at(size_t index) {
		if (index + 1 > length) {
			throw std::out_of_range("Index out of range");
		}
		return array[index];
	}
	[[nodiscard]] const double& at(size_t index) const {
		if (index + 1 > length) {
			throw std::out_of_range("Index out of range");
		}
		return array[index];
	}
	double& front() {
		if (array == nullptr) {
			throw std::out_of_range("Empty vector");
		}
		return array[0];
	}
	[[nodiscard]] const double& front() const {
		if (array == nullptr) {
			throw std::out_of_range("Empty vector");
		}
		return array[0];
	}
	double& back() {
		if (array == nullptr) {
			throw std::out_of_range("Empty vector");
		}
		return array[length - 1];
	}
	[[nodiscard]] const double& back() const {
		if (array == nullptr) {
			throw std::out_of_range("Empty vector");
		}
		return array[length - 1];
	}
	double* data() { return array; }
	[[nodiscard]] const double* data() const { return array; }
	[[nodiscard]] bool empty() const { return length == 0; }
	[[nodiscard]] size_t size() const { return length; }
	[[nodiscard]] size_t capacity() const { return max_amount; }
	void reserve(size_t new_amount) {
		if (new_amount <= max_amount) return;
		double* new_array = new double[new_amount]();
		std::copy(array, array + length, new_array);
		delete[] array;
		array = new_array;
		max_amount = new_amount;
	}
	void shrink_to_fit() {
		if (max_amount == length) return;
		double* copy_array = new double[length]();
		std::copy(array, array + length, copy_array);
		delete[] array;
		array = copy_array;
		max_amount = length;
	}
	void clear() {
		delete[] array;
		array = nullptr;
		max_amount = 0;
		length = 0;
	}
	void insert(size_t index, double elem) {
		if (index > length) {
			throw std::out_of_range("Index out of range");
		}
		if (max_amount == length) {
			reserve(max_amount == 0 ? 1 : max_amount * 2);
		}
		for (size_t i = length; i > index; --i) {
			array[i] = array[i - 1];
		}
		length++;
		array[index] = elem;
	}
	void erase(size_t index) {
		if (index + 1 > length) {
			throw std::out_of_range("Index out of range");
		}
		for (size_t i = index; i < length - 1; ++i) {
			array[i] = array[i + 1];
		}
		length--;
	}
	void push_back(double elem) {
		if (length == max_amount) {
			reserve(max_amount == 0 ? 1 : max_amount * 2);
		}
		array[length++] = elem;
	}
	void pop_back() {
		if (length == 0) {
			throw std::out_of_range("Empty vector");
		}
		length--;
		if (length == 0) {
			delete[] array;
			array = nullptr;
			max_amount = 0;
		}
	}
	void resize(size_t new_size, double elem = 0.0) {
		if (new_size > max_amount) {
			reserve(new_size);
		}

		if (new_size > length) {
			std::fill(array + length, array + new_size, elem);
		}
		length = new_size;
	}
	bool operator==(const Vector& right) const {
		if (length != right.length) return false;
		for (size_t i = 0; i < length; i++) {
			if (std::abs(array[i] - right.array[i]) > EPSILON) return false;
		}
		return true;
	}
	std::partial_ordering operator<=>(const Vector& right) const {
		if (length != right.length) {
			return length <=> right.length;
		}
		for (size_t i = 0; i < length; ++i) {
			if (std::abs(array[i] - right.array[i]) > EPSILON) {
				return array[i] <=> right.array[i];
			}
		}
		return std::partial_ordering::equivalent;
	}
	class iterator {
	   public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = double;
		using difference_type = std::ptrdiff_t;
		using pointer = double*;
		using reference = double&;

		iterator(pointer ptr) { _ptr = ptr; }
		reference operator*() const { return *_ptr; }
		pointer operator->() { return _ptr; }
		iterator& operator++() {
			_ptr++;
			return *this;
		}
		iterator operator++(int) {
			iterator iter = *this;
			++(*this);
			return iter;
		}
		iterator& operator--() {
			_ptr--;
			return *this;
		}
		iterator operator--(int) {
			iterator iter = *this;
			--(*this);
			return iter;
		}
		friend bool operator==(const iterator& first, const iterator& second) { return first._ptr == second._ptr; }
		friend bool operator!=(const iterator& first, const iterator& second) { return first._ptr != second._ptr; }
		iterator operator+(difference_type type) const {
			iterator iter = *this;
			iter += type;
			return iter;
		}

		iterator operator-(difference_type type) const {
			iterator iter = *this;
			iter -= type;
			return iter;
		}
		difference_type operator-(const iterator& right) const { return _ptr - right._ptr; }
		iterator& operator+=(difference_type type) {
			_ptr += type;
			return *this;
		}
		iterator& operator-=(difference_type type) {
			_ptr -= type;
			return *this;
		}
		bool operator<(const iterator& right) const { return _ptr < right._ptr; }
		bool operator>(const iterator& right) const { return _ptr > right._ptr; }
		bool operator<=(const iterator& right) const { return _ptr <= right._ptr; }
		bool operator>=(const iterator& right) const { return _ptr >= right._ptr; }
		reference operator[](difference_type type) const { return *(_ptr + type); }

	   private:
		pointer _ptr;
	};

	void print_vector() const {
		if (length == 0) {
			std::cout << "Vector is empty" << std::endl;
			return;
		}
		for (int i = 0; i < length; i++) {
			std::cout << array[i] << " ";
		}
		std::cout << std::endl;
	}

	iterator begin() { return {array}; }
	[[nodiscard]] iterator begin() const { return {array}; }
	iterator end() { return {array + length}; }
	[[nodiscard]] iterator end() const { return {array + length}; }
};

void print_line() {
	for (int i = 0; i < 40; i++) {
		std::cout << "*-";
	}
	std::cout << std::endl;
}

void print_elem(double elem) { std::cout << elem << " "; }

int main() {
	std::cout << "initializing" << std::endl;
	print_line();
	Vector vect_without_init;
	std::cout << "let's create a vector without any initialization: (it contains 10 elements)" << std::endl;
	Vector vect_only_amount(20);
	vect_without_init.print_vector();
	std::cout << "let's create a vector with 20 elements that initialized by 0.0:" << std::endl;
	vect_only_amount.print_vector();
	std::cout << "let's create a vector which is initialized using a list:" << std::endl;
	Vector vect_with_init({0.8, 0.4, 3, 5, 56.45, 34, 9.3});
	vect_with_init.print_vector();
	double* numbers = new double[5]{4, 5, 34, 23, 3};
	Vector vect_by_iterators(numbers, numbers + 5);
	std::cout << "let's create a vector which filled by iterators:" << std::endl;
	vect_by_iterators.print_vector();
	delete[] numbers;
	print_line();
	std::cout << "let's try method add:" << std::endl;
	std::cout << "3 element of vector 4: " << vect_by_iterators.at(3) << std::endl;
	try {
		std::cout << "45 element of vector 4: " << vect_with_init.at(45) << std::endl;
	} catch (...) {
		std::cout << "error: this vector doesn't contain 45 element " << std::endl;
	}
	print_line();
	std::cout << "let's try the method front: " << std::endl;
	std::cout << vect_by_iterators.front() << std::endl;
	print_line();
	std::cout << "let's try the method back: " << std::endl;
	std::cout << vect_by_iterators.back() << std::endl;
	print_line();
	std::cout << "let's try the method data and print elements: " << std::endl;
	double* current = vect_by_iterators.data();
	for (int i = 0; i < vect_by_iterators.size(); i++) std::cout << current[i] << " ";
	std::cout << std::endl;
	print_line();
	std::cout << "let's insert an element 56 on the index 3: " << std::endl;
	vect_by_iterators.insert(3, 56);
	vect_by_iterators.print_vector();
	print_line();
	std::cout << "let's pop an element: " << std::endl;
	vect_by_iterators.pop_back();
	vect_by_iterators.print_vector();
	print_line();
	std::cout << "let's erase an element on the second position: " << std::endl;
	vect_by_iterators.erase(2);
	vect_by_iterators.print_vector();
	print_line();
	std::cout << "let's clear the vector: " << std::endl;
	vect_by_iterators.clear();
	std::cout << "current vector size: " << vect_by_iterators.size() << std::endl;
	std::cout << "let's check if it is empty: " << std::endl;
	bool result = vect_by_iterators.empty();
	if (result == true)
		std::cout << "is empty" << std::endl;
	else
		std::cout << "not empty" << std::endl;
	std::cout << "let's add an element 67: " << std::endl;
	vect_by_iterators.push_back(67);
	result = vect_by_iterators.empty();
	if (result == true)
		std::cout << "is empty";
	else
		std::cout << "not empty now" << std::endl;
	print_line();
	std::cout << "current size = " << vect_by_iterators.size() << std::endl;
	std::cout << "current capacity = " << vect_by_iterators.capacity() << std::endl;
	std::cout << "let's change resize to 10 and fill by 4: " << std::endl;
	vect_by_iterators.resize(10, 4);
	vect_by_iterators.print_vector();
	std::cout << "new size: " << vect_by_iterators.size() << std::endl;
	std::cout << "new capacity: " << vect_by_iterators.capacity() << std::endl;
	std::cout << "lets reserve place for 12 elements: " << std::endl;
	vect_by_iterators.reserve(12);
	std::cout << "new size: " << vect_by_iterators.size() << std::endl;
	std::cout << "new capacity: " << vect_by_iterators.capacity() << std::endl;
	std::cout << "now i can push back some elements: " << std::endl;
	vect_by_iterators.push_back(13);
	vect_by_iterators.print_vector();
	std::cout << "let's shrink this vector to fit: " << std::endl;
	vect_by_iterators.shrink_to_fit();
	std::cout << "new size: " << vect_by_iterators.size() << std::endl;
	std::cout << "new capacity: " << vect_by_iterators.capacity() << std::endl;
	print_line();
	std::cout << "let's check if vectors are equal: " << std::endl;
	vect_by_iterators.print_vector();
	vect_with_init.print_vector();
	if (vect_by_iterators == vect_with_init) {
		std::cout << "Vector by iterators are equal to vector by init" << std::endl;
	} else
		std::cout << "Vectors are not equal" << std::endl;
	std::cout << "let's use the copy constructor and make a new vector (same):" << std::endl;
	Vector copy_init = vect_with_init;
	copy_init.print_vector();
	if (vect_with_init == copy_init) {
		std::cout << "these vectors are equal" << std::endl;
	} else
		std::cout << "this vectors are not equal" << std::endl;
	print_line();
	std::cout << "let's check out iterators using function for_each (printing vector): " << std::endl;
	std::for_each(vect_by_iterators.begin(), vect_by_iterators.end(), print_elem);
	std::cout << std::endl;
	print_line();
	std::cout << "let's check our iterators: " << std::endl;
	Vector::iterator iter = vect_with_init.begin();
	std::cout << "now we will check the iterator begin() for vector:" << std::endl;
	vect_with_init.print_vector();
	std::cout << "iter++ " << *(iter++) << std::endl;
	std::cout << "*(++iter) " << *(++iter) << std::endl;
	std::cout << "*(--iter) " << *(--iter) << std::endl;
	std::cout << "*(iter--) " << *(iter--) << std::endl;
	std::cout << "iter[5] " << iter[5] << std::endl;
	std::cout << "iter += 5: ";
	iter += 5;
	std::cout << *iter << std::endl;
	iter -= 3;
	std::cout << "iter -= 3: ";
	std::cout << *iter << std::endl;
	std::cout << "let's create the iterator for vector: " << std::endl;
	vect_by_iterators.print_vector();
	Vector::iterator iter2 = vect_by_iterators.begin();
	std::cout << "do first iter is equal to second? " << std::endl;
	if (iter == iter2) {
		std::cout << "equal iterators" << std::endl;
	} else
		std::cout << "different iterators" << std::endl;
	std::cout << "let's create the copy of first iterator " << std::endl;
	std::cout << "do first iter is equal to third?" << std::endl;
	Vector::iterator iter3 = vect_by_iterators.begin();
	if (iter2 == iter3) {
		std::cout << "equal iterators" << std::endl;
	} else
		std::cout << "different iterators" << std::endl;
	std::cout << "let's add 2 to iterator 3" << std::endl;
	iter3 += 2;
	std::cout << "iter3 > iter2 ?" << std::endl;
	if (iter3 > iter2) {
		std::cout << "bigger" << std::endl;
	} else
		std::cout << "less" << std::endl;
	std::cout << "difference before first and second: " << iter3 - iter2 << std::endl;
	print_line();
	std::cout << "let's create a const vector by copy of vector by iterators: " << std::endl;
	const Vector const_vect = vect_by_iterators;
	const_vect.print_vector();
	std::cout << "size: " << const_vect.size() << std::endl;
	std::cout << "capacity: " << const_vect.capacity() << std::endl;
	bool is_empty = const_vect.empty();
	std::cout << "is empty? ";
	if (is_empty)
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
	std::cout << "third elem: " << const_vect.at(3) << std::endl;
	const double* data = const_vect.data();
	for (int i = 0; i < const_vect.size(); i++) std::cout << data[i] << " ";
	std::cout << std::endl;
	std::cout << "do this vector is equal to his parent?" << std::endl;
	if (const_vect == vect_by_iterators) {
		std::cout << "yes" << std::endl;
	} else
		std::cout << "no" << std::endl;
	print_line();
	std::partial_ordering resultOrdering = vect_with_init <=> vect_by_iterators;
	vect_with_init.resize(7, 4);
	vect_with_init.print_vector();
	vect_by_iterators.resize(7);
	vect_by_iterators.print_vector();
	if (resultOrdering == std::partial_ordering::less)
		std::cout << "first vector is less";
	else if (resultOrdering == std::partial_ordering::equivalent)
		std::cout << "they are equivalent";
	else if (resultOrdering == std::partial_ordering::greater)
		std::cout << "first vector is greater";
	else
		std::cout << "can't check";
	std::cout << std::endl;
	print_line();
	return 0;
}