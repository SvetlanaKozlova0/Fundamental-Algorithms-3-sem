#include "ex4lab4library.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../stringLibrary.h"

int getIndex(char c) {
	if (c >= 'a' && c <= 'z') return c - 'a';
	if (c >= 'A' && c <= 'Z') return c - 'A';
	return -1;
}

int operationDisjunction(int a, int b) { return a || b; }

int operationConjunction(int a, int b) { return a && b; }

int operationImplication(int a, int b) { return !a || b; }

int operationReverseImplication(int a, int b) { return a || !b; }

int operationEquivalent(int a, int b) { return (a && b) || (!a && !b); }

int operationMod2(int a, int b) { return (!a && b) || (a && !b); }

int operationCoimplication(int a, int b) { return !operationImplication(a, b); }

int operationSchaeffer(int a, int b) { return !(a && b); }

int operationPierce(int a, int b) { return !(a || b); }

function functions[] = {operationDisjunction,        operationConjunction, operationImplication,
                        operationReverseImplication, operationEquivalent,  operationMod2,
                        operationCoimplication,      operationSchaeffer,   operationPierce};

typedef struct {
	const char* symbol;
	function func;
} signs;

signs operations[] = {{"+>", operationCoimplication},
                      {"+", operationDisjunction},
                      {"&", operationConjunction},
                      {"->", operationImplication},
                      {"<-", operationReverseImplication},
                      {"<>", operationMod2},
                      {"~", operationEquivalent},
                      {"?", operationSchaeffer},
                      {"!", operationPierce},
                      {NULL, NULL}};

const char* errors[] = {"Operation terminated.\n",
                        "First operand in binary operation is empty vector.\n",
                        "Second operand in binary operation is empty vector.\n",
                        "Operation is invalid (not found).\n",
                        "Memory allocation error while allocating memory for smallest vector.\n",
                        "Memory allocation error while allocating memory for result of binary operation.\n",
                        "Memory allocation error while allocating memory for values of result of binary operation.\n",
                        "Operand of unary operation is empty.\n",
                        "Memory allocation error while allocating memory for result of unary operation.\n",
                        "Memory allocation error while allocating memory for values of result of\nunary operation.\n",
                        "Invalid index for the result of unary operation.\n",
                        "Invalid index for the operand of unary operation.\n",
                        "Input file open error.\n",
                        "Memory allocation error while allocating memory for buffer.\n",
                        "Memory allocation error.\n",
                        "Incorrect braces in expression.\n",
                        "Wrong number of base. Base can't be < 2 or > 36.\n",
                        "An error while reading the number of base.\n",
                        "Invalid index for the operand of read operation.\n",
                        "Not empty vector for the operation read.\n",
                        "An error while scanning the number of base of read operation.\n",
                        "Memory allocation error while allocating memory for vector by reading operation.\n",
                        "Memory allocation error while allocating memory for values of vector by reading\noperation.\n",
                        "Number to read contains incorrect symbols.\n",
                        "Invalid index for the operand of write operation.\n",
                        "Empty vector for the operation write.\n",
                        "Memory allocation error while allocating memory for string by writing operation.\n",
                        "Invalid index for the result of binary operation.\n",
                        "Invalid index for the first operand of binary operation.\n",
                        "Invalid binary operation (not found).\n",
                        "Invalid index for the second operand of binary operation.\n",
                        "Empty vector for the unary operation.\n",
                        "Number contains invalid symbols in that base.\n",
                        "An error in syntax.\n",
                        "Unknown operation.\n"};

statusCode ResultOperation(vector** result, vector* first, vector* second, function op, FILE* output) {
	if (first == NULL) {
		fprintf(output, "%s%s", errors[1], errors[0]);
		return EMPTY_VECTOR;
	}
	if (second == NULL) {
		fprintf(output, "%s%s", errors[2], errors[0]);
		return EMPTY_VECTOR;
	}
	if (op == NULL) {
		fprintf(output, "%s%s", errors[3], errors[0]);
		return INVALID_OPERATION;
	}
	vector* secondClone;
	if (first->length != second->length) {
		secondClone = (vector*)malloc(sizeof(vector));
		if (secondClone == NULL) {
			fprintf(output, "%s%s", errors[4], errors[0]);
			return MEMORY_ALLOCATION_ERROR;
		}
		secondClone->values = (int*)malloc(sizeof(int) * first->length);
		if (secondClone->values == NULL) {
			free(secondClone);
			fprintf(output, "%s%s", errors[4], errors[0]);
			return MEMORY_ALLOCATION_ERROR;
		}
		for (int i = 0; i < second->length; i++) secondClone->values[i] = second->values[i];
		for (int i = second->length; i < first->length; i++) secondClone->values[i] = 0;
		secondClone->length = first->length;
	} else {
		secondClone = second;
	}
	vector* newResult = (vector*)malloc(sizeof(vector));
	if (newResult == NULL) {
		if (secondClone != second) {
			free(secondClone->values);
			free(secondClone);
		}
		fprintf(output, "%s%s", errors[5], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	newResult->values = (int*)malloc(sizeof(int) * first->length);
	if (newResult->values == NULL) {
		free(newResult);
		if (secondClone != second) {
			free(secondClone->values);
			free(secondClone);
		}
		fprintf(output, "%s%s", errors[6], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	newResult->length = first->length;
	fprintf(output, "Operands:\n");
	PrintVector(first, output);
	PrintVector(secondClone, output);
	for (int i = 0; i < newResult->length; i++) {
		newResult->values[i] = op(first->values[i], secondClone->values[i]);
	}
	if (*result != NULL) {
		free((*result)->values);
		free(*result);
	}
	fprintf(output, "Result:\n");
	PrintVector(newResult, output);
	*result = newResult;
	if (secondClone != second) {
		free(secondClone->values);
		free(secondClone);
	}
	return NORMAL;
}

statusCode ResultUnaryOperation(vector** result, vector* operand, FILE* output) {
	if (operand == NULL) {
		fprintf(output, "%s%s", errors[7], errors[0]);
		return EMPTY_VECTOR;
	}
	vector* newResult = (vector*)malloc(sizeof(vector));
	if (newResult == NULL) {
		fprintf(output, "%s%s", errors[8], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	newResult->values = (int*)malloc(sizeof(int) * operand->length);
	if (newResult->values == NULL) {
		free(newResult);
		fprintf(output, "%s%s", errors[9], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	newResult->length = operand->length;
	for (int i = 0; i < operand->length; i++) newResult->values[i] = !operand->values[i];
	fprintf(output, "Operand:\n");
	PrintVector(operand, output);
	fprintf(output, "Result:\n");
	PrintVector(newResult, output);
	if (*result != NULL) {
		free((*result)->values);
		free(*result);
	}
	*result = newResult;

	return NORMAL;
}

int StringNCompare(const char* first, const char* second, int length) {
	if (StringLength(first) < length || StringLength(second) < length) return -2;
	for (int i = 0; i < length; i++) {
		if (ToLower(first[i]) > ToLower(second[i]))
			return 1;
		else if (ToLower(first[i]) < ToLower(second[i]))
			return -1;
	}
	return 0;
}

function findOperation(const char* expression, int* index) {
	for (int i = 0; operations[i].symbol != NULL; i++) {
		const char* op = operations[i].symbol;
		int len = StringLength(op);
		if (StringNCompare(expression + *index, op, len) == 0) {
			*index += len;
			return operations[i].func;
		}
	}
	return NULL;
}

statusCode BinaryOperation(vector** values, const char* expression, FILE* output) {
	int index = 0;
	int result, first, second;
	result = getIndex(expression[index++]);
	if (result == -1) {
		fprintf(output, "%s%s", errors[27], errors[0]);
		return INVALID_RESULT_INDEX;
	}
	index += 2;
	first = getIndex(expression[index++]);
	if (first == -1) {
		fprintf(output, "%s%s", errors[28], errors[0]);
		return INVALID_FIRST_INDEX;
	}
	function op = findOperation(expression, &index);
	if (op == NULL) {
		fprintf(output, "%s%s", errors[29], errors[0]);
		return INVALID_OPERATION;
	}
	second = getIndex(expression[index++]);
	if (second == -1) {
		fprintf(output, "%s%s", errors[30], errors[0]);
		return INVALID_SECOND_INDEX;
	}
	if (values[first] == NULL) {
		fprintf(output, "%s%s", errors[1], errors[0]);
		return EMPTY_VECTOR;
	}
	if (values[second] == NULL) {
		fprintf(output, "%s%s", errors[2], errors[0]);
		return EMPTY_VECTOR;
	}
	if (values[first]->length < values[second]->length) {
		int temp = first;
		first = second;
		second = temp;
	}
	return ResultOperation(&values[result], values[first], values[second], op, output);
}

statusCode UnaryOperation(vector** values, const char* expression, FILE* output) {
	int index = 0;
	int first, second;
	first = getIndex(expression[index++]);
	if (first == -1) {
		fprintf(output, "%s%s", errors[10], errors[0]);
		return INVALID_RESULT_INDEX;
	}
	index += 3;
	second = getIndex(expression[index++]);
	if (second == -1) {
		fprintf(output, "%s%s", errors[11], errors[0]);
		return INVALID_SECOND_INDEX;
	}
	if (values[second] == NULL) {
		fprintf(output, "%s%s", errors[31], errors[0]);
		return EMPTY_VECTOR;
	}
	return ResultUnaryOperation(&values[first], values[second], output);
}

int getNumber(char c) {
	if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
	if (c >= 'a' && c <= 'z') return c - 'a' + 10;
	if (c >= '0' && c <= '9') return c - '0';
	return -1;
}

statusCode FileReading(const char* fileName, vector** vectors, FILE* output) {
	FILE* input = fopen(fileName, "r");
	if (input == NULL) {
		fprintf(output, "%s", errors[12]);
		return FILE_OPEN_ERROR;
	}
	char* buffer = (char*)malloc(sizeof(char) * START_LENGTH_BUFFER);
	if (buffer == NULL) {
		fclose(input);
		fprintf(output, "%s", errors[13]);
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = '0';
	int currentLength = 0;
	int totalLength = START_LENGTH_BUFFER;
	statusCode status = NORMAL;
	int count = 1;
	while ((c = getc(input)) != EOF) {
		if (c == '}') {
			free(buffer);
			fclose(input);
			return INCORRECT_BRACES;
		}
		if (c == '{') {
			int countBraces = 1;
			while (c != EOF && countBraces != 0) {
				c = getc(input);
				if (c == '{') countBraces++;
				if (c == '}') countBraces--;
			}
			if (c != EOF && countBraces == 0) {
				continue;
			} else if (c == EOF && countBraces == 0) {
				free(buffer);
				fclose(input);
				return NORMAL;
			} else {
				free(buffer);
				fclose(input);
				return INCORRECT_BRACES;
			}
		}
		if (c == ';') {
			buffer[currentLength] = '\0';
			currentLength = 0;
			printf("%s\n", buffer);
			fprintf(output, "%d. ", count++);
			status = ProcessingString(buffer, vectors, output);
			if (status == MEMORY_ALLOCATION_ERROR) {
				free(buffer);
				fclose(input);
				return MEMORY_ALLOCATION_ERROR;
			}
		}

		else {
			if (currentLength + 1 >= totalLength) {
				totalLength *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * totalLength);
				if (tempBuffer == NULL) {
					free(buffer);
					fclose(input);
					fprintf(output, "%s%s", errors[13], errors[0]);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[currentLength++] = (char)c;
		}
	}
	free(buffer);
	fclose(input);
	return NORMAL;
}

type DetectType(const char* line) {
	int index = 0;
	int flag = 0;
	if (StringNCompare(line, "read(", 5) == 0) {
		if (!IsAlpha(line[5]) || line[6] != ',') return SYNTAX_ERROR;
		index = 7;
		flag = 0;
		while (IsDigit(line[index])) {
			index++;
			flag = 1;
		}
		if (flag == 0) return SYNTAX_ERROR;
		if (line[index] != ')' || line[index + 1] != '\0') return SYNTAX_ERROR;
		return OPERATION_READ;
	} else if (StringNCompare(line, "write(", 6) == 0) {
		if (!IsAlpha(line[6]) || line[7] != ',') return SYNTAX_ERROR;
		index = 8;
		flag = 0;
		while (IsDigit(line[index])) {
			index++;
			flag = 1;
		}
		if (flag == 0) return SYNTAX_ERROR;
		if (line[index] != ')' || line[index + 1] != '\0') return SYNTAX_ERROR;
		return OPERATION_WRITE;
	}
	if (!IsAlpha(line[0]) || line[1] != ':' || line[2] != '=') return SYNTAX_ERROR;
	if (line[3] == '\\' && IsAlpha(line[4]) && line[5] == '\0') return UNARY_OPERATION;
	if (!IsAlpha(line[3])) return SYNTAX_ERROR;
	index = 5;
	if (!IsAlpha(line[5])) index++;
	if (!IsAlpha(line[index]) && line[index + 1] != '\0') return SYNTAX_ERROR;
	return BINARY_OPERATION;
}

statusCode ReadOperation(vector** values, const char* expression, const char* number, FILE* output) {
	int index = getIndex(expression[5]);
	if (index == -1) {
		fprintf(output, "%s%s", errors[18], errors[0]);
		return INVALID_RESULT_INDEX;
	}
	if (values[index] != NULL) {
		fprintf(output, "%s%s", errors[19], errors[0]);
		return NOT_EMPTY_VECTOR;
	}
	int base = 0;
	int scanResult = sscanf(expression + 7, "%d", &base);
	if (scanResult == -1) {
		fprintf(output, "%s%s", errors[20], errors[0]);
		return INVALID_NUMBER;
	}
	if (base < 2 || base > 36) {
		fprintf(output, "%s%s", errors[16], errors[0]);
		return INVALID_BASE;
	}
	if (ValidNumber(number, base) == 0) {
		fprintf(output, "%s%s", errors[32], errors[0]);
		return INVALID_NUMBER;
	}
	values[index] = (vector*)malloc(sizeof(vector));
	if (values[index] == NULL) {
		fprintf(output, "%s%s", errors[21], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	values[index]->values = (int*)malloc(sizeof(int) * START_LENGTH_STRING);
	if (values[index]->values == NULL) {
		fprintf(output, "%s%s", errors[22], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	int numberInt = 0;
	for (int i = 0; i < StringLength(number); i++) {
		numberInt *= base;
		if (number[i] >= '0' && number[i] <= '9') {
			numberInt += number[i] - '0';
		} else if (number[i] >= 'A' && number[i] <= 'Z') {
			numberInt += number[i] - 'A' + 10;
		} else if (number[i] >= 'a' && number[i] <= 'z') {
			numberInt += number[i] - 'a' + 10;
		}
	}
	int totalLength = START_LENGTH_STRING;
	int length = 0;
	if (numberInt == 0) {
		values[index]->values[length++] = 0;
	} else {
		while (numberInt > 0) {
			int elem = numberInt % 2;
			if (length + 1 >= totalLength) {
				totalLength *= 2;
				int* temp = (int*)realloc(values[index]->values, sizeof(int) * totalLength);
				if (temp == NULL) {
					fprintf(output, "%s%s", errors[21], errors[0]);
					return MEMORY_ALLOCATION_ERROR;
				}
				values[index]->values = temp;
			}
			values[index]->values[length++] = elem;
			numberInt /= 2;
		}
	}
	values[index]->length = length;
	return NORMAL;
}

statusCode WriteOperation(vector** values, const char* expression, char** result, FILE* output) {
	int index = getIndex(expression[6]);
	if (index == -1) {
		fprintf(output, "%s%s", errors[24], errors[0]);
		return INVALID_RESULT_INDEX;
	}
	if (values[index] == NULL) {
		fprintf(output, "%s%s", errors[25], errors[0]);
		return EMPTY_VECTOR;
	}
	int base = 0;
	int scanResult = sscanf(expression + 8, "%d", &base);
	if (scanResult != 1) {
		fprintf(output, "%s%s", errors[17], errors[0]);
		return INVALID_NUMBER;
	}
	if (base < 2 || base > 36) {
		fprintf(output, "%s%s", errors[16], errors[0]);
		return INVALID_BASE;
	}
	*result = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (*result == NULL) {
		fprintf(output, "%s%s", errors[26], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	int current = 0;
	for (int i = values[index]->length - 1; i >= 0; i--) {
		current *= 2;
		current += values[index]->values[i];
	}
	int length = 0;
	int totalLength = START_LENGTH_STRING;
	if (current == 0) {
		(*result)[length++] = '0';
	} else
		while (current > 0) {
			int elem = current % base;
			if (length + 1 >= totalLength) {
				totalLength *= 2;
				char* temp = (char*)realloc(*result, sizeof(char) * totalLength);
				if (temp == NULL) {
					free(*result);
					fprintf(output, "%s%s", errors[26], errors[0]);
					return MEMORY_ALLOCATION_ERROR;
				}
				*result = temp;
			}
			if (elem < 10)
				(*result)[length++] = (char)('0' + elem);
			else
				(*result)[length++] = (char)('A' + elem - 10);
			current /= base;
		}
	for (int i = 0; i < length / 2; i++) {
		char temp = (*result)[i];
		(*result)[i] = (*result)[length - i - 1];
		(*result)[length - i - 1] = temp;
	}
	(*result)[length] = '\0';
	return NORMAL;
}

statusCode ProcessingString(const char* line, vector** values, FILE* output) {
	char* newString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (newString == NULL) {
		fprintf(output, "%s%s", errors[13], errors[0]);
		return MEMORY_ALLOCATION_ERROR;
	}
	int openBrackets = 0;
	int currentLength = 0;
	int totalLength = START_LENGTH_STRING;
	int i = 0;
	while (i < StringLength(line)) {
		if (line[i] == '{') {
			openBrackets = 1;
			while (openBrackets != 0 && line[i] != '\0') {
				i++;
				if (line[i] == '{') openBrackets++;
				if (line[i] == '}') openBrackets--;
			}
			if (line[i] == '\0' && openBrackets != 0) {
				free(newString);
				fprintf(output, "%s%s", errors[15], errors[0]);
				return INCORRECT_BRACES;
			}
			if (line[i] == '\0' || line[i + 1] == '\0') break;
			i++;
		}
		if (line[i] == '}') {
			free(newString);
			fprintf(output, "%s%s", errors[15], errors[0]);
			return INCORRECT_BRACES;
		}
		if (line[i] == '%') {
			while (line[i] != '\n' && line[i] != '\0') {
				i++;
			}
		}
		if (currentLength + 1 >= totalLength) {
			totalLength *= 2;
			char* tempString = (char*)realloc(newString, sizeof(char) * totalLength);
			if (tempString == NULL) {
				free(newString);
				fprintf(output, "%s%s", errors[13], errors[0]);
				return MEMORY_ALLOCATION_ERROR;
			}
			newString = tempString;
		}
		if (line[i] != '\n' && line[i] != ' ' && line[i] != '\r' && line[i] != '\t')
			newString[currentLength++] = line[i++];
		else
			i++;
	}
	if (openBrackets != 0) {
		free(newString);
		fprintf(output, "%s%s", errors[15], errors[0]);
		return INCORRECT_BRACES;
	}
	newString[currentLength] = '\0';
	fprintf(output, "Operation: %s\n", newString);
	type op = DetectType(newString);
	statusCode status = NORMAL;
	char* result;
	char* number;
	switch (op) {
		case UNARY_OPERATION:
			status = UnaryOperation(values, newString, output);
			free(newString);
			return status;
		case BINARY_OPERATION:
			status = BinaryOperation(values, newString, output);
			free(newString);
			return status;
		case OPERATION_READ:
			status = GetString(&number);
			if (status != NORMAL) {
				free(newString);
				fprintf(output, "%s%s", errors[13], errors[0]);
				return status;
			}
			status = ReadOperation(values, newString, number, output);
			free(newString);
			free(number);
			return status;
		case OPERATION_WRITE:
			status = WriteOperation(values, newString, &result, output);
			if (status == NORMAL) {
				fprintf(output, "%s\n", result);
				free(result);
			}
			free(newString);
			return status;
		case UNKNOWN_OPERATION:
			fprintf(output, "%s%s\n", errors[34], errors[0]);
			free(newString);
			return INVALID_EXPRESSION;
		case SYNTAX_ERROR:
			fprintf(output, "%s%s\n", errors[33], errors[0]);
			free(newString);
			return INVALID_EXPRESSION;
	}
}

statusCode GetString(char** currentString) {
	*currentString = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (*currentString == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = getc(stdin);
	int currentLength = START_LENGTH_STRING;
	int index = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (index + 1 >= currentLength) {
			currentLength *= 2;
			char* tempString = (char*)realloc(*currentString, sizeof(char) * (currentLength + 1));
			if (tempString == NULL) {
				free(*currentString);
				return MEMORY_ALLOCATION_ERROR;
			}
			*currentString = tempString;
		}
		(*currentString)[index++] = (char)c;
		c = getc(stdin);
	}
	(*currentString)[index] = '\0';
	return NORMAL;
}

statusCode GetFileName(int argc, char** argv, char** inputFile, char** outputFile) {
	if (!(argc == 2 || argc == 4)) return INVALID_NUMBER;
	if (argc == 2) {
		*inputFile = argv[1];
		return NORMAL;
	}
	if (CompareChars(argv[2], "/trace") == 0) {
		*outputFile = argv[3];
	} else
		*outputFile = NULL;
	*inputFile = argv[1];
	return NORMAL;
}

void PrintVector(vector* binaryVector, FILE* output) {
	fprintf(output, "Vector: (");
	for (int i = 0; i < binaryVector->length; i++) {
		fprintf(output, " %d", binaryVector->values[i]);
	}
	fprintf(output, " )\n");
}

int ValidNumber(const char* number, int base) {
	for (int i = 0; i < StringLength(number); i++) {
		if (IsDigit(number[i]) && (number[i] - '0') >= base) {
			return 0;
		} else if (number[i] >= 'a' && number[i] <= 'z') {
			if (number[i] - 'a' + 10 >= base) return 0;
		} else if (number[i] >= 'A' && number[i] <= 'Z') {
			if (number[i] - 'A' + 10 >= base) return 0;
		} else if (!IsDigit(number[i]) && !IsAlpha(number[i]))
			return 0;
	}
	return 1;
}

statusCode PathsAreSame(const char* inputFile, const char* outputFile) {
	char* firstPath = (char*)malloc(PATH_MAX * sizeof(char));
	if (firstPath == NULL) {
		return SAME_FILES;
	}
	if (realpath(inputFile, firstPath) == NULL) {
		free(firstPath);
		return SAME_FILES;
	}
	char* secondPath = (char*)malloc(PATH_MAX * sizeof(char));
	if (secondPath == NULL) {
		free(firstPath);
		return SAME_FILES;
	}
	if (realpath(outputFile, secondPath) == NULL) {
		free(firstPath);
		free(secondPath);
		return SAME_FILES;
	}
	char result = 0;
	if (CompareChars(firstPath, secondPath) == 0) {
		result = 0;
	} else {
		result = 1;
	}
	free(firstPath);
	free(secondPath);
	if (result == 0) return SAME_FILES;
	return NORMAL;
}