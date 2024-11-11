#include <stdio.h>
#include <stdlib.h>

#include "ex1lab4library.h"

int main(int argc, char** argv) {
	//	char* fileName;
	//	statusCode status = GetFileName(argc, argv, &fileName);
	//	if (status == WRONG_ARGUMENTS) {
	//		printf("Wrong number of arguments. Must be one - file name.\n");
	//		return 1;
	//	}
	struct hashMap* map = (hashMap*)malloc(sizeof(hashMap));
	if (map == NULL) {
		printf("Memory allocation error.\n");
		return 1;
	}
	map->head = NULL;
	CreateHashMap(map);
	statusCode status = InsertHashMap(map, "m", "kjslfj");
	InsertHashMap(map, "p", "jlsjfs");
	InsertHashMap(map, "d", "slkdjf");
	printf("%s\n", SearchHashMap(map, "p"));
	printf("%s\n", SearchHashMap(map, "d"));
	printf("%s\n", SearchHashMap(map, "m"));
	FreeHashMap(&map);
	return 0;
}