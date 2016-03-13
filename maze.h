#ifndef __MAZE_H
#define __MAZE_H


#include "heap.h"

#define MAZE_WALL 219
#define MAZE_PATH ' '
#define MAZE_WIDTH_MAX 1000
#define MAZE_LENGTH_MAX 1000
#define MAZE_ENTRANCE_TOP 0
#define MAZE_ENTRANCE_LEFT 1
#define MAZE_ENTRANCE_RIGHT 2
#define MAZE_ENTRANCE_BOTTOM 3


struct Maze {
	char *matrix;
	unsigned int rowSize;
	unsigned int colSize;
};


void deleteMaze(struct Maze * const maze) {
	free(maze->matrix);
	maze->matrix = NULL;
	maze->rowSize = 0;
	maze->colSize = 0;
}


void initMaze(struct Maze * const maze,
			  const unsigned int rowCount,
			  const unsigned int colCount) {
	
	int i, size;
	char *matrix;
	deleteMaze(maze);
	//keeping the dimensions odd
	maze->rowSize = (rowCount % 2) ? rowCount + 1 : rowCount;
	maze->colSize = (colCount % 2) ? colCount + 1 : colCount;

	size = maze->rowSize * maze->colSize;

	maze->matrix = calloc(sizeof(char), size);
	matrix = maze->matrix;

	for(i = 0; i < size; i++)
		matrix[i] = MAZE_WALL;
	
}


void addCellWallToHeap(struct Maze * const maze,
					   struct BinaryHeap * const heap,
					   int row,
					   int col) {
    // make cell a path
	// add all adjacent walls to heap
}


void generateMaze(struct Maze * const maze) {
	
	if ( (maze->matrix == NULL) || 
		 (maze->rowSize == 0) || 
		 (maze->colSize == 0) ) {
		return;
	}
	
	int size = maze->rowSize * maze->colSize;
	
	struct BinaryHeap heap;
    initHeap(&heap,size);
	
	switch( rand() % 4 ) {
		case MAZE_ENTRANCE_TOP:
			addCellWallToHeap(maze, &heap, 0, rand() % maze->colSize);
			break;
		case MAZE_ENTRANCE_LEFT:
			addCellWallToHeap(maze, &heap, rand() % maze->rowSize, 0);
			break;
		case MAZE_ENTRANCE_RIGHT:
			addCellWallToHeap(maze, &heap, rand() % maze->rowSize, maze->colSize -1);
			break;
		case MAZE_ENTRANCE_BOTTOM:
			addCellWallToHeap(maze, &heap, maze->rowSize - 1, rand() % maze->colSize);
			break;
	}
	
	deleteHeap(&heap);
}



#endif