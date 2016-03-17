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

int startGame(const int rowSize, const int colSize) {
	struct Maze maze = {NULL, 0, 0};
	initMaze(&maze,rowSize, colSize);
	generateMaze(&maze);
	clearScreen();
	printMazeCenterOnCurrentPos(&maze, 10, 15);
	printf("Press q to quit\n");
	fflush(stdout);
	while (1) {		
		int key;
		set_mode(1);
		while (!(key = get_key())) {
			fflush(stdin);
			usleep(10000);
		}
		if (key == 'q') { // quit game
			deleteMaze(&maze);
			return 1;
		}
		else if (key == 'w' || key == 'W' || key == 400) {
			if (!moveUp(&maze)) continue;
		}
		else if (key == 's' || key == 'S' || key == 401) {
			if (!moveDown(&maze)) continue;
		}
		else if (key == 'd' || key == 'D' || key == 402) {
			if (!moveRight(&maze)) continue;
		}
		else if (key == 'a' || key == 'A' || key == 403) {
			if (!moveLeft(&maze)) continue;
		}
		setPosition(0,0);
		printMazeCenterOnCurrentPos(&maze, 10, 15);
		printf("Press q to quit\n");
		fflush(stdout);
		if(atExit(&maze)) {
			showWinScreen();
			break;
		}
	}
	deleteMaze(&maze);
	return 0;
}

int main(void) {
	clearScreen();
	int row = 13,
		col = 20,
		diff = 1;
	for (diff = 1; diff < 5; diff++) {
		if (startGame(row * diff, col * diff)) {
			clearScreen();
			printf("Thanks for playing!\n");
			fflush(stdout);
			break;
		}
	}
	return 0;
}