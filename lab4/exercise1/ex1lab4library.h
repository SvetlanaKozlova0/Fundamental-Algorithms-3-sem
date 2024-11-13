#ifndef EX1LAB4LIBRARY_H
#define EX1LAB4LIBRARY_H
#include <stdio.h>

typedef enum statusCode {
	NORMAL,
	MEMORY_ALLOCATION_ERROR,
	FILE_OPEN_ERROR,
	WRONG_ARGUMENTS,
	USUAL_LINE,
	DEFINE_LINE,
	NOT_FOUND,
	UNEQUAL_TABLE,
	NORMAL_TABLE,
	FILE_WRITING_ERROR
} statusCode;

typedef struct nodeTable {
	char* key;
	char* value;
	struct nodeTable* next;
} nodeTable;

typedef struct hashTable {
	// current length && hashSize
	int hashSize;
	// array of nodes, where we have value, key, ptr
	nodeTable** array;
	// array of ints, where lying amount of elements in every node
	int* countElements;
} hashTable;

typedef enum constants {
	HASH_SIZE = 128,
	SYSTEM_BASE = 62,
	START_LENGTH_BUFFER = 100,
	START_LENGTH_WORD = 10
} constants;

// getting file name
statusCode GetFileName(int argc, char** argv, char** fileName);


// file processing. this function is reading file and writing to output changed version
statusCode FileProcessing(const char* inputFile, const char* outputFile);

//creating hash table,
statusCode CreateHashTable(hashTable* table, int hashSize);

//creating node table, returns the status
statusCode CreateNodeTable(nodeTable* node, const char* key, const char* value);

//insert into hash table
statusCode InsertHashTable(hashTable* table, const char* key, const char* value);

//find in hash table , writing to char** value
statusCode FindHashTable(hashTable* table, const char* key, char** value);

//checking are we need to reorganise table
statusCode TableIsRight(hashTable* table);

// destroying hash table
void DestroyHashTable(hashTable* table);

//hash function
int HashFunctionModule(const char* key, int hashSize);

statusCode ProcessingString(const char* string, FILE* output, hashTable* table);

statusCode InsertAndCheck(hashTable* table, const char* key, const char* value);

statusCode IsReplacementString(const char* string);

statusCode GetReplace(const char* string, char** key, char** value);

statusCode OverwriteTheFile(const char* inputFile, const char* outputFile);

statusCode ClearFile(const char* fileName);
#endif