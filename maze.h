#ifndef __MAZE_H
#define __MAZE_H

#include "heap.h"
#include "terminalFunctions.h"
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

#define MAZE_WALL '*'
#define MAZE_PATH ' '
#define MAZE_SIDE_MAX 1000
#define MAZE_TOP 0
#define MAZE_LEFT 1
#define MAZE_RIGHT 2
#define MAZE_BOTTOM 3
#define AVG(a,b) ( (a+b) / 2.0)


double distance(const int x1, const int y1, const int x2, const int y2){
    return (sqrt(((x2-x1) * (x2-x1)) + ((y2-y1) * (y2-y1))));
}


struct Maze {
    char *matrix;
    short int rowSize;
    short int colSize;
    unsigned int startRow, startCol,
                 endRow, endCol;
    unsigned int currentPosRow,
                 currentPosCol;
};


/**
 * @return  1  if cell is a path
 * @return  0  if cell is a wall
 * @return -1  if cell is out of bound horizontally
 * @return -2  if cell is out of bound vertically
 * @return -3  if cell is corner of map
 */
int getCellType(const struct Maze * const maze, int row, int col) {
    if ( (row < 0) || (row >= maze->rowSize) ) {
        if ( (col < 0) || (col >= maze->colSize) ) 
            return -3;
        return -1;
    }
    if ( (col < 0) || (col >= maze->colSize) ) {
        if ( (row < 0) || (row >= maze->rowSize) )
            return -3;
        return -2;
    }
    if ( (maze->matrix[(row * maze->colSize)+col] == MAZE_WALL) )
        return 0;
    return 1;
}


char getWallChar(const struct Maze * const maze,
               int row, int col) {
    int i, count = 0;
    int sides[] = {0,0,0,0};
    int adjacentRow[] = {row-1,row,row,row+1},
        adjacentCol[] = {col,col-1,col+1,col};
    for(i = 0; i < 4; i++) {
        if (getCellType(maze, adjacentRow[i], adjacentCol[i]) == 0) {
            sides[i] = 1;
            if (++count > 2) {
                return '+';
            }
        }
    }
    if (sides[0] || sides[3]) {
        if (sides[1] || sides[2])
            return '+';
        return '|';
    }
    return '-';
}

void printMazeCenterOnCurrentPos(const struct Maze * const maze,
								 int length, int width) {
	int i, j, row = maze->currentPosRow, col = maze->currentPosCol;
    printf("Maze Size: %d x %d           \n", maze->rowSize, maze->colSize);
    printf("Current Position: %d, %d           \n", maze->currentPosRow, maze->currentPosCol);
	for (i = (row - length); i < (row + length); i++) {
		for (j = (col - width); j < (col + width); j++) {
			if ( i == maze->currentPosRow && j == maze->currentPosCol) {
                putchar('@');
            }
            else if ( i == maze->startRow && j == maze->startCol) {
                putchar('S');
            }
            else if ( i == maze->endRow && j == maze->endCol ) {
                putchar('E');
            }
            else {
                switch(getCellType(maze, i ,j)) {
                    case 0: // normal wall
                        putchar(getWallChar(maze, i, j)); break;
                    case -1:
                        putchar('-'); break;
                    case -2:
                        putchar('|'); break;
                    case -3:
                        putchar('+'); break;
                    default:
                        putchar(MAZE_PATH); break;
                }
            }
		}
        putchar('\n');
	}
}

/**
 * Prints the maze
 * @param maze the maze to be printed
 */
void printMaze(const struct Maze * const maze) {
    int i, j;
    printf("Maze Size: %d x %d           \n", maze->rowSize, maze->colSize);
    printf("Current Position: %d, %d           \n", maze->currentPosRow, maze->currentPosCol);
	for (i = -1; i <= maze->rowSize; i++) {
        for (j = -1; j <= maze->colSize; j++) {
            if ( i == maze->currentPosRow && j == maze->currentPosCol) {
                putchar('@');
            }
            else if ( i == maze->startRow && j == maze->startCol) {
                putchar('S');
            }
            else if ( i == maze->endRow && j == maze->endCol ) {
                putchar('E');
            }
            else {
                switch(getCellType(maze, i ,j)) {
                    case 0: // normal wall
                        putchar(getWallChar(maze, i, j)); break;
                    case -1:
                        putchar('-'); break;
                    case -2:
                        putchar('|'); break;
                    case -3:
                        putchar('+'); break;
                    default:
                        putchar(MAZE_PATH); break;
                }
            }
        }
        putchar('\n');
    }
}


/**
 * Set the maze to empty
 * 
 */
void deleteMaze(struct Maze * const maze) {
    if (maze->matrix != NULL) {
        free(maze->matrix);
        maze->matrix = NULL;
        maze->rowSize = 0;
        maze->colSize = 0;
    }
}


void initMaze(struct Maze * const maze,
              const unsigned int rowCount,
              const unsigned int colCount) {
    
    if (rowCount <= 2 || colCount <= 2) {
        printf("Dimension is too small! Max is %d x %d\n",MAZE_SIDE_MAX, MAZE_SIDE_MAX);
        return;    
    }
    
    if (rowCount > MAZE_SIDE_MAX || colCount > MAZE_SIDE_MAX) {
        printf("Dimension is too large! Max is %d x %d\n",MAZE_SIDE_MAX, MAZE_SIDE_MAX);
        return;
    }
    
    deleteMaze(maze);
    
    int i, size;
    char *matrix;
    deleteMaze(maze);
    //keeping the dimensions odd
    maze->rowSize = (rowCount % 2) ? rowCount : rowCount + 1;
    maze->colSize = (colCount % 2) ? colCount : colCount + 1;

    size = maze->rowSize * maze->colSize;

    maze->matrix = calloc(sizeof(char), size);
    assert(maze->matrix != NULL);
    matrix = maze->matrix;

    for(i = 0; i < size; i++)
        matrix[i] = MAZE_WALL;	
}


/**
 * Add the given cell to the maze.
 * For every adjancet wall of the cell, if the cell in that direction is not part of the maze,
 * add the wall to the heap
 */
void addCellToMaze(struct Maze * const maze,
                       struct BinaryHeap * const heap,
                       int row,
                       int col) {
    maze->matrix[(row * maze->colSize) + col] = MAZE_PATH;
    
    // add all valid walls of cell into heap
    int i;
    int
        targetCellRow[] = {row-2, row, row, row+2},
        targetCellCol[] = {col, col-2, col+2, col};
    short int targetWallRow[] = {row-1, row, row, row+1},
              targetWallCol[] = {col, col-1, col+1, col};
    static const char
        dRow[] = {-1, 0, 0, 1},
        dCol[] = {0, -1, 1, 0};
    
    for (i = 0; i < 4; i++) {
        if (getCellType(maze, targetCellRow[i], targetCellCol[i]) == 0) {
            struct HeapNode nextWall = {dRow[i], dCol[i], targetWallRow[i], targetWallCol[i], 
                                        (unsigned char)(rand() % UCHAR_MAX)};
            insertHeap(heap, &nextWall);
        }
    }
}

void getRandomEdgePoint(const struct Maze * const maze, int *outputRow, int *outputCol) {
    switch( rand() % 4 ) {
        case MAZE_TOP:
            *outputRow = 0;
            *outputCol = (rand() % (maze->colSize / 2)) * 2;
            break;
        case MAZE_LEFT:
            *outputRow = (rand() % (maze->rowSize / 2)) * 2;
            *outputCol = 0;
            break;
        case MAZE_RIGHT:
            *outputRow = (rand() % (maze->rowSize / 2)) * 2;
            *outputCol = maze->colSize - 1;
            break;
        case MAZE_BOTTOM:
            *outputRow = maze->rowSize - 1;
            *outputCol = (rand() % (maze->colSize / 2)) * 2;
            break;
    }
}

void generateMaze(struct Maze * const maze) {
    
    srand(time(NULL));
    
    if ( (maze->matrix == NULL) || 
         (maze->rowSize == 0) || 
         (maze->colSize == 0) ) {
        return;
    }
    
    int size = maze->rowSize * maze->colSize;
    
    struct BinaryHeap heap = {0,0,NULL};
    initHeap(&heap,(double)size / 0.3 + 1);
    
    /**
     * Randomly picks an entrance on the edge
     */
    getRandomEdgePoint(maze, &maze->startRow, &maze->startCol);
    addCellToMaze(maze, &heap, maze->startRow, maze->startCol);
    
    //clearScreen();
    //struct timespec delaytime = {0,10000000}; // used to sleep for 500ms
    while(heap.size > 0) {
        //setPosition(0,0);
        //printMaze(maze);
        //nanosleep(&delaytime, NULL);
        struct HeapNode wall;
        popMinHeap(&heap, &wall);

        // choose a random valid path in four direction, or until all four posibility is exhausted
        // always travel two distances in a chosen direction,
        // thus there will always be a wall between path
        int targetRow = wall.row + wall.dRow,
            targetCol = wall.col + wall.dCol;
        struct HeapNode nextCell;
        /* if the cell in the target direction is not part of the maze,
         * make the current wall a path, and add the cell in the target direction to the maze
         */
        if (getCellType(maze, targetRow, targetCol) == 0) {
            maze->matrix[(wall.row * maze->colSize) + wall.col] = MAZE_PATH;
            addCellToMaze(maze, &heap, targetRow, targetCol);
        }
    }
    deleteHeap(&heap);
    
    // pick random end point
    double minDistance = (AVG(maze->rowSize, maze->colSize)) / 2.0;
    const int startRow = maze->startRow, startCol = maze->startCol;
    while(1) {
        int targetRow, targetCol;
        getRandomEdgePoint(maze, &targetRow, &targetCol);
        if ( (getCellType(maze, targetRow, targetCol) == 1) ) { 
            double distanceFromStart = distance(startRow, startCol, targetRow, targetCol);
            if(distanceFromStart > minDistance) {
                maze->endRow = targetRow;
                maze->endCol = targetCol;
                break;
            }
        }
        printf("%4d x %4d: BAD END POINT\r",maze->rowSize, maze->colSize);
        fflush(stdout);
    }
    
    maze->currentPosRow = maze->startRow;
    maze->currentPosCol = maze->startCol;
}

int moveUp(struct Maze * const maze) {
    if (getCellType(maze, maze->currentPosRow - 1, maze->currentPosCol) == 1) {
        maze->currentPosRow--;
    	return 1;
	}
	return 0;
}

int moveLeft(struct Maze * const maze) {
    if (getCellType(maze, maze->currentPosRow, maze->currentPosCol - 1) == 1) {
        maze->currentPosCol--;
		return 1;
    }
	return 0;
}

int moveRight(struct Maze * const maze) {
    if (getCellType(maze, maze->currentPosRow, maze->currentPosCol+1) == 1) {
        maze->currentPosCol++;
		return 1;
    }
	return 0;
}

int moveDown(struct Maze * const maze) {
    if (getCellType(maze, maze->currentPosRow+1, maze->currentPosCol) == 1) {
        maze->currentPosRow++;
		return 1;
    }
	return 0;
}

int atExit(const struct Maze * const maze) {
    if (maze->currentPosRow == maze->endRow &&
        maze->currentPosCol == maze->endCol)
        return 1;
    return 0;
}

#endif