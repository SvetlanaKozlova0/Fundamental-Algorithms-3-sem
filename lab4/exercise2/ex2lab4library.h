#ifndef EX2LAB4LIBRARY_H
#define EX2LAB4LIBRARY_H

typedef struct Array {
	int* numbers;
	int length;
} Array;

typedef enum statusCode {
	NORMAL,
	MEMORY_ALLOCATION_ERROR,
	FILE_OPEN_ERROR,
	INCORRECT_COMMAND,
	INDEX_OUT_RANGE,
	INVALID_SYNTAX
} statusCode;

typedef enum constants {
	START_BUFFER_SIZE = 100,
	ALPHABET_LENGTH = 26,
	START_AMOUNT_NUMBERS = 10
} constants;

typedef enum typeInstruction {
	TYPE_LOAD,
	TYPE_SAVE,
	TYPE_RAND,
	TYPE_CONCAT,
	TYPE_FREE,
	TYPE_REMOVE,
	TYPE_COPY,
	TYPE_SORT,
	TYPE_SHUFFLE,
	TYPE_STATS,
	TYPE_PRINT,
	TYPE_UNKNOWN
} typeInstruction;

statusCode TypeInstruction(const char* string, typeInstruction* type);
statusCode SaveInFile(char* fileName, Array* array);
statusCode LoadFromFile(char* fileName, Array* array);
statusCode FillRandom(Array* array, int count, int left, int right);
statusCode ConcatenateArrays(Array* first, Array* second);
void FreeArray(Array* array);
statusCode RemoveElements(Array* array, int index, int amount);
statusCode CopyArray(Array* oldArray, Array* newArray, int left, int right);
void PermutationVariations(Array* array, typeInstruction type, int flagIncrease);
statusCode PrintVariations(Array* array, int start, int end, int current, int flagAll, int flagStat);
statusCode IsValidCommand(const char* string, typeInstruction type);
statusCode InstructionsProcessing(const char* fileName);
#endif