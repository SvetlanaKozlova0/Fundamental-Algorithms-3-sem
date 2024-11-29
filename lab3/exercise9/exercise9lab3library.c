#include "exercise9lab3library.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../stringLibrary.h"

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
		} else
			current_node = &(*current_node)->right;
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
		if (result_comparing == -1) {
			current = current->left;
		}
		if (result_comparing == 1) {
			current = current->right;
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
	*separators = (char*)malloc(sizeof(char) * (argc));
	if (*separators == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	int index = 0;
	for (int i = 2; i < argc; i++) {
		if (StringLength(argv[i]) != 1) {
			free(*separators);
			return INCORRECT_SEPARATORS;
		}
		(*separators)[index++] = argv[i][0];
	}
	(*separators)[argc - 2] = '\0';
	return NORMAL;
}

int is_separator(int current_char, char* separators) {
	for (int i = 0; i < StringLength(separators); i++) {
		if (separators[i] == (char)current_char) return 1;
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
		if (is_separator(c, separators) || c == ' ' || c == '\t' || c == '\n' || c == '\r') {
			buffer[currentLength] = '\0';
			if (StringLength(buffer) != 0) {
				status = add_search_tree(tree, buffer);
				if (status != NORMAL) {
					fclose(input);
					destroy_search_tree(tree);
					return status;
				}
			}
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
		if (StringLength(buffer) != 0) {
			status = add_search_tree(tree, buffer);
			if (status != NORMAL) {
				fclose(input);
				destroy_search_tree(tree);
				return status;
			}
		}
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
	if (*tree != NULL) return NOT_EMPTY;
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
	int currentLength = 0;
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

status_code get_string_console(char** string) {
	*string = (char*)malloc(sizeof(char) * START_LENGTH_STRING);
	if (*string == NULL) return MEMORY_ALLOCATION_ERROR;
	int c = getc(stdin);
	int total_length = START_LENGTH_STRING;
	int current_length = 0;
	while (c == ' ' || c == '\t') c = getc(stdin);
	while (c != '\n') {
		if (current_length + 1 >= total_length) {
			total_length *= 2;
			char* temp = (char*)realloc(*string, sizeof(char) * total_length);
			if (temp == NULL) {
				free(*string);
				return MEMORY_ALLOCATION_ERROR;
			}
			*string = temp;
		}
		(*string)[current_length++] = (char)c;
		c = getc(stdin);
	}
	(*string)[current_length] = '\0';
	return NORMAL;
}

type_operation recognize_operation(char* string) {
	if (string == NULL) return TYPE_UNKNOWN;

	if (StringLength(string) == 4 && CompareCharsOpt(string, "find") == 0) return TYPE_FIND;
	if (StringLength(string) == 4 && CompareChars(string, "most") == 0) return TYPE_FIND_MAX;
	if (StringLength(string) == 5 && CompareChars(string, "least") == 0) return TYPE_FIND_MIN;
	if (StringLength(string) == 5 && CompareChars(string, "depth") == 0) return TYPE_GET_DEPTH;
	if (StringLength(string) == 4 && CompareChars(string, "load") == 0) return TYPE_LOAD_TO_FILE;
	if (StringLength(string) == 7 && CompareChars(string, "recover") == 0) return TYPE_RELOAD_FROM_FILE;
	if (StringLength(string) == 5 && CompareChars(string, "check") == 0) return TYPE_MOST_FREQUENT;
	if (StringLength(string) == 3 && CompareChars(string, "end") == 0) return TYPE_END;
	return TYPE_UNKNOWN;
}

void get_menu() {
	printf("----------------MENU----------------\n");
	printf("find frequency of word          find\n");
	printf("find most frequent              most\n");
	printf("find least frequent            least\n");
	printf("get depth                      depth\n");
	printf("load tree to file               load\n");
	printf("recover tree from file       recover\n");
	printf("Watch N most frequents words   check\n");
	printf("end                              end\n");
	printf("------------------------------------\n");
}


status_code interactive_dialogue(search_tree** tree, int count) {
	type_operation current_operation = TYPE_UNKNOWN;
	char* string = NULL;
	status_code status = NORMAL;
	int freq = 0;
	get_menu();
	while (current_operation != TYPE_END) {
		status = get_string_console(&string);
		if (status != NORMAL) {
			return status;
		}
		current_operation = recognize_operation(string);
		switch (current_operation) {
			case TYPE_END:
				free(string);
				return NORMAL;
			case TYPE_UNKNOWN:
				get_menu();
				free(string);
				break;
			case TYPE_GET_DEPTH:
				printf("------------Operation DEPTH---------\n");
				printf("Determine the length of the search tree.\n");
				if (tree == NULL || *tree == NULL) {
					printf("Now search tree is empty (you have uploaded it to a file or\n"
					    "the file in which the words are searched is empty.\n");
					printf("Please download the tree back or start the program again.\n");
					printf("------------------------------------\n");
					free(string);
					break;
				}
				printf("Depth of search tree: %d\n", tree_depth((*tree)->head));
				printf("------------------------------------\n");
				free(string);
				break;
			case TYPE_FIND:
				printf("------------Operation FIND----------\n");
				printf("Find out how many times the search word was found in the file.\n");
				if (tree == NULL || *tree == NULL) {
					printf("Now search tree is empty (you have uploaded it to a file or\n"
					    "the file in which the words are searched is empty.\n");
					printf("Please download the tree back or start the program again.\n");
					printf("------------------------------------\n");
					free(string);
					break;
				}
				printf("Please, enter the word: ");
				free(string);
				status = get_string_console(&string);
				if (status != NORMAL) return status;
				freq = find_search_tree(*tree, string);
				printf("Your word: %s\n", string);
				if (freq <= 0)
					printf("File doesn't contain this word. Frequency = 0.\n");
				else
					printf("Frequency = %d\n", freq);
				printf("------------------------------------\n");
				free(string);
				break;
			case TYPE_FIND_MIN:
				printf("----------Operation LEAST-----------\n");
				printf("Find the word that is least common in the file.\n");
				if (tree == NULL || *tree == NULL) {
					printf("Now search tree is empty (you have uploaded it to a file or\n"
					    "the file in which the words are searched is empty.\n");
					printf("Please download the tree back or start the program again.\n");
					printf("------------------------------------\n");
					free(string);
					break;
				}
				free(string);
				freq = count + 1;
				char* current = NULL;
				get_min_word((*tree)->head, &freq, &current);
				if (string != NULL) {
					printf("Word: %s\n", current);
					printf("Frequency: %d\n", freq);
				} else
					printf("Not found.\n");
				printf("------------------------------------\n");
				break;
			case TYPE_FIND_MAX:
				printf("-----------Operation MOST-----------\n");
				printf("Finds the word that is most often found in the file.\n");
				if (tree == NULL || *tree == NULL) {
					printf("Now search tree is empty (you have uploaded it to a file or\n"
					    "the file in which the words are searched is empty.\n");
					printf("Please download the tree back or start the program again.\n");
					printf("------------------------------------\n");
					free(string);
					break;
				}
				free(string);
				freq = 0;
				char* current2 = NULL;
				get_max_word((*tree)->head, &freq, &current2);
				if (freq != 0) {
					printf("Word: %s\n", current2);
					free(string);
					string = NULL;
					printf("Frequency: %d\n", freq);
				} else
					printf("Not found.\n");
				printf("------------------------------------\n");
				break;
			case TYPE_MOST_FREQUENT:
				printf("----------Operation CHECK-----------\n");
				printf("Finds the N most common words in the file.\n");
				if (tree == NULL || *tree == NULL) {
					printf("Now search tree is empty (you have uploaded it to a file or\n"
					    "the file in which the words are searched is empty.\n");
					printf("Please download the tree back or start the program again.\n");
					printf("------------------------------------\n");
					free(string);
					break;
				}
				printf("Please, enter the amount of words you want to find: ");
				free(string);
				status = get_string_console(&string);
				if (status != NORMAL) return status;
				int flag = 1;
				for (int i = 0; i < StringLength(string); i++) {
					if (!IsDigit(string[i])) {
						flag = 0;
						break;
					}
				}
				if (flag != 1) {
					free(string);
					printf("Incorrect, number must contain only digits.\n");
					printf("------------------------------------\n");
					break;
				}
				int amount = 0;
				ssize_t result_scan = sscanf(string, "%d", &amount);
				if (result_scan == -1) {
					free(string);
					printf("An error while scanning number.\n");
					printf("------------------------------------\n");
					break;
				}
				free(string);
				status = print_freq_words(*tree, amount);
				if (status != NORMAL) return status;
				printf("------------------------------------\n");
				break;
			case TYPE_LOAD_TO_FILE:
				printf("----------Operation LOAD------------\n");
				printf("Uploads the tree to a file.\n");
				if (tree == NULL || *tree == NULL) {
					printf("Now search tree is empty (you have uploaded it to a file or\n"
					    "the file in which the words are searched is empty.\n");
					printf("Please download the tree back or start the program again.\n");
					printf("------------------------------------\n");
					free(string);
					break;
				}
				free(string);
				printf("Please enter the name of the file you want to upload the tree to:\n");
				status = get_string_console(&string);
				if (status != NORMAL) return status;
				status = load_tree_in_file(string, *tree);
				if (status != NORMAL) {
					free(string);
					return status;
				}
				free(string);
				destroy_search_tree(tree);
				*tree = NULL;
				printf("Everything was uploaded successfully. The search tree has been deleted.\n"
				    "To restore the tree, use the recover function.\n");
				printf("------------------------------------\n");
				break;
			case TYPE_RELOAD_FROM_FILE:
				printf("--------Operation RECOVER-----------\n");
				printf("Restore a tree from a file.\n");
				if (*tree != NULL) {
					printf("The current tree is not empty.\n"
					    "This means that it has not been deleted.\n"
					    "The operation cannot be performed with a non-empty tree.\n"
					    "Please first load the tree into a file using the load function.\n");
					printf("------------------------------------\n");
					free(string);
					break;
				}
				free(string);
				printf("Please enter the name of the file you want to restore the tree from:\n");
				status = get_string_console(&string);
				if (status != NORMAL) return status;
				status = read_tree_from_file(string, tree);
				if (status != NORMAL) {
					free(string);
					return status;
				}
				free(string);
				printf("Everything was recovered successfully.\n");
				printf("------------------------------------\n");
				break;
		}
	}
}

void amount_elements_search_tree(tree_node* tree, int* amount) {
	if (tree == NULL) return;
	(*amount)++;
	amount_elements_search_tree(tree->right, amount);
	amount_elements_search_tree(tree->left, amount);
}