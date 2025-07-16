#include "ui.h"

#include <ncurses.h>

void draw_border(int width, int height);    /* 枠線を描画する */
char cell_to_symbol(const CellState *cell); /* セルの状態から表示用文字を返す */

void draw_field(GameState *state) {
    /* 枠線 */
    draw_border(state->field_width, state->field_height);

    for (int y = 0; y < state->field_height; y++) {
        for (int x = 0; x < state->field_width; x++) {
            CellState *cell = &state->field[y][x];
            char symbol = cell_to_symbol(cell);

            if (y == state->cursor_y && x == state->cursor_x) { /* カーソル位置の描画 */
                attron(A_REVERSE);                              /* 色を反転させる */
                mvaddch(y + 1, x + 1, symbol);
                attroff(A_REVERSE);
            } else {
                mvaddch(y + 1, x + 1, symbol);
            }
        }
    }

    refresh();
}

void draw_border(int width, int height) {
    /* 上面 */
    mvaddch(0, 0, '+');
    for (int x = 0; x < width; x++) {
        addch('-');
    }
    addch('+');

    /* 側面 */
    for (int y = 0; y < height; y++) {
        mvaddch(y + 1, 0, '|');
        for (int x = 0; x < width; x++) {
            addch(' ');
        }
        addch('|');
    }

    /* 下面 */
    mvaddch(height + 1, 0, '+');
    for (int x = 0; x < width; x++) {
        addch('-');
    }
    addch('+');
}

char cell_to_symbol(const CellState *cell) {
    switch (cell->mark) {
        case CELL_OPENED:
            if (cell->is_mine) {
                return 'M';
            }
            if (cell->adjacent_mines == 0) {
                return ' ';
            }
            return '0' + cell->adjacent_mines;
        case CELL_FLAGGED:
            return 'F';
        case CELL_QUESTIONED:
            return '?';
        case CELL_NOT_MARKED:
        default:
            return '.';
    }
}
