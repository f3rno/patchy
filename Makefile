CC=gcc
LDFLAGS=-I include/
CFLAGS=-c -Os -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wswitch-default -Wswitch-enum -Wunreachable-code -march=native -I include/

DISASSEMBLER_SRC=disassembler.c src/disassembly.c src/data/instruction.c src/arch.c
DISASSEMBLER_OBJ=$(DISASSEMBLER_SRC:.c=.o)
DISASSEMBLER_EXE=bin/disassembler

ASSEMBLER_SRC=assembler.c
ASSEMBLER_OBJ=$(ASSEMBLER_SRC:.c=.o)
ASSEMBLER_EXE=bin/assembler

PATCHY_SRC=patchy.c src/data/instruction.c src/data/memory.c src/core.c src/load.c src/exec.c src/debug.c src/arch.c src/disassembly.c src/render.c
PATCHY_OBJ=$(PATCHY_SRC:.c=.o)
PATCHY_EXE=bin/patchy
PATCHY_LDFLAGS=-lncurses

DUMPINSP_SRC=dump-inspector.c src/render.c
DUMPINSP_OBJ=$(DUMPINSP_SRC:.c=.o)
DUMPINSP_EXE=bin/dump-inspector
DUMPINSP_LDFLAGS=-lncurses

all: $(PATCHY_SRC) $(DISASSEMBLER_SRC) $(DUMPINSP_SRC) $(ASSEMBLER_SRC) $(PATCHY_EXE) $(DISASSEMBLER_EXE) $(DUMPINSP_EXE) $(ASSEMBLER_EXE)

$(PATCHY_EXE): $(PATCHY_OBJ)
	$(CC) $(LDFLAGS) $(PATCHY_LDFLAGS) $(PATCHY_OBJ) -o $@

$(DISASSEMBLER_EXE): $(DISASSEMBLER_OBJ)
	$(CC) $(LDFLAGS) $(DISASSEMBLER_OBJ) -o $@

$(DUMPINSP_EXE): $(DUMPINSP_OBJ)
	$(CC) $(LDFLAGS) $(DUMPINSP_LDFLAGS) $(DUMPINSP_OBJ) -o $@

$(ASSEMBLER_EXE): $(ASSEMBLER_OBJ)
	$(CC) $(LDFLAGS) $(ASSEMBLER_OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o src/*.o src/data/*.o
	rm $(PATCHY_EXE)
	rm $(DISASSEMBLER_EXE)
