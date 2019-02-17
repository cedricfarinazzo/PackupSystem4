# Makefile

# Compiler argument

CC=$(shell gcc --version > /dev/null 2>&1 && echo "gcc" || echo "clang")

GTK_CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
GTK_LDLIBS=$(shell pkg-config --libs gtk+-3.0)

CRITERION_CFLAGS=$(shell pkg-config --cflags criterion)
CRITERION_LDLIBS=$(shell pkg-config --libs criterion)

CFLAGS=-Wall -Wextra -std=c99 -pedantic -Wformat=2 -rdynamic -lpthread ${GTK_CFLAGS}
CPPFLAGS=-MMD

LDFLAGS=-lasan
LDLIBS=-lm ${GTK_LDLIBS}

DEBUG=-fsanitize=address -g3
RELEASE=-Ofast -march=native
MOD=${DEBUG}

ifdef release
$(shell ${MAKE} clean)
MOD=${RELEASE}
endif

# Command

CP=cp -r
MKDIR=mkdir -p
RM=rm -rf

# Variable

EXE=packup
SRC_DIR=src/
BIN_DIR=bin/
TEST_DIR=tests/

SRC_SUBDIR=$(shell find ${SRC_DIR} -type d)
BIN_SUBDIR=$(addprefix ${BIN_DIR}, $(subst ${SRC_DIR},,${SRC_SUBDIR}))

SRC=$(shell find ${SRC_DIR} -type f -name "*.c")
OBJ=$(addprefix ${BIN_DIR}, $(subst ${SRC_DIR},,$(SRC:.c=.o)))
DEP=$(OBJ:.o=.d)

# Target

.SILENT: clean ${OBJ_SUBDIR}
.PHONY: all clean

all: ${EXE}

${EXE}: ${BIN_DIR} ${BIN_SUBDIR} ${OBJ} ${GLADE}
	${CC} ${CFLAGS} -o ${EXE} ${OBJ} ${LDFLAGS} ${LDLIBS} ${MOD}

${BIN_DIR}%.o: ${SRC_DIR}%.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c -o $@ $< ${MOD}

${BIN_DIR}: 
	${MKDIR} ${BIN_DIR}

${BIN_SUBDIR}: 
	${MKDIR} ${BIN_SUBDIR}

${TEST_DIR}%.c: ${BIN_DIR} ${BIN_SUBDIR} ${OBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} -o ${EXE}_test_$(shell basename $@ .c) $@ $(shell find ${BIN_DIR}$(shell basename $@ .c)/  -type f -name "*.o") ${LDFLAGS} ${LDLIBS} ${CRITERION_LDLIBS} ${MOD}

clean:
	${RM} ${OBJ}
	${RM} ${BIN_SUBDIR}
	${RM} ${BIN_DIR}
	${RM} ${DEP}
	${RM} *.gcov *.gcda *.gcno
	${RM} ${EXE}
	${RM} ${EXE}_*

-include ${DEP}

# END
