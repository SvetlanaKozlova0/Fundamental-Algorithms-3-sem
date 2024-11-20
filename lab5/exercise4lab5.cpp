#include <cmath>
#include <iostream>

class Complex {
   private:
	double realPart = 0;
	double imaginaryPart = 0;

   public:
	double GetRealP() const;
	double GetImaginaryP() const;
	Complex(double realP, double imaginaryP);
	Complex() = default;
	Complex(const Complex& c) = default;
	Complex& operator=(const Complex& right) = default;
	~Complex() = default;
	Complex Addition(const Complex& first, const Complex& second) const;
	Complex Subtraction(const Complex& first, const Complex& second) const;
	Complex Multiplication(const Complex& first, const Complex& second) const;
	Complex Division(const Complex& first, const Complex& second, double epsilon) const;
	double Module() const;
	double Argument(double epsilon) const;
	void PrintComplex() const;
};

void Complex::PrintComplex() const { std::cout << realPart << " + (" << imaginaryPart << ")i"; }

Complex::Complex(double realP, double imaginaryP) {
	realPart = realP;
	imaginaryPart = imaginaryP;
}

double Complex::GetRealP() const { return realPart; }

double Complex::GetImaginaryP() const { return imaginaryPart; }

Complex Complex::Addition(const Complex& first, const Complex& second) const {
	double realP = first.GetRealP() + second.GetRealP();
	double imaginaryP = first.GetImaginaryP() + second.GetImaginaryP();
	Complex result(realP, imaginaryP);
	return result;
}

Complex Complex::Subtraction(const Complex& first, const Complex& second) const {
	double realP = first.GetRealP() - second.GetRealP();
	double imaginaryP = first.GetImaginaryP() - second.GetImaginaryP();
	Complex result(realP, imaginaryP);
	return result;
}

Complex Complex::Multiplication(const Complex& first, const Complex& second) const {
	double realP = first.GetRealP() * second.GetRealP() - first.GetImaginaryP() * second.GetImaginaryP();
	double imaginaryP = first.GetRealP() * second.GetImaginaryP() + first.GetImaginaryP() * second.GetRealP();
	Complex result(realP, imaginaryP);
	return result;
}

Complex Complex::Division(const Complex& first, const Complex& second, double epsilon) const {
	double denominator = second.GetRealP() * second.GetRealP() + second.GetImaginaryP() * second.GetImaginaryP();
	if (denominator > -epsilon && denominator < epsilon) {
		Complex result;
		return result;
	}
	double realP =
	    (first.GetRealP() * second.GetRealP() + first.GetImaginaryP() * second.GetImaginaryP()) / denominator;
	double imaginaryP =
	    (first.GetImaginaryP() * second.GetRealP() - first.GetRealP() * second.GetImaginaryP()) / denominator;
	Complex result(realP, imaginaryP);
	return result;
}

double Complex::Module() const { return sqrt(realPart * realPart + imaginaryPart * imaginaryPart); }

double Complex::Argument(double epsilon) const {
	if (realPart < epsilon && realPart > -epsilon || imaginaryPart < epsilon && imaginaryPart > -epsilon) return 0;
	return atan2(imaginaryPart, realPart);
}

int main() {
	Complex a(1, 4);
	std::cout << "a = ";
	a.PrintComplex();
	std::cout << "\n";
	Complex b(2, -3);
	std::cout << "b = ";
	b.PrintComplex();
	std::cout << '\n';
	std::cout << "result of addition a and b: ";
	Complex d = a.Complex::Addition(a, b);
	d.PrintComplex();
	std::cout << '\n';
	std::cout << "result of division a and b: ";
	d = a.Division(a, b, 0.001);
	d.PrintComplex();
	std::cout << '\n';
	std::cout << "result of multiplication a and b: ";
	d = a.Multiplication(a, b);
	d.PrintComplex();
	std::cout << '\n';
	std::cout << "result of subtraction a and b: ";
	d = a.Subtraction(a, b);
	d.PrintComplex();
	std::cout << '\n';
	double module = a.Module();
	double argument = a.Argument(0.001);
	std::cout << "module a = " << module << ", argument a = " << argument << "\n";
	return 0;
}