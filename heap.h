#ifndef __BINARY_HEAP_H
#define __BINARY_HEAP_H

#include <stdlib.h>

struct MazeCell {
    char type;
    int x, y;
};

int compareCell(struct *cellA, struct *cellB) {
    
}

void createCell(struct MazeCell *cell,
                char cellType,
                int x,
                int y) {
    cell->type = cellType;
    cell->x = x;
    cell->y = y;
}

struct BinaryHeap {
    unsigned int capacity; // memory size
    unsigned int size; // elements count
    HeapNode *heapArray;
};


void initHeap(struct BinaryHeap *heap, size_t size) {
    heap->heapArray = malloc((sizeof(char) * size) + 1);
    heap->size = 0;
    heap->capacity = size + 1;
}

void cleanHeap(struct BinaryHeap *heap) {
    free(heap->heapArray);
}



#endif