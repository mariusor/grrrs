CC ?= clang
COMPILE_FLAGS = -std=c11 -Wpedantic -Wall -Wextra
LINK_FLAGS =
DCOMPILE_FLAGS = -g -DDEBUG -Og
RLINK_FLAGS = -shared
DLINK_FLAGS =

TARGET = grrrs.so
SOURCES = strings.h
DESTDIR = /
INSTALL_PREFIX = usr/local

ifeq ($(shell git describe > /dev/null 2>&1 ; echo $$?), 0)
	VERSION := $(shell git describe --tags --long --dirty=-git --always )
endif
ifneq ($(VERSION), )
	override CFLAGS := $(CFLAGS) -DVERSION_HASH=\"$(VERSION)\"
endif

.PHONY: all test release debug clean install uninstall

all: release

test:
	$(MAKE) -C tests test

release: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) $(RCOMPILE_FLAGS)
release: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(RLINK_FLAGS)
release: $(TARGET)

debug: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) $(DCOMPILE_FLAGS)
debug: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(DLINK_FLAGS)
debug: $(TARGET)

clean:
	-$(RM) $(TARGET)
	-$(MAKE) -C tests clean

install: $(TARGET) $(TARGET).1
	install $(TARGET) $(DESTDIR)$(INSTALL_PREFIX)/lib

uninstall:
	$(RM) $(DESTDIR)$(INSTALL_PREFIX)/lib/$(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) $(LDFLAGS) -o$(TARGET)
