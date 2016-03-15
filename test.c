#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "terminalFunctions.h"
#include "maze.h"

void showWinScreen() {
	printf("YOU'VE WON!\n");
}

void test(int rowSize, int colSize) {
	printf("%4d x %4d\r", rowSize, colSize);
	struct Maze maze = {NULL, 0, 0};
	initMaze(&maze,rowSize, colSize);
	generateMaze(&maze);
	clearScreen();
	while (1) {
		setPosition(0,0);
		printMaze(&maze);
		printf("Press q to quit");
		fflush(stdout);
		if(atExit(&maze)) {
			showWinScreen();
			break;
		}
		int key;
		set_mode(1);
		while (!(key = get_key())) {
			fflush(stdin);
			usleep(10000);
		}
		if (key == 'q')
			break;
		else if (key == 'w' || key == 'W' || key == 400)
			moveUp(&maze);
		else if (key == 's' || key == 'S' || key == 401)
			moveDown(&maze);
		else if (key == 'd' || key == 'D' ||  key == 402)
			moveRight(&maze);
		else if (key == 'a' || key == 'A' || key == 403)
			moveLeft(&maze);
	}
	deleteMaze(&maze);
}

int main(void) {
	clearScreen();
	int row = 13,
		col = 20,
		diff = 1;
	for (diff = 1; diff < 5; diff++) {
		test(row * diff, col * diff);
	}
	/*
	for (row = 3; row < 200; row += 2) {
		for (col = 3; col < 200; col += 2) {
			test(row,col);
		}
	}*/
	return 0;
}