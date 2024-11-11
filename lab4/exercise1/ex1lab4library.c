#include "ex1lab4library.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../stringLibrary.h"

statusCode GetFileName(int argc, char** argv, char** fileName) {
	if (argc != 2) {
		return WRONG_ARGUMENTS;
	}
	*fileName = argv[1];
	return NORMAL;
}

void CreateNodeMap(nodeMap* currentNode, char* key, char* value) {
	currentNode->key = key;
	currentNode->value = value;
	currentNode->next = NULL;
}

statusCode CreateHashMap(hashMap* currentHashMap) {
	currentHashMap->amountElements = 0;
	currentHashMap->capacity = HASHSIZE;
	currentHashMap->head = (nodeMap**)malloc(sizeof(nodeMap*) * HASHSIZE);
	if (currentHashMap->head == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < currentHashMap->capacity; i++) {
		currentHashMap->head[i] = NULL;
	}
	currentHashMap->maximumChain = 0;
	currentHashMap->minimumChain = 0;
	return NORMAL;
}

int getDigit(char c) {
	if (IsDigit(c)) return c - '0';
	if (c >= 'a' && c <= 'z') return c - 'a' + 10;
	return c - 'A' + 36;
}

unsigned long long int HashFunction(char* key, int hashSize) {
	unsigned long long int hashResult = 0;
	for (int i = 0; i < StringLength(key); i++) {
		hashResult *= SYSTEM_BASE;
		hashResult += getDigit(key[i]);
	}
	return hashResult % hashSize;
}

statusCode InsertHashMap(hashMap* headMap, char* key, char* value) {
	unsigned long long int bucketIndex = HashFunction(key, headMap->capacity);
	nodeMap* newNode = (nodeMap*)malloc(sizeof(nodeMap));
	if (newNode == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	CreateNodeMap(newNode, key, value);
	if (headMap->head[bucketIndex] == NULL) {
		headMap->head[bucketIndex] = newNode;
		if (headMap->minimumChain == 0) headMap->minimumChain = 1;
		if (headMap->maximumChain == 0) headMap->maximumChain = 1;
	} else {
		headMap->maximumChain++;
		newNode->next = headMap->head[bucketIndex];
		headMap->head[bucketIndex] = newNode;
	}
	headMap->amountElements++;
	return NORMAL;
}

char* SearchHashMap(hashMap* headMap, char* key) {
	unsigned long long int bucketIndex = HashFunction(key, headMap->capacity);
	nodeMap* startNode = headMap->head[bucketIndex];
	while (startNode != NULL) {
		if (CompareChars(key, startNode->key) == 0) {
			return startNode->value;
		}
		startNode = startNode->next;
	}
	return NULL;
}

void FreeHashMap(hashMap** headMap) {
	for (int i = 0; i < (*headMap)->capacity; i++) {
		nodeMap* startNode = (*headMap)->head[i];
		while (startNode != NULL) {
			nodeMap* tempNode = startNode->next;
			free(startNode);
			startNode = tempNode;
		}
	}
	free((*headMap)->head);
	free(*headMap);
}