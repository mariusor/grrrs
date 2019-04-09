CC ?= clang
LIBS =
COMPILE_FLAGS = -std=c11 -Wpedantic -Wall -Wextra
LINK_FLAGS =
DCOMPILE_FLAGS = -g -DDEBUG -O1
RLINK_FLAGS = -shared
DLINK_FLAGS =

TARGET = grrrs.so
SOURCES = src/strings.h
DESTDIR = /
INSTALL_PREFIX = usr/local

ifneq ($(LIBS),)
	CFLAGS += $(shell pkg-config --cflags $(LIBS))
	LDFLAGS += $(shell pkg-config --libs $(LIBS))
endif

ifeq ($(shell git describe > /dev/null 2>&1 ; echo $$?), 0)
	VERSION := $(shell git describe --tags --long --dirty=-git --always )
endif
ifneq ($(VERSION), )
	override CFLAGS := $(CFLAGS) -DVERSION_HASH=\"$(VERSION)\"
endif

.PHONY: all
all: release

.PHONY: tests
tests:
	make -C tests run

release: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) $(RCOMPILE_FLAGS)
release: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(RLINK_FLAGS)
debug: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) $(DCOMPILE_FLAGS)
debug: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(DLINK_FLAGS)

.PHONY: release
release: $(TARGET)

.PHONY: debug
debug: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET) $(TARGET).1
	install $(TARGET) $(DESTDIR)$(INSTALL_PREFIX)/lib

.PHONY: uninstall
uninstall:
	$(RM) $(DESTDIR)$(INSTALL_PREFIX)/lib/$(TARGET)

$(TARGET): $(SOURCES) src/*.h
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) $(LDFLAGS) -o$(TARGET)
