#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum constants { ROMAN_SYMBOLS = 13, INPUT_BUFFER_SIZE = 1000, MAX_ROMAN_NUMBER = 3999, MAX_LEN_RESULT = 20, LENGTH_SPECIFICATOR = 1000 } constants;

typedef enum statusCode { NORMAL, MEMORY_ALLOCATION_ERROR, NEGATIVE_NUMBER, WRONG_NUMBER } statusCode;

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

int StringLength(const char*);
char* StringCopy(char*, const char*);
statusCode FlagRo(char**, int);
statusCode FlagZr(char**, unsigned int);
statusCode FlagCv(char**, int, int);
statusCode FlagCV(char**, int, int);
statusCode FlagTo(const char*, long long int*, int);
statusCode FlagTO(const char*, long long int*, int);
statusCode FlagMi(char**, int32_t*);
statusCode FlagMu(char**, uint32_t*);
statusCode FlagMD(char**, double*);
statusCode FlagMf(char**, float*);
statusCode IsValid(const char*, int);
statusCode oversprintf(char*, const char*, ...);
statusCode overfprintf(FILE*, const char*, ...);

int main() {
	char* buffer = (char*)malloc(sizeof(char) * INPUT_BUFFER_SIZE);
	statusCode status = oversprintf(buffer,
	                                "1. roman number 19 = %Ro.\n"
	                                "2. zeckendorf number 54 = %Zr.\n"
	                                "3. 10 in 5 system = %Cv.\n"
	                                "4. 11 in 13 system = %CV.\n"
	                                "5. 'a' from 11 to 10 system = %to.\n"
	                                "6. 'Z' from 36 to 10 system = %TO.\n"
	                                "7. memory dump of 5 = %mi.\n"
	                                "8. memory dump of 45 = %mu.\n"
	                                "9. memory dump of 5.4 = %md.\n"
	                                "10. memory dump of 52.3 = %mf.\n"
	                                "11. percent symbol = %%.\n"
	                                "12. standard specification with flag = %.2f.\n",
	                                19, 54, 10, 5, 11, 13, "a", 11, "Z", 36, 5, 45, 5.4, 52.3, 45.3);
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			free(buffer);
			return 1;
		case NEGATIVE_NUMBER:
			printf("Negative number for Roman system.\n");
			free(buffer);
			return 1;
		case WRONG_NUMBER:
			printf("Wrong arguments.\n");
			free(buffer);
			return 1;
	}
	printf("%s", buffer);
	free(buffer);
	FILE* file = fopen("input.txt", "w");
	if (file == NULL) {
		printf("An error while opening file.\n");
		return 1;
	}
	status = overfprintf(file,
	                     "1. roman number 19 = %Ro.\n"
	                     "2. zeckendorf number 54 = %Zr.\n"
	                     "3. 10 in 5 system = %Cv.\n"
	                     "4. 11 in 13 system = %CV.\n"
	                     "5. 'a' from 11 to 10 system = %to.\n"
	                     "6. 'Z' from 36 to 10 system = %TO.\n"
	                     "7. memory dump of 5 = %mi.\n"
	                     "8. memory dump of 45 = %mu.\n"
	                     "9. memory dump of 5.4 = %md.\n"
	                     "10. memory dump of 52.3 = %mf.\n"
	                     "11. percent symbol = %%.\n"
	                     "12. standard specification with flag = %.2f.\n",
	                     19, 54, 10, 5, 11, 13, "a", 11, "Z", 36, 5, 45, 5.4, 52.3, 45.3);
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case NEGATIVE_NUMBER:
			printf("Negative number.\n");
			return 1;
		case WRONG_NUMBER:
			printf("Wrong number.\n");
			return 1;
	}
	fclose(file);
	return 0;
}

statusCode oversprintf(char* buffer, const char* format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char* bufferPtr = buffer;
	char* tempBuffer = (char*)malloc(sizeof(char) * (StringLength(format) + 1));
	if (tempBuffer == NULL) {
		va_end(arguments);
		return MEMORY_ALLOCATION_ERROR;
	}
	StringCopy(tempBuffer, format);
	char* position = tempBuffer;
	char* numberString;
	statusCode status;
	char* specifier;
	while (*position != '\0') {
		if (*position == '%') {
			position++;
			if (*position == '%') {
				*bufferPtr++ = '%';
				position++;
				continue;
			}
			switch (*position) {
				case 'R':
					if (*(position + 1) == 'o') {
						int elem = va_arg(arguments, int);
						if (elem < 0) {
							free(tempBuffer);
							va_end(arguments);
							return NEGATIVE_NUMBER;
						}
						if (elem > MAX_ROMAN_NUMBER) {
							free(tempBuffer);
							va_end(arguments);
							return WRONG_NUMBER;
						}
						status = FlagRo(&numberString, elem);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(tempBuffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						StringCopy(bufferPtr, numberString);
						bufferPtr += StringLength(numberString);
						free(numberString);
						position += 2;
					}
					break;
				case 'Z':
					if (*(position + 1) == 'r') {
						unsigned int elem = va_arg(arguments, unsigned int);
						status = FlagZr(&numberString, elem);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(tempBuffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						StringCopy(bufferPtr, numberString);
						bufferPtr += StringLength(numberString);
						free(numberString);
						position += 2;
					}
					break;
				case 'C':
					if (*(position + 1) == 'v' || *(position + 1) == 'V') {
						int number = va_arg(arguments, int);
						int base = va_arg(arguments, int);
						if (*(position + 1) == 'v') {
							status = FlagCv(&numberString, number, base);
						} else if (*(position + 1) == 'V') {
							status = FlagCV(&numberString, number, base);
						}
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(tempBuffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						StringCopy(bufferPtr, numberString);
						bufferPtr += StringLength(numberString);
						free(numberString);
						position += 2;
					}
					break;
				case 't':
					if (*(position + 1) == 'o') {
						char* argString = va_arg(arguments, char*);
						int base = va_arg(arguments, int);
						if (IsValid(argString, base) == WRONG_NUMBER) {
							free(tempBuffer);
							va_end(arguments);
							return WRONG_NUMBER;
						}
						long long int result;
						status = FlagTo(argString, &result, base);
						int length = snprintf(bufferPtr, MAX_LEN_RESULT, "%lld", result);
						bufferPtr += length;
						position += 2;
					}
					break;
				case 'T':
					if (*(position + 1) == 'O') {
						char* argString = va_arg(arguments, char*);
						int base = va_arg(arguments, int);
						if (IsValid(argString, base) == WRONG_NUMBER) {
							free(tempBuffer);
							va_end(arguments);
							return WRONG_NUMBER;
						}
						long long int result;
						status = FlagTO(argString, &result, base);
						int length = snprintf(bufferPtr, MAX_LEN_RESULT, "%lld", result);
						bufferPtr += length;
						position += 2;
					}
					break;
				case 'm':
					if (*(position + 1) == 'i') {
						int32_t number = va_arg(arguments, int32_t);
						status = FlagMi(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(tempBuffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						StringCopy(bufferPtr, numberString);
						bufferPtr += StringLength(numberString);
						free(numberString);
						position += 2;
					} else if (*(position + 1) == 'u') {
						uint32_t number = va_arg(arguments, uint32_t);
						status = FlagMu(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(tempBuffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						StringCopy(bufferPtr, numberString);
						bufferPtr += StringLength(numberString);
						free(numberString);
						position += 2;
					} else if (*(position + 1) == 'd') {
						double number = va_arg(arguments, double);
						status = FlagMD(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(tempBuffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						StringCopy(bufferPtr, numberString);
						bufferPtr += StringLength(numberString);
						free(numberString);
						position += 2;
					} else if (*(position + 1) == 'f') {
						float number = va_arg(arguments, double);
						status = FlagMf(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(tempBuffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						StringCopy(bufferPtr, numberString);
						bufferPtr += StringLength(numberString);
						free(numberString);
						position += 2;
					}
					break;
				default:
					specifier = (char*)malloc(sizeof(int) * LENGTH_SPECIFICATOR);
					if (specifier == NULL) {
						free(tempBuffer);
						va_end(arguments);
						return MEMORY_ALLOCATION_ERROR;
					}
					int index = 0;
					specifier[index++] = '%';
					while (*position != '\0' && *position != '%') {
						specifier[index++] = *position++;
					}
					specifier[index] = '\0';
					int length = vsnprintf(bufferPtr, LENGTH_SPECIFICATOR, specifier, arguments);
					bufferPtr += length;
					free(specifier);
					break;
			}
		} else {
			*bufferPtr++ = *position++;
		}
	}
	*bufferPtr = '\0';
	free(tempBuffer);
	va_end(arguments);
	return NORMAL;
}

statusCode overfprintf(FILE* file, const char* format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char* buffer = (char*)malloc(sizeof(char) * (StringLength(format) + 1));
	if (buffer == NULL) {
		va_end(arguments);
		return MEMORY_ALLOCATION_ERROR;
	}
	StringCopy(buffer, format);
	char* position = buffer;
	char* numberString;
	statusCode status;
	char* specifier;

	while (*position != '\0') {
		if (*position == '%') {
			position++;
			if (*position == '%') {
				fprintf(file, "%%");
				position++;
				continue;
			}
			switch (*position) {
				case 'R':
					if (*(position + 1) == 'o') {
						int elem = va_arg(arguments, int);
						if (elem < 0) {
							free(buffer);
							va_end(arguments);
							return NEGATIVE_NUMBER;
						}
						if (elem > MAX_ROMAN_NUMBER) {
							free(buffer);
							va_end(arguments);
							return WRONG_NUMBER;
						}
						status = FlagRo(&numberString, elem);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(buffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						fprintf(file, "%s", numberString);
						free(numberString);
						position += 2;
					}
					break;
				case 'Z':
					if (*(position + 1) == 'r') {
						unsigned int elem = va_arg(arguments, unsigned int);
						status = FlagZr(&numberString, elem);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(buffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						fprintf(file, "%s", numberString);
						free(numberString);
						position += 2;
					}
					break;
				case 'C':
					if (*(position + 1) == 'v' || *(position + 1) == 'V') {
						int number = va_arg(arguments, int);
						int base = va_arg(arguments, int);
						if (*(position + 1) == 'v') {
							status = FlagCv(&numberString, number, base);
						} else if (*(position + 1) == 'V') {
							status = FlagCV(&numberString, number, base);
						}
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(buffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						fprintf(file, "%s", numberString);
						free(numberString);
						position += 2;
					}
					break;
				case 't':
					if (*(position + 1) == 'o') {
						char* argString = va_arg(arguments, char*);
						int base = va_arg(arguments, int);
						if (IsValid(argString, base) == WRONG_NUMBER) {
							free(buffer);
							va_end(arguments);
							return WRONG_NUMBER;
						}
						long long int result;
						status = FlagTo(argString, &result, base);
						fprintf(file, "%lld", result);
						position += 2;
					}
					break;
				case 'T':
					if (*(position + 1) == 'O') {
						char* argString = va_arg(arguments, char*);
						int base = va_arg(arguments, int);
						if (IsValid(argString, base) == WRONG_NUMBER) {
							free(buffer);
							va_end(arguments);
							return WRONG_NUMBER;
						}
						long long int result;
						status = FlagTO(argString, &result, base);
						fprintf(file, "%lld", result);
						position += 2;
					}
					break;
				case 'm':
					if (*(position + 1) == 'i') {
						int32_t number = va_arg(arguments, int32_t);
						status = FlagMi(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(buffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						fprintf(file, "%s", numberString);
						free(numberString);
						position += 2;
					} else if (*(position + 1) == 'u') {
						uint32_t number = va_arg(arguments, uint32_t);
						status = FlagMu(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(buffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						fprintf(file, "%s", numberString);
						free(numberString);
						position += 2;
					} else if (*(position + 1) == 'd') {
						double number = va_arg(arguments, double);
						status = FlagMD(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(buffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						fprintf(file, "%s", numberString);
						free(numberString);
						position += 2;
					} else if (*(position + 1) == 'f') {
						float number = va_arg(arguments, double);
						status = FlagMf(&numberString, &number);
						if (status == MEMORY_ALLOCATION_ERROR) {
							free(buffer);
							va_end(arguments);
							return MEMORY_ALLOCATION_ERROR;
						}
						fprintf(file, "%s", numberString);
						free(numberString);
						position += 2;
					}
					break;
				default:
					specifier = (char*)malloc(sizeof(char) * LENGTH_SPECIFICATOR);
					if (specifier == NULL) {
						free(buffer);
						va_end(arguments);
						return MEMORY_ALLOCATION_ERROR;
					}
					int index = 0;
					specifier[index++] = '%';
					while (*position != '\0' && *position != '%') {
						specifier[index++] = *position++;
					}
					specifier[index] = '\0';
					int length = vfprintf(file, specifier, arguments);
					if (length < 0) {
						free(buffer);
						free(specifier);
						va_end(arguments);
						return MEMORY_ALLOCATION_ERROR;
					}
					free(specifier);
					break;
			}
		} else {
			fprintf(file, "%c", *position);
			position++;
		}
	}
	free(buffer);
	va_end(arguments);
	return NORMAL;
}

statusCode FlagRo(char** numberString, int intNumber) {
	RomanNumbers numbers[ROMAN_SYMBOLS] = {romM,  romCM, romD,  romCD, romC,  romXC, romL,
	                                       romXL, romX,  romIX, romV,  romIV, romI};
	char* symbols[ROMAN_SYMBOLS] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
	int total = 0;
	int stringSize = 100;
	*numberString = (char*)malloc(sizeof(char) * (stringSize + 2));
	if (*numberString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	if (intNumber < 0) {
		intNumber = -intNumber;
		(*numberString)[total++] = '-';
	}
	if (intNumber == 0) {
		(*numberString)[0] = '0';
		(*numberString)[1] = '\0';
		return NORMAL;
	}
	for (int i = 0; i < ROMAN_SYMBOLS; i++) {
		while (intNumber >= numbers[i]) {
			intNumber -= numbers[i];
			if (total + StringLength(symbols[i]) + 1 >= stringSize) {
				stringSize *= 2;
				char* tempString = (char*)realloc(*numberString, sizeof(char) * (stringSize + 2));
				if (tempString == NULL) {
					free(*numberString);
					return MEMORY_ALLOCATION_ERROR;
				}
				*numberString = tempString;
			}
			StringCopy(*numberString + total, symbols[i]);
			total += StringLength(symbols[i]);
		}
	}
	(*numberString)[total] = '\0';
	return NORMAL;
}

statusCode FlagZr(char** numberString, unsigned int number) {
	int fibonacciCount = 100;
	unsigned int* fibonacciNumbers = (unsigned int*)malloc(sizeof(unsigned int) * fibonacciCount);
	if (fibonacciNumbers == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	unsigned int previousFib = 1;
	unsigned int nextFib = 2;
	fibonacciNumbers[0] = 1;
	fibonacciNumbers[1] = 2;
	int index = 2;
	int total = 2;
	while (nextFib < number) {
		unsigned int temp = nextFib;
		nextFib = previousFib + nextFib;
		previousFib = temp;
		total++;
		if (index + 1 > fibonacciCount) {
			fibonacciCount *= 2;
			unsigned int* tempNumbers = (unsigned int*)realloc(fibonacciNumbers, sizeof(unsigned int) * fibonacciCount);
			if (tempNumbers == NULL) {
				free(fibonacciNumbers);
				return MEMORY_ALLOCATION_ERROR;
			}
			fibonacciNumbers = tempNumbers;
		}
		fibonacciNumbers[index] = nextFib;
		index++;
	}
	*numberString = (char*)malloc(sizeof(char) * (total + 2));
	if (*numberString == NULL) {
		free(fibonacciNumbers);
		return MEMORY_ALLOCATION_ERROR;
	}
	int j = 0;
	for (int i = total - 1; i >= 0; i--) {
		if (number >= fibonacciNumbers[i]) {
			(*numberString)[j++] = '1';
			number -= fibonacciNumbers[i];
		} else {
			(*numberString)[j++] = '0';
		}
	}
	int len = j;
	for (int i = 0; i < len / 2; i++) {
		char temp = (*numberString)[i];
		(*numberString)[i] = (*numberString)[len - i - 1];
		(*numberString)[len - i - 1] = temp;
	}
	(*numberString)[j++] = '1';
	(*numberString)[j] = '\0';
	free(fibonacciNumbers);
	return NORMAL;
}

statusCode FlagCv(char** numberString, int number, int base) {
	int bufferSize = 100;
	*numberString = (char*)malloc(sizeof(char) * bufferSize);
	if (base < 2 || base > 36) {
		base = 10;
	}
	if (*numberString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	if (number == 0) {
		(*numberString)[0] = '0';
		(*numberString)[1] = '\0';
		return NORMAL;
	}
	int total = 0;
	int index = 0;
	char isNegative = 0;
	if (number < 0) {
		isNegative = 1;
		number = -number;
	}
	while (number > 0) {
		int digit = number % base;
		if (digit < 10) {
			total++;
			if (total >= bufferSize) {
				bufferSize *= 2;
				char* tempString = (char*)realloc(*numberString, sizeof(char) * bufferSize);
				if (tempString == NULL) {
					free(*numberString);
					return MEMORY_ALLOCATION_ERROR;
				}
				*numberString = tempString;
			}
			(*numberString)[index++] = digit + '0';
		} else {
			if (total >= bufferSize) {
				bufferSize *= 2;
				char* tempString = (char*)realloc(*numberString, sizeof(char) * bufferSize);
				if (tempString == NULL) {
					free(*numberString);
					return MEMORY_ALLOCATION_ERROR;
				}
				*numberString = tempString;
			}
			(*numberString)[index++] = digit + 'a' - 10;
		}
		number /= base;
	}
	if (isNegative) {
		total++;
		if (total >= bufferSize) {
			bufferSize *= 2;
			char* tempString = (char*)realloc(*numberString, sizeof(char) * bufferSize);
			if (tempString == NULL) {
				free(*numberString);
				return MEMORY_ALLOCATION_ERROR;
			}
			*numberString = tempString;
		}
		(*numberString)[index++] = '-';
	}
	(*numberString)[index] = '\0';
	int i = 0;
	for (i; i < index / 2; i++) {
		char tempChar = (*numberString)[i];
		(*numberString)[i] = (*numberString)[index - i - 1];
		(*numberString)[index - i - 1] = tempChar;
	}
	return NORMAL;
}

statusCode FlagCV(char** numberString, int number, int base) {
	int bufferSize = 100;
	*numberString = (char*)malloc(sizeof(char) * bufferSize);
	if (base < 2 || base > 36) {
		base = 10;
	}
	if (*numberString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	if (number == 0) {
		(*numberString)[0] = '0';
		(*numberString)[1] = '\0';
		return NORMAL;
	}
	int total = 0;
	int index = 0;
	char isNegative = 0;
	if (number < 0) {
		isNegative = 1;
		number = -number;
	}
	while (number > 0) {
		int digit = number % base;
		if (digit < 10) {
			total++;
			if (total >= bufferSize) {
				bufferSize *= 2;
				char* tempString = (char*)realloc(*numberString, sizeof(char) * bufferSize);
				if (tempString == NULL) {
					free(*numberString);
					return MEMORY_ALLOCATION_ERROR;
				}
				*numberString = tempString;
			}
			(*numberString)[index++] = digit + '0';
		} else {
			if (total >= bufferSize) {
				bufferSize *= 2;
				char* tempString = (char*)realloc(*numberString, sizeof(char) * bufferSize);
				if (tempString == NULL) {
					free(*numberString);
					return MEMORY_ALLOCATION_ERROR;
				}
				*numberString = tempString;
			}
			(*numberString)[index++] = digit + 'A' - 10;
		}
		number /= base;
	}
	if (isNegative) {
		total++;
		if (total >= bufferSize) {
			bufferSize *= 2;
			char* tempString = (char*)realloc(*numberString, sizeof(char) * bufferSize);
			if (tempString == NULL) {
				free(*numberString);
				return MEMORY_ALLOCATION_ERROR;
			}
			*numberString = tempString;
		}
		(*numberString)[index++] = '-';
	}
	(*numberString)[index] = '\0';
	int i = 0;
	for (i; i < index / 2; i++) {
		char tempChar = (*numberString)[i];
		(*numberString)[i] = (*numberString)[index - i - 1];
		(*numberString)[index - i - 1] = tempChar;
	}
	return NORMAL;
}

statusCode FlagTo(const char* stringNumber, long long int* intNumber, int base) {
	if (base > 36 || base < 2) base = 10;
	*intNumber = 0;
	int isNegative = 0;
	if (*stringNumber == '-') {
		isNegative = 1;
		stringNumber++;
	}
	for (int i = 0; i < StringLength(stringNumber); i++) {
		char elem = stringNumber[i];
		int digit;
		if (elem >= '0' && elem <= '9') {
			digit = elem - '0';
		} else if (elem >= 'a' && elem <= 'z') {
			digit = elem - 'a' + 10;
		} else {
			digit = -1;
		}
		if (digit >= base) {
			return WRONG_NUMBER;
		}
		*intNumber = *intNumber * base + digit;
	}
	if (isNegative) {
		*intNumber = -(*intNumber);
	}
	return NORMAL;
}

statusCode FlagTO(const char* stringNumber, long long int* intNumber, int base) {
	if (base > 36 || base < 2) base = 10;
	*intNumber = 0;
	int isNegative = 0;
	if (*stringNumber == '-') {
		isNegative = 1;
		stringNumber++;
	}
	for (int i = 0; i < StringLength(stringNumber); i++) {
		char elem = stringNumber[i];
		int digit;
		if (elem >= '0' && elem <= '9') {
			digit = elem - '0';
		} else if (elem >= 'A' && elem <= 'Z') {
			digit = elem - 'A' + 10;
		} else {
			digit = -1;
		}
		if (digit >= base) {
			return WRONG_NUMBER;
		}
		*intNumber = *intNumber * base + digit;
	}
	if (isNegative) {
		*intNumber = -(*intNumber);
	}
	return NORMAL;
}

statusCode FlagMi(char** stringDump, int32_t* number) {
	*stringDump = (char*)malloc(sizeof(char) * 36);
	if (*stringDump == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	unsigned char* ptr = (unsigned char*)number;
	for (int i = 0; i < sizeof(int32_t); i++) {
		for (int j = 7; j >= 0; j--) {
			(*stringDump)[index++] = ((ptr[i] >> j) & 1) + '0';
		}
		if (i < sizeof(int32_t) - 1) {
			(*stringDump)[index++] = ' ';
		}
	}
	(*stringDump)[index] = '\0';
	return NORMAL;
}

statusCode FlagMu(char** stringDump, uint32_t* number) {
	*stringDump = (char*)malloc(sizeof(char) * 36);
	if (*stringDump == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	unsigned char* ptr = (unsigned char*)number;
	for (int i = 0; i < sizeof(uint32_t); i++) {
		for (int j = 7; j >= 0; j--) {
			(*stringDump)[index++] = ((ptr[i] >> j) & 1) + '0';
		}
		if (i < sizeof(uint32_t) - 1) {
			(*stringDump)[index++] = ' ';
		}
	}
	(*stringDump)[index] = '\0';
	return NORMAL;
}

statusCode FlagMD(char** stringDump, double* number) {
	*stringDump = (char*)malloc(sizeof(char) * 72);
	if (*stringDump == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	unsigned char* ptr = (unsigned char*)number;
	for (int i = 0; i < sizeof(double); i++) {
		for (int j = 7; j >= 0; j--) {
			(*stringDump)[index++] = ((ptr[i] >> j) & 1) + '0';
		}
		if (i < sizeof(double) - 1) {
			(*stringDump)[index++] = ' ';
		}
	}
	(*stringDump)[index] = '\0';
	return NORMAL;
}

statusCode FlagMf(char** stringDump, float* number) {
	*stringDump = (char*)malloc(sizeof(char) * 36);
	if (*stringDump == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	unsigned char* ptr = (unsigned char*)number;
	for (int i = 0; i < sizeof(float); i++) {
		for (int j = 7; j >= 0; j--) {
			(*stringDump)[index++] = ((ptr[i] >> j) & 1) + '0';
		}
		if (i < sizeof(float) - 1) {
			(*stringDump)[index++] = ' ';
		}
	}
	(*stringDump)[index] = '\0';
	return NORMAL;
}

int StringLength(const char* str) {
	int length = 0;
	while (*str != '\0') {
		length++;
		str++;
	}
	return length;
}

char* StringCopy(char* newString, const char* oldString) {
	if (newString == NULL || oldString == NULL) {
		return NULL;
	}
	char* pointerNewString = newString;
	while (*oldString != '\0') {
		*newString = *oldString;
		newString++;
		oldString++;
	}
	*newString = '\0';
	return pointerNewString;
}

int IsDigit(char c) { return (c >= '0' && c <= '9'); }

int IsAlpha(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }

char ToLower(char c) { return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }

statusCode IsValid(const char* number, int base) {
	while (*number == ' ') number++;
	if (*number == '-') number++;
	while (*number != '\0') {
		if (IsDigit(*number)) {
			if (*number - '0' >= base) return WRONG_NUMBER;
		} else if (IsAlpha(*number)) {
			if (ToLower(*number) - 'a' >= base - 10) return WRONG_NUMBER;
		} else {
			return WRONG_NUMBER;
		}
		number++;
	}
	return NORMAL;
}