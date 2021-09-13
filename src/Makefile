.POSIX:

name   = ticTacToe
srcdir = src/
objdir = ../build/
exedir = ../bin/
insdir = ~/.bin/

EXE = $(exedir)$(name)
#SRC = $(wildcard $(srcdir)*.c)
#HDR = $(wildcard $(srcdir)*.h)
OBJ = $(objdir)ticTacToe.o \
			$(objdir)logic.o \
			$(objdir)draw.o \
			$(objdir)endscene.o
OUT = $(dir $(objdir) $(exedir))
CC  = gcc

# Generate the build directory if it is not present.
$(shell mkdir -p $(objdir) $(exedir))

# Enable Debian/Ubuntu build hardening unless already
# enabled/disabled  — see hardened-cc(1) —  and ensure
# the setting, whatever it is, is seen by the compiler
# (and linker, hardened-ld(1)).
DEB_BUILD_HARDENING ?= 1
export DEB_BUILD_HARDENING

# CFLAGS += -fsanitize=address
# CFLAGS += -fsanitize=undefined
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -fsanitize=float-divide-by-zero
CFLAGS += -fno-sanitize-recover=all
CFLAGS += -Wextra -Wall -pedantic
CFLAGS += -Wstrict-prototypes
CFLAGS += -g
#CFLAGS += -O

#LDFLAGS += -lm

.PHONY: all install clean distclean run
all: $(EXE)

install: $(EXE)
	cp $(EXE) $(insdir)

clean:
	rm -f -- $(OBJ)

distclean: clean
	rm -f -- $(EXE)
	rm -f -- $(insdir)$(name)
	-rmdir --ignore-fail-on-non-empty -- $(OUT)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

run: 
	$(EXE)

$(objdir)ticTacToe.o: ticTacToe.h logic.h draw.h
	$(CC) -o $@ -c $(CFLAGS) ticTacToe.c
$(objdir)logic.o: ticTacToe.h logic.h draw.h error.h
	$(CC) -o $@ -c $(CFLAGS) logic.c
$(objdir)draw.o: ticTacToe.h logic.h draw.h endscene.h error.h
	$(CC) -o $@ -c $(CFLAGS) draw.c
$(objdir)endscene.o: endscene.h error.h
	$(CC) -o$@ -c $(CFLAGS) endscene.c
