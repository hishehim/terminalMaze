#ifndef __BINARY_HEAP_H
#define __BINARY_HEAP_H


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct HeapNode {
	char dRow, dCol; 
	short int row, col;
    unsigned char weight;
};

struct BinaryHeap {
    unsigned int size; // elements count
    unsigned int capacity; // memory size
    struct HeapNode *heapArray;
};


void deleteHeap(struct BinaryHeap * const heap) {
    if (heap->heapArray != NULL) {
        free(heap->heapArray);
        heap->heapArray = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

void initHeap(struct BinaryHeap * const heap, const unsigned int size) {
    deleteHeap(heap);
    heap->heapArray = calloc(sizeof(struct HeapNode),  size + 1);
	assert(heap->heapArray != NULL);
    heap->size = 0;
    heap->capacity = size + 1;
}


/**
 * Using mark Allen Weiss's implementation of binary heap in c++
 */
int insertHeap(struct BinaryHeap * const heap, const struct HeapNode * const node) {
    if ( heap->size == (heap->capacity - 1) )
        return -1;
    struct HeapNode *array = heap->heapArray;
    
    // Percolate up
    int hole = ++(heap->size);
    for (; hole > 1 && node->weight < array[ hole / 2].weight; hole /= 2)
        array[ hole ] = array[ hole / 2 ];
    array[ hole ] = *node;
}

void percolateDown(struct BinaryHeap * const heap, int hole) {
    int child;
    struct HeapNode *array = heap->heapArray; 
    struct HeapNode tmp = array[hole];
    
    for(; hole * 2 <= heap->size; hole = child) {
        child = hole * 2;
        if (child != heap->size && array[child+1].weight < array[child].weight)
            child++;
        if (array[child].weight < tmp.weight)
            array[hole] = array[child];
        else
            break;
    }
    array[hole] = tmp;
}

int popMinHeap(struct BinaryHeap * const heap, struct HeapNode * const output) {
    if (heap->size == 0)
        return -1;
    struct HeapNode *array = heap->heapArray;
    *output = array[1];
    array[1] = array[heap->size--];
    percolateDown(heap,1);
    return 0;
}
/**
 * Using mark Allen Weiss's implementation of binary heap in c++
 */
#endif