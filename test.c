#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "heap.h"
#include "maze.h"

#define WALL 'W'
#define PATH ' '

int main(void) {
    int i, j;
    int row = 5, col = 5, size = row * col;
    time_t seed;
    srand((unsigned) time(&seed));
    
    struct MazeCell *maze = calloc(sizeof(struct MazeCell), size);
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            struct MazeCell *cell = &maze[(i*col)+j];
            cell->type = WALL;
            cell->x = i;
            cell->y = j;
        }
    }
        
    struct BinaryHeap heap;
    initHeap(&heap,size);
    for(i = 0; i < size; i++) {
        struct HeapNode newNode = {&maze[i], rand() % 20};
        insertHeap(&heap, &newNode);
        printHeap(&heap);
        printf("\n");
    }
    cleanHeap(&heap);
    
    return 0;
}

