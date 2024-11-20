#include <stdio.h>
#include "ex4lab4library.h"
#include <stdlib.h>


int main(int argc, char** argv) {
	vector* vectors[26] = {NULL};
	char* inputFile = NULL;
	char* outputFile = NULL;
	statusCode status = GetFileName(argc, argv, &inputFile, &outputFile);
	if (status == INVALID_NUMBER) {
		printf("Invalid number of arguments. It must be 1 or 3 (input file name, flag '/trace' and output file name.\n");
		return 1;
	}
	if (outputFile == NULL) {
		status = FileReading(inputFile, vectors, stdout);
	} else {
		if (PathsAreSame(inputFile, outputFile) == SAME_FILES) {
			printf("You can't use one file to read and write.\n");
			return 1;
		}
		FILE* output = fopen(outputFile, "w");
		if (output == NULL) {
			printf("An error while opening file to write.\n");
			return 1;
		}
		status = FileReading(inputFile, vectors, output);
		fclose(output);
	}
	switch (status) {
		case FILE_OPEN_ERROR:
			printf("An error while opening input file.\n");
			return 1;
		case MEMORY_ALLOCATION_ERROR:
			for (int i = 0; i < LENGTH_ALPHABET; i++) {
				if (vectors[i] != NULL) free(vectors[i]->values);
				free(vectors[i]);
			}
			return 1;
	}
	for (int i = 0; i < LENGTH_ALPHABET; i++) {
		if (vectors[i] != NULL) free(vectors[i]->values);
		free(vectors[i]);
	}
	return 0;
}