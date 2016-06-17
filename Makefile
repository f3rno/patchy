CC=gcc
LDFLAGS=-I include/
CFLAGS=-c -Os -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wswitch-default -Wswitch-enum -Wunreachable-code -march=native -I include/

DISASSEMBLER_SRC=disassembler.c src/disassembly.c src/data/instruction.c src/arch.c
DISASSEMBLER_OBJ=$(DISASSEMBLER_SRC:.c=.o)
DISASSEMBLER_EXE=bin/disassembler
PATCHY_SRC=patchy.c src/data/instruction.c src/data/memory.c src/core.c src/load.c src/exec.c src/debug.c src/arch.c src/disassembly.c
PATCHY_OBJ=$(PATCHY_SRC:.c=.o)
PATCHY_EXE=bin/patchy

all: $(PATCHY_SRC) $(DISASSEMBLER_SRC) $(PATCHY_EXE) $(DISASSEMBLER_EXE)

$(PATCHY_EXE): $(PATCHY_OBJ)
	$(CC) $(LDFLAGS) $(PATCHY_OBJ) -o $@

$(DISASSEMBLER_EXE): $(DISASSEMBLER_OBJ)
	$(CC) $(LDFLAGS) $(DISASSEMBLER_OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o src/*.o src/data/*.o
	rm $(PATCHY_EXE)
	rm $(DISASSEMBLER_EXE)
