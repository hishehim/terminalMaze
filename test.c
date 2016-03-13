#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "maze.h"



int main(void) {
	
}

int test() {
	int rowSize = 50, colSize = 50;
	struct Maze maze = {NULL, 0, 0};
	
	initMaze(&maze,rowSize, colSize);
	generateMaze(&maze);
	deleteMaze(&maze);
}
