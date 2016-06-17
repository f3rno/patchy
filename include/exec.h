#ifndef _EXEC_H
#define _EXEC_H

#include <stdint.h>

struct PatchyCore;
struct Instruction;

void patchy_exec(struct PatchyCore* core);
void jmp_to_reg_by_adr(struct PatchyCore* core, uint8_t addr);

void patchy_exec_mov(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_ldi(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_ldm(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_lpx(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_spx(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_str(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_push(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_pop(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_add(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_addi(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_sub(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_subi(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_cmp(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_and(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_or(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_xor(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_shl(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_shr(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jmp(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_je(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jne(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jg(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jge(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jl(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jle(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jz(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_jnz(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_call(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_calli(struct PatchyCore* core, struct Instruction* i);
void patchy_exec_ret(struct PatchyCore* core);
void patchy_exec_hlt(struct PatchyCore* core);

#endif
