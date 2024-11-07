#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "stringLibrary.h"

typedef enum statusCode {
	NORMAL,
	WRONG_NUMBER_OF_ARGUMENTS,
	MEMORY_ALLOCATION_ERROR,
	OUTPUT_FILE_OPEN_ERROR,
	INPUT_FILE_OPEN_ERROR,
	SAME_FILES
} statusCode;

typedef enum constants {
	START_BUFFER_LENGTH = 100
} constants;

statusCode GetOpts(int argc, char** argv, char** inputFile, char** outputFile);
statusCode PrintTreeToFile(const char* string, FILE* output);
statusCode FileProcessing(const char* inputFile, const char* outputFile);
statusCode PathsAreSame(const char* inputFile, const char* outputFile);

int main(int argc, char** argv) {
	char* inputFile;
	char* outputFile;
	statusCode status = GetOpts(argc, argv, &inputFile, &outputFile);
	switch (status) {
		case WRONG_NUMBER_OF_ARGUMENTS:
			printf("Wrong number of arguments (must be 2).\n");
			return 1;
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
	}
	printf("%s %s\n", inputFile, outputFile);
	status = PathsAreSame(inputFile, outputFile);
	if (status == SAME_FILES) {
		printf("Files should be different.\n");
		free(inputFile);
		free(outputFile);
		return 1;
	}
	status = FileProcessing(inputFile, outputFile);
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			free(inputFile);
			free(outputFile);
			return 1;
		case INPUT_FILE_OPEN_ERROR:
			printf("An error while opening input file.\n");
			free(inputFile);
			free(outputFile);
			return 1;
		case OUTPUT_FILE_OPEN_ERROR:
			printf("An error while opening output file.\n");
			free(inputFile);
			free(outputFile);
			return 1;
	}
	free(inputFile);
	free(outputFile);
	return 0;
}

statusCode GetOpts(int argc, char** argv, char** inputFile, char** outputFile) {
	if (argc != 3) {
		return WRONG_NUMBER_OF_ARGUMENTS;
	}
	*inputFile = (char*)malloc(sizeof(char) * (StringLength(argv[1]) + 1));
	if (*inputFile == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	*outputFile = (char*)malloc(sizeof(char) * (StringLength(argv[2]) + 1));
	if (*outputFile == NULL) {
		free(*inputFile);
		return MEMORY_ALLOCATION_ERROR;
	}
	StringCopy(*inputFile, argv[1]);
	StringCopy(*outputFile, argv[2]);
	return NORMAL;
}

statusCode PrintTreeToFile(const char* string, FILE* output) {
	int level = 0;
	int index = 0;
	fprintf(output, "Tree for expression %s:\n", string);
	while (string[index] != '\0') {
		char c = string[index];
		if (c == '(') {
			level++;
		} else if (c == ')') {
			level--;
		} else if (IsAlpha(c)) {
			for (int i = 0; i < level; i++) {
				fprintf(output, "   ");
			}
			while (IsAlpha(c)) {
				fprintf(output, "%c", c);
				c = string[++index];
			}
			index--;
			fprintf(output, "\n");
		}
		index++;
	}
	return NORMAL;
}

statusCode FileProcessing(const char* inputFile, const char* outputFile) {
	FILE* input = fopen(inputFile, "r");
	if (input == NULL) {
		return INPUT_FILE_OPEN_ERROR;
	}
	FILE* output = fopen(outputFile, "w");
	if (output == NULL) {
		fclose(input);
		return OUTPUT_FILE_OPEN_ERROR;
	}
	char* buffer = (char*)malloc(sizeof(char) * START_BUFFER_LENGTH);
	if (buffer == NULL) {
		fclose(input);
		fclose(output);
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = '0';
	int currentLength = 0;
	int totalLength = START_BUFFER_LENGTH;
	while ((c = getc(input)) != EOF) {
		if (c == '\n') {
			buffer[currentLength - 1] = '\0';
			PrintTreeToFile(buffer, output);
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
		PrintTreeToFile(buffer, output);
	}
	free(buffer);
	fclose(input);
	fclose(output);
	return NORMAL;
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