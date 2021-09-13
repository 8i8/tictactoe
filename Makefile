.POSIX:

NAME = ticTacToe
SRC  = src/
OUT  = build/
BIN  = bin/
INL  = ~/.bin/

EXE = $(BIN)$(NAME)
BLD = $(dir $(OUT) $(BIN))
CC  = gcc

OBJ += $(OUT)ticTacToe.o \
	$(OUT)logic.o \
	$(OUT)draw.o \
	$(OUT)endscene.o

# Generate the build directory if it is not present.
$(shell mkdir -p $(BLD) $(BIN))

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

include src/Makefile 

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

install: $(EXE)
	cp $(EXE) $(INL)

clean:
	rm -f -- $(OBJ)

distclean: clean
	rm -f -- $(EXE)
	rm -f -- $(INL)$(NAME)
	-rmdir --ignore-fail-on-non-empty -- $(BLD)

run: 
	$(EXE)
