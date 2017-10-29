PREFIX ?= /usr
MANPREFIX = $(DESTDIR)$(PREFIX)/share/man

CFLAGS ?= -O2
CFLAGS += -Wall -pedantic
CFLAGS += -std=c99

CPPFLAGS += -D_DEFAULT_SOURCE
