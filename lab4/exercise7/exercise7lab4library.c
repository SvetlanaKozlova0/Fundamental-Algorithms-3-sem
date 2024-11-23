#include "exercise7lab4library.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../stringLibrary.h"

statusCode FileReading(const char* fileName, MemoryCell** cells) {
	FILE* input = fopen(fileName, "r");
	if (input == NULL) {
		return FILE_OPEN_ERROR;
	}
	char* buffer = (char*)malloc(sizeof(char) * START_LENGTH_BUFFER);
	if (buffer == NULL) {
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = '0';
	int currentLength = 0;
	int totalLength = START_LENGTH_BUFFER;
	statusCode status = NORMAL;
	int count = 1;
	int currentAmountCells = 0;
	int totalAmountCells = START_AMOUNT_CELLS;
	(*cells) = (MemoryCell*)malloc(sizeof(MemoryCell) * totalAmountCells);
	if (*cells == NULL) {
		free(buffer);
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	while ((c = getc(input)) != EOF) {
		if (c == ';') {
			buffer[currentLength] = '\0';
			currentLength = 0;
			status = NORMAL;
			type t = DetectType(buffer);
			switch (t) {
				case TYPE_MATH:
					status = MathOperation(cells, buffer, &currentAmountCells);
					if (status == UNINIT_VALUE || status == DIVISION_BY_ZERO) {
						if (status == UNINIT_VALUE)
						printf("An error while performing operation %d (uninitialized value).\n", count);
						else
							    printf("An error while performing operation %d (division by zero).\n", count);
						printf("Operation %d: %s.\n", count, buffer);
						free(buffer);
						fclose(input);
						for (int i = 0; i < currentAmountCells; i++) {
							if ((*cells)[i].name != NULL) {
								free((*cells)[i].name);
							}
						}
						free(*cells);
						return status;
					} else if (status == MEMORY_ALLOCATION_ERROR) {
						printf("Memory allocation  error.\n");
						free(buffer);
						fclose(input);
						for (int i = 0; i < currentAmountCells; i++) {
							if ((*cells)[i].name != NULL) {
								free((*cells)[i].name);
							}
						}
						free(*cells);
						return status;
					}
					count++;
					break;
				case TYPE_PRINT:
					status = PrintCurrentCell(cells, buffer, currentAmountCells);
					if (status == UNINIT_VALUE) {
						printf("An error while performing operation %d (uninitialized value).\n", count);
						printf("Operation %d: print %s.\n", count, buffer + 5);
						free(buffer);
						fclose(input);
						for (int i = 0; i < currentAmountCells; i++) {
							if ((*cells)[i].name != NULL) {
								free((*cells)[i].name);
							}
						}
						free(*cells);
						return status;
					}
					if (status == MEMORY_ALLOCATION_ERROR) {
						printf("Memory allocation error while performing operation %d.\n", count);
						printf("Operation %d: %s.\n", count, buffer);
						free(buffer);
						fclose(input);
						for (int i = 0; i < currentAmountCells; i++) {
							if ((*cells)[i].name != NULL) {
								free((*cells)[i].name);
							}
						}
						free(*cells);
						return MEMORY_ALLOCATION_ERROR;
					}
					count++;
					break;
				case TYPE_ASSIGN:
					status = AssignOperation(cells, buffer, &totalAmountCells, &currentAmountCells);
					if (status == MEMORY_ALLOCATION_ERROR) {
						printf("Memory allocation error while performing operation %d.\n", count);
						printf("Operation %d: %s.\n", count, buffer);
						for (int i = 0; i < currentAmountCells; i++) {
							if ((*cells)[i].name != NULL) {
								free((*cells)[i].name);
							}
						}
						free(*cells);
						free(buffer);
						fclose(input);
						return MEMORY_ALLOCATION_ERROR;
					} else if (status == UNINIT_VALUE) {
						printf("An error while performing operation %d (uninitialized value).\n", count);
						printf("Operation %d: %s.\n", count, buffer);
						free(buffer);
						fclose(input);
						for (int i = 0; i < currentAmountCells; i++) {
							if ((*cells)[i].name != NULL) {
								free((*cells)[i].name);
							}
						}
						free(*cells);
						return UNINIT_VALUE;
					}
					count++;
					break;
				case UNKNOWN_TYPE:
					printf("Unknown operation %d.\n", count);
					printf("Operation %d: %s.\n", count, buffer);
					free(buffer);
					fclose(input);
					for (int i = 0; i < currentAmountCells; i++) {
						if ((*cells)[i].name != NULL) {
							free((*cells)[i].name);
						}
					}
					free(*cells);
					count++;
					return UNINIT_VALUE;
			}
		}
		else {
			if (currentLength + 1 >= totalLength) {
				totalLength *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * totalLength);
				if (tempBuffer == NULL) {
					free(buffer);
					fclose(input);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			if (c != '\n' && c != '\r' && c != ' ' && c != '\t' && c != '(' && c != ')')
				buffer[currentLength++] = (char)c;
		}
	}
	for (int i = 0; i < currentAmountCells; i++) {
		if ((*cells)[i].name != NULL) {
			free((*cells)[i].name);
		}
	}
	free(*cells);
	free(buffer);
	fclose(input);
	return NORMAL;
}

statusCode AssignOperation(MemoryCell** cells, char* line, int* totalAmount, int* currentAmount){
	if (*currentAmount + 1 >= *totalAmount) {
		*totalAmount = (*totalAmount) * 2;
		MemoryCell* temp = (MemoryCell*)realloc((*cells), sizeof(MemoryCell) * (*totalAmount));
		if (temp == NULL) {
			return MEMORY_ALLOCATION_ERROR;
		}
		*cells = temp;
	}
	(*cells)[*currentAmount] = (MemoryCell) { .name = NULL, .value = 0 };
	if (IsDigit(line[StringLength(line) - 1])) {
		char* name;
		int value;
		statusCode status = GetNameAndValue(line, &name, &value);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
		int index = FindInCellsArray(cells, *currentAmount, name);
		if (index == -1) {
			(*cells)[*currentAmount].value = value;
			(*cells)[*currentAmount].name = (char*)malloc(sizeof(char) * (StringLength(name) + 1));
			if ((*cells)[*currentAmount].name == NULL) {
				free(name);
				free(&(*cells)[*currentAmount]);
				return MEMORY_ALLOCATION_ERROR;
			}
			char* resultCopy = StringCopy((*cells)[*currentAmount].name, name);
			if (resultCopy == NULL) {
				free(name);
				free((*cells)[*currentAmount].name);
				free(&(*cells)[*currentAmount]);
				return MEMORY_ALLOCATION_ERROR;
			}
			(*currentAmount)++;
			free(name);
			qsort(*cells, *currentAmount, sizeof(MemoryCell), CompareMemoryCells);
		} else {
			(*cells)[index].value = value;
			free(name);
		}
	} else {
		char* first;
		char* second;
		statusCode status = GetTwoNames(line, &first, &second);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
		int index = FindInCellsArray(cells, *currentAmount, second);
		if (index == -1) {
			free(first);
			free(second);
			return UNINIT_VALUE;
		}
		(*cells)[*currentAmount].name = (char*)malloc(sizeof(char) * (StringLength(first) + 1));
		if ((*cells)[*currentAmount].name == NULL) {
			free(first);
			free(second);
			free(&(*cells)[*currentAmount]);
			return MEMORY_ALLOCATION_ERROR;
		}
		char* resultCopy = StringCopy((*cells)[*currentAmount].name, first);
		if (resultCopy == NULL) {
			free(first);
			free(second);
			free((*cells)[*currentAmount].name);
			free(&(*cells)[*currentAmount]);
			return MEMORY_ALLOCATION_ERROR;
		}
		(*cells)[*currentAmount].value = (*cells)[index].value;
		(*currentAmount)++;
		free(first);
		free(second);
		qsort(*cells, *currentAmount, sizeof(MemoryCell), CompareMemoryCells);
	}
	return NORMAL;
}

statusCode GetNameAndValue(const char* line, char** name, int* value) {
	*value = 0;
	*name = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (*name == NULL) return MEMORY_ALLOCATION_ERROR;
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	int index = 0;
	while (index < StringLength(line) && (IsAlpha(line[index]) || IsDigit(line[index]))) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(*name, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(*name);
				return MEMORY_ALLOCATION_ERROR;
			}
			*name = temp;
		}
		(*name)[currentLength++] = line[index++];
	}
	(*name)[currentLength] = '\0';
	index++;
	char flagNegative = 0;
	if (line[index] == '-') {
		flagNegative = 1;
		index++;
	}
	while (IsDigit(line[index])) {
		*value = *value * 10 + (line[index] - '0');
		index++;
	}
	if (flagNegative) *value = -(*value);
	return NORMAL;
}

statusCode GetTwoNames(const char* line, char** first, char** second) {
	*first = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (*first == NULL) return MEMORY_ALLOCATION_ERROR;
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	int index = 0;
	while (index < StringLength(line) && (IsAlpha(line[index]) || IsDigit(line[index]))) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(*first, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(*first);
				return MEMORY_ALLOCATION_ERROR;
			}
			*first = temp;
		}
		(*first)[currentLength++] = line[index++];
	}
	(*first)[currentLength] = '\0';
	index += 1;
	*second = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (*second == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	currentLength = 0;
	totalLength = START_LENGTH_NAME;
	while (index < StringLength(line) && (IsAlpha(line[index]) || IsDigit(line[index]))) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(*second, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(*first);
				free(*second);
				return MEMORY_ALLOCATION_ERROR;
			}
			*second = temp;
		}
		(*second)[currentLength++] = line[index++];
	}
	(*second)[currentLength] = '\0';
	return NORMAL;
}

int StringNCompare(const char* first, const char* second, int length) {
	if (StringLength(first) < length || StringLength(second) < length) return -2;
	for (int i = 0; i < length; i++) {
		if (first[i] > second[i])
			return 1;
		else if (first[i] < second[i])
			return -1;
	}
	return 0;
}

int IsOperation(char c) {
	if (c == '-' || c == '+' || c == '*' || c == '/' || c == '%') return 1;
	return 0;
}

type DetectType(const char* line) {
	int index = 0;
	if (StringLength(line) >= 5 && StringNCompare(line, "print", 5) == 0) {
		return TYPE_PRINT;
	}
	while (IsAlpha(line[index])) index++;
	if (line[index] == '=') index++;
	if (line[index] == '-') index++;
	while (IsAlpha(line[index]) || IsDigit(line[index])) index++;
	if (line[index] == '\0') return TYPE_ASSIGN;
	else if (IsOperation(line[index])) return TYPE_MATH;
	return UNKNOWN_TYPE;
}

statusCode GetFileName(int argc, char** argv, char** fileName) {
	if (argc != 2) {
		return INVALID_NUMBER_ARGUMENTS;
	}
	*fileName = argv[1];
	return NORMAL;
}

int CompareMemoryCells(const void* first, const void* second) {
	MemoryCell* firstCell = (MemoryCell*)first;
	MemoryCell* secondCell = (MemoryCell*)second;
	return CompareChars(firstCell->name, secondCell->name);
}

int FindInCellsArray(MemoryCell** array, int amount, char* key) {
	int left = 0;
	int right = amount - 1;
	int resultCompare = 0;
	while (left <= right) {
		int middle = (left + right) / 2;
		resultCompare = CompareChars(key, (*array)[middle].name);
		if (resultCompare == -1) right = middle - 1;
		else if (resultCompare == 1) left = middle + 1;
		else return middle;
	}
	return -1;
}

statusCode MathOperation(MemoryCell** cells, char* line, int* amount) {
	char* result = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (result == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	while (line[index] != '=') {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(result, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(result);
				return MEMORY_ALLOCATION_ERROR;
			}
			result = temp;
		}
		result[currentLength++] = line[index++];
	}
	result[currentLength] = '\0';
	index++;
	int indexResult = FindInCellsArray(cells, *amount, result);
	char* firstOperand = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (firstOperand == NULL) {
		free(result);
		return MEMORY_ALLOCATION_ERROR;
	}
	currentLength = 0;
	totalLength = START_LENGTH_NAME;
	if (line[index] == '-') {
		index++;
		currentLength = 1;
		firstOperand[0] = '-';
	}
	while (IsDigit(line[index]) || IsAlpha(line[index])) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(firstOperand, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(firstOperand);
				free(result);
				return MEMORY_ALLOCATION_ERROR;
			}
			firstOperand = temp;
		}
		firstOperand[currentLength++] = line[index++];
	}
	firstOperand[currentLength] = '\0';
	int valueFirstOperand = 0;
	int indexFirst = FindInCellsArray(cells, *amount, firstOperand);
	if (indexFirst == -1 && IsAlpha(firstOperand[0])) {
		free(result);
		return UNINIT_VALUE;
	}
	if (IsDigit(firstOperand[0]) || firstOperand[0] == '-') sscanf(firstOperand, "%d", &valueFirstOperand);
	else valueFirstOperand = (*cells)[indexFirst].value;
	free(firstOperand);
	char operation = line[index];
	index++;
	char* secondOperand = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (secondOperand == NULL) {
		free(result);
		return MEMORY_ALLOCATION_ERROR;
	}
	currentLength = 0;
	totalLength = START_LENGTH_NAME;
	if (line[index] == '-') {
		index++;
		currentLength = 1;
		secondOperand[0] = '-';
	}
	while (IsDigit(line[index]) || IsAlpha(line[index])) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(secondOperand, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(secondOperand);
				free(result);
				return MEMORY_ALLOCATION_ERROR;
			}
			secondOperand = temp;
		}
		secondOperand[currentLength++] = line[index++];
	}
	secondOperand[currentLength] = '\0';
	int indexSecond = FindInCellsArray(cells, *amount, secondOperand);
	if (indexSecond == -1 && IsAlpha(secondOperand[0])) {
		free(result);
		free(secondOperand);
		return UNINIT_VALUE;
	}
	int valueSecondOperand = 0;
	if (IsDigit(secondOperand[0]) || secondOperand[0] == '-') sscanf(secondOperand, "%d", &valueSecondOperand);
	else valueSecondOperand = (*cells)[indexSecond].value;
	free(secondOperand);
	int flagNew = 0;
	if (indexResult == -1) {
		indexResult = *amount;
		(*amount)++;
		flagNew = 1;
	}
	if (flagNew) {
		(*cells)[indexResult] = (MemoryCell){.value = 0, .name = NULL};
		(*cells)[indexResult].name = (char*)malloc(sizeof(char) * (StringLength(result) + 1));
		if ((*cells)[indexResult].name == NULL) {
			free(result);
			return MEMORY_ALLOCATION_ERROR;
		}
		char* res = StringCopy((*cells)[indexResult].name, result);
		if (res == NULL) {
			free(result);
			return MEMORY_ALLOCATION_ERROR;
		}
	}
	free(result);

	switch (operation) {
		case '+':
			(*cells)[indexResult].value = valueFirstOperand + valueSecondOperand;
			break;
		case '-':
			(*cells)[indexResult].value = valueFirstOperand - valueSecondOperand;
			break;
		case '*':
			(*cells)[indexResult].value = valueFirstOperand * valueSecondOperand;
			break;
		case '/':
			if (valueSecondOperand == 0) {
				return DIVISION_BY_ZERO;
			}
			(*cells)[indexResult].value = valueFirstOperand / valueSecondOperand;
			break;
		case '%':
			if (valueSecondOperand == 0) {
				return DIVISION_BY_ZERO;
			}
			(*cells)[indexResult].value = valueFirstOperand % valueSecondOperand;
			break;
		default:
			return UNINIT_VALUE;
	}
	qsort(*cells, *amount, sizeof(MemoryCell), CompareMemoryCells);
	return NORMAL;
}

statusCode PrintCurrentCell(MemoryCell** cells, char* line, int amount) {
	if (line == NULL) {
		return IS_EMPTY;
	}
	int index = 5;
	if (StringLength(line) == 5) {
		for (int i = 0; i < amount; i++) {
			printf("Name: %s, value: %d\n", (*cells)[i].name, (*cells)[i].value);
		}
		printf("\n");
		return NORMAL;
	}
	char* name = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (name == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	while (IsDigit(line[index]) || IsAlpha(line[index])) {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(name, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(name);
				return MEMORY_ALLOCATION_ERROR;
			}
			name = temp;
		}
		name[currentLength++] = line[index++];
	}
	name[currentLength] = '\0';
	int indexCell = FindInCellsArray(cells, amount, name);
	free(name);
	if (indexCell == -1) {
		printf("Not found\n");
		return UNINIT_VALUE;
	}
	printf("Name: %s, value: %d\n", (*cells)[indexCell].name, (*cells)[indexCell].value);
	return NORMAL;
}