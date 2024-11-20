#include <stdio.h>
#include <stdlib.h>

#include "ex2lab4library.h"

int main() {
	statusCode status = InstructionsProcessing("input.txt");
	if (status != NORMAL) printf("ERROR: ");
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case FILE_OPEN_ERROR:
			printf("File open error.\n");
			return 1;
		case INCORRECT_COMMAND:
			printf("Incorrect command.\n");
			return 1;
		case INDEX_OUT_RANGE:
			printf("Array index out of range.\n");
			return 1;
		case INVALID_SYNTAX:
			printf("Incorrect syntax when writing commands.\n");
			return 1;
		case NOT_EMPTY:
			printf("A non-empty array where an empty one is needed.\n");
			return 1;
		case EMPTY_ARRAY:
			printf("An empty array where not empty is needed.\n");
			return 1;
		case INVALID_BOUNDARIES:
			printf("Overstepping the bounds of the array (invalid indexes).\n");
			return 1;
		case INVALID_BORDER_ORDER:
			printf("Invalid border order (right border must be greater than left).\n");
			return 1;
		case INCORRECT_FREE:
			printf("Invalid free (this array is already free).\n");
			return 1;
	}
	if (status != NORMAL) {
		printf("error.\n");
		return 1;
	}
	return 0;
}
