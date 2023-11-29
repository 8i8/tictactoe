.POSIX:

APP = ticTacToe
SRC  = src/
OUT  = build/
BIN  = bin/
INL  = ~/.bin/

EXE = ${BIN}${APP}
BLD = $(dir ${OUT} ${BIN})
CC  = clang

OBJ += ${OUT}ticTacToe.o \
	${OUT}logic.o \
	${OUT}draw.o \
	${OUT}endscene.o

# Generate the build directory if it is not present.
$(shell mkdir -p ${BLD} ${BIN})

# Enable Debian/Ubuntu build hardening unless already
# enabled/disabled  — see hardened-cc(1) —  and ensure
# the setting, whatever it is, is seen by the compiler
# (and linker, hardened-ld(1)).
# DEB_BUILD_HARDENING ?= 1
# export DEB_BUILD_HARDENING

# CFLAGS += -fsanitize=address
# CFLAGS += -fsanitize=undefined
# CFLAGS += -fno-omit-frame-pointer
# CFLAGS += -fsanitize=float-divide-by-zero
# CFLAGS += -fno-sanitize-recover=all
# CFLAGS += -Wextra -Wall -pedantic
# CFLAGS += -Wstrict-prototypes
CFLAGS += -g
#CFLAGS += -O
#LDFLAGS += -lm

.PHONY: all install clean distclean run entitle
all: ${EXE}

entitle: ${BIN}${APP}
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" ${BIN}${APP}.entitlements

include src/Makefile

${EXE}: ${OBJ}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJ}
	codesign -s mrig ${BIN}${APP}
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" ${BIN}${APP}.entitlements
	codesign -s - -f --entitlements ${BIN}${APP}.entitlements ${BIN}${APP}
	ulimit -c unlimited

install: ${EXE}
	cp ${EXE} ${INL}

clean:
	rm -f -- ${OBJ}

distclean: clean
	-rm -f -- ${EXE}
	-rm -f -- ${INL}${APP}
	-rm -f ${APP}.entitlements
	-rm -f ${BIN}${APP}.entitlements
	-rm -rf ${APP}.dSYM
	-rm -rf ${BLD}

run:
	${EXE}
