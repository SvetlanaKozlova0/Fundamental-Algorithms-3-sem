#include <stdio.h>
#include <stdlib.h>
#include "ex2lab4library.h"

int main() {
	statusCode status = InstructionsProcessing("output.txt");
	switch (status) {
		case INVALID_SYNTAX:
			printf("Your command has incorrect syntax.\n"
			    "The program is ending.\n");
			return 1;
	}
	return 0;
}