#ifndef __BINARY_HEAP_H
#define __BINARY_HEAP_H


#include <stdlib.h>
#include <stdio.h>
#include "maze.h"


struct HeapNode {
    struct MazeCell *cell;
    int weight;
};

struct BinaryHeap {
    int capacity; // memory size
    int size; // elements count
    struct HeapNode *heapArray;
};


void initHeap(struct BinaryHeap * const heap, size_t size) {
    heap->heapArray = calloc(sizeof(struct HeapNode),  size + 1);
    heap->size = 0;
    heap->capacity = size + 1;
}

void cleanHeap(struct BinaryHeap *heap) {
    free(heap->heapArray);
}

/**
 * Using mark Allen Weiss's implementation of binary heap in c++
 */
int insertHeap(struct BinaryHeap *heap, const struct HeapNode * const node) {
    if ( heap->size == (heap->capacity - 1) )
        return -1;
    struct HeapNode *array = heap->heapArray;
    
    // Percolate up
    int hole = ++(heap->size);
    for (; hole > 1 && node->weight < array[ hole / 2].weight; hole /= 2)
        array[ hole ] = array[ hole / 2 ];
    array[ hole ] = *node;
}

void percolateDown(struct BinaryHeap *heap, int hole) {
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

int getMinHeap(struct BinaryHeap *heap, struct HeapNode *output) {
    if (heap->size == 0)
        return -1;
    struct HeapNode *array = heap->heapArray;
    *output = array[1];
    array[1] = array[heap->size--];
    percolateDown(heap,1);
    return 0;
}

void printHeap(struct BinaryHeap *heap) {
    int i;
    struct HeapNode *array = heap->heapArray;
    for (i = 1; i <= heap->size; i++) {
        struct MazeCell *cell = array[i].cell;
        printf("(%d,%d)[%d]\n",cell->x, cell->y, array[i].weight);
    }
}

/**
 * Using mark Allen Weiss's implementation of binary heap in c++
 */

#endif