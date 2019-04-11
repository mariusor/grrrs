CC ?= clang
COMPILE_FLAGS = -std=c11 -Wpedantic -Wall -Wextra -Werror -DSNOW_ENABLED -DDEBUG
LINK_FLAGS = -g -Og
INCLUDES = -Isnow -I..

SOURCES = basic.c

SNOWFLAGS ?= --no-maybes --no-cr --no-quiet

VGFLAGS ?= \
	--quiet --leak-check=full --show-leak-kinds=all \
	--track-origins=yes

export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS)
export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS)

.c: ../*.h
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

run: clean basic allocations
	./basic $(SNOWFLAGS)
	./allocations $(SNOWFLAGS)

test: clean basic allocations
	valgrind $(VGFLAGS) -- ./basic $(SNOWFLAGS)
	valgrind $(VGFLAGS) -- ./allocations $(SNOWFLAGS)

clean:
	-$(RM) ./basic ./allocations
