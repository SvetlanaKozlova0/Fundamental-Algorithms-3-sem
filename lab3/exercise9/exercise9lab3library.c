#include "exercise9lab3library.h"
#include "../../stringLibrary.h"
#include <stdlib.h>
#include <stdio.h>

status_code create_tree_node(tree_node** current, char* word) {
	if (*current != NULL) return NOT_EMPTY;
	*current = (tree_node*)malloc(sizeof(tree_node));
	if (*current == NULL) return MEMORY_ALLOCATION_ERROR;
	(*current)->word = (char*)malloc(sizeof(char) * (StringLength(word) + 1));
	if ((*current)->word == NULL) {
		free(*current);
		return MEMORY_ALLOCATION_ERROR;
	}
	char* result_copy = StringCopy((*current)->word, word);
	if (result_copy == NULL) {
		free(*current);
		return MEMORY_ALLOCATION_ERROR;
	}
	(*current)->freq = 1;
	(*current)->left = NULL;
	(*current)->right = NULL;
	return NORMAL;
}

void delete_tree_node(tree_node** current) {
	if (*current == NULL) return;
	free((*current)->word);
	free(*current);
	*current = NULL;
}

status_code create_search_tree(search_tree** tree) {
	*tree = (search_tree*)malloc(sizeof(search_tree));
	if (*tree == NULL) return MEMORY_ALLOCATION_ERROR;
	(*tree)->head = NULL;
	return NORMAL;
}

status_code add_search_tree(search_tree** tree, char* word) {
	status_code status = NORMAL;
	if (*tree == NULL) {
		tree_node* current = NULL;
		status = create_tree_node(&current, word);
		if (status != NORMAL) return status;
		(*tree)->head = current;
		return NORMAL;
	}
	tree_node** current_node = &(*tree)->head;
	while (*current_node != NULL) {
		if (CompareChars((*current_node)->word, word) == 0) {
			(*current_node)->freq++;
			return NORMAL;
		}
		if (CompareChars((*current_node)->word, word) == 1) {
			current_node = &(*current_node)->left;
		} else current_node = &(*current_node)->right;
	}
	status = create_tree_node(current_node, word);
	if (status != NORMAL) return status;
	return NORMAL;
}

int find_search_tree(search_tree* tree, char* word) {
	if (tree == NULL) return -1;
	tree_node* current = tree->head;
	int result_comparing = 0;
	while (current != NULL) {
		result_comparing = CompareChars(word, current->word);
		if (result_comparing == 0) return current->freq;
		if (result_comparing == 1) {
			current = current->right;
		}
		if (result_comparing == -1) {
			current = current->left;
		}
	}
	return 0;
}

void destroy_search_nodes(tree_node* tree) {
	if (tree == NULL) return;
	destroy_search_nodes(tree->left);
	destroy_search_nodes(tree->right);
	delete_tree_node(&tree);
}

void destroy_search_tree(search_tree** tree) {
	if (*tree == NULL) return;
	destroy_search_nodes((*tree)->head);
	free(*tree);
	*tree = NULL;
}

status_code get_arguments(int argc, char** argv, char** filename, char** separators) {
	if (argc < 3) return WRONG_NUMBER_ARGUMENTS;
	*filename = argv[1];
	*separators = (char*)malloc(sizeof(char) * (argc - 1));
	if (*separators == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 2; i < argc; i++) {
		if (StringLength(argv[i]) != 1) {
			free(*separators);
			return INCORRECT_SEPARATORS;
		}
		(*separators)[i] = argv[i][0];
	}
	(*separators)[argc - 2] = '\0';
	return NORMAL;
}

int is_separator(int current_char, char* separators) {
	for (int i = 0; i < StringLength(separators); i++) {
		if (separators[i] == current_char) return 1;
	}
	return 0;
}

status_code file_reading(const char* file_name, char* separators, search_tree** tree) {
	if (*tree != NULL) {
		return NOT_EMPTY;
	}
	status_code status = create_search_tree(tree);
	if (status != NORMAL) {
		return status;
	}
	FILE* input = fopen(file_name, "r");
	if (input == NULL) {
		return FILE_OPEN_ERROR;
	}
	char* buffer = (char*)malloc(sizeof(char) * START_BUFFER_SIZE);
	if (buffer == NULL) {
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	int c = '0';
	int currentLength = 0;
	int totalLength = START_BUFFER_SIZE;
	while ((c = getc(input)) != EOF) {
		if (is_separator(c, separators)) {
			buffer[currentLength] = '\0';
			if (StringLength(buffer) == 0) printf("empty string: ");
			if (StringLength(buffer) != 0) {
				status = add_search_tree(tree, buffer);
				if (status != NORMAL) {
					fclose(input);
					destroy_search_tree(tree);
					return status;
				}
			}
			printf("%s\n", buffer);
			currentLength = 0;
		} else {
			if (currentLength + 1 >= totalLength) {
				totalLength *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * totalLength);
				if (tempBuffer == NULL) {
					free(buffer);
					fclose(input);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[currentLength++] = (char)c;
		}
	}
	if (currentLength > 0) {
		buffer[currentLength] = '\0';
		printf("%s\n", buffer);
	}
	free(buffer);
	fclose(input);
	return NORMAL;
}

int max(int a, int b) {
	if (a > b) return a;
	return b;
}

int tree_depth(tree_node* tree) {
	if (tree == NULL) {
		return 0;
	}
	return max(tree_depth(tree->left), tree_depth(tree->right)) + 1;
}

void get_max_word(tree_node* tree, int* max_freq, char** max_word) {
	if (tree == NULL) return;
	if (tree->freq > *max_freq) {
		*max_freq = tree->freq;
		*max_word = tree->word;
	}
	get_max_word(tree->right, max_freq, max_word);
	get_max_word(tree->left, max_freq, max_word);
}

void get_min_word(tree_node* tree, int* min_freq, char** min_word) {
	if (tree == NULL) return;
	if (tree->freq < *min_freq) {
		*min_freq = tree->freq;
		*min_word = tree->word;
	}
	get_min_word(tree->right, min_freq, min_word);
	get_min_word(tree->left, min_freq, min_word);
}

void file_load_recursive(FILE* input, tree_node* current) {
	if (current == NULL) return;
	fprintf(input, "%s %d\n", current->word, current->freq);
	file_load_recursive(input, current->left);
	file_load_recursive(input, current->right);
}

status_code load_tree_in_file(char* file_name, search_tree* tree) {
	if (tree == NULL) {
		return NOT_EMPTY;
	}
	FILE* input = fopen(file_name, "w");
	if (input == NULL) {
		return FILE_OPEN_ERROR;
	}
	file_load_recursive(input, tree->head);
	fclose(input);
	return NORMAL;
}

status_code recover_tree_node(tree_node** tree, const char* word, int freq) {
	if (*tree != NULL) return NOT_EMPTY;
	*tree = (tree_node*)malloc(sizeof(tree_node));
	if (*tree == NULL) return MEMORY_ALLOCATION_ERROR;
	(*tree)->word = (char*)malloc(sizeof(char) * (StringLength(word) + 1));
	if ((*tree)->word == NULL) {
		free(*tree);
		return MEMORY_ALLOCATION_ERROR;
	}
	char* result_copy = StringCopy((*tree)->word, word);
	if (result_copy == NULL) {
		free((*tree)->word);
		free(*tree);
		return MEMORY_ALLOCATION_ERROR;
	}
	(*tree)->freq = freq;
	(*tree)->right = NULL;
	(*tree)->left = NULL;
	return NORMAL;
}

status_code join_node_to_tree(tree_node** tree, const char* word, int freq) {
	if (*tree == NULL) {
		return recover_tree_node(tree, word, freq);
	}
	int compare_result = CompareChars((*tree)->word, word);
	if (compare_result == 0) {
		(*tree)->freq = freq;
		return NORMAL;
	} else if (compare_result == 1) {
		return join_node_to_tree(&(*tree)->left, word, freq);
	} else {
		return join_node_to_tree(&(*tree)->right, word, freq);
	}
}

status_code get_word_and_freq(const char* string, char** result, int* freq) {
	if (string == NULL) return EMPTY;
	if (*result != NULL) {
		return NOT_EMPTY;
	}
	*result = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (*result == NULL) return MEMORY_ALLOCATION_ERROR;
	int current_length = 0;
	int total_length = START_LENGTH_STRING;
	int index = 0;
	while (string[index] != ' ') {
		if (current_length + 1 >= total_length) {
			total_length *= 2;
			char* temp = (char*)realloc(*result, sizeof(char) * total_length);
			if (temp == NULL) {
				free(*result);
				return MEMORY_ALLOCATION_ERROR;
			}
			*result = temp;
		}
		(*result)[current_length++] = string[index++];
	}
	(*result)[current_length] = '\0';
	while (!IsDigit(string[index])) index++;
	while (IsDigit(string[index])) {
		*freq *= 10;
		*freq += string[index] - '0';
		index++;
	}
	return NORMAL;
}

status_code read_tree_from_file(char* file_name, search_tree** tree) {
	FILE* input = fopen(file_name, "r");
	if (input == NULL) {
		return FILE_OPEN_ERROR;
	}
	*tree = (search_tree*)malloc(sizeof(search_tree));
	if (*tree == NULL) {
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	(*tree)->head = NULL;
	int c = '0';
	char* buffer = (char*)malloc(sizeof(char) * START_BUFFER_SIZE);
	if (buffer == NULL) {
		fclose(input);
		return MEMORY_ALLOCATION_ERROR;
	}
	char* current_word = NULL;
	int current_freq = 0;
	int currentLength= 0;
	status_code status = NORMAL;
	int totalLength = START_BUFFER_SIZE;
	while ((c = getc(input)) != EOF) {
		if (c == '\n') {
			buffer[currentLength] = '\0';
			currentLength = 0;

			status = get_word_and_freq(buffer, &current_word, &current_freq);
			if (status != NORMAL) {
				free(buffer);
				fclose(input);
				destroy_search_tree(tree);
				return status;
			}
			printf("%s %d \n", current_word, current_freq);
			status = join_node_to_tree(&(*tree)->head, current_word, current_freq);
			if (status != NORMAL) {
				free(buffer);
				fclose(input);
				destroy_search_tree(tree);
				return status;
			}
			free(current_word);
			current_word = NULL;
			current_freq = 0;
		} else {
			if (currentLength + 1 > totalLength) {
				totalLength *= 2;
				char* tempBuffer = (char*)realloc(buffer, sizeof(char) * totalLength);
				if (tempBuffer == NULL) {
					free(buffer);
					fclose(input);
					return MEMORY_ALLOCATION_ERROR;
				}
				buffer = tempBuffer;
			}
			buffer[currentLength++] = (char)c;
		}
	}
	if (currentLength > 0) {
		buffer[currentLength] = '\0';
		status = get_word_and_freq(buffer, &current_word, &current_freq);
		if (status != NORMAL) {
			free(buffer);
			fclose(input);
			destroy_search_tree(tree);
			return status;
		}
		printf("%s %d \n", current_word, current_freq);
		status = join_node_to_tree(&(*tree)->head, current_word, current_freq);
		if (status != NORMAL) {
			free(buffer);
			fclose(input);
			destroy_search_tree(tree);
			return status;
		}
		free(current_word);
		current_word = NULL;
		current_freq = 0;
	}
	free(buffer);
	fclose(input);

	return NORMAL;
}

int compare_by_freq(const void* first, const void* second) {
	tree_node* first_node = *(tree_node**)first;
	tree_node* second_node = *(tree_node**)second;
	return second_node->freq - first_node->freq;
}

void fill_array(tree_node* head, tree_node*** array, int* amount) {
	if (head == NULL) return;
	if (array != NULL) {
		(*array)[(*amount)] = head;
	}
	(*amount)++;
	fill_array(head->left, array, amount);
	fill_array(head->right, array, amount);
}

status_code print_freq_words(search_tree* tree, int amount) {
	if (tree == NULL || tree->head == NULL) return EMPTY;
	int count = 0;
	fill_array(tree->head, NULL, &count);
	tree_node** array = (tree_node**)malloc(sizeof(tree_node*) * count);
	if (array == NULL) return MEMORY_ALLOCATION_ERROR;
	count = 0;
	fill_array(tree->head, &array, &count);
	qsort(array, count, sizeof(tree_node*), compare_by_freq);
	for (int i = 0; i < amount && i < count; i++) {
		printf("%s %d\n", array[i]->word, array[i]->freq);
	}
	free(array);
	return NORMAL;
}