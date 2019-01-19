# Makefile

# Compiler argument

CPPFLAGS=-MMD
CC=gcc

GTK_CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
GTK_LDLIBS=$(shell pkg-config --libs gtk+-3.0)

CFLAGS=-Wall -Wextra -Werror -std=c99 -pedantic -Wformat=2 ${GTK_CFLAGS}

LDFLAGS=
LDLIBS=-lm ${GTK_LDLIBS}

DEBUG=-fsanitize=address -g3
RELEASE=-Ofast -march=native
MOD=${DEBUG}

# Command

CP=cp -r
MKDIR=mkdir -p
RM=rm -rf

# Variable

EXE=packup
SRC_DIR=src/
BIN_DIR=bin/

SRC_SUBDIR=$(shell find ${SRC_DIR} -type d)
OBJ_SUBDIR=$(addprefix ${BIN_DIR}, $(subst ${SRC_DIR},,${SRC_SUBDIR}))

SRC=$(shell find ${SRC_DIR} -type f -name "*.c")
OBJ=$(addprefix ${BIN_DIR}, $(subst ${SRC_DIR},,$(SRC:.c=.o)))
DEP=$(OBJ:.o=.d)

# Target

.SILENT: clean ${OBJ_SUBDIR}
.PHONY: clean

all: ${EXE}

${EXE}: ${OBJ_SUBDIR} ${OBJ}
	${CC} ${CFLAGS} -o ${EXE} ${OBJ} ${LDFLAGS} ${LDLIBS} ${MOD}

${BIN_DIR}%.o: ${SRC_DIR}%.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c -o $@ $< ${MOD}

${OBJ_SUBDIR}:
	${MKDIR} ${OBJ_SUBDIR}

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_SUBDIR}
	${RM} ${DEP}
	${RM} ${EXE}

-include ${DEP}

# END
