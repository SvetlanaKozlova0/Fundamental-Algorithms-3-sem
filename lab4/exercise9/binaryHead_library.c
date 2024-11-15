#include "ex9library.h"
#include <stdlib.h>
#include <stdio.h>

statusCode CreateBinaryHeap(BinaryHeap* heap) {
	heap->capacity = START_CAPACITY;
	heap->amount = 0;
	heap->array = (int*)malloc(sizeof(int) * START_CAPACITY);
	if (heap->array == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	return NORMAL;
}

statusCode InsertBinaryHeap(BinaryHeap* heap, int element) {
	if (heap->amount == 0) {
		heap->array[0] = element;
		heap->amount++;
		return NORMAL;
	}
	if (heap->amount + 1 > heap->capacity) {
		heap->capacity *= 2;
		int* tempHeap = (int*)realloc(heap->array, sizeof(int) * heap->capacity);
		if (tempHeap == NULL) {
			free(heap->array);
			return MEMORY_ALLOCATION_ERROR;
		}
		heap->array = tempHeap;
	}
	heap->array[heap->amount] = element;
	int index = heap->amount;
	heap->amount++;
	int parent = (index - 1) / 2;
	while (parent >= 0) {
		if (heap->array[parent] > heap->array[index]) {
			int temp = heap->array[parent];
			heap->array[parent] = heap->array[index];
			heap->array[index] = temp;
		} else {
			return NORMAL;
		}
		index = (index - 1) / 2;
		if (index == 0) {
			return NORMAL;
		}
		parent = (index - 1) / 2;
	}
	return NORMAL;
}

void RemoveBinaryHeap(BinaryHeap* heap) {
	if (heap->amount == 0) {
		return;
	}
	if (heap->amount == 1) {
		heap->amount = 0;
		return;
	}
	int temp = heap->array[0];
	heap->array[0] = heap->array[heap->amount - 1];
	heap->array[heap->amount - 1] = temp;
	heap->amount--;
	int index = 0;
	int firstChild = 2 * index + 1;
	int secondChild = 2 * index + 2;
	while (secondChild < heap->amount - 1) {
		if (heap->array[index] < heap->array[firstChild] && heap->array[index] < heap->array[secondChild]) return;
		if (heap->array[firstChild] < heap->array[secondChild]) {
			temp = heap->array[firstChild];
			heap->array[firstChild] = heap->array[index];
			heap->array[index] = temp;
			index = firstChild;
		} else {
			temp = heap->array[secondChild];
			heap->array[secondChild] = heap->array[index];
			heap->array[index] = temp;
			index = secondChild;
		}
		firstChild = 2 * index + 1;
		secondChild = 2 * index + 2;
	}
}

statusCode getPriorityBinaryHeap(BinaryHeap* heap, int* priority) {
	if (heap->amount == 0) {
		return EMPTY_HEAP;
	}
	*priority = heap->array[0];
	return NORMAL;
}