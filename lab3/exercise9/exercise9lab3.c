#include "exercise9lab3library.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	char* file_name = NULL;
	char* separators = NULL;
	status_code status = get_arguments(argc, argv, &file_name, &separators);
	switch (status) {
		case WRONG_NUMBER_ARGUMENTS:
			printf("Wrong number of arguments (must be at least two).\n");
			return 1;
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			return 1;
		case INCORRECT_SEPARATORS:
			printf("Incorrect separators (separator must symbol, not string).\n");
			return 1;
	}
	printf("File name: %s\n", file_name);
	printf("Separators: %s\n", separators);
	search_tree* tree = NULL;
	status = file_reading(file_name, separators, &tree);
	switch (status) {
		case NOT_EMPTY:
			printf("Search tree is not empty at the start.\n");
			free(separators);
			return 1;
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			free(separators);
			return 1;
		case FILE_OPEN_ERROR:
			printf("An error while opening input file.\n");
			free(separators);
			return 1;
	}
	printf("Please, enter the name of file to load tree: ");
	char* output_file_name = NULL;
	status = get_string_console(&output_file_name);
	switch (status) {
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error.\n");
			free(separators);
			destroy_search_tree(&tree);
			return 1;
		case NOT_EMPTY:
			printf("File name string is not empty where empty needed.\n");
			free(separators);
			destroy_search_tree(&tree);
			return 1;
	}
	FILE* output = fopen(output_file_name, "w");
	if (output == NULL) {
		destroy_search_tree(&tree);
		free(separators);
		printf("An error while opening output.txt.\n");
		return 1;
	}
	file_load_recursive(output, tree->head);
	free(output_file_name);
	fclose(output);
	int amount = 0;
	amount_elements_search_tree(tree->head, &amount);
	printf("Amount elements: %d\n", amount);
	status = interactive_dialogue(&tree, amount);
	if (status != NORMAL) {
		destroy_search_tree(&tree);
		free(separators);
		return 1;
	}
	destroy_search_tree(&tree);
	free(separators);
	return 0;
}