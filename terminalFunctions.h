#ifndef __TERMINAL_FUNCTION_H
#define __TERMINAL_FUNCTION_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


void set_mode(int want_key)
{
	static struct termios old, new;
	if (!want_key) {
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
		return;
	}
 
	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO | ECHOK);
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
}
 
// return 0 if no key was read
int get_key()
{
	int c = 0;
	struct timeval tv;
	fd_set fs;
	tv.tv_usec = tv.tv_sec = 0;
 
	FD_ZERO(&fs);
	FD_SET(STDIN_FILENO, &fs);
	select(STDIN_FILENO + 1, &fs, 0, 0, &tv);

	if (FD_ISSET(STDIN_FILENO, &fs)) {
		c = getchar();
		if (c == '\033') {
			getchar();
			switch(getchar()) {
				case 'A': // up arrow
					c = 400;
					break;
				case 'B': // down arrow
					c = 401;
					break;
				case 'C': // right arrow
					c = 402;
					break;
				case 'D': // left arrow
					c = 403;
					break;
			}
		}
		set_mode(0);
	}
	return c;
}

int readKeys() {
	while (1) {
		int key;
		set_mode(1);
		while (!(key = get_key())) {
			fflush(stdin);
			usleep(10000);
		}
		if (key == 'q')
			break;
		else if (key == 'w' || key == 'W' || key == 400)
			printf("up      \r");
		else if (key == 's' || key == 'S' || key == 401)
			printf("down    \r");
		else if (key == 'd' || key == 'D' ||  key == 402)
			printf("right   \r");
		else if (key == 'a' || key == 'A' || key == 403)
			printf("left    \r");
		else
			printf("unknown\r", key);
		fflush(stdout);
	}
}



void putCharAt(int x, int y, char c) {
    printf("\033[%d;%dH%c", (x), (y),c);
}

void setPosition(int x, int y) {
    printf("\033[%d;%dH",x,y);
}

void clearScreen() {
    int i;
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    for (i = 0; i <= w.ws_row; i++) {
	    printf("\n");
    }
    setPosition(0,0);
	fflush(stdout);
}

void cleanScreen() {
	int i, j;
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    for (i = 0; i <= w.ws_row; i++) {
		for (j = 0; j <= w.ws_col; j++) {
			putCharAt(i,j,' ');
		}
	}
	setPosition(0,0);
//	fflush(stdout);
}

#endif