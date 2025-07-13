#ifndef MSWEEPER_TYPES_H
#define MSWEEPER_TYPES_H

#include <stdbool.h>

typedef struct {
    int width;  /* 盤面の幅 */
    int height; /* 盤面の高さ */
    int mines;  /* 総地雷数 */
} GameConfig;

typedef enum {
    CELL_NOT_MARKED,    /* マークされていない */
    CELL_OPENED,        /* 開かれている */
    CELL_FLAGGED,       /* 旗が立てられている */
    CELL_QUESTIONED,    /* ？が立てられている */
} CellMark;

typedef struct {
    int adjacent_mines; /* 隣接する地雷の数 */
    bool is_mine;       /* 地雷か？ */
    CellMark mark;      /* マーク状態 */
} CellState;

typedef enum {
    GAME_ABORT, /* ゲーム中断 */
    GAME_CLEAR, /* ゲームクリア */
    GAME_OVER,  /* ゲームオーバ */
} GameResult;

typedef struct {
    int field_width;        /* 盤面の幅 */
    int field_height;       /* 盤面の高さ */
    int mines;              /* 総地雷数 */
    int remaining_cells;    /* 開くべき残りセル数 */
    bool is_initialized;    /* セルが何も開かれていないか？ */
    int cursor_x;   /* カーソル位置 (x) */
    int cursor_y;   /* カーソル位置 (y) */
    CellState **field;  /* 盤面 */
} GameState;

#endif  /* MSWEEPER_TYPES_H */
