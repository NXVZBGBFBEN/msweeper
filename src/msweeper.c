#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include <ncurses.h>

#include "types.h"
#include "game_logic.h"
#include "ui.h"
#include "util.h"

GameState setup(int argc, char *argv[]);   /* プログラム本体の初期化 */
void run(GameState *state);     /* ゲームの開始 */
void cleanup(GameState *state); /* プログラムの終了処理を行う */
int validate_argument_value(const char *optarg);
void print_usage(void);
void parse_arguments(GameConfig *config, int argc, char *argv[]);
GameResult game_loop(GameState *state);

int main(int argc, char *argv[]) {
    GameState state = setup(argc, argv);
    run(&state);
    cleanup(&state);

    return EXIT_SUCCESS;
}

GameState setup(int argc, char *argv[]) {
    srand((unsigned)time(NULL));

    GameConfig config = {
        .width = 8,
        .height = 8,
        .mines = 10,
    };
    parse_arguments(&config, argc, argv);

    GameState state;
    init_game_state(&state, &config);
    
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    return state;
}

void run(GameState *state) {
    draw_field(state);
    switch (game_loop(state)) {
        case GAME_ABORT:
            return;
        case GAME_OVER:
            mvprintw(state->field_height + 1, state->field_width + 3, "GAME OVER");
            getch();
            return;
        case GAME_CLEAR:
            mvprintw(state->field_height + 1, state->field_width + 3, "GAME CLEAR");
            getch();
            return;
    }
}

void cleanup(GameState *state) {
    if (state->field != NULL) {
        for (int i = 0; i < state->field_height; i++) {
            free(state->field[i]);
        }
        free(state->field);
        state->field = NULL;
    }
    endwin();
}

void parse_arguments(GameConfig *config, int argc, char *argv[]) {
    int option;
    while ((option = getopt(argc, argv, "w:h:m:")) != -1) {
        switch (option) {
            case 'w':
                config->width = validate_argument_value(optarg);
                break;
            case 'h':
                config->height = validate_argument_value(optarg);
                break;
            case 'm':
                config->mines = validate_argument_value(optarg);
                break;
            default:
                print_usage();
                die("Failed to parse command-line arguments");
        }
    }

    if (config->mines <= 0 ||                           /* 地雷の数が負 */
        config->width * config->height <= config->mines /* 盤面より地雷が多い */) {
        die("Invalid number of mines: must be between 1 and (width x height - 1)");
    }
}

int validate_argument_value(const char *optarg) {
    errno = 0;

    char *endptr;
    long value = strtol(optarg, &endptr, 10);

    if (errno != 0 ||       /* strtol内部で失敗 */
        *endptr != '\0' ||  /* 数値以外が含まれている */
        value <= 0 ||       /* 負の値 */
        INT_MAX < value     /* intの範囲より大きい */) {
        print_usage();
        die("Failed to parse command-line arguments");
    }

    return (int)value;
}

void print_usage(void) {
    fprintf(stderr, "Usage: msweeper [-w width] [-h height] [-m mines]\n");
}

GameResult game_loop(GameState *state) {
    while (true) {
        switch (getch()) {
            case KEY_UP:    move_cursor(state, 0, -1); break;
            case KEY_DOWN:  move_cursor(state, 0, 1);  break;
            case KEY_LEFT:  move_cursor(state, -1, 0); break;
            case KEY_RIGHT: move_cursor(state, 1, 0);  break;
            case '\n':
                if (state->field[state->cursor_y][state->cursor_x].is_mine) {
                    open_cell_all(state);
                    draw_field(state);
                    return GAME_OVER;
                }
                open_cell(state, state->cursor_x, state->cursor_y);
                if (state->remaining_cells == 0) {
                    open_cell_all(state);
                    draw_field(state);
                    return GAME_CLEAR;
                }
                break;
            case ' ':
                toggle_mark(state, state->cursor_x, state->cursor_y);
                break;
            case 'q':
                return GAME_ABORT;
        }
        draw_field(state);
    }
}
