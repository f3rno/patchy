CC=gcc
LDFLAGS=-I include/
CFLAGS=-std=c++11 -c -Wall -I include/
SOURCES=disassembler.c src/disassembly.c src/data/instruction.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bin/disassembler

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
