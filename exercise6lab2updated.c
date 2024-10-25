#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum constants {
	MAX_LENGTH_NUMBER = 100,
	MAX_LENGTH_ROMAN = 16,
	MAX_LENGTH_ZECKENDORF = 100,
	FILE_BUFFER = 1024,
	CUSTOM_AMOUNT = 10,
	LENGTH_FORMAT = 100
} constants;

typedef enum statusCode { NORMAL, MEMORY_ALLOCATION_ERROR, NOT_FOUND } statusCode;

typedef enum RomanNumbers {
	romI = 1,
	romIV = 4,
	romV = 5,
	romIX = 9,
	romX = 10,
	romXL = 40,
	romL = 50,
	romXC = 90,
	romC = 100,
	romCD = 400,
	romD = 500,
	romCM = 900,
	romM = 1000
} RomanNumbers;

statusCode oversscanf(const char* string, const char* format, ...);
statusCode overfscanf(FILE* file, const char* format, ...);

int main() {
	int firstNumber, secondNumber, thirdNumber;
	unsigned int fourthNumber;
	statusCode status = oversscanf("Numbers: XIX a Z % 1010101", "Numbers: %Ro %Cv %CV %% %Zr", &firstNumber,
	                               &secondNumber, 11, &thirdNumber, 36, &fourthNumber);
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case NOT_FOUND:
			printf("Not found (wrong data).\n");
	}
	printf("firstNumber = %d, secondNumber = %d, thirdNumber = %d, fourthNumber = %u\n", firstNumber, secondNumber,
	       thirdNumber, fourthNumber);
	FILE* file = fopen("input.txt", "r");
	if (file == NULL) {
		printf("An error while opening the file.\n");
		return 1;
	}
	status = overfscanf(file, "%Ro %Cv %CV %% %Zr", &firstNumber, &secondNumber, 11, &thirdNumber, 36, &fourthNumber);
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case NOT_FOUND:
			printf("Not found (wrong data).\n");
	}
	printf("firstNumber = %d, secondNumber = %d, thirdNumber = %d, fourthNumber = %u\n", firstNumber, secondNumber,
	       thirdNumber, fourthNumber);
	fclose(file);
	return 0;
}

int RomanToDecimal(const char*);
unsigned int ZeckendorfToDecimal(const char*, statusCode*);
long long int ToDecimal(const char* number, int base);
size_t StringLength(const char*);
int IsAlnum(char c);
int IsAlpha(char c);

statusCode oversscanf(const char* string, const char* format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char isFlag = 0;
	char previousSymbol = '0';
	const char* currentString = string;
	int* vaInt;
	unsigned int* unsInt;
	long long int* llInt;
	char* newFormat;
	for (size_t i = 0; i < StringLength(format); i++) {
		if (isFlag && format[i] == '%') {
			if (*currentString == '%') {
				currentString++;
			} else {
				va_end(arguments);
				return NOT_FOUND;
			}
			isFlag = 0;
			continue;
		} else if (format[i] == ' ') {
			while (*currentString == ' ') {
				currentString++;
			}
		} else if (format[i] == '%') {
			isFlag = 1;
			continue;
		} else if (isFlag && format[i] == 'R' && previousSymbol == '0') {
			previousSymbol = 'R';
			continue;
		} else if (isFlag && format[i] == 'o' && previousSymbol == 'R') {
			previousSymbol = '0';
			isFlag = 0;
			char* romanNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_ROMAN);
			if (romanNumber == NULL) {
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			int j = 0;
			while (IsAlpha(*currentString) && j < MAX_LENGTH_ROMAN - 1) {
				romanNumber[j++] = *currentString++;
			}
			romanNumber[j] = '\0';
			int decimalNumber = RomanToDecimal(romanNumber);
			vaInt = va_arg(arguments, int*);
			if (vaInt != NULL) {
				*vaInt = decimalNumber;
			}
			free(romanNumber);
		} else if (isFlag && format[i] == 'Z' && previousSymbol == '0') {
			previousSymbol = 'Z';
			continue;
		} else if (isFlag && format[i] == 'r' && previousSymbol == 'Z') {
			previousSymbol = '0';
			isFlag = 0;
			char* zeckendorfNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_ZECKENDORF);
			if (zeckendorfNumber == NULL) {
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			int j = 0;
			while ((*currentString == '0' || *currentString == '1') && j < MAX_LENGTH_ZECKENDORF - 1) {
				zeckendorfNumber[j++] = *currentString++;
			}
			zeckendorfNumber[j] = '\0';
			statusCode zecStatus = NORMAL;
			unsigned int decimalNumber = ZeckendorfToDecimal(zeckendorfNumber, &zecStatus);
			if (zecStatus == MEMORY_ALLOCATION_ERROR) {
				free(zeckendorfNumber);
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			unsInt = va_arg(arguments, unsigned int*);
			if (unsInt != NULL) {
				*unsInt = decimalNumber;
			}
			free(zeckendorfNumber);
		} else if (isFlag && format[i] == 'C' && previousSymbol == '0') {
			previousSymbol = 'C';
			continue;
		} else if (isFlag && format[i] == 'v' && previousSymbol == 'C') {
			previousSymbol = '0';
			isFlag = 0;
			char* stringNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_NUMBER);
			if (stringNumber == NULL) {
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			llInt = va_arg(arguments, long long int*);
			int base = va_arg(arguments, int);
			if (base < 2 || base > 36) {
				base = 10;
			}
			int j = 0;
			while (IsAlnum(*currentString) && j < MAX_LENGTH_NUMBER - 1) {
				stringNumber[j++] = *currentString++;
			}
			stringNumber[j] = '\0';
			long long int decimalNumber = ToDecimal(stringNumber, base);
			if (llInt != NULL) {
				*llInt = decimalNumber;
			}
			free(stringNumber);
		} else if (isFlag && format[i] == 'V' && previousSymbol == 'C') {
			previousSymbol = '0';
			isFlag = 0;
			char* stringNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_NUMBER);
			if (stringNumber == NULL) {
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			llInt = va_arg(arguments, long long int*);
			int base = va_arg(arguments, int);
			if (base < 2 || base > 36) {
				base = 10;
			}
			int j = 0;
			while (IsAlnum(*currentString) && j < MAX_LENGTH_NUMBER - 1) {
				stringNumber[j++] = *currentString++;
			}
			stringNumber[j] = '\0';
			long long int decimalNumber = ToDecimal(stringNumber, base);
			if (llInt != NULL) {
				*llInt = decimalNumber;
			}
			free(stringNumber);
		} else if (isFlag) {
			newFormat = (char*)malloc(sizeof(char) * LENGTH_FORMAT);
			if (newFormat == NULL) {
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			newFormat[0] = '%';
			int count = 0;
			int totalLength = LENGTH_FORMAT;
			while (format[i] != ' ' && format[i] != '\0' && format[i] != '%') {
				count++;
				if (count + 1 > totalLength) {
					totalLength *= 2;
					char* tempFormat = (char*)realloc(newFormat, sizeof(char) * totalLength);
					if (tempFormat == NULL) {
						free(newFormat);
						va_end(arguments);
						return MEMORY_ALLOCATION_ERROR;
					}
					newFormat = tempFormat;
				}
				newFormat[count] = format[i++];
			}
			newFormat[count + 1] = '\0';
			int scanResult = sscanf(currentString, newFormat, arguments);
			free(newFormat);
			if (scanResult != 1) {
				va_end(arguments);
				return NOT_FOUND;
			}
			while (*currentString != ' ' && *currentString != '\0') currentString++;
			while (*currentString == ' ') currentString++;
			isFlag = 0;
		} else {
			while (*currentString != '\0' && *currentString != format[i]) {
				currentString++;
			}
			if (*currentString == format[i]) {
				currentString++;
			} else {
				va_end(arguments);
				return NOT_FOUND;
			}
		}
	}
	va_end(arguments);
	return NORMAL;
}

statusCode overfscanf(FILE* file, const char* format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char isFlag = 0;
	char previousSymbol = '0';
	char* currentString = (char*)malloc(sizeof(char) * FILE_BUFFER);
	if (currentString == NULL) {
		va_end(arguments);
		return MEMORY_ALLOCATION_ERROR;
	}
	char* ptrCurrent = currentString;
	char c;
	int k = 0;
	size_t bufferSize = FILE_BUFFER;
	while ((c = getc(file)) != EOF) {
		currentString[k++] = c;
		if (k >= bufferSize - 1) {
			bufferSize *= 2;
			char* tempString = (char*)realloc(currentString, sizeof(char) * bufferSize);
			if (tempString == NULL) {
				free(currentString);
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			currentString = tempString;
		}
	}
	currentString[k] = '\0';
	int* vaInt;
	unsigned int* unsInt;
	long long int* llInt;
	char* newFormat;
	for (size_t i = 0; i < StringLength(format); i++) {
		if (isFlag && format[i] == '%') {
			if (*currentString == '%') {
				currentString++;
			} else {
				free(ptrCurrent);
				va_end(arguments);
				return NOT_FOUND;
			}
			isFlag = 0;
			continue;
		} else if (format[i] == ' ') {
			while (*currentString == ' ') {
				currentString++;
			}
		} else if (format[i] == '%') {
			isFlag = 1;
			continue;
		} else if (isFlag && format[i] == 'R' && previousSymbol == '0') {
			previousSymbol = 'R';
			continue;
		} else if (isFlag && format[i] == 'o' && previousSymbol == 'R') {
			previousSymbol = '0';
			isFlag = 0;
			char* romanNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_ROMAN);
			if (romanNumber == NULL) {
				free(ptrCurrent);
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			int j = 0;
			while (IsAlpha(*currentString) && j < MAX_LENGTH_ROMAN - 1) {
				romanNumber[j++] = *currentString++;
			}
			romanNumber[j] = '\0';
			int decimalNumber = RomanToDecimal(romanNumber);
			vaInt = va_arg(arguments, int*);
			if (vaInt != NULL) {
				*vaInt = decimalNumber;
			}
			free(romanNumber);
		} else if (isFlag && format[i] == 'Z' && previousSymbol == '0') {
			previousSymbol = 'Z';
			continue;
		} else if (isFlag && format[i] == 'r' && previousSymbol == 'Z') {
			previousSymbol = '0';
			isFlag = 0;
			char* zeckendorfNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_ZECKENDORF);
			if (zeckendorfNumber == NULL) {
				free(ptrCurrent);
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			int j = 0;
			while ((*currentString == '0' || *currentString == '1') && j < MAX_LENGTH_ZECKENDORF - 1) {
				zeckendorfNumber[j++] = *currentString++;
			}
			zeckendorfNumber[j] = '\0';
			statusCode zecStatus = NORMAL;
			unsigned int decimalNumber = ZeckendorfToDecimal(zeckendorfNumber, &zecStatus);
			if (zecStatus == MEMORY_ALLOCATION_ERROR) {
				free(zeckendorfNumber);
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			unsInt = va_arg(arguments, unsigned int*);
			if (unsInt != NULL) {
				*unsInt = decimalNumber;
			}
			free(zeckendorfNumber);
		} else if (isFlag && format[i] == 'C' && previousSymbol == '0') {
			previousSymbol = 'C';
			continue;
		} else if (isFlag && format[i] == 'v' && previousSymbol == 'C') {
			previousSymbol = '0';
			isFlag = 0;
			char* stringNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_NUMBER);
			if (stringNumber == NULL) {
				free(ptrCurrent);
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			llInt = va_arg(arguments, long long int*);
			int base = va_arg(arguments, int);
			if (base < 2 || base > 36) {
				base = 10;
			}
			int j = 0;
			while (IsAlnum(*currentString) && j < MAX_LENGTH_NUMBER - 1) {
				stringNumber[j++] = *currentString++;
			}
			stringNumber[j] = '\0';
			long long int decimalNumber = ToDecimal(stringNumber, base);
			if (llInt != NULL) {
				*llInt = decimalNumber;
			}
			free(stringNumber);
		} else if (isFlag && format[i] == 'V' && previousSymbol == 'C') {
			previousSymbol = '0';
			isFlag = 0;
			char* stringNumber = (char*)malloc(sizeof(char) * MAX_LENGTH_NUMBER);
			if (stringNumber == NULL) {
				free(ptrCurrent);
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			llInt = va_arg(arguments, long long int*);
			int base = va_arg(arguments, int);
			if (base < 2 || base > 36) {
				base = 10;
			}
			int j = 0;
			while (IsAlnum(*currentString) && j < MAX_LENGTH_NUMBER - 1) {
				stringNumber[j++] = *currentString++;
			}
			stringNumber[j] = '\0';
			long long int decimalNumber = ToDecimal(stringNumber, base);
			if (llInt != NULL) {
				*llInt = decimalNumber;
			}
			free(stringNumber);
		} else if (isFlag) {
			newFormat = (char*)malloc(sizeof(char) * LENGTH_FORMAT);
			if (newFormat == NULL) {
				va_end(arguments);
				return MEMORY_ALLOCATION_ERROR;
			}
			newFormat[0] = '%';
			int count = 0;
			int totalLength = LENGTH_FORMAT;
			while (format[i] != ' ' && format[i] != '\0' && format[i] != '%') {
				count++;
				if (count + 1 > totalLength) {
					totalLength *= 2;
					char* tempFormat = (char*)realloc(newFormat, sizeof(char) * totalLength);
					if (tempFormat == NULL) {
						free(newFormat);
						va_end(arguments);
						return MEMORY_ALLOCATION_ERROR;
					}
					newFormat = tempFormat;
				}
				newFormat[count] = format[i++];
			}
			newFormat[count + 1] = '\0';
			int scanResult = vsscanf(currentString, newFormat, arguments);
			va_arg(arguments, void*);
			free(newFormat);
			if (scanResult != 1) {
				va_end(arguments);
				return NOT_FOUND;
			}
			while (*currentString != ' ') currentString++;
			while (*currentString == ' ') currentString++;
			isFlag = 0;
		} else {
			while (*currentString != '\0' && *currentString != format[i]) {
				currentString++;
			}
			if (*currentString == format[i]) {
				currentString++;
			} else {
				va_end(arguments);
				return NOT_FOUND;
			}
		}
		free(ptrCurrent);
		va_end(arguments);
		return NORMAL;
	}
}

int RomanToDecimal(const char* string) {
	int result = 0;
	int previous = 0;
	int length = StringLength(string);
	for (int i = length - 1; i >= 0; i--) {
		int current = 0;
		switch (string[i]) {
			case 'M':
				current = romM;
				break;
			case 'D':
				current = romD;
				break;
			case 'C':
				current = romC;
				break;
			case 'L':
				current = romL;
				break;
			case 'X':
				current = romX;
				break;
			case 'V':
				current = romV;
				break;
			case 'I':
				current = romI;
				break;
		}

		if (current < previous) {
			result -= current;
		} else {
			result += current;
		}

		previous = current;
	}

	return result;
}

unsigned int ZeckendorfToDecimal(const char* numberString, statusCode* status) {
	size_t length = StringLength(numberString) - 1;
	if (length == 0) {
		*status = MEMORY_ALLOCATION_ERROR;
		return 0;
	}
	int* fibonacciNumbers = (int*)malloc(sizeof(int) * length);
	if (fibonacciNumbers == NULL) {
		*status = MEMORY_ALLOCATION_ERROR;
		return 0;
	}
	*status = NORMAL;
	fibonacciNumbers[0] = 1;
	fibonacciNumbers[1] = 2;
	for (int i = 2; i < length; i++) {
		fibonacciNumbers[i] = fibonacciNumbers[i - 1] + fibonacciNumbers[i - 2];
	}
	unsigned int result = 0;
	for (int i = 0; i < length - 1; i++) {
		if (numberString[i] == '1') {
			result += fibonacciNumbers[i];
		}
	}
	free(fibonacciNumbers);
	return result;
}

char ToLower(char c) { return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }

long long int ToDecimal(const char* number, int base) {
	long long int result = 0;
	for (int i = 0; i < StringLength(number); i++) {
		char elem = number[i];
		int digit;
		if (elem >= '0' && elem <= '9') {
			digit = elem - '0';
		} else if (ToLower(elem) >= 'a' && ToLower(elem) <= 'z') {
			digit = ToLower(elem) - 'a' + 10;
		} else {
			digit = -1;
		}
		if (digit >= base) {
			return -1;
		}
		result = result * base + digit;
	}
	return result;
}

size_t StringLength(const char* str) {
	size_t length = 0;
	while (*str) {
		length++;
		str++;
	}
	return length;
}

int IsAlnum(char c) {
	if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {
		return 1;
	}
	return 0;
}

int IsAlpha(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
