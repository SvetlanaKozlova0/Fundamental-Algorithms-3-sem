#ifndef EX9LIBRARY_H
#define EX9LIBRARY_H

typedef struct BinaryHeap {
	int* array;
	int capacity;
	int amount;
} BinaryHeap;


typedef enum statusCode {
	NORMAL,
	MEMORY_ALLOCATION_ERROR,
	EMPTY_HEAP
} statusCode;

typedef enum constants {
	START_CAPACITY = 100
} constants;

statusCode CreateBinaryHeap(BinaryHeap* heap);
statusCode InsertBinaryHeap(BinaryHeap* heap, int element);
void RemoveBinaryHeap(BinaryHeap* heap);
statusCode getPriorityBinaryHeap(BinaryHeap* heap, int* priority);
#endif