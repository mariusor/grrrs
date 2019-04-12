CC ?= clang
COMPILE_FLAGS = -std=c11 -Wpedantic -Wall -Wextra -Werror -DSNOW_ENABLED -DDEBUG
LINK_FLAGS = -g -Og
INCLUDES = -Isnow -I..

TARGETS = $(patsubst %.c,%,$(wildcard *.c))

SNOWFLAGS ?= --no-maybes --no-cr --no-quiet

VGFLAGS ?= \
	--quiet --leak-check=full --show-leak-kinds=all \
	--track-origins=yes

export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS)
export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS)

.PHONY: check_leak check check_memory check_undefined run test clean

run: clean $(TARGETS)
	./basic $(SNOWFLAGS)
	./allocations $(SNOWFLAGS)

check_leak: run
check_leak: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=address

check_memory: run
check_memory: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=memory

check_undefined: run
check_undefined: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) -fsanitize=undefined

test: clean $(TARGETS)
	valgrind $(VGFLAGS) -- ./basic $(SNOWFLAGS)
	valgrind $(VGFLAGS) -- ./allocations $(SNOWFLAGS)

clean:
	$(RM) -f $(TARGETS)

$(TARGETS): *.c ../*.h
	$(CC) $(CFLAGS) $(INCLUDES) $< $(LDFLAGS) -o$@
