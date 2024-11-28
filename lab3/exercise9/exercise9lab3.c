#include "exercise9lab3library.h"
#include <stdio.h>

int main() {
//	search_tree* tree = NULL;
//	status_code res = file_reading("inputEX9.txt", " 4 5", &tree);
//	if (res != NORMAL) {
//		return 1;
//	}
//	int result = tree_depth(tree->head);
//	printf("%d ", result);
//	int min_freq = 1000;
//	char* min = NULL;
//	get_min_word(tree->head, &min_freq, &min);
//	printf("%s %d", min, min_freq);
//	res = load_tree_in_file("output.txt", tree);
//	if (res != NORMAL) printf("error");
//	destroy_search_tree(&tree);
	search_tree* current = NULL;
	status_code res = read_tree_from_file("inputEX9.txt", &current);
	if (res != NORMAL) {
		return 1;
	}
	int k = find_search_tree(current, "jaja");
	printf("K = %d \n", k);
	res = print_freq_words(current, 4);
	if (res != NORMAL) printf("error");
	destroy_search_tree(&current);
	return 0;
}