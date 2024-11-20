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
	INVALID_SYNTAX,
	NOT_EMPTY,
	EMPTY_ARRAY,
	INVALID_BOUNDARIES,
	INVALID_BORDER_ORDER,
	INCORRECT_FREE
} statusCode;

typedef enum constants {
	START_BUFFER_SIZE = 100,
	ALPHABET_LENGTH = 26,
	START_AMOUNT_NUMBERS = 10,
	START_LENGTH_NAME = 10
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

typedef struct Command {
	typeInstruction type;
	statusCode (*function)(Array**, char*);
} Command;

int getNumberArray(char c);

statusCode CommandLoad(Array** allArrays, char* string);
statusCode CommandSave(Array** allArrays, char* string);
statusCode CommandRand(Array** allArrays, char* string);
statusCode CommandFree(Array** allArrays, char* string);
statusCode CommandConcat(Array** allArrays, char* string);
statusCode CommandRemove(Array** allArrays, char* string);
statusCode CommandCopy(Array** allArrays, char* string);
statusCode CommandSort(Array** allArrays, char* string);
statusCode CommandShuffle(Array** allArrays, char* string);
statusCode CommandStats(Array** allArrays, char* string);
statusCode CommandPrint(Array** allArrays, char* string);


statusCode TypeInstruction(const char* string, typeInstruction* type);
statusCode IsValidCommand(const char* string, typeInstruction type);
statusCode InstructionsProcessing(const char* fileName);
statusCode OneInstructionProcessing(Array** allArrays, char* string, typeInstruction type);

void ReadPrintLine(const char* line, int* numberArray, int* start, int* end, int* current, int* flagAll, int* flagStat);
void ReadPermutationLine(const char* line, int* numberArray, int* flagIncrease);
void ReadFreeLine(const char* line, int* numberArray);
void ReadConcatLine(const char* line, int* firstArray, int* secondArray);
void ReadRandomLine(const char* line, int* numberArray, int* count, int* lb, int* rb);
void ReadRemoveLine(const char* line, int* numberArray, int* first, int* second);
void ReadCopyLine(const char* line, int* numberArray, int* first, int* second, int* secondArray);
statusCode ReadLoadSave(const char* line, int* numberArray, char** fileName);

statusCode ValidateLoad(const char* string);
statusCode ValidateRand(const char* string);
statusCode ValidateConcat(const char* string);
statusCode ValidateFree(const char* string);
statusCode ValidateRemove(const char* string);
statusCode ValidateCopy(const char* string);
statusCode ValidateSort(const char* string);
statusCode ValidateShuffle(const char* string);
statusCode ValidatePrint(const char* string) ;
#endif