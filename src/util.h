#ifndef MSWEEPER_UTIL_H
#define MSWEEPER_UTIL_H

#include <stdlib.h>

/* 安全に配列をmalloc()する */
#define MALLOC_ARRAY(number, type) ((type *)malloc((number) * sizeof(type)))

/* エラーを出力してプログラムを終了する */
void die(const char *error_message);

#endif /* MSWEEPER_UTIL_H */
