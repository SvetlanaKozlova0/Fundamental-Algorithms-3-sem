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

void CreateNodeTable(nodeTable* currentNode, char* key, char* value, int index) {
	currentNode->key = key;
	currentNode->value = value;
	currentNode->index = index;
	currentNode->next = NULL;
	currentNode->countChildren = 0;
}

statusCode CreateHashTable(hashTable* currentHashTable, int hashSize) {
	currentHashTable->amountElements = 0;
	currentHashTable->capacity = hashSize;
	currentHashTable->head = (nodeTable**)malloc(sizeof(nodeTable*) * hashSize);
	if (currentHashTable->head == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	currentHashTable->lengthIndexes = hashSize;
	currentHashTable->indexes = (unsigned long int*)malloc(sizeof(unsigned long int) * hashSize);
	if (currentHashTable->indexes == NULL) {
		free(currentHashTable->head);
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < hashSize; i++) {
		currentHashTable->head[i] = NULL;
	}
	return NORMAL;
}

int getDigit(char c) {
	if (IsDigit(c)) return c - '0';
	if (c >= 'a' && c <= 'z') return c - 'a' + 10;
	return c - 'A' + 36;
}

unsigned long int HashFunction(char* key) {
	unsigned long int hashResult = 0;
	for (int i = 0; i < StringLength(key); i++) {
		hashResult *= SYSTEM_BASE;
		hashResult += getDigit(key[i]);
	}
	return hashResult;
}

statusCode InsertHashTable(hashTable* table, char* key, char* value) {
	unsigned long int elemHash = HashFunction(key);
	if (table->lengthIndexes < table->amountElements + 1) {
		table->lengthIndexes *= 2;
		unsigned long int* tempIndexes = (unsigned long int*)realloc(table->indexes, table->lengthIndexes);
		if (tempIndexes == NULL) {
			return MEMORY_ALLOCATION_ERROR;
		}
		table->indexes = tempIndexes;
	}
	table->indexes[table->amountElements] = elemHash;
	elemHash = elemHash % table->capacity;
	nodeTable* newNode = (nodeTable*)malloc(sizeof(nodeTable));
	if (newNode == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	CreateNodeTable(newNode, key, value, table->amountElements);
	if (table->head[elemHash] == NULL) {
		table->head[elemHash] = newNode;
	} else {
		nodeTable* temp = table->head[elemHash];
		while (temp->next != NULL) {
			temp->countChildren++;
			temp = temp->next;
		}
		temp->next = newNode;
	}
	table->amountElements++;
	return NORMAL;
}

char* FindHashTable(hashTable* table, char* key) {
	unsigned long int elemHash = HashFunction(key) % table->capacity;
	if (table->head == NULL) {
		return NULL;
	}
	if (table->head[elemHash] == NULL) {
		return NULL;
	}
	nodeTable* temp = table->head[elemHash];
	while (temp != NULL) {
		if (CompareChars(temp->key, key) == 0) {
			return temp->value;
		}
		temp = temp->next;
	}
	return NULL;
}

int isPrime(int number) {
	if (number <= 1) return 0;
	for (int i = 2; i * i <= number; i++) {
		if (number % i == 0) return 0;
	}
	return 1;
}

int getNewHashSize(int hashSize) {
	hashSize = hashSize * 2;
	int current = hashSize;
	while (1) {
		if (isPrime(current)) return current;
		current++;
	}
}

statusCode InsertAndCheck(hashTable* table, char* key, char* value) {
	statusCode status = InsertHashTable(table, key, value);
	if (status == MEMORY_ALLOCATION_ERROR) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int minimumChain = table->capacity;
	int maximumChain = 0;
	for (int i = 0; i < table->capacity; i++) {
		if (table->head[i] == NULL) {
			minimumChain = 0;
		} else {
			int countChildren = table->head[i]->countChildren;
			if (minimumChain > countChildren) minimumChain = countChildren;
			if (maximumChain < countChildren) maximumChain = countChildren;
		}
	}
	if (minimumChain == 0 || maximumChain / minimumChain < 2) {
		return NORMAL;
	}
	hashTable* newTable = (hashTable*)malloc(sizeof(hashTable));
	if (newTable == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int newHashSize = getNewHashSize(table->capacity);
	status = CreateHashTable(newTable, newHashSize);
	if (status == MEMORY_ALLOCATION_ERROR) {
		free(newTable);
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < table->capacity; i++) {
		if (table->head[i] == NULL) {
			continue;
		} else {
			nodeTable* tempNode = table->head[i];
			while (tempNode != NULL) {
				status = InsertHashTable(newTable, tempNode->key, tempNode->value);
				//add nice deleting
				if (status == MEMORY_ALLOCATION_ERROR) {
					return MEMORY_ALLOCATION_ERROR;
				}
				tempNode = tempNode -> next;
			}
		}
	}
	DestroyHashTable(table);
	return NORMAL;
}

void DestroyHashTable(hashTable* table) {
	for (int i = 0; i < table->capacity; i++) {
		nodeTable* tempNode = table->head[i];
		while (tempNode != NULL) {
			nodeTable* cur = tempNode;
			tempNode = tempNode -> next;
			free(cur);
		}
	}
	free(table->indexes);
	free(table->head);
	free(table);
}

statusCode IsReplacementString(const char* string) {
	if (StringLength(string) < 7) {
		return USUAL_LINE;
	}
	char* goodStart = (char*)malloc(sizeof(char) * 8);
	if (goodStart == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < 7; i++) goodStart[i] = string[i];
	if (CompareChars(string, goodStart) != 0) {
		free(goodStart);
		return USUAL_LINE;
	}
	free(goodStart);
	int index = 7;
	int flagOldWord = 0;
	int flagNewWord = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) || IsDigit(string[index])) {
		index++;
		flagOldWord = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (string[index] != ' ' && string[index] != '\t' && string[index] != '\0') {
		index++;
		flagNewWord = 1;
	}
	if (flagOldWord && flagNewWord == 1) {
		return DEFINE_LINE;
	}
	return USUAL_LINE;
}

statusCode GetReplace(const char* string, char** key, char** value) {
	int index = 0;
	int lengthKey = 0;
	int lengthValue = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (string[index] == '#' || IsDigit(string[index]) || IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index]) || IsAlpha(string[index])) {
		index++;
		lengthKey = 0;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (string[index] != '\0') {
		index++;
		lengthValue++;
	}
	*key = (char*)malloc(sizeof(char) * (lengthKey + 1));
	if (*key == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	*value = (char*)malloc(sizeof(char) * (lengthValue + 1));
	if (*value == NULL) {
		free(*key);
		return MEMORY_ALLOCATION_ERROR;
	}
	index = 0;
	int currentLengthKey = 0;
	int currentLengthValue = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (string[index] == '#' || IsDigit(string[index]) || IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index]) || IsAlpha(string[index])) (*key)[currentLengthKey++] = string[index++];
	(*key)[currentLengthKey] = '\0';
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (string[index] != '\0') (*value)[currentLengthValue++] = string[index++];
	(*value)[currentLengthValue] = '\0';
	return NORMAL;
}

statusCode FileProcessing(const char* inputFile, const char* outputFile) {
	if (inputFile == NULL || outputFile == NULL) {
		return WRONG_ARGUMENTS;
	}
	FILE* input = fopen(inputFile, "r");
	if (input == NULL) {
		return FILE_OPEN_ERROR;
	}
	FILE* output = fopen(outputFile, "w");
	if (output == NULL) {
		fclose(input);
		return FILE_OPEN_ERROR;
	}
	char* buffer = (char*)malloc(sizeof(char) * START_LENGTH_BUFFER);
	if (buffer == NULL) {
		fclose(input);
		fclose(output);
		return MEMORY_ALLOCATION_ERROR;
	}
	hashTable* table = (hashTable*)malloc(sizeof(hashTable));
	if (table == NULL) {
		fclose(input);
		fclose(output);
		free(buffer);
		return MEMORY_ALLOCATION_ERROR;
	}
	statusCode status = CreateHashTable(table, HASH_SIZE);
	if (status == MEMORY_ALLOCATION_ERROR) {
		fclose(input);
		fclose(output);
		free(buffer);
		free(table);
		return MEMORY_ALLOCATION_ERROR;
	}
	int currentLength = 0;
	int totalLength = START_LENGTH_BUFFER;
	int c = '0';
	while ((c = getc(input)) != EOF) {
		if (c == '\n') {
			buffer[currentLength - 1] = '\0';
			statusCode typeString = IsReplacementString(buffer);
			if (typeString == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(input);
				fclose(output);
				return MEMORY_ALLOCATION_ERROR;
			}
			//if a usual line, we need to seek replaces - it should be changed
			if (typeString == USUAL_LINE) {
				fputs(buffer, output);
			} else if (typeString == DEFINE_LINE) {
				char *key, *value;
				status = GetReplace(buffer, &key, &value);
				if (status == MEMORY_ALLOCATION_ERROR) {
					free(buffer);
					fclose(input);
					fclose(output);
					return MEMORY_ALLOCATION_ERROR;
				}
				status = InsertAndCheck(table, key, value);
				if (status == MEMORY_ALLOCATION_ERROR) {
					free(buffer);
					fclose(input);
					fclose(output);
					free(key);
					free(value);
					return MEMORY_ALLOCATION_ERROR;
				}

			}
			currentLength = 0;
		} else {
			if (currentLength + 1 > totalLength) {
				totalLength *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * totalLength);
				if (tempBuffer == NULL) {
					free(buffer);
					fclose(input);
					fclose(output);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[currentLength++] = (char)c;
		}
	}
	if (currentLength > 0) {
		buffer[currentLength] = '\0';
	}
	free(buffer);
	fclose(input);
	fclose(output);
	return NORMAL;
}