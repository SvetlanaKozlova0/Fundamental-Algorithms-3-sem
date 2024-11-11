#ifndef EX1LAB4LIBRARY_H
#define EX1LAB4LIBRARY_H

typedef enum statusCode {
	NORMAL,
	MEMORY_ALLOCATION_ERROR,
	FILE_OPEN_ERROR,
	WRONG_ARGUMENTS
} statusCode;

typedef struct nodeMap {
	char* key;
	char* value;
	struct nodeMap* next;
} nodeMap;

typedef struct hashMap {
	int amountElements;
	int capacity;
	nodeMap** head;
	int maximumChain;
	int minimumChain;
} hashMap;

typedef enum constants {
	HASHSIZE = 128,
	SYSTEM_BASE = 62
} constants;

//getting file name
statusCode GetFileName(int argc, char** argv, char** fileName);

//create new node
void CreateNodeMap(nodeMap* currentNode, char* key, char* value);
//create hash map
statusCode CreateHashMap(hashMap* currentHashMap);
//insert into
statusCode InsertHashMap(hashMap* headMap, char* key, char* value);
//search
char* SearchHashMap(hashMap* headMap, char* key);
//free hashMap
void FreeHashMap(hashMap** headMap);


#endif