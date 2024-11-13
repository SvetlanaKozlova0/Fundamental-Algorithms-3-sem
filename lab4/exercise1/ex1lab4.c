#include <stdio.h>
#include <stdlib.h>

#include "ex1lab4library.h"

int main(int argc, char** argv) {
	char* fileName;
	statusCode status = GetFileName(argc, argv, &fileName);
	if (status == WRONG_ARGUMENTS) {
		printf("The number of arguments should be one (file name).\n");
		return 1;
	}
	status = FileProcessing(fileName, "output.txt");
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case FILE_OPEN_ERROR:
			printf("An error while opening files.\n");
			return 1;
	}
	status = OverwriteTheFile("output.txt", fileName);
	switch (status) {
		case WRONG_ARGUMENTS:
			printf("Arguments are invalid.\n");
			return 1;
		case FILE_OPEN_ERROR:
			printf("An error while opening files.\n");
			return 1;
		case FILE_WRITING_ERROR:
			printf("An error while writing in file.\n");
			return 1;
	}
	status = ClearFile("output.txt");
	switch (status) {
		case FILE_OPEN_ERROR:
			printf("An error while opening file.\n");
			return 1;
		case WRONG_ARGUMENTS:
			printf("Arguments are invalid.\n");
			return 1;
	}
	printf("Everything is successful.\n");
	return 0;
}