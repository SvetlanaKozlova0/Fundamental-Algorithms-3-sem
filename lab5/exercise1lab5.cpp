#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

int add_one(int a) {
	int b = 1;
	while (a & b) {
		a = a ^ b;
		b = b << 1;
	}
	a = a ^ b;
	return a;
}

int subtract_numbers(int a, int b) {
	while (b != 0) {
		int c = (~a) & b;
		a = a ^ b;
		b = c << 1;
	}
	return a;
}

int sum_numbers(int a, int b) {
	while (b != 0) {
		b = subtract_numbers(b, 1);
		a = add_one(a);
	}
	return a;
}

int multiply_numbers(int a, int b) {
	int result = 0;
	while (b > 0) {
		if (b & 1) {
			result = sum_numbers(result, a);
		}
		a = a << 1;
		b = b >> 1;
	}
	return result;
}

class binary_int {
   private:
	int number = 0;

   public:
	binary_int(int current) { number = current; }
	binary_int() = default;
	int _get_int() const { return number; }
	binary_int(const binary_int& c) = default;
	binary_int& operator=(const binary_int& right) = default;
	~binary_int() = default;
	binary_int operator-() const { return {add_one(~number)}; }
	binary_int& operator++() {
		number = add_one(number);
		return *this;
	}
	binary_int& operator--() {
		number = subtract_numbers(number, 1);
		return *this;
	}
	binary_int operator++(int) {
		binary_int copy{*this};
		number = add_one(number);
		return copy;
	}
	binary_int operator--(int) {
		binary_int copy{*this};
		number = subtract_numbers(number, 1);
		return copy;
	}

	binary_int operator+(const binary_int& right) const { return {sum_numbers(right._get_int(), number)}; }

	binary_int operator-(const binary_int& right) const { return {subtract_numbers(number, right._get_int())}; }

	binary_int operator*(const binary_int& right) const { return {multiply_numbers(right._get_int(), number)}; }

	binary_int operator<<(const binary_int& right) const { return {number << right._get_int()}; }
	binary_int operator>>(const binary_int& right) const { return {number >> right._get_int()}; }

	binary_int& operator+=(const binary_int& right) {
		number = sum_numbers(right._get_int(), number);
		return *this;
	}
	binary_int& operator-=(const binary_int& right) {
		number = subtract_numbers(number, right._get_int());
		return *this;
	}
	binary_int& operator*=(const binary_int& right) {
		number = multiply_numbers(number, right._get_int());
		return *this;
	}
	binary_int& operator<<=(const binary_int& right) {
		number = number << right._get_int();
		return *this;
	}
	binary_int& operator>>=(const binary_int& right) {
		number = number >> right._get_int();
		return *this;
	}
	std::pair<binary_int, binary_int> get_half() const {
		if (number == 0) {
			return {binary_int(0), binary_int(0)};
		}
		int current = number;
		int amount_bits = 0;
		while (current > 0) {
			amount_bits = add_one(amount_bits);
			current = current >> 1;
		}
		int half = (add_one(amount_bits)) >> 1;
		int low_bits = number & subtract_numbers((1 << (subtract_numbers(amount_bits, half) >> 1)), 1);
		int high_bits = number >> (subtract_numbers(amount_bits, half >> 1));
		return {binary_int(high_bits), binary_int(low_bits)};
	}
};

std::ostream& operator<<(std::ostream& out, const binary_int& number) {
	int result = number._get_int();
	if (result == 0) {
		out << "0";
		return out;
	}
	std::vector<char> values;
	while (result > 0) {
		values.push_back((result & 1) ? '1' : '0');
		result = result >> 1;
	}
	std::reverse(values.begin(), values.end());
	for (char elem : values) {
		out << elem;
	}
	return out;
}

int main() {
	binary_int a(22222);
	binary_int b;
	std::cout << "a is 100: " << a << " (" << a._get_int() << ")\n";
	std::cout << "b is 0 (default): " << b << " (" << b._get_int() << ")\n";
	std::cout << "++b, b = " << ++b << " (" << b._get_int() << ")\n";
	std::cout << "b++, b = " << b++ << " (" << b._get_int() << ")\n";
	std::cout << "b = " << b._get_int() << "\n";
	std::cout << "a--, a = " << a-- << " (" << a._get_int() << ")\n";
	std::cout << "--a, a = " << --a << " (" << a._get_int() << ")\n";
	std::cout << "Operation a+=b, a before: " << a._get_int() << ", b before: " << b._get_int() << "\n";
	a += b;
	std::cout << "a = " << a << " (" << a._get_int() << ")\n";
	std::cout << "b = " << b << " (" << b._get_int() << ")\n";
	std::cout << "Operation a-=b, a before: " << a._get_int() << ", b before: " << b._get_int() << "\n";
	a -= b;
	std::cout << "a = " << a << " (" << a._get_int() << ")\n";
	std::cout << "b = " << b << " (" << b._get_int() << ")\n";
	binary_int c = a + b + a;
	std::cout << "c = a + b + a, a = " << a._get_int() << ", b = " << b._get_int() << "\n";
	std::cout << "c = " << c << " (" << c._get_int() << ")\n";
	c = a - b;
	std::cout << "c = a - b, a = " << a._get_int() << ", b = " << b._get_int() << "\n";
	std::cout << "c = " << c << " (" << c._get_int() << ")\n";
	c = a * b;
	std::cout << "c = a * b, a = " << a._get_int() << ", b = " << b._get_int() << "\n";
	std::cout << "c = " << c << " (" << c._get_int() << ")\n";
	std::cout << "Operation a*=b, a before: " << a._get_int() << ", b before: " << b._get_int() << "\n";
	a *= b;
	std::cout << "a = " << a << " (" << a._get_int() << ")\n";
	std::cout << "b = " << b << " (" << b._get_int() << ")\n";
	std::cout << "Operation a <<= binary_int(1), a before: " << a._get_int() << "\n";
	a <<= binary_int(1);
	std::cout << "a = " << a << " (" << a._get_int() << ")\n";
	std::cout << "Operation a >>= binary_int(1), a before: " << a._get_int() << "\n";
	a >>= binary_int(1);
	std::cout << "a = " << a << " (" << a._get_int() << ")\n";
	c = a >> binary_int(1);
	std::cout << "c = a >> binary_int(1), a = " << a._get_int() << "\n";
	std::cout << "c = " << c << " (" << c._get_int() << ")\n";
	c = a << binary_int(1);
	std::cout << "c = a << binary_int(1), a = " << a._get_int() << "\n";
	std::cout << "c = " << c << " (" << c._get_int() << ")\n";
	std::pair<binary_int, binary_int> current_pair;
	current_pair = a.get_half();
	std::cout << "pair of highest and lowest bits of a: (a = " << a << ", " << a._get_int() << ")\n";
	std::cout << "highest bits: " << current_pair.first << " (" << current_pair.first._get_int() << ")\n";
	std::cout << "lowest bits: " << current_pair.second << " (" << current_pair.second._get_int() << ")\n";
	return 0;
}