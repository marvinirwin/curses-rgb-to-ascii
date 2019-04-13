
/*int main() {
    printf("cursses colors: %d\n", COLORS);
    return 0;
}*/



#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <memory.h>

typedef struct s_color {
    uint8_t R,G,B;
    char color[200];
    short curse_color;
} color;

color colors[] = {
        {0, 0, 0, "black"},
        {255, 0, 0, "red"},
        {0, 255, 0, "green"},
        {0, 0, 255, "blue"},
        {255, 255, 0, "yellow"},
        {255, 0, 255, "magenta"},
        {0, 255, 255, "cyan"},
        {255, 255, 255, "white"},
};

double distance(color * p1, color * p2) {
    int a = p2->R - p1->R;
    int b = p2->G - p1->G;
    int c = p2->B - p1->B;
    double r = sqrt((a*a) + (b*b) + (c*c));
    return r;
}

color * getClosestColor(color * c) {
    color * closestCandidate = &colors[0];
    for (int i = 1; i < 8; ++i) {
        if (distance(closestCandidate, c) > distance(&colors[i], c)) {
            closestCandidate = &colors[i];
        }
    }
    return closestCandidate;
}

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

int main(void)
{
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
    for (int j = 0; j < sizeof(colors) / sizeof(color); ++j) {
        color * c = &colors[j];
        short i = (short) (((c->R / 255) << 0) |
                           ((c->G / 255) << 1) |
                           ((c->B / 255) << 2));
        c->curse_color = (short) (j + 1);
        if (can_change_color()) {
            init_color(c->curse_color, c->R, c->G, c->B);
        }

        init_pair(c->curse_color, i, COLOR_BLACK);
    }
    for (int i = 0; i < sizeof(testPixels) / sizeof(color); ++i) {
        color * p = &testPixels[i];
        color * match = getClosestColor(p);
        attron(COLOR_PAIR(match->curse_color));
        mvprintw(i, 0, "%s -> %s", p->color, match->color);
    }

    getch();
    endwin();
    return 0;
}
