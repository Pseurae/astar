TOOLCHAIN 	?= 

ifeq (windows, $(MAKECMDGOALS))
TOOLCHAIN	:= x86_64-w64-mingw32
endif

ifeq (,$(TOOLCHAIN))
CC			:= gcc
else
CC			:= $(TOOLCHAIN)-gcc
endif
CFLAGS		:= -I. -O3 -ffunction-sections

windows: all

all: astar.c astar.h
	$(CC) $(CFLAGS) -o astar astar.c
	@echo Done.

.PHONY: all

clean:
	rm -rf $(OBJ_DIR)
	rm -f gible