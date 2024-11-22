#include <stdio.h>

typedef struct node_trie {
	char* variable;
	char is_terminate;
	struct node_trie* siblings;
	int degree;
} node_trie;


typedef struct monom {
	double coefficient;
	node_trie* trie_head;
} monom;

typedef struct node_polynomial {
	monom* monom_value;
	monom* next;
	monom* previous;
} node_polynomial;

typedef struct polynom {
	node_polynomial* head_polynomial;
	int length;
} polynomial;

typedef enum status_code {
	 NORMAL,
	MEMORY_ALLOCATION_ERROR,
	NOT_EMPTY
} status_code;

typedef enum constants {
	START_LENGTH_VARIABLE = 5;
} constants;