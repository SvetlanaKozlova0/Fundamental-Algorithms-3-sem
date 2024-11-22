#include <stdio.h>
#include "exercise7lab4library.h"
#include <stdlib.h>

int main(int argc, char** argv) {
	char* fileName = NULL;
	statusCode status = GetFileName(argc, argv, &fileName);
	if (status == INVALID_NUMBER_ARGUMENTS) {
		printf("Invalid number of arguments.\n");
		return 1;
	}
	MemoryCell* cells = NULL;
	status = FileReading(fileName, &cells);
	switch (status) {
		case FILE_OPEN_ERROR:
			printf("File open error.\n");
			return 1;
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case UNINIT_VALUE:
			printf("Uninitialized value.\n");
			return 1;
	}
	return 0;
}