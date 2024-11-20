#ifndef EX4LAB4LIBRARY_H
#define EX4LAB4LIBRARY_H
#include <stdio.h>

typedef enum statusCode {
	NORMAL,
	MEMORY_ALLOCATION_ERROR,
	INVALID_EXPRESSION,
	INVALID_RESULT_INDEX,
	INVALID_FIRST_INDEX,
	INVALID_OPERATION,
	INVALID_SECOND_INDEX,
	FILE_OPEN_ERROR,
	INCORRECT_BRACES,
	EMPTY_VECTOR,
	WRONG_NUMBER,
	NOT_EMPTY_VECTOR,
	INVALID_NUMBER,
	INVALID_BASE,
	SAME_FILES
} statusCode;

typedef enum operation {
	DISJUNCTION,
	CONJUNCTION,
	IMPLICATION,
	REVERSE_IMPLICATION,
	EQUIVALENT,
	MOD_2,
	COIMPLICATION,
	SCHAEFFER,
	PEIRCE,
	UNKNOWN
} operation;

typedef struct vector {
	int* values;
	int length;
} vector;

typedef int (*function)(int, int);

typedef enum constants {
	START_LENGTH_BUFFER = 100,
	START_LENGTH_STRING = 10,
	START_AMOUNT_NUMBERS = 10,
	LENGTH_ALPHABET = 26
} constants;

typedef enum type {
	BINARY_OPERATION,
	UNARY_OPERATION,
	OPERATION_READ,
	OPERATION_WRITE,
	UNKNOWN_OPERATION,
	SYNTAX_ERROR
} type;

statusCode FileReading(const char* file, vector** vectors, FILE* output);
statusCode ProcessingString(const char* line, vector** values, FILE* output);
type DetectType(const char* line);
statusCode GetString(char** currentString);
statusCode GetFileName(int argc, char** argv, char** inputFile, char** outputFile);
void PrintVector(vector* binaryVector, FILE* output);
int ValidNumber(const char* number, int base);
statusCode PathsAreSame(const char* inputFile, const char* outputFile);
#endif