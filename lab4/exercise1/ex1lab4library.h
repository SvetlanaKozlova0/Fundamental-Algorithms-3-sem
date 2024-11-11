#ifndef EX1LAB4LIBRARY_H
#define EX1LAB4LIBRARY_H

typedef enum statusCode {
	NORMAL,
	MEMORY_ALLOCATION_ERROR,
	FILE_OPEN_ERROR,
	WRONG_ARGUMENTS,
	USUAL_LINE,
	DEFINE_LINE
} statusCode;

typedef struct nodeTable {
	int index;
	char* key;
	char* value;
	struct nodeTable* next;
	int countChildren;
} nodeTable;

typedef struct hashTable {
	int amountElements;
	int capacity;
	nodeTable** head;
	unsigned long int* indexes;
	int lengthIndexes;
} hashTable;

typedef enum constants {
	HASH_SIZE = 128,
	SYSTEM_BASE = 62,
	START_LENGTH_BUFFER = 100
} constants;

//getting file name
statusCode GetFileName(int argc, char** argv, char** fileName);

//create new node
void CreateNodeMap(nodeTable* currentNode, char* key, char* value, int index);
//create hash table
statusCode CreateHashTable(hashTable* currentHashTable, int hashSize);
//destroy hash table
void DestroyHashTable(hashTable* table);
//inserts an elem and checks if table needs to reorganize
statusCode InsertAndCheck(hashTable* table, char* key, char* value);
//finds and elem in hash table
char* FindHashTable(hashTable* table, char* key);

#endif