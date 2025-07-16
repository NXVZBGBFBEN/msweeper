#include "game_logic.h"

#include <stdlib.h>

#include "types.h"
#include "util.h"

/* 初期位置を避けるように地雷を配置する */
void place_mines(GameState *state, int start_x, int start_y);

void init_game_state(GameState *state, GameConfig *config) {
    state->field_width = config->width;
    state->field_height = config->height;
    state->mines = config->mines;
    state->remaining_cells = config->width * config->height - config->mines;
    state->is_initialized = false;
    state->cursor_x = 0;
    state->cursor_y = 0;

    state->field = MALLOC_ARRAY(state->field_height, CellState *);
    if (state->field == NULL) {
        die("Failed to allocate memory for field");
    }
    for (int i = 0; i < state->field_height; i++) {
        state->field[i] = MALLOC_ARRAY(state->field_width, CellState);
        if (state->field[i] == NULL) {
            die("Failed to allocate memory for field");
        }
        for (int j = 0; j < state->field_width; j++) {
            state->field[i][j].adjacent_mines = 0;
            state->field[i][j].is_mine = false;
            state->field[i][j].mark = CELL_NOT_MARKED;
        }
    }
}

void place_mines(GameState *state, int start_x, int start_y) {
    int mines_to_place = state->mines; /* 置くべき地雷の数 */
    while (0 < mines_to_place) {
        int x = rand() % state->field_width;
        int y = rand() % state->field_height;

        if ((x == start_x && y == start_y) || /* 初期位置 */
            state->field[y][x].is_mine        /* すでに地雷がある */
        ) {
            continue;
        }

        state->field[y][x].is_mine = true;
        mines_to_place--;

        /* 隣接地雷数の更新 */
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) { /* 置いた地雷の周囲を走査 */
                if (dx == 0 && dy == 0) {      /* 地雷自身はスキップ */
                    continue;
                }
                int nx = x + dx;
                int ny = y + dy;
                if (nx < 0 || state->field_width <= nx || /* x方向の範囲チェック */
                    ny < 0 || state->field_height <= ny   /* y方向の範囲チェック */
                ) {
                    continue;
                }
                state->field[ny][nx].adjacent_mines++;
            }
        }
    }
    state->is_initialized = true;
}

void move_cursor(GameState *state, int dx, int dy) {
    int nx = state->cursor_x + dx;
    int ny = state->cursor_y + dy;
    if (0 <= nx && nx < state->field_width && /* x方向の範囲チェック */
        0 <= ny && ny < state->field_height   /* y方向の範囲チェック */
    ) {
        state->cursor_x = nx;
        state->cursor_y = ny;
    }
}

void toggle_mark(GameState *state, int x, int y) {
    CellState *cell = &state->field[y][x];
    switch (cell->mark) {
        case CELL_NOT_MARKED:
            cell->mark = CELL_FLAGGED;
            break;
        case CELL_FLAGGED:
            cell->mark = CELL_QUESTIONED;
            break;
        case CELL_QUESTIONED:
            cell->mark = CELL_NOT_MARKED;
            break;
        default:
            break;
    }
}

void open_cell(GameState *state, int x, int y) {
    CellState *cell = &state->field[y][x];
    if (cell->mark == CELL_OPENED || cell->mark == CELL_FLAGGED || cell->mark == CELL_QUESTIONED) {
        return;
    }
    if (!state->is_initialized) {
        place_mines(state, x, y);
    }
    cell->mark = CELL_OPENED;
    state->remaining_cells--;
    /* 隣接地雷数が0の場合周囲のセルを自動で開く */
    if (cell->adjacent_mines == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                if (0 <= nx && nx < state->field_width && 0 <= ny && ny < state->field_height) {
                    open_cell(state, nx, ny);
                }
            }
        }
    }
}

void open_cell_all(GameState *state) {
    for (int y = 0; y < state->field_height; y++) {
        for (int x = 0; x < state->field_width; x++) {
            state->field[y][x].mark = CELL_OPENED;
        }
    }
}
