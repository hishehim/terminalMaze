#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>



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


int main(void) {
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    clearScreen();
    return 0;
}

