include config.mk
-include local.mk

BIN = \
	passget
OBJ = $(BIN:=.o) \
	pass.o
MAN1 = $(BIN:=.1)

.PHONY: all
all: $(BIN)

$(OBJ): config.mk
passget.o pass.o: pass.h
passget: pass.o

.PHONY: install
install: $(BIN)
	@echo "Installing binaries $(BIN) to $(DESTDIR)$(PREFIX)/bin"
	@mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	@cp -f $(BIN) "$(DESTDIR)$(PREFIX)/bin"
	@echo "Installing man pages $(MAN1) to $(DESTDIR)$(MANPREFIX)/man1"
	@mkdir -p "$(DESTDIR)$(MANPREFIX)/man1"
	@cp -f $(MAN1) "$(DESTDIR)$(MANPREFIX)/man1"

.PHONY: clean
clean:
	$(RM) $(OBJ) $(BIN)