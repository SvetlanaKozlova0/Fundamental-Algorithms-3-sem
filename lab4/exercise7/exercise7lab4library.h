#ifndef EXERCISE7LAB4LIBRARY_H
#define EXERCISE7LAB4LIBRARY_H
typedef enum statusCode {
	NORMAL,
	IS_EMPTY,
	FILE_OPEN_ERROR,
	MEMORY_ALLOCATION_ERROR,
	INVALID_EXPRESSION,
	INVALID_NUMBER_ARGUMENTS,
	UNINIT_VALUE,
	DIVISION_BY_ZERO
} statusCode;

typedef struct MemoryCell {
	char* name;
	int value;
} MemoryCell;

typedef enum constants {
	START_LENGTH_BUFFER = 100,
	START_LENGTH_STRING = 20,
	START_LENGTH_NAME = 10,
	START_AMOUNT_CELLS = 10
} constants;

typedef enum type { TYPE_MATH, TYPE_ASSIGN, TYPE_PRINT, UNKNOWN_TYPE } type;

statusCode GetFileName(int argc, char** argv, char** fileName);

statusCode FileReading(const char* fileName, MemoryCell** cells);
type DetectType(const char* line);
statusCode AssignOperation(MemoryCell** cells, char* line, int* totalAmount, int* currentAmount);
statusCode PrintOperation(const MemoryCell** cells, char* line, int amountCells);
statusCode MathOperation(MemoryCell** cells, char* line, int *amount);
statusCode FindVariable(char* nameVariable, const MemoryCell** cells, MemoryCell** current);
statusCode GetNameAndValue(const char* line, char** name, int* value);
statusCode GetTwoNames(const char* line, char** first, char** second);
int CompareMemoryCells(const void* first, const void* second);
int FindInCellsArray(MemoryCell** array, int amount, char* key);
statusCode PrintCurrentCell(MemoryCell** cells, char* line, int amount);
#endif