#include "ex2lab4library.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../stringLibrary.h"

Command commands[] = {{TYPE_LOAD, CommandLoad},   {TYPE_SAVE, CommandSave},     {TYPE_RAND, CommandRand},
                      {TYPE_FREE, CommandFree},   {TYPE_CONCAT, CommandConcat}, {TYPE_REMOVE, CommandRemove},
                      {TYPE_COPY, CommandCopy},   {TYPE_SORT, CommandSort},     {TYPE_SHUFFLE, CommandShuffle},
                      {TYPE_STATS, CommandStats}, {TYPE_PRINT, CommandPrint}};

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
	statusCode status = NORMAL;
	typeInstruction type = TYPE_UNKNOWN;
	while ((c = getc(input)) != EOF) {
		if (c == '\n') {
			buffer[currentLength] = '\0';
			currentLength = 0;
			status = TypeInstruction(buffer, &type);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(input);
				for (int i = 0; i < ALPHABET_LENGTH; i++) {
					if (allArrays[i] != NULL) free(allArrays[i]->numbers);
					free(allArrays[i]);
				}
				free(allArrays);
				return MEMORY_ALLOCATION_ERROR;
			}
			if (type == TYPE_UNKNOWN) {
				int flag = 0;
				if (StringLength(buffer) == 1 && buffer[0] == '\r') flag = 1;
				free(buffer);
				fclose(input);
				for (int i = 0; i < ALPHABET_LENGTH; i++) {
					if (allArrays[i] != NULL) free(allArrays[i]->numbers);
					free(allArrays[i]);
				}
				free(allArrays);
				if (flag == 1) return NORMAL;
				return INVALID_SYNTAX;
			} else {
				if (IsValidCommand(buffer, type) == NORMAL) {
					status = OneInstructionProcessing(allArrays, buffer, type);
					if (status != NORMAL) {
						free(buffer);
						fclose(input);
						for (int i = 0; i < ALPHABET_LENGTH; i++) {
							if (allArrays[i] != NULL) free(allArrays[i]->numbers);
							free(allArrays[i]);
						}
						free(allArrays);
						return status;
					}
				} else {
					free(buffer);
					fclose(input);
					for (int i = 0; i < ALPHABET_LENGTH; i++) {
						if (allArrays[i] != NULL) free(allArrays[i]->numbers);
						free(allArrays[i]);
					}
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
					for (int i = 0; i < ALPHABET_LENGTH; i++) {
						if (allArrays[i] != NULL) free(allArrays[i]->numbers);
						free(allArrays[i]);
					}
					free(allArrays);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[currentLength++] = (char)c;
		}
	}
	if (currentLength > 0) {
		return INVALID_SYNTAX;
	}
	for (int i = 0; i < ALPHABET_LENGTH; i++) {
		if (allArrays[i] != NULL) {
			free(allArrays[i]->numbers);
			free(allArrays[i]);
		}
	}
	free(allArrays);
	free(buffer);
	fclose(input);
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

statusCode IsValidCommand(const char* string, typeInstruction type) {
	switch (type) {
		case TYPE_LOAD:
		case TYPE_SAVE:
			return ValidateLoad(string);
		case TYPE_RAND:
			return ValidateRand(string);
		case TYPE_CONCAT:
			return ValidateConcat(string);
		case TYPE_FREE:
			return ValidateFree(string);
		case TYPE_REMOVE:
			return ValidateRemove(string);
		case TYPE_COPY:
			return ValidateCopy(string);
		case TYPE_SORT:
			return ValidateSort(string);
		case TYPE_SHUFFLE:
		case TYPE_STATS:
			return ValidateShuffle(string);
		case TYPE_PRINT:
			return ValidatePrint(string);
		case TYPE_UNKNOWN:
			return INVALID_SYNTAX;
	}
	return NORMAL;
}

statusCode TypeInstruction(const char* string, typeInstruction* type) {
	char* fourSymbols = (char*)malloc(sizeof(char) * 5);
	if (fourSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	for (int i = 0; i < 4; i++) {
		if (string[index] != '\0') {
			fourSymbols[i] = string[index++];
		} else {
			free(fourSymbols);
			*type = TYPE_UNKNOWN;
			return NORMAL;
		}
	}
	fourSymbols[4] = '\0';
	if (CompareInstructions(fourSymbols, "load") == 0)
		*type = TYPE_LOAD;
	else if (CompareInstructions(fourSymbols, "save") == 0)
		*type = TYPE_SAVE;
	else if (CompareInstructions(fourSymbols, "rand") == 0)
		*type = TYPE_RAND;
	else if (CompareInstructions(fourSymbols, "free") == 0)
		*type = TYPE_FREE;
	else if (CompareInstructions(fourSymbols, "copy") == 0)
		*type = TYPE_COPY;
	else if (CompareInstructions(fourSymbols, "sort") == 0)
		*type = TYPE_SORT;
	else
		*type = TYPE_UNKNOWN;
	free(fourSymbols);
	if (*type != TYPE_UNKNOWN) {
		return NORMAL;
	}
	char* fiveSymbols = (char*)malloc(sizeof(char) * 6);
	if (fiveSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	index = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	for (int i = 0; i < 5; i++) {
		if (string[index] != '\0') {
			fiveSymbols[i] = string[index++];
		} else {
			free(fiveSymbols);
			*type = TYPE_UNKNOWN;
			return NORMAL;
		}
	}
	fiveSymbols[5] = '\0';
	if (CompareInstructions(fiveSymbols, "stats") == 0)
		*type = TYPE_STATS;
	else if (CompareInstructions(fiveSymbols, "print") == 0)
		*type = TYPE_PRINT;
	free(fiveSymbols);
	if (*type != TYPE_UNKNOWN) {
		return NORMAL;
	}
	index = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	char* sixSymbols = (char*)malloc(sizeof(char) * 7);
	if (sixSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < 6; i++) {
		if (string[index] != '\0') {
			sixSymbols[i] = string[index++];
		} else {
			free(sixSymbols);
			*type = TYPE_UNKNOWN;
			return NORMAL;
		}
	}
	sixSymbols[6] = '\0';
	if (CompareInstructions(sixSymbols, "concat") == 0)
		*type = TYPE_CONCAT;
	else if (CompareInstructions(sixSymbols, "remove") == 0)
		*type = TYPE_REMOVE;
	free(sixSymbols);
	if (*type != TYPE_UNKNOWN) {
		return NORMAL;
	}
	char* sevenSymbols = (char*)malloc(sizeof(char) * 8);
	if (sevenSymbols == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	index = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	for (int i = 0; i < 7; i++) {
		if (string[index] != '\0') {
			sevenSymbols[i] = string[index++];
		} else {
			free(sevenSymbols);
			*type = TYPE_UNKNOWN;
			return NORMAL;
		}
	}
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
int RandomCompareElements(const void* first, const void* second) { return rand() % 3 - 1; }

int CompareElementsIncrease(const void* first, const void* second) { return (*(int*)first - *(int*)second); }

int CompareElementsDecrease(const void* first, const void* second) { return -CompareElementsIncrease(first, second); }

void ReadFreeLine(const char* line, int* numberArray) {
	int index = 0;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t' || line[index] == '(') index++;
	*numberArray = getNumberArray(line[index]);
}

void ReadRandomLine(const char* line, int* numberArray, int* count, int* lb, int* rb) {
	int amount = 0;
	int left = 0;
	int right = 0;
	int index = 0;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	*numberArray = getNumberArray(line[index]);
	while (!IsDigit(line[index])) index++;
	while (IsDigit(line[index])) {
		amount *= 10;
		amount += line[index++] - '0';
	}
	int flagNegative = 0;
	while (!IsDigit(line[index])) {
		index++;
		if (line[index] == '-') flagNegative = 1;
	}
	while (IsDigit(line[index])) {
		left *= 10;
		left += line[index++] - '0';
	}
	if (flagNegative) left *= -1;
	flagNegative = 0;
	while (!IsDigit(line[index])) {
		index++;
		if (line[index] == '-') flagNegative = 1;
	}
	while (IsDigit(line[index])) {
		right *= 10;
		right += line[index++] - '0';
	}
	if (flagNegative) right *= -1;
	*count = amount;
	*lb = left;
	*rb = right;
}

void ReadRemoveLine(const char* line, int* numberArray, int* first, int* second) {
	int index = 0;
	int firstNumber = 0;
	int secondNumber = 0;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	*numberArray = getNumberArray(line[index]);
	while (!IsDigit(line[index])) index++;
	while (IsDigit(line[index])) {
		firstNumber *= 10;
		firstNumber += line[index] - '0';
		index++;
	}
	while (!IsDigit(line[index])) index++;
	while (IsDigit(line[index])) {
		secondNumber *= 10;
		secondNumber += line[index] - '0';
		index++;
	}
	*first = firstNumber;
	*second = secondNumber;
}

void ReadPermutationLine(const char* line, int* numberArray, int* flagIncrease) {
	int index = 0;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	*numberArray = getNumberArray(line[index]);
	index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	if (line[index] == '+')
		*flagIncrease = 1;
	else if (line[index] == '-')
		*flagIncrease = 0;
	else
		*flagIncrease = -1;
}

void ReadConcatLine(const char* line, int* firstArray, int* secondArray) {
	int index = 0;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	*firstArray = getNumberArray(line[index]);
	index++;
	while (!IsAlpha(line[index])) index++;
	*secondArray = getNumberArray(line[index]);
}

void ReadCopyLine(const char* line, int* numberArray, int* first, int* second, int* secondArray) {
	int index = 0;
	int firstNumber = 0;
	int secondNumber = 0;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	*numberArray = getNumberArray(line[index]);
	while (!IsDigit(line[index])) index++;
	while (IsDigit(line[index])) {
		firstNumber *= 10;
		firstNumber += line[index] - '0';
		index++;
	}
	while (!IsDigit(line[index])) index++;
	while (IsDigit(line[index])) {
		secondNumber *= 10;
		secondNumber += line[index] - '0';
		index++;
	}
	while (!IsAlpha(line[index])) index++;
	*secondArray = getNumberArray(line[index]);
	*first = firstNumber;
	*second = secondNumber;
}

statusCode ReadLoadSave(const char* line, int* numberArray, char** fileName) {
	int index = 0;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsAlpha(line[index])) index++;
	while (!IsAlpha(line[index])) index++;
	*numberArray = getNumberArray(line[index]);
	index++;
	while (!IsAlpha(line[index])) index++;
	*fileName = (char*)malloc(sizeof(char) * START_LENGTH_NAME);
	if (*fileName == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int currentLength = 0;
	int totalLength = START_LENGTH_NAME;
	while (line[index] != ' ' && line[index] != '\t' && line[index] != '\r' && line[index] != '\0' &&
	       line[index] != ';') {
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* temp = (char*)realloc(*fileName, sizeof(char) * totalLength);
			if (temp == NULL) {
				free(*fileName);
				return MEMORY_ALLOCATION_ERROR;
			}
			*fileName = temp;
		}
		(*fileName)[currentLength++] = line[index++];
	}
	(*fileName)[currentLength] = '\0';
	return NORMAL;
}

void ReadPrintLine(const char* line, int* numberArray, int* start, int* end, int* current, int* flagAll,
                   int* flagStat) {
	*start = -1;
	*end = -1;
	*current = -1;
	*flagAll = 0;
	*flagStat = 0;
	int index = 0;
	while (line[index] == '\t' || line[index] == ' ') index++;
	if (ToLower(line[index]) == 's') *flagStat = 1;
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	*numberArray = getNumberArray(line[index]);
	while (IsAlpha(line[index])) index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	if (line[index] == ',') index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	if (IsAlpha(line[index]) && *flagStat == 0) {
		*start = -1;
		*end = -1;
		*current = -1;
		*flagAll = 1;
		return;
	} else if (IsAlpha(line[index]) && *flagStat == 1) {
		*start = -1;
		*end = -1;
		*current = -1;
		*flagAll = 0;
		return;
	}
	int firstNumber = 0;
	while (IsDigit(line[index])) {
		firstNumber *= 10;
		firstNumber += line[index] - '0';
		index++;
	}
	while (line[index] == ' ' || line[index] == '\t') index++;
	if (line[index] == '\r' || line[index] == '\0') {
		*start = -1;
		*end = -1;
		*current = firstNumber;
		*flagAll = 0;
		*flagStat = 0;
		return;
	}
	while (line[index] == ' ' || line[index] == '\t') index++;
	int secondNumber = -1;
	if (line[index] != ';') secondNumber = 0;
	index++;
	if (line[index] == ',') index++;
	while (line[index] == ' ' || line[index] == '\t') index++;
	while (IsDigit(line[index])) {
		secondNumber *= 10;
		secondNumber += line[index] - '0';
		index++;
	}
	*start = firstNumber;
	*end = secondNumber;
	*current = -1;
	*flagAll = 0;
	*flagStat = 0;
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
		if (tempArray[i] == lastElem)
			lastFreq++;
		else {
			if (lastFreq > maxFreq) {
				maxFreq = lastFreq;
				maxElem = lastElem;
			}
			lastElem = tempArray[i];
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

statusCode OneInstructionProcessing(Array** allArrays, char* string, typeInstruction type) {
	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		if (commands[i].type == type) return commands[i].function(allArrays, string);
	}
	return INVALID_SYNTAX;
}

statusCode CommandLoad(Array** allArrays, char* string) {
	int numberArray;
	char* fileName;
	statusCode status = ReadLoadSave(string, &numberArray, &fileName);
	if (status == MEMORY_ALLOCATION_ERROR) {
		return MEMORY_ALLOCATION_ERROR;
	}
	if (allArrays[numberArray] != NULL) {
		free(allArrays[numberArray]->numbers);
		free(fileName);
		return NOT_EMPTY;
	}
	FILE* input = fopen(fileName, "r");
	if (input == NULL) {
		free(fileName);
		return FILE_OPEN_ERROR;
	}
	allArrays[numberArray] = (Array*)malloc(sizeof(Array));
	if (allArrays[numberArray] == NULL) {
		fclose(input);
		free(fileName);
		return MEMORY_ALLOCATION_ERROR;
	}
	int totalLength = START_AMOUNT_NUMBERS;
	allArrays[numberArray]->numbers = (int*)malloc(sizeof(int) * totalLength);
	if (allArrays[numberArray]->numbers == NULL) {
		fclose(input);
		free(fileName);
		free(allArrays[numberArray]);
		return MEMORY_ALLOCATION_ERROR;
	}
	allArrays[numberArray]->length = 0;
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
				if (allArrays[numberArray]->length >= totalLength) {
					totalLength *= 2;
					int* tempNumbers = (int*)realloc(allArrays[numberArray]->numbers, totalLength * sizeof(int));
					if (tempNumbers == NULL) {
						free(allArrays[numberArray]->numbers);
						fclose(input);
						free(fileName);
						free(allArrays[numberArray]);
						return MEMORY_ALLOCATION_ERROR;
					}
					allArrays[numberArray]->numbers = tempNumbers;
				}
				allArrays[numberArray]->numbers[allArrays[numberArray]->length++] =
				    negative ? -currentNumber : currentNumber;
				currentNumber = 0;
				negative = 0;
			}
		}
	}
	if (currentNumber != 0) {
		if (allArrays[numberArray]->length >= totalLength) {
			totalLength *= 2;
			int* tempNumbers = (int*)realloc(allArrays[numberArray]->numbers, totalLength * sizeof(int));
			if (tempNumbers == NULL) {
				free(allArrays[numberArray]->numbers);
				fclose(input);
				free(fileName);
				free(allArrays[numberArray]);
				return MEMORY_ALLOCATION_ERROR;
			}
			allArrays[numberArray]->numbers = tempNumbers;
		}
		allArrays[numberArray]->numbers[allArrays[numberArray]->length++] = negative ? -currentNumber : currentNumber;
	}
	fclose(input);
	free(fileName);
	return NORMAL;
}

statusCode CommandSave(Array** allArrays, char* string) {
	int numberArray;
	char* fileName;
	statusCode status = ReadLoadSave(string, &numberArray, &fileName);
	if (allArrays[numberArray] == NULL) {
		free(fileName);
		return EMPTY_ARRAY;
	}
	FILE* output = fopen(fileName, "w");
	if (output == NULL) {
		free(fileName);
		return FILE_OPEN_ERROR;
	}
	if (allArrays[numberArray]->length == 0) {
		fprintf(output, "Amount of elements is 0.\n");
		fclose(output);
		free(fileName);
		return NORMAL;
	}
	fprintf(output, "%d", allArrays[numberArray]->numbers[0]);
	for (int i = 1; i < allArrays[numberArray]->length; i++) {
		fprintf(output, ", %d", allArrays[numberArray]->numbers[i]);
	}
	fclose(output);
	free(fileName);
	return NORMAL;
}

statusCode CommandRand(Array** allArrays, char* string) {
	int amount, left, right, numberArray;
	ReadRandomLine(string, &numberArray, &amount, &left, &right);
	if (left >= right) return INVALID_BORDER_ORDER;
	if (allArrays[numberArray] != NULL) {
		return NOT_EMPTY;
	}
	allArrays[numberArray] = (Array*)malloc(sizeof(Array));
	if (allArrays[numberArray] == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	allArrays[numberArray]->length = amount;
	allArrays[numberArray]->numbers = (int*)malloc(sizeof(int) * amount);
	if (allArrays[numberArray]->numbers == NULL) {
		free(allArrays[numberArray]);
		return MEMORY_ALLOCATION_ERROR;
	}
	srand(time(NULL));
	for (int i = 0; i < amount; i++) {
		(allArrays[numberArray])->numbers[i] = rand() % (right - left) + left;
	}
	return NORMAL;
}

statusCode CommandFree(Array** allArrays, char* string) {
	int number = 0;
	ReadFreeLine(string, &number);
	if (allArrays[number] == NULL) {
		return INCORRECT_FREE;
	}
	free(allArrays[number]->numbers);
	free(allArrays[number]);
	allArrays[number] = NULL;
	return NORMAL;
}

statusCode CommandConcat(Array** allArrays, char* string) {
	int firstArray, secondArray;
	ReadConcatLine(string, &firstArray, &secondArray);
	if (allArrays[firstArray] == NULL || allArrays[secondArray] == NULL) {
		return EMPTY_ARRAY;
	}
	int length = allArrays[firstArray]->length + allArrays[secondArray]->length;
	int* tempNumbers = (int*)realloc(allArrays[firstArray]->numbers, length * sizeof(int));
	if (tempNumbers == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	allArrays[firstArray]->numbers = tempNumbers;
	int index = 0;
	for (int i = allArrays[firstArray]->length; i < length; i++) {
		allArrays[firstArray]->numbers[i] = allArrays[secondArray]->numbers[index++];
	}
	allArrays[firstArray]->length = length;
	free(allArrays[secondArray]->numbers);
	free(allArrays[secondArray]);
	allArrays[secondArray] = NULL;
	return NORMAL;
}
statusCode CommandRemove(Array** allArrays, char* string) {
	int numberArray, index, amount;
	ReadRemoveLine(string, &numberArray, &index, &amount);
	if (allArrays[numberArray] == NULL) {
		return EMPTY_ARRAY;
	}
	if (index + amount > allArrays[numberArray]->length) {
		return INVALID_BOUNDARIES;
	}
	int* tempNumbers = (int*)malloc(sizeof(int) * (allArrays[numberArray]->length - amount));
	if (tempNumbers == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < index; i++) tempNumbers[i] = allArrays[numberArray]->numbers[i];
	int k = index;
	for (int i = index + amount; i < allArrays[numberArray]->length; i++) {
		tempNumbers[k++] = allArrays[numberArray]->numbers[i];
	}
	free(allArrays[numberArray]->numbers);
	allArrays[numberArray]->numbers = tempNumbers;
	allArrays[numberArray]->length -= amount;
	return NORMAL;
}

statusCode CommandCopy(Array** allArrays, char* string) {
	int old, current, left, right;
	ReadCopyLine(string, &old, &left, &right, &current);
	if (allArrays[current] != NULL) return NOT_EMPTY;
	if (left >= right) return INVALID_BOUNDARIES;
	if (allArrays[old] == NULL) {
		return EMPTY_ARRAY;
	}
	if (allArrays[old]->length - 1 < right) return INDEX_OUT_RANGE;
	allArrays[current] = (Array*)malloc(sizeof(Array));
	if (allArrays[current] == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	allArrays[current]->length = right - left + 1;
	allArrays[current]->numbers = (int*)malloc(sizeof(int) * allArrays[current]->length);
	if (allArrays[current]->numbers == NULL) {
		free(allArrays[current]);
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	for (int i = left; i <= right; i++) {
		allArrays[current]->numbers[index++] = allArrays[old]->numbers[i];
	}
	return NORMAL;
}

statusCode CommandSort(Array** allArrays, char* string) {
	int numberArray, flagIncrease;
	ReadPermutationLine(string, &numberArray, &flagIncrease);
	if (allArrays[numberArray] == NULL) {
		return EMPTY_ARRAY;
	}
	if (flagIncrease)
		qsort(allArrays[numberArray]->numbers, allArrays[numberArray]->length, sizeof(int), CompareElementsIncrease);
	else
		qsort(allArrays[numberArray]->numbers, allArrays[numberArray]->length, sizeof(int), CompareElementsDecrease);
	return NORMAL;
}

statusCode CommandShuffle(Array** allArrays, char* string) {
	int numberArray, flagIncrease;
	ReadPermutationLine(string, &numberArray, &flagIncrease);
	if (allArrays[numberArray] == NULL) {
		return EMPTY_ARRAY;
	}
	qsort(allArrays[numberArray]->numbers, allArrays[numberArray]->length, sizeof(int), RandomCompareElements);
	return NORMAL;
}

statusCode CommandStats(Array** allArrays, char* string) {
	int numberArray, start, end, current, flagAll, flagStat;
	ReadPrintLine(string, &numberArray, &start, &end, &current, &flagAll, &flagStat);
	printf("Content of array %c:\n", numberArray + 'a');
	printf("-------------------------------------------------------------------\n");
	if (allArrays[numberArray] == NULL) {
		printf("Nothing to print (array is empty).\n");
		return NORMAL;
	}
	printf("Length: %d\n", allArrays[numberArray]->length);
	printf("-------------------------------------------------------------------\n");
	int maximum, maximumIndex, minimum, minimumIndex, mostFreq;
	getMaxMin(allArrays[numberArray], &maximum, &maximumIndex, &minimum, &minimumIndex);
	double average, maxDif;
	getAverage(allArrays[numberArray], &average, &maxDif);
	statusCode status = getMostFrequent(allArrays[numberArray], &mostFreq);
	if (status == MEMORY_ALLOCATION_ERROR) {
		return MEMORY_ALLOCATION_ERROR;
	}
	printf("Maximum: %d, index of maximum: %d\n", maximum, maximumIndex);
	printf("-------------------------------------------------------------------\n");
	printf("Minimum: %d, index of minimum: %d\n", minimum, minimumIndex);
	printf("-------------------------------------------------------------------\n");
	printf("Most frequent: %d\n", mostFreq);
	printf("-------------------------------------------------------------------\n");
	printf("Average: %lf, maximum deviation: %lf\n\n", average, maxDif);
	return NORMAL;
}

statusCode CommandPrint(Array** allArrays, char* string) {
	int numberArray, start, end, current, flagAll, flagStat;
	ReadPrintLine(string, &numberArray, &start, &end, &current, &flagAll, &flagStat);
	if (allArrays[numberArray] == NULL) {
		printf("Nothing to print (array is empty).\n");
		return NORMAL;
	}
	if (flagAll) {
		printf("All elements of array %c:\n", numberArray + 'a');
		printf("%d", allArrays[numberArray]->numbers[0]);
		for (int i = 1; i < allArrays[numberArray]->length; i++) {
			printf(", %d", allArrays[numberArray]->numbers[i]);
		}
		printf("\n\n");
		return NORMAL;
	} else if (start != -1 && end != -1) {
		if (start >= end) {
			return INVALID_BOUNDARIES;
		}
		if (allArrays[numberArray]->length - 1 < end) {
			return INDEX_OUT_RANGE;
		}
		printf("Elements of array %c starting from %d and ending with %d:\n", numberArray + 'a', start, end);
		printf("%d", allArrays[numberArray]->numbers[start]);
		for (int i = start; i <= end; i++) {
			printf(", %d", allArrays[numberArray]->numbers[i]);
		}
		printf("\n\n");
		return NORMAL;
	} else if (start != -1) {
		if (allArrays[numberArray]->length - 1 < start) {
			return INDEX_OUT_RANGE;
		}
		printf("Element %d of array %c:\n", start, numberArray + 'a');
		printf("%d\n\n", allArrays[numberArray]->numbers[start]);
		return NORMAL;
	}
	return INCORRECT_COMMAND;
}

statusCode ValidateLoad(const char* string) {
	int index = 0;
	int flagCommand = 0;
	int flagFileFirst = 0;
	int flagComma = 0;
	int point = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) {
		index++;
		if (flagCommand == 0) flagCommand = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		index++;
		flagComma = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index]) || IsDigit(string[index]) || string[index] == '_') {
		index++;
		flagFileFirst = 1;
	}
	if (string[index] == '.') {
		point = 1;
		index++;
	}
	if (!(string[index] == 't' && string[index + 1] == 'x' && string[index + 2] == 't')) return INVALID_SYNTAX;
	index += 3;
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		flagEnd = 1;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\r' && string[index] != '\0') return INVALID_SYNTAX;
	if (flagCommand && flagFileFirst && point && flagComma == 1 && flagEnd) return NORMAL;
	return INVALID_SYNTAX;
}

statusCode ValidateRand(const char* string) {
	int flagIsCommand = 0;
	int flagCount = 0;
	int flagIsLeft = 0;
	int flagIsRight = 0;
	int index = 0;
	int comma = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) {
		index++;
		flagIsCommand = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index])) {
		index++;
		flagCount = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == '-') index++;
	while (IsDigit(string[index])) {
		index++;
		flagIsLeft = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == '-') index++;
	while (IsDigit(string[index])) {
		index++;
		flagIsRight = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		flagEnd = 1;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0' && string[index] != '\r') return INVALID_SYNTAX;
	if (comma == 3 && flagIsLeft && flagIsRight && flagCount && flagIsCommand && flagEnd) return NORMAL;
	return INVALID_SYNTAX;
}

statusCode ValidateConcat(const char* string) {
	int index = 0;
	int flagCommand = 0;
	int comma = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) {
		index++;
		flagCommand = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		index++;
		comma++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		index++;
		flagEnd = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0' && string[index] != '\r') return INVALID_SYNTAX;
	if (comma && flagCommand && flagEnd) return NORMAL;
	return INVALID_SYNTAX;
}

statusCode ValidateFree(const char* string) {
	int index = 0;
	int openFlag = 0;
	int endFlag = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == '(') {
		openFlag = 1;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ')') {
		endFlag = 1;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		index++;
		flagEnd = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0' && string[index] != '\r') return INVALID_SYNTAX;
	if (flagEnd == 1 && openFlag == 1 && endFlag == 1) return NORMAL;
	return INVALID_SYNTAX;
}

statusCode ValidateRemove(const char* string) {
	int flagIsCommand = 0;
	int flagFirst = 0;
	int flagSecond = 0;
	int index = 0;
	int comma = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) {
		index++;
		flagIsCommand = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index])) {
		index++;
		flagFirst = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index])) {
		index++;
		flagSecond = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		index++;
		flagEnd = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0' && string[index] != '\r') return INVALID_SYNTAX;
	if (comma == 2 && flagFirst && flagSecond && flagIsCommand && flagEnd) return NORMAL;
	return INVALID_SYNTAX;
}

statusCode ValidateCopy(const char* string) {
	int flagIsCommand = 0;
	int flagFirst = 0;
	int flagSecond = 0;
	int index = 0;
	int comma = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) {
		index++;
		flagIsCommand = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index])) {
		index++;
		flagFirst = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsDigit(string[index])) {
		index++;
		flagSecond = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		comma++;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		flagEnd = 1;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\0' && string[index] != '\r') return INVALID_SYNTAX;
	if (comma == 3 && flagFirst && flagSecond && flagIsCommand && flagEnd) return NORMAL;
	return INVALID_SYNTAX;
}

statusCode ValidateSort(const char* string) {
	int index = 0;
	int flag = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == '-' || string[index] == '+') {
		flag = 1;
		index++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		index++;
		flagEnd = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\r' && string[index] != '\0') return INVALID_SYNTAX;
	if (flag == 0 || flagEnd == 0) return INVALID_SYNTAX;
	return NORMAL;
}

statusCode ValidateShuffle(const char* string) {
	int index = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	int flagEnd = 0;
	if (string[index] == ';') {
		index++;
		flagEnd = 1;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] != '\r' && string[index] != '\0') return INVALID_SYNTAX;
	if (flagEnd == 0) return INVALID_SYNTAX;
	return NORMAL;
}

statusCode ValidatePrint(const char* string) {
	int index = 0;
	int comma = 0;
	while (string[index] == ' ' || string[index] == '\t') index++;
	while (IsAlpha(string[index])) index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (!IsAlpha(string[index])) return INVALID_SYNTAX;
	index++;
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == ',') {
		index++;
		comma++;
	}
	while (string[index] == ' ' || string[index] == '\t') index++;
	if (string[index] == '\0' || string[index] == '\r') return INVALID_SYNTAX;
	if (IsDigit(string[index])) {
		while (IsDigit(string[index])) {
			index++;
		}
		while (string[index] == ' ' || string[index] == '\t') index++;
		int flagEnd = 0;
		if (string[index] == ';') {
			index++;
			flagEnd = 1;
		}
		while (string[index] == ' ' || string[index] == '\t') index++;
		if (string[index] == '\0' || string[index] == '\r' && flagEnd == 1) return NORMAL;
		if (string[index] == ',') {
			index++;
			comma++;
		}
		while (string[index] == ' ' || string[index] == '\t') index++;
		if (!IsDigit(string[index])) return INVALID_SYNTAX;
		while (IsDigit(string[index])) index++;
		while (string[index] == ' ' || string[index] == '\t') index++;
		if (string[index] == ';') {
			index++;
			flagEnd++;
		}
		while (string[index] == ' ' || string[index] == '\t') index++;
		if (string[index] != '\0' && string[index] != '\r') return INVALID_SYNTAX;
		if (comma != 2 || flagEnd != 1) return INVALID_SYNTAX;
		return NORMAL;
	} else if (IsAlpha(string[index])) {
		int flag = 0;
		if (ToLower(string[index]) == 'a' && ToLower(string[index + 1]) == 'l' && ToLower(string[index + 2]) == 'l') {
			index += 3;
			flag = 1;
		}
		while (string[index] == ' ' || string[index] == '\t') index++;
		int flagEnd = 0;
		if (string[index] == ';') {
			index++;
			flagEnd = 1;
		}
		while (string[index] == ' ' || string[index] == '\t') index++;
		if (string[index] != '\0' && string[index] != '\r') return INVALID_SYNTAX;
		if (flag != 1) return INVALID_SYNTAX;
		if (comma != 1) return INVALID_SYNTAX;
		if (flagEnd != 1) return INVALID_SYNTAX;
		return NORMAL;
	} else
		return INVALID_SYNTAX;
}