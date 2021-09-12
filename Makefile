.POSIX:

name   = ticTacToe
srcdir = src/
objdir = build/
exedir = bin/
insdir = ~/.bin/

EXE = $(exedir)$(name)
SRC = $(wildcard $(srcdir)*.c)
HDR = $(wildcard $(srcdir)*.h)
OBJ = $(SRC:$(srcdir)%.c=$(objdir)%.o) 
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

.PHONY: all install clean distclean
all: $(EXE)

install: $(EXE)
	cp $(EXE) $(insdir)

clean:
	rm -f -- $(OBJ)

distclean: clean
	rm -f -- $(EXE)
	rm -f -- $(insdir)$(name)
	-rmdir --ignore-fail-on-non-empty -- $(OUT)

$(EXE): $(OBJ) $(HDR)
	$(CC) -o $@ $(OBJ)

run: 
	$(EXE)

# When srcdir == objdir, make's default implicit rule works fine.
# However, when srcdir ≠ objdir, it never matches, because (e.g.,
# with `make objdir=foo- ...') foo-draw.o differs not just in the
# suffix (.c → .o) but also in the prefix (src/ → foo-).  Hence,
# we redefine the implicit rule to also work in that situation.
$(objdir)%.o: $(srcdir)%.c
	$(CC) -c $(CFLAGS) $< -o $@
