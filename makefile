CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -O2 -pipe
CPPFLAGS =
TARGET_ARCH = -march=native
LDFLAGS =
LDLIBS = -lncurses -ltinfo

RM = rm -f

OUTPUT_OPTION = -o $@
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%: %.o
	$(LINK.o) $^ $(LDLIBS) -o $@

vpath %.c src

.PHONY: all
all: msweeper

msweeper: msweeper.o game_logic.o ui.o util.o

.PHONY: clean
clean:
	$(RM) *.o msweeper
