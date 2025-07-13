#include "util.h"

#include <errno.h>
#include <stdio.h>

void die(const char *error_message) {
    if (errno == 0) {
        fprintf(stderr, "Error: %s\n", error_message);
    } else {
        perror(error_message);
    }
    exit(EXIT_FAILURE);
}
