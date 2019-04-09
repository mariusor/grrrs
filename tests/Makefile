CC ?= clang
COMPILE_FLAGS = -std=c11 -Wpedantic -Wall -Wextra -Werror -DSNOW_ENABLED -DDEBUG
LINK_FLAGS = -g -O1
INCLUDES = -Isnow -I..

SOURCES = basic.c

SNOWFLAGS ?= #--no-maybes --cr

VGFLAGS ?= \
	--quiet --leak-check=full --show-leak-kinds=all \
	--track-origins=yes --error-exitcode=77

export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS)
export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS)

.c: ../src/*.h
	$(CC) $(CFLAGS) $(INCLUDES) $< $(LDFLAGS) -o$@

.PHONY: check_leak check check_memory check_undefined run test clean

check_leak: basic
check_leak: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=address
check_leak: run clean

check_leak: basic
check_memory: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=memory
check_memory: run clean

check_leak: basic
check_undefined: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=undefined
check_undefined: run clean

run: basic
	./basic $(SNOWFLAGS)

test: basic
	valgrind $(VGFLAGS) ./basic -- $(SNOWFLAGS)

clean: basic
	@rm ./basic
