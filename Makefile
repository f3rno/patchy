CC=gcc
LDFLAGS=-I include/
CFLAGS=-std=c++11 -c -Wall -I include/

DISASSEMBLER_SRC=disassembler.c src/disassembly.c src/data/instruction.c
DISASSEMBLER_OBJ=$(DISASSEMBLER_SRC:.c=.o)
DISASSEMBLER_EXE=bin/disassembler
PATCHY_SRC=patcy.c src/data/instruction.c src/data/memory.c src/core.c
PATCHY_OBJ=$(PATCHY_SRC:.c=.o)
PATCHY_EXE=bin/patchy

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
