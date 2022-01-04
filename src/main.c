#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MAX_COL_LEN 50
#define MIN_COL_LEN 3
#define DELAY 60 /* in miliseconds */
#define KEY_SPACE ' '
#define LEAD  0
#define LINE  1

enum {
key_escape = 27,
};

static char rand_char() {
    return rand() % ('z' + 1 - '!') + '!';
}

static int rand_len() {
    return rand()%(MAX_COL_LEN-MIN_COL_LEN)+MIN_COL_LEN;
}

static int rand_start_y(int wheight) {
    return -(rand()%(wheight));
}

struct column {
    int x, y, len;
    char last;
};

static struct column *new_column(int y, int x, int len) {
    struct column *col = (struct column *)malloc(sizeof(*col));

    col->y = y;
    col->x = x;
    col->len = len;
    col->last = rand_char();
    return col;
}

static void update(struct column *col, int wheight) {
    col->y = rand_start_y(wheight);
    col->len = rand_len();
    col->last = rand_char();
}

static void step(struct column *col, int wheight) {
    if (col == NULL) {
        return;
    }

    ++(col->y);
    if (col->y >= 0 && col->y < wheight) {
        attron(COLOR_PAIR(LINE));
        mvaddch(col->y-1, col->x, col->last);
        attroff(COLOR_PAIR(LINE));

        col->last = rand_char();
        attron(COLOR_PAIR(LEAD) | A_BOLD);
        mvaddch(col->y, col->x, col->last);
        attroff(COLOR_PAIR(LEAD) | A_BOLD);
    }

    if (col->y - col->len - 1 < wheight) {
        mvaddch(col->y-col->len-1, col->x, ' ');
    } else {
        update(col, wheight);
    }
    curs_set(0);
}

int main() {
    int i;
    int win_height = 0, win_width = 0;
    time_t t;
    int key;
    struct column **columns;
    int pause = 0;

    srand((unsigned int)time(&t));
    initscr();
    timeout(DELAY);

    start_color();
    init_pair(LEAD, COLOR_WHITE, COLOR_WHITE);
    init_pair(LINE, COLOR_GREEN, COLOR_BLACK);

    getmaxyx(stdscr, win_height, win_width);

    columns = (struct column **)malloc(sizeof(*columns)*win_width);

    for(i = 0; i < win_width; ++i)
        columns[i] = new_column(rand_start_y(win_height), i, rand_len());


    while((key = getch()) != key_escape)  {
        switch (key) {
            case KEY_SPACE:
                pause = !pause;
                break;
            case ERR:
                break;
            case KEY_RESIZE:
                /* TODO */
                break;
        }
        if (!pause) {
            for(i = 0; i < win_width; ++i) {
                step(columns[i], win_height);
            }
            refresh();
        }
    }

    for(i = 0; i < win_width; ++i)
        free(columns[i]);
    free(columns);
    endwin();
    return 0;
}
