#ifndef _DEBUG_H
#define _DEBUG_H

struct Memory32;
struct PatchyCore;

void print_memory32(struct Memory32* mem);
void print_coredump(struct PatchyCore* core);

#endif
