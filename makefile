CC = gcc
# -O2 for optimalization, -Wall for turing on most compiler warning, -std=c99 for c99 standard
CFLAGS = -O2 -Wall -std=c99
# target executable fle
TARGET = hospital
TARGET_DBG = hospital.dbg

all: $(TARGET)

$(TARGET): parse.o structure.o hospital.o
	$(CC) $(CFLAGS) -o $(TARGET) parse.o structure.o hospital.o

structure.o: structure.h structure.c
	$(CC) $(CFLAGS) -c structure.c

parse.o: parse.h structure.h parse.c
	$(CC) $(CFLAGS) -c parse.c

hospital.o: parse.h structure.h hospital.c
	$(CC) $(CFLAGS) -c hospital.c

debug: TARGET = hospital.dbg
debug: CFLAGS += -g
debug: all

.PHONY: clean
clean:
	$(RM) $(TARGET) $(TARGET_DBG) *.o *~debug: TARGET = hospital.dbg