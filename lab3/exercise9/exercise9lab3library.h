#ifndef EXERCISE9LAB3LIBRARY_H
#define EXERCISE9LAB3LIBRARY_H

typedef struct tree_node {
	char* word;
	int freq;
	struct tree_node* left;
	struct tree_node* right;
} tree_node;

typedef enum status_code {
	NORMAL,
	MEMORY_ALLOCATION_ERROR,
	NOT_EMPTY,
	WRONG_NUMBER_ARGUMENTS,
	INCORRECT_SEPARATORS,
	FILE_OPEN_ERROR,
	EMPTY,
	INVALID_BORDER
} status_code;

typedef struct search_tree{
	tree_node* head;
} search_tree;

typedef enum constants {
	START_BUFFER_SIZE = 20,
	START_LENGTH_STRING = 10
} constants;

status_code get_arguments(int argc, char** argv, char** filename, char** separators);

status_code create_tree_node(tree_node** current, char* word);
void delete_tree_node(tree_node** current);
status_code create_search_tree(search_tree** tree);
status_code add_search_tree(search_tree** tree, char* word);
int find_search_tree(search_tree* tree, char* word);
void destroy_search_tree(search_tree ** tree);

int tree_depth(tree_node* tree);
int is_separator(int current_char, char* separators);
status_code file_reading(const char* file_name, char* separators, search_tree** tree);
void get_max_word(tree_node* tree, int* max_freq, char** max_word);
void get_min_word(tree_node* tree, int* min_freq, char** min_word);
status_code load_tree_in_file(char* file_name, search_tree* tree);
status_code read_tree_from_file(char* file_name, search_tree** tree);
status_code print_freq_words(search_tree* tree, int amount);
#endif