#include "ex1lab4library.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../stringLibrary.h"

statusCode ClearFile(const char* fileName) {
	if (fileName == NULL) {
		return WRONG_ARGUMENTS;
	}
	FILE* file = fopen(fileName, "w");
	if (file == NULL) {
		return FILE_OPEN_ERROR;
	}
	fclose(file);
	return NORMAL;
}

statusCode WriteToFile(const char* string, hashTable* table, FILE* output) {
	char* buffer = (char*)malloc(sizeof(char) * START_LENGTH_BUFFER);
	if (buffer == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	int currentLength = 0;
	int totalLength = START_LENGTH_BUFFER;
	statusCode status = NORMAL;
	while (string[index] != '\0') {
		if (!(IsDigit(string[index]) || IsAlpha(string[index]))) {
			if (currentLength > 0) {
				buffer[currentLength] = '\0';
				char* value;
				status = FindHashTable(table, buffer, &value);
				if (status == MEMORY_ALLOCATION_ERROR) {
					free(buffer);
					return MEMORY_ALLOCATION_ERROR;
				}
				if (status == NOT_FOUND) {
					fprintf(output, "%s", buffer);
				} else {
					fprintf(output, "%s", value);
					free(value);
				}
				currentLength = 0;
			}
			fputc(string[index], output);
		} else {
			if (currentLength + 1 >= totalLength) {
				totalLength *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * totalLength);
				if (tempBuffer == NULL) {
					free(buffer);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[currentLength++] = string[index];
		}
		index++;
	}
	if (currentLength > 0) {
		buffer[currentLength] = '\0';
		char* value;
		status = FindHashTable(table, buffer, &value);
		if (status == MEMORY_ALLOCATION_ERROR) {
			free(buffer);
			return MEMORY_ALLOCATION_ERROR;
		}
		if (status == NOT_FOUND) {
			fprintf(output, "%s", buffer);
		} else {
			fprintf(output, "%s", value);
			free(value);
		}
		currentLength = 0;
	}
	free(buffer);
	return NORMAL;
}

statusCode OverwriteTheFile(const char* inputFile, const char* outputFile) {
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
	int c;
	while ((c = getc(input)) != EOF) {
		if (fputc(c, output) == EOF) {
			fclose(input);
			fclose(output);
			return FILE_WRITING_ERROR;
		}
	}
	fclose(input);
	fclose(output);
	return NORMAL;
}

statusCode FileProcessing(const char* inputFile, const char* outputFile) {
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
	int c = '0';
	int currentLength = 0;
	int totalLength = START_LENGTH_BUFFER;
	statusCode status = NORMAL;
	hashTable* table = (hashTable*)malloc(sizeof(hashTable));
	if (table == NULL) {
		free(buffer);
		fclose(input);
		fclose(output);
		return MEMORY_ALLOCATION_ERROR;
	}
	status = CreateHashTable(table, HASH_SIZE);
	if (status == MEMORY_ALLOCATION_ERROR) {
		free(buffer);
		fclose(input);
		fclose(output);
		free(table);
		return MEMORY_ALLOCATION_ERROR;
	}
	while ((c = getc(input)) != EOF) {
		if (c == '\n') {
			if (currentLength == 0) {
				fprintf(output, "\n");
				continue;
			}
			buffer[currentLength] = '\0';
			status = IsReplacementString(buffer);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(input);
				fclose(output);
				DestroyHashTable(table);
				return MEMORY_ALLOCATION_ERROR;
			}
			if (status == DEFINE_LINE) {
				char *key, *value;
				status = GetReplace(buffer, &key, &value);
				if (status == MEMORY_ALLOCATION_ERROR) {
					free(buffer);
					fclose(input);
					fclose(output);
					DestroyHashTable(table);
					return MEMORY_ALLOCATION_ERROR;
				}
				status = InsertAndCheck(table, key, value);
				if (status == MEMORY_ALLOCATION_ERROR) {
					free(buffer);
					fclose(input);
					fclose(output);
					free(key);
					free(value);
					DestroyHashTable(table);
					return MEMORY_ALLOCATION_ERROR;
				}
				free(key);
				free(value);
			} else {
				status = WriteToFile(buffer, table, output);
				if (status == MEMORY_ALLOCATION_ERROR) {
					DestroyHashTable(table);
					free(buffer);
					fclose(input);
					fclose(output);
					return MEMORY_ALLOCATION_ERROR;
				}
				fprintf(output, "\n");
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
	DestroyHashTable(table);
	free(buffer);
	fclose(input);
	fclose(output);
	return NORMAL;
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
		lengthKey++;
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
	while (IsDigit(string[index]) || IsAlpha(string[index])) {
		(*key)[currentLengthKey++] = string[index++];
	}
	(*key)[currentLengthKey] = '\0';
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (lengthValue == 0) {
		(*value)[0] = '\0';
		return NORMAL;
	}
	while (string[index] != '\0') (*value)[currentLengthValue++] = string[index++];
	(*value)[currentLengthValue] = '\0';
	return NORMAL;
}

statusCode IsReplacementString(const char* string) {
	if (StringLength(string) < 8) {
		return USUAL_LINE;
	}
	char* goodStart = (char*)malloc(sizeof(char) * 8);
	if (goodStart == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < 7; i++) goodStart[i] = string[i];
	if (CompareChars("#define", goodStart) != 0) {
		free(goodStart);
		return USUAL_LINE;
	}
	free(goodStart);
	int index = 7;
	int flagOldWord = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) || IsDigit(string[index])) {
		index++;
		flagOldWord = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (string[index] != ' ' && string[index] != '\t' && string[index] != '\0') {
		index++;
	}
	if (flagOldWord) {
		return DEFINE_LINE;
	}
	return USUAL_LINE;
}

statusCode GetFileName(int argc, char** argv, char** fileName) {
	if (argc != 2) {
		return WRONG_ARGUMENTS;
	}
	*fileName = argv[1];
	return NORMAL;
}

int getDigit(char c) {
	if (IsDigit(c)) return c - '0';
	if (c >= 'a' && c <= 'z') return c - 'a' + 10;
	return c - 'A' + 36;
}

int HashFunctionModule(const char* key, int hashSize) {
	int hash = 0;
	for (int i = 0; i < StringLength(key); i++) {
		int digit = getDigit(key[i]);
		hash = (hash * SYSTEM_BASE + digit) % hashSize;
	}
	return hash;
}

statusCode CreateHashTable(hashTable* table, int hashSize) {
	table->hashSize = hashSize;
	table->array = (nodeTable**)calloc(hashSize, sizeof(nodeTable*));
	if (table->array == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	table->countElements = (int*)calloc(hashSize, sizeof(int));
	if (table->countElements == NULL) {
		free(table->array);
		return MEMORY_ALLOCATION_ERROR;
	}
	return NORMAL;
}

statusCode CreateNodeTable(nodeTable* node, const char* key, const char* value) {
	node->key = (char*)malloc(sizeof(char) * (StringLength(key) + 1));
	if (node->key == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	char* temp = StringCopy(node->key, key);
	if (temp == NULL) {
		free(node->key);
		return MEMORY_ALLOCATION_ERROR;
	}
	node->value = (char*)malloc(sizeof(char) * (StringLength(value) + 1));
	if (node->value == NULL) {
		free(node->key);
		return MEMORY_ALLOCATION_ERROR;
	}
	temp = StringCopy(node->value, value);
	if (temp == NULL) {
		free(node->key);
		free(node->value);
		return MEMORY_ALLOCATION_ERROR;
	}
	node->next = NULL;
	return NORMAL;
}

statusCode InsertHashTable(hashTable* table, const char* key, const char* value) {
	nodeTable* newNode = (nodeTable*)malloc(sizeof(nodeTable));
	if (newNode == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	statusCode status = CreateNodeTable(newNode, key, value);
	if (status == MEMORY_ALLOCATION_ERROR) {
		free(newNode);
		return MEMORY_ALLOCATION_ERROR;
	}
	int hash = HashFunctionModule(key, table->hashSize);
	if (table->array[hash] == NULL) {
		table->array[hash] = newNode;
	} else {
		nodeTable* temp = table->array[hash];
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
	table->countElements[hash]++;
	return NORMAL;
}

statusCode FindHashTable(hashTable* table, const char* key, char** value) {
	int hash = HashFunctionModule(key, table->hashSize);
	nodeTable* result = table->array[hash];
	if (result == NULL) {
		return NOT_FOUND;
	}
	while (result != NULL && CompareChars(result->key, key) != 0) {
		result = result->next;
	}
	if (result == NULL) {
		return NOT_FOUND;
	}
	*value = (char*)malloc(sizeof(char) * (StringLength(table->array[hash]->value) + 1));
	if (*value == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	char* temp = StringCopy(*value, result->value);
	if (temp == NULL) {
		free(*value);
		return MEMORY_ALLOCATION_ERROR;
	}
	return NORMAL;
}

statusCode TableIsRight(hashTable* table) {
	int minimum = table->hashSize + 1;
	int maximum = 0;
	for (int i = 0; i < table->hashSize; i++) {
		int cur = table->countElements[i];
		if (cur == 0) continue;
		if (cur < minimum) minimum = cur;
		if (cur > maximum) maximum = cur;
	}
	if (minimum == 0) {
		return UNEQUAL_TABLE;
	}
	if (maximum / minimum >= 2) {
		return UNEQUAL_TABLE;
	}
	return NORMAL_TABLE;
}

statusCode InsertAndCheck(hashTable* table, const char* key, const char* value) {
	statusCode status = InsertHashTable(table, key, value);
	if (status == MEMORY_ALLOCATION_ERROR) {
		return MEMORY_ALLOCATION_ERROR;
	}
	status = TableIsRight(table);
	if (status == NORMAL_TABLE) {
		return NORMAL;
	}
	hashTable* newHashTable = (hashTable*)malloc(sizeof(hashTable));
	if (newHashTable == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	status = CreateHashTable(newHashTable, table->hashSize * 2);
	if (status == MEMORY_ALLOCATION_ERROR) {
		free(newHashTable);
		return MEMORY_ALLOCATION_ERROR;
	}
	int amountElements = 0;
	for (int i = 0; i < table->hashSize; i++) {
		amountElements += table->countElements[i];
	}
	int* cacheElements = (int*)malloc(sizeof(int) * amountElements);
	if (cacheElements == NULL) {
		free(newHashTable);
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	for (int i = 0; i < table->hashSize; i++) {
		nodeTable* current = table->array[i];
		while (current != NULL) {
			cacheElements[index++] = HashFunctionModule(current->key, newHashTable->hashSize);
			current = current->next;
		}
	}
	index = 0;
	for (int i = 0; i < table->hashSize; i++) {
		nodeTable* current = table->array[i];
		while (current != NULL) {
			nodeTable* next = current->next;
			current->next = NULL;
			int newHash = cacheElements[index++];
			if (newHashTable->array[newHash] == NULL) {
				newHashTable->array[newHash] = current;
			} else {
				nodeTable* temp = newHashTable->array[newHash];
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = current;
			}
			newHashTable->countElements[newHash]++;
			current = next;
		}
	}
	free(table->array);
	free(table->countElements);
	free(cacheElements);
	*table = *newHashTable;
	free(newHashTable);
	return NORMAL;
}

void DestroyHashTable(hashTable* table) {
	free(table->countElements);
	nodeTable* tempNode = table->array[0];
	for (int i = 0; i < table->hashSize; i++) {
		tempNode = table->array[i];
		while (tempNode != NULL) {
			free(tempNode->key);
			free(tempNode->value);
			nodeTable* t = tempNode;
			tempNode = tempNode->next;
			free(t);
		}
	}
	free(table->array);
	free(table);
}