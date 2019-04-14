
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <memory.h>

// The first non blank ASCII characters
char charList[] = ".-`',:_;~\"/!|\\i^trc*v?s()+lj1=e{[]z}<xo7f>aJy3Iun542b6Lw9k#dghq80VpT$YACSFPUZ%mEGXNO&DKBR@HQWM";
// greyscale will be at or between 0, 1
char lookupChar(double greyscale) {
    size_t max = strlen(charList);
    // Round our greyscale value to the closest whole number
    int place = (int) floor((max * greyscale) + 0.5);
    return charList[place];
}

typedef struct s_color {
    uint8_t R,G,B;
    char color[200];
    short curse_color;
} color;
// The colors our terminal can display
color terminalColors[] = {
        {0, 0, 0, "black"},
        {255, 0, 0, "red"},
        {0, 255, 0, "green"},
        {0, 0, 255, "blue"},
        {255, 255, 0, "yellow"},
        {255, 0, 255, "magenta"},
        {0, 255, 255, "cyan"},
        {255, 255, 255, "white"},
};
// The colors we have to "round" to be shown in the terminal
color testPixels[] = {
        {244, 75, 66, "red-ish"},
        {244, 128, 66, "orange?"},
        {244, 226, 66, "sickly yellow"},
        {146, 244, 66, "pale light green"},
        {66, 244, 89, "yellow"},
        {66, 244, 158, "kind of turqois, I would expect mapped to cyan"},
        {66, 226, 244, "baby blue"},
        {66, 158, 244, "almost navy blue"},
        {72, 66, 244, "blue, almost purple"},
        {164, 66, 244, "light purple"},
        {238, 66, 244, "pink, violet"},
        {244, 66, 131, "dark pink?"},
        {0, 0, 255, "this better be blue"}
};

double distance(color * p1, color * p2) {
    int a = p2->R - p1->R;
    int b = p2->G - p1->G;
    int c = p2->B - p1->B;
    double r = sqrt((a*a) + (b*b) + (c*c));
    return r;
}
color * getClosestColor(color * c) {
    color * closestCandidate = &terminalColors[0];
    for (int i = 1; i < 8; ++i) {
        if (distance(closestCandidate, c) > distance(&terminalColors[i], c)) {
            closestCandidate = &terminalColors[i];
        }
    }
    return closestCandidate;
}


int main(void)
{
    // Initialize curses
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    if (has_colors() == FALSE) {
        endwin();
        puts("Your terminal does not support color");
        exit(1);
    }

    start_color();
    for (int j = 0; j < sizeof(terminalColors) / sizeof(color); ++j) {
        color * c = &terminalColors[j];
        short i = (short) (((c->R / 255) << 0) |
                           ((c->G / 255) << 1) |
                           ((c->B / 255) << 2));
        c->curse_color = (short) (j + 1);
        init_pair(c->curse_color, i, COLOR_BLACK);
    }
    for (int i = 0; i < sizeof(testPixels) / sizeof(color); ++i) {
        color * c = &testPixels[i];
        color * match = getClosestColor(c);
        attron(COLOR_PAIR(match->curse_color));
        double g = ((.2126 * c->R) + (.7152 * c->G) + (.0722 * c->B)) / 255;
        int ch = lookupChar(g);
        mvprintw(i, 0, "%s -> %s -> %c", c->color, match->color, ch);
    }

    getch();
    endwin();
    return 0;
}
