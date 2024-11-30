#ifndef EXERCISE9LAB3LIBRARY_H
#define EXERCISE9LAB3LIBRARY_H
#include <stdio.h>

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
	INVALID_BORDER,
	INCORRECT_FILE_NAME,
	INCORRECT_STRING,
	INCORRECT_FILE
} status_code;

typedef struct search_tree{
	tree_node* head;
} search_tree;

typedef enum constants {
	START_BUFFER_SIZE = 20,
	START_LENGTH_STRING = 10
} constants;

typedef enum type_operation {
	TYPE_UNKNOWN,
	TYPE_END,
	TYPE_FIND,
	TYPE_MOST_FREQUENT,
	TYPE_FIND_MIN,
	TYPE_FIND_MAX,
	TYPE_GET_DEPTH,
	TYPE_LOAD_TO_FILE,
	TYPE_RELOAD_FROM_FILE
} type_operation;

status_code get_arguments(int argc, char** argv, char** filename, char** separators);
status_code get_string_console(char** string);
int is_separator(int current_char, char* separators);
status_code file_reading(const char* file_name, char* separators, search_tree** tree);

status_code create_tree_node(tree_node** current, char* word);
void delete_tree_node(tree_node** current);
status_code create_search_tree(search_tree** tree);
status_code add_search_tree(search_tree** tree, char* word);
int find_search_tree(search_tree* tree, char* word);
void destroy_search_nodes(tree_node* tree);
void destroy_search_tree(search_tree ** tree);


int tree_depth(tree_node* tree);
void amount_elements_search_tree(tree_node* tree, int* amount);
void get_max_word(tree_node* tree, int* max_freq, char** max_word);
void get_min_word(tree_node* tree, int* min_freq, char** min_word);
status_code load_tree_in_file(char* file_name, search_tree* tree);
status_code read_tree_from_file(char* file_name, search_tree** tree);
status_code print_freq_words(search_tree* tree, int amount);
void file_load_recursive(FILE* input, tree_node* current);
status_code recover_tree_node(tree_node** tree, const char* word, int freq);
status_code join_node_to_tree(tree_node** tree, const char* word, int freq);
status_code get_word_and_freq(const char* string, char** result, int* freq);
void fill_array(tree_node* head, tree_node*** array, int* amount);
status_code print_freq_words(search_tree* tree, int amount);
status_code interactive_dialogue(search_tree** tree, int amount);
status_code is_correct_file(const char* file);
int is_correct_file_name(const char* file_name) ;
#endif