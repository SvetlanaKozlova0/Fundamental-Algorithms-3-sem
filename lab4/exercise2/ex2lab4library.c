#include "ex2lab4library.h"
#include <stdio.h>
#include <stdlib.h>
#include "../..//stringLibrary.h"
#include <time.h>

statusCode SaveInFile(char* fileName, Array* array) {
	FILE* output = fopen(fileName, "w");
	if (output == NULL) {
		return FILE_OPEN_ERROR;
	}
	if (array->length == 0) {
		fprintf(output, "Amount of elements is 0.\n");
		fclose(output);
		return NORMAL;
	}
	fprintf(output, "%d", array->numbers[0]);
	for (int i = 1; i < array->length; i++) {
		fprintf(output, ", %d", array->numbers[i]);
	}
	fclose(output);
	return NORMAL;
}

//in the start array is null
statusCode LoadFromFile(char* fileName, Array* array) {
	if (array == NULL) {
		return INCORRECT_COMMAND;
	}
	FILE* input = fopen(fileName, "r");
	if (input == NULL) {
		return FILE_OPEN_ERROR;
	}
	int totalLength = START_AMOUNT_NUMBERS;
	array->numbers = (int*)malloc(sizeof(int) * totalLength);
	if (array->numbers == NULL) {
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	array->length = 0;
	int c;
	int currentNumber = 0;
	int negative = 0;
	while ((c = getc(input)) != EOF) {
		if (IsDigit((char)c)) {
			currentNumber = currentNumber * 10 + (c - '0');
		} else if (c == '-' && currentNumber == 0) {
			negative = 1;
		} else {
			if (currentNumber != 0) {
				if (array->length >= totalLength) {
					totalLength *= 2;
					int* tempNumbers = (int*)realloc(array->numbers, totalLength * sizeof(int));
					if (tempNumbers == NULL) {
						free(array->numbers);
						fclose(input);
						return MEMORY_ALLOCATION_ERROR;
					}
					array->numbers = tempNumbers;
				}
				array->numbers[array->length++] = negative ? -currentNumber : currentNumber;
				currentNumber = 0;
				negative = 0;
			}
		}
	}
	if (currentNumber != 0) {
		if (array->length >= totalLength) {
			totalLength *= 2;
			int* tempNumbers = (int*)realloc(array->numbers, totalLength * sizeof(int));
			if (tempNumbers == NULL) {
				free(array->numbers);
				fclose(input);
				return MEMORY_ALLOCATION_ERROR;
			}
			array->numbers = tempNumbers;
		}
		array->numbers[array->length++] = negative ? -currentNumber : currentNumber;
	}

	fclose(input);
	return NORMAL;
}

void FreeArray(Array* array) {
	free(array->numbers);
	free(array);
}

statusCode InstructionsProcessing(const char* fileName) {
	FILE* input = fopen(fileName, "r");
	if (input == NULL) {
		return FILE_OPEN_ERROR;
	}
	char* buffer = (char*)malloc(sizeof(char) * START_BUFFER_SIZE);
	if (buffer == NULL) {
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	Array** allArrays = (Array**)calloc(ALPHABET_LENGTH, sizeof(Array*));
	if (allArrays == NULL) {
		free(buffer);
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = '0';
	int currentLength = 0;
	int totalLength = START_BUFFER_SIZE;
	while ((c = getc(input)) != EOF) {
		if (c == '\n') {
			buffer[currentLength] = '\0';
			currentLength = 0;
			typeInstruction  type;
			statusCode status = TypeInstruction(buffer, &type);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(input);
				free(allArrays);
				return MEMORY_ALLOCATION_ERROR;
			}
			if (type == TYPE_UNKNOWN) {
				free(buffer);
				fclose(input);
				free(allArrays);
				return INVALID_SYNTAX;
			}
			else {
				if (IsValidCommand(buffer, type) == NORMAL) {

				}
				else {
					free(buffer);
					fclose(input);
					free(allArrays);
					return INVALID_SYNTAX;
				}
			}
		} else {
			if (currentLength + 1 > totalLength) {
				totalLength *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * totalLength);
				if (tempBuffer == NULL) {
					free(buffer);
					fclose(input);
					free(allArrays);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[currentLength++] = (char)c;
		}
	}
	if (currentLength > 0) {
		buffer[currentLength] = '\0';
		typeInstruction  type;
		statusCode status = TypeInstruction(buffer, &type);
		if (type == TYPE_UNKNOWN) printf("Unknown.\n");
		else printf("Instruction: %s\n", buffer);
	}
	free(allArrays);
	free(buffer);
	fclose(input);
	return NORMAL;
}

statusCode OneInstructionProcessing(Array** allArrays, char* string, typeInstruction type) {
	switch (type) {
		case TYPE_LOAD:
			break;
		case TYPE_SAVE:
			break;
		case TYPE_RAND:
			break;
		case TYPE_CONCAT:
			break;
		case TYPE_FREE:
			break;
		case TYPE_REMOVE:
			break;
		case TYPE_COPY:
			break;
		case TYPE_SORT:
			break;
		case TYPE_SHUFFLE:
			break;
		case TYPE_STATS:
			break;
		case TYPE_PRINT:
			break;
	}
	return NORMAL;
}

statusCode ReadPrintLine(const char* line, int* numberArray, int* start, int* end, int* current) {
	int index = 0;
	return NORMAL;
}



int CompareInstructions(const char* firstChars, const char* secondChars) {
	size_t firstLen = StringLength(firstChars);
	for (int i = 0; i < firstLen; i++) {
		char first = ToLower(firstChars[i]);
		char second = ToLower(secondChars[i]);
		if (first == second) continue;
		if (first > second)
			return 1;
		else
			return -1;
	}
	return 0;
}

statusCode TypeInstruction(const char* string, typeInstruction* type) {
	if (StringLength(string) < 6) {
		*type = TYPE_UNKNOWN;
		return NORMAL;
	}
	char* fourSymbols = (char*)malloc(sizeof(char) * 5);
	if (fourSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < 4; i++) fourSymbols[i] = string[i];
	fourSymbols[4] = '\0';
	if (CompareInstructions(fourSymbols, "load") == 0) *type = TYPE_LOAD;
	else if (CompareInstructions(fourSymbols, "save") == 0) *type = TYPE_SAVE;
	else if (CompareInstructions(fourSymbols, "rand") == 0) *type = TYPE_RAND;
	else if (CompareInstructions(fourSymbols, "free") == 0) *type = TYPE_FREE;
	else if (CompareInstructions(fourSymbols, "copy") == 0) *type = TYPE_COPY;
	else if (CompareInstructions(fourSymbols, "sort") == 0) *type = TYPE_SORT;
	else *type = TYPE_UNKNOWN;
	free(fourSymbols);
	if (*type != TYPE_UNKNOWN) {
		return NORMAL;
	}
	if (StringLength(string) < 7) {
		*type = TYPE_UNKNOWN;
		return NORMAL;
	}
	char* fiveSymbols = (char*)malloc(sizeof(char) * 6);
	if (fiveSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < 5; i++) fiveSymbols[i] = string[i];
	fiveSymbols[5] = '\0';
	if (CompareInstructions(fiveSymbols, "stats") == 0) *type = TYPE_STATS;
	else if (CompareInstructions(fiveSymbols, "print") == 0) *type = TYPE_PRINT;
	free(fiveSymbols);
	if (*type != TYPE_UNKNOWN) {
		return NORMAL;
	}
	if (StringLength(string) < 8) {
		*type = TYPE_UNKNOWN;
		return NORMAL;
	}
	char* sixSymbols = (char*)malloc(sizeof(char) * 7);
	if (sixSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < 6; i++) sixSymbols[i] = string[i];
	sixSymbols[6] = '\0';
	if (CompareInstructions(sixSymbols, "concat") == 0) *type = TYPE_CONCAT;
	else if (CompareInstructions(sixSymbols, "remove") == 0) *type = TYPE_REMOVE;
	free(sixSymbols);
	if (*type != TYPE_UNKNOWN) {
		return NORMAL;
	}
	if (StringLength(string) < 9) {
		*type = TYPE_UNKNOWN;
		return NORMAL;
	}
	char* sevenSymbols = (char*)malloc(sizeof(char) * 8);
	if (sevenSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < 7; i++) sevenSymbols[i] = string[i];
	sevenSymbols[7] = '\0';
	if (CompareInstructions(sevenSymbols, "shuffle") == 0) *type = TYPE_SHUFFLE;
	free(sevenSymbols);
	return NORMAL;
}

int getNumberArray(char c) {
	if (c >= 'A' && c <= 'Z') return c - 'A';
	if (c >= 'a' && c <= 'z') return c - 'a';
	return -1;
}

void getMaxMin(Array* array, int* maximum, int* maximumIndex, int* minimum, int* minimumIndex) {
	int maximumElement = array->numbers[0];
	int minimumElement = array->numbers[0];
	*maximumIndex = 0;
	*minimumIndex = 0;
	for (int i = 0; i < array->length; i++) {
		if (maximumElement < array->numbers[i]) {
			maximumElement = array->numbers[i];
			*maximumIndex = i;
		}
		if (minimumElement > array->numbers[i]) {
			minimumElement = array->numbers[i];
			*minimumIndex = i;
		}
	}
	*minimum = minimumElement;
	*maximum = maximumElement;
}

void getAverage(Array* array, double* average, double* maxDif) {
	*average = 0;
	for (int i = 0; i < array->length; i++) {
		*average += array->numbers[i];
	}
	*average = *average / array->length;
	*maxDif = 0;
	double epsilon = 0.000000000001;
	for (int i = 0; i < array->length; i++) {
		double difference = *average - array->numbers[i];
		if (difference - *maxDif > epsilon) *maxDif = difference;
	}
}

int CompareElementsIncrease(const void* first, const void* second) {
	return (*(int*)first - *(int*)second);
}

int CompareElementsDecrease(const void* first, const void* second) {
	return -CompareElementsIncrease(first, second);
}

int RandomCompareElements(const void* first, const void* second) {
	return rand() % 3 - 1;
}

statusCode getMostFrequent(Array* array, int* frequent) {
	int* tempArray = (int*)malloc(sizeof(int) * array->length);
	if (tempArray == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < array->length; i++) tempArray[i] = array->numbers[i];
	qsort(tempArray, array->length, sizeof(int), CompareElementsDecrease);
	int lastElem = tempArray[0];
	int lastFreq = 1;
	int maxElem = tempArray[0];
	int maxFreq = 1;
	for (int i = 1; i < array->length; i++) {
		if (tempArray[i] == lastElem) lastFreq++;
		else {
			if (lastFreq > maxFreq) {
				maxFreq = lastFreq;
				maxElem = lastElem;
			}
			lastElem = tempArray[0];
			lastFreq = 1;
		}
	}
	if (lastFreq > maxFreq) {
		maxFreq = lastFreq;
		maxElem = lastElem;
	}
	*frequent = maxElem;
	free(tempArray);
	return NORMAL;
}

statusCode PrintVariations(Array* array, int start, int end, int current, int flagAll, int flagStat) {
	if (array == NULL) {
		printf("Nothing to print.\n");
		return NORMAL;
	}
	if (flagStat) {
		printf("Length: %d\n", array->length);
		int maximum, maximumIndex, minimum, minimumIndex, mostFreq;
		getMaxMin(array, &maximum, &maximumIndex, &minimum, &minimumIndex);
		double average, maxDif;
		getAverage(array, &average, &maxDif);
		statusCode status = getMostFrequent(array, &mostFreq);
		if (status == MEMORY_ALLOCATION_ERROR) {
			return MEMORY_ALLOCATION_ERROR;
		}
		printf("Maximum: %d, index of maximum: %d\n", maximum, maximumIndex);
		printf("Minimum: %d, index of minimum: %d\n", minimum, minimumIndex);
		printf("Most frequent: %d\n", mostFreq);
		printf("Average: %lf, maximum deviation: %lf\n", average, maxDif);

	}
	if (flagAll) {
		printf("%d", array->numbers[0]);
		for (int i = 1; i < array->length; i++) {
			printf(", %d", array->numbers[i]);
		}
		printf("\n");
		return NORMAL;
	}
	if (start != -1 && end != -1) {
		if (start >= end) {
			printf("Start should be less than end.\n");
			return NORMAL;
		}
		if (array->length - 1 < end) {
			printf("Array index out of range.\n");
			return INDEX_OUT_RANGE;
		}
		printf("%d", array->numbers[start]);
		for (int i = start; i <= end; i++) {
			printf(", %d", array->numbers[i]);
		}
		printf("\n");
		return NORMAL;
	}
	if (current != -1) {
		if (array->length - 1 < current) {
			printf("Array index out of range.\n");
			return INDEX_OUT_RANGE;
		}
		printf("%d\n", array->numbers[current]);
		return NORMAL;
	}
	return INCORRECT_COMMAND;
}

void PermutationVariations(Array* array, typeInstruction type, int flagIncrease) {
	if (type == TYPE_SHUFFLE) {
		qsort(array->numbers, array->length, sizeof(int), RandomCompareElements);
		return;
	}
	if (flagIncrease) {
		qsort(array->numbers, array->length, sizeof(int), CompareElementsIncrease);
		return;
	}
	qsort(array->numbers, array->length, sizeof(int), CompareElementsDecrease);
}

statusCode FillRandom(Array* array, int count, int left, int right) {
	if (left >= right) return INCORRECT_COMMAND;
	array->length = count;
	array->numbers = (int*)malloc(sizeof(int) * count);
	if (array->numbers == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	srand(time(NULL));
	for (int i = 0; i < array->length; i++) {
		array->numbers[i] = rand() % (right - left) + left;
	}
	return NORMAL;
}

statusCode CopyArray(Array* oldArray, Array* newArray, int left, int right) {
	if (newArray != NULL) {
		return INCORRECT_COMMAND;
	}
	if (left >= right) return INCORRECT_COMMAND;
	if (oldArray->length - 1 < right) return INDEX_OUT_RANGE;
	newArray = (Array*)malloc(sizeof(Array));
	if (newArray == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	newArray->length = left - right + 1;
	newArray->numbers = (int*)malloc(sizeof(int) * newArray->length);
	if (newArray->numbers == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	for (int i = left; i <= right; i++) {
		newArray[index++] = oldArray[i];
	}
	return NORMAL;
}

statusCode ConcatenateArrays(Array* first, Array* second) {
	int length = first->length + second->length;
	int* tempNumbers = (int*)realloc(first->numbers, length);
	if (tempNumbers == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	first->numbers = tempNumbers;
	int index = 0;
	for (int i = first->length; i < length; i++) {
		first[i] = second[index++];
	}
	first->length = length;
	FreeArray(second);
	return NORMAL;
}

statusCode RemoveElements(Array* array, int index, int amount) {
	if (array == NULL) {
		return INCORRECT_COMMAND;
	}
	if (index + amount > array->length) {
		return INCORRECT_COMMAND;
	}
	int* tempNumbers = (int*)malloc(sizeof(int) * (array->length - amount));
	if (tempNumbers == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < index; i++) tempNumbers[i] = array->numbers[i];
	int k = index;
	for (int i = index + amount; i < array->length; i++) tempNumbers[k++] = array->numbers[i];
	free(array->numbers);
	array->numbers = tempNumbers;
	return NORMAL;
}

statusCode IsValidLoadSave(const char* string) {
	int index = 4;
	int firstPartFile = 0;
	int point = 0;
	int secondPartFile = 0;
	if (!(string[index] == ' ' || string[index] == '\t')) return INVALID_SYNTAX;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	if (string[index] != ' ' && string[index] != '\t' && string[index] != ',') return INVALID_SYNTAX;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (string[index] != ' ' && string[index] != '\t') {
		index++;
		firstPartFile = 1;
	}
	if (string[index] == '.') {
		index++;
		point = 1;
	}
	if (string[index] == 't' && string[index + 1] == 'x' && string[index + 2] == 't') secondPartFile = 1;
	index += 3;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0') return INVALID_SYNTAX;
	if (firstPartFile && secondPartFile && point != 1) return INVALID_SYNTAX;
	return NORMAL;
}

statusCode IsValidCommand(const char* string, typeInstruction type) {
	int index = 0;
	int flagIsArray = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (IsAlpha(string[index])) {
		index++;
		flagIsArray = 1;
	}
	if (flagIsArray == 0) return INVALID_SYNTAX;
	while (string[index] == ' ' || string[index] == '\t') index++;
	switch (type) {
		case TYPE_FREE:
		case TYPE_STATS:
		case TYPE_SHUFFLE:
			if (string[index] != '\0') return INVALID_SYNTAX;
			break;
		case TYPE_SORT:
			if ((string[index] == '-' || string[index] == '+') && string[index + 1] == '\0') return NORMAL;
			return INVALID_SYNTAX;
		case TYPE_PRINT:
		case TYPE_REMOVE:
		case TYPE_COPY:
		case TYPE_RAND:
			if (string[index] != ',') return INVALID_SYNTAX;
			index++;
			if (string[index] != ' ') return INVALID_SYNTAX;
			index++;
			if (IsDigit(string[index])) {
				while (IsDigit(string[index])) index++;
				if (string[index] == '\0' && type != TYPE_REMOVE) return NORMAL;
				if (string[index] != ',') return INVALID_SYNTAX;
				index++;
				if (string[index] != ' ') return INVALID_SYNTAX;
				index++;
				if (!IsDigit(string[index])) return INVALID_SYNTAX;
				while (IsDigit(string[index])) index++;
				if (type == TYPE_COPY) {
					if (string[index] != ',') return INVALID_SYNTAX;
					index++;
					if (string[index] != ' ') return INVALID_SYNTAX;
					index++;
					if (!IsAlpha(string[index])) return INVALID_SYNTAX;
					index++;
					if (string[index] != '\0') return INVALID_SYNTAX;
				} else if (type == TYPE_RAND) {
					if (string[index] != ',') return INVALID_SYNTAX;
					index++;
					if (string[index] != ' ') return INVALID_SYNTAX;
					index++;
					if (!IsDigit(string[index])) return INVALID_SYNTAX;
					while (IsDigit(string[index])) index++;
					if (string[index] != '\0') return INVALID_SYNTAX;
				} else {
					if (string[index] != '\0' && (type == TYPE_REMOVE || type == TYPE_PRINT)) return INVALID_SYNTAX;
				}
				return NORMAL;
			} else if (IsAlpha(string[index]) && type != TYPE_REMOVE) {
				if (string[index] == 'a' && string[index + 1] == 'l' && string[index + 2] == 'l' && string[index + 3] == '\0') return NORMAL;
				return INVALID_SYNTAX;
			} else return INVALID_SYNTAX;
		case TYPE_CONCAT:
			if (string[index] != ',') return INVALID_SYNTAX;
			index++;
			if (string[index] != ' ') return INVALID_SYNTAX;
			index++;
			if (!IsAlpha(string[index])) return INVALID_SYNTAX;
			index++;
			if (string[index] != '\0') return INVALID_SYNTAX;
			break;
		case TYPE_LOAD:
		case TYPE_SAVE:
			if (string[index] != ',') return INVALID_SYNTAX;
			index++;
			if (string[index] != ' ') return INVALID_SYNTAX;
			index++;
			if (!IsAlpha(string[index])) return INVALID_SYNTAX;
			while (IsAlpha(string[index]) || IsDigit(string[index]) || string[index] == '.' || string[index] == '_') index++;
			if (string[index] != '\0') return INVALID_SYNTAX;
			break;
		default:
			return INVALID_SYNTAX;
	}
	return NORMAL;
}
