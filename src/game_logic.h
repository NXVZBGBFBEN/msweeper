#ifndef MSWEEPER_GAME_LOGIC_H
#define MSWEEPER_GAME_LOGIC_H

#include "types.h"

/* ゲーム状態を初期化する */
void init_game_state(GameState *state, GameConfig *config);

/* カーソルを指定量だけ移動する */
void move_cursor(GameState *state, int dx, int dy);

/* 指定位置のセルのマークを切り替える */
void toggle_mark(GameState *state, int x, int y);

/* 指定位置のセルを開ける */
void open_cell(GameState *state, int x, int y);

/* セルを全て開ける */
void open_cell_all(GameState *state);

#endif  /* MSWEEPER_GAME_LOGIC_H */
