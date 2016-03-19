# Originated from LemonBoy's Makefile in lemonbar

CC	?= gcc
CFLAGS	+= -Wall -std=c99 -Os -g
LDFLAGS += -lusb-1.0

EXEC	= g930
SRCS	= ./src/main.c
OBJS	= ${SRCS:.c=.o}
MKDIR_P = mkdir -p
OUTDIR  = ./bin

PREFIX	?=/usr
BINDIR	=${PREFIX}/bin

all: ${EXEC}

.c.o:
	${CC} ${CFLAGS} -o $@ -c $<

${EXEC}: ${OBJS}
	mkdir -p ./bin
	${CC} -o ./bin/${EXEC} ${OBJS} ${LDFLAGS}

clean:
	rm -f ./src/*.o
	rm -rf ./bin/${EXEC}
