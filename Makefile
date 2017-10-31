include config.mk
-include local.mk

BIN = \
	passadd \
	passgen \
	passget \
	passinfo
OBJ = pass.o
MAN1 = $(BIN:=.1)

.PHONY: all
all: $(BIN)

$(OBJ): config.mk
pass.o: pass.h
$(BIN): pass.o

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
