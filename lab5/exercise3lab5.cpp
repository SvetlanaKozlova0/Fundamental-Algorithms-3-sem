#include <iomanip>
#include <iostream>

class logical_values_array {
   private:
	unsigned int value = 0;

   public:
	logical_values_array(unsigned int number) { value = number; }
	logical_values_array() = default;
	logical_values_array(const logical_values_array& c) = default;
	logical_values_array& operator=(const logical_values_array& right) = default;
	~logical_values_array() = default;
	unsigned int get_value() const { return value; }
	logical_values_array inversion() const { return {~value}; }
	logical_values_array conjunction(const logical_values_array& right) const { return {value & right.get_value()}; }
	logical_values_array disjunction(const logical_values_array& right) const { return {value | right.get_value()}; }
	logical_values_array implication(const logical_values_array& right) const {
		return this->inversion().disjunction(right);
	}
	logical_values_array coimplication(const logical_values_array& right) const {
		return this->implication(right).inversion();
	}
	logical_values_array addition_module_2(const logical_values_array& right) const {
		return this->conjunction(right.inversion()).disjunction(right.conjunction(this->inversion()));
	}
	logical_values_array equivalent(const logical_values_array& right) const {
		return this->addition_module_2(right).inversion();
	}
	logical_values_array pierce(const logical_values_array& right) const {
		return this->disjunction(right).inversion();
	}
	logical_values_array shaeffer(const logical_values_array& right) const {
		return this->conjunction(right).inversion();
	}
	static bool equals(const logical_values_array& first, const logical_values_array& second) {
		return first.get_value() == second.get_value();
	}
	unsigned int get_bit(unsigned int position) const { return (value >> position) & 1; }
	int in_string(char** result) const {
		if (*result != nullptr) {
			return 0;
		}
		if (value == 0) {
			*result = (char*)malloc(sizeof(char) * 2);
			if (*result == nullptr) {
				return 0;
			}
			(*result)[0] = '0';
			(*result)[1] = '\0';
			return 1;
		}
		unsigned int copy_number = value;
		int count_digits = 0;
		while (copy_number > 0) {
			count_digits++;
			copy_number = copy_number >> 1;
		}
		*result = (char*)malloc(sizeof(char) * (count_digits + 1));
		if (*result == nullptr) {
			return 0;
		}
		copy_number = value;
		int index = 0;
		while (copy_number > 0) {
			(*result)[index++] = (copy_number & 1) ? '1' : '0';
			copy_number = copy_number >> 1;
		}
		(*result)[index] = '\0';
		for (int i = 0; i < count_digits / 2; i++) {
			char temp = (*result)[i];
			(*result)[i] = (*result)[count_digits - i - 1];
			(*result)[count_digits - i - 1] = temp;
		}
		return 1;
	}
};

void print_separator() {
	for (int i = 0; i < 50; i++) std::cout << "*-";
	std::cout << "\n";
}

int main() {
	logical_values_array first(100);
	logical_values_array second(120);
	logical_values_array third;
	char* string = nullptr;
	first.in_string(&string);
	std::cout << "first array (number 100): " << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << "second array (number 120): " << string << "\n";
	free(string);
	string = nullptr;
	third.in_string(&string);
	std::cout << "third array (number 0): " << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	first.in_string(&string);
	std::cout << "inversion of first array (first array = " << string << "): ";
	free(string);
	string = nullptr;
	first.inversion().in_string(&string);
	std::cout << std::right << std::setw(32) << string << "\n";
	print_separator();
	std::cout << "conjunction of first and second array: \n";
	free(string);
	string = nullptr;
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.conjunction(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "disjunction of first and second array: \n";
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.disjunction(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "implication of first and second array: \n";
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.implication(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "coimplication of first and second array: \n";
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.coimplication(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "xor of first and second array: \n";
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.addition_module_2(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "equivalent of first and second array: \n";
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.equivalent(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "pierce of first and second array: \n";
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.pierce(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "shaeffer of first and second array: \n";
	first.in_string(&string);
	std::cout << std::right << std::setw(32) << "first: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	second.in_string(&string);
	std::cout << std::right << std::setw(32) << "second: " << std::right << std::setw(32) << string << "\n";
	free(string);
	string = nullptr;
	first.shaeffer(second).in_string(&string);
	std::cout << std::right << std::setw(32) << "result: " << std::right << std::setw(32) << string << "\n";
	print_separator();
	free(string);
	string = nullptr;
	std::cout << "first bit of first array: " << first.get_bit(1) << "\n";
	std::cout << "second bit of first array: " << first.get_bit(2) << "\n";
	print_separator();
	std::cout << "first == second ? " << logical_values_array::equals(first, second) << "\n";
	logical_values_array fourth(120);
	std::cout << "fourth = array(120). second == fourth? " << logical_values_array::equals(second, fourth) << "\n";
	return 0;
}
