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
	new.c_lflag &= ~(ICANON | ECHO);
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
		set_mode(0);
	}
	return c;
}


void printat(int x, int y, char c) {
    printf("\033[%d;%dH%c", (x), (y),c);
}

void setPosition(int x, int y) {
    printf("\033[%d;%dH",x,y);
}

void clearScreen() {
    int i, j;
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    for (i = 0; i <= w.ws_row; i++) {
        printf("\n");
    }
    setPosition(0,0);
}

void generateMaze() {
    
}
