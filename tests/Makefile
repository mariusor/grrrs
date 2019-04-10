CC ?= clang
COMPILE_FLAGS = -std=c11 -Wpedantic -Wall -Wextra -Werror -DSNOW_ENABLED -DDEBUG
LINK_FLAGS = -g -Og
INCLUDES = -Isnow -I..

SOURCES = basic.c

SNOWFLAGS ?=

VGFLAGS ?= \
	--quiet --leak-check=full --show-leak-kinds=all \
	--track-origins=yes

export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS)
export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS)

.c:
	$(CC) $(CFLAGS) $(INCLUDES) $< $(LDFLAGS) -o$@

.PHONY: check_leak check check_memory check_undefined run test clean

check_leak: basic allocations
check_leak: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=address
check_leak: run clean

check_leak: basic allocations
check_memory: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=memory
check_memory: run clean

check_leak: basic allocations
check_undefined: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=undefined
check_undefined: run clean

run: basic allocations ../*.h
	./basic $(SNOWFLAGS)
	./allocations $(SNOWFLAGS)

test: basic allocations ../*.h
	valgrind $(VGFLAGS) ./basic -- $(SNOWFLAGS)
	valgrind $(VGFLAGS) ./allocations -- $(SNOWFLAGS)

clean:
	@rm ./basic ./allocations
