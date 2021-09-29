#include <stdint.h>

extern void (*ops[35])(uint16_t instr, struct system* state);

void NOP(uint16_t instr, struct system* state);

void CLS(uint16_t instr, struct system* state);

void RET(uint16_t instr, struct system* state);

void JP(uint16_t instr, struct system* state);

void CALL(uint16_t instr, struct system* state);

void SEi(uint16_t instr, struct system* state);

void SNEi(uint16_t instr, struct system* state);

void SE(uint16_t instr, struct system* state);

void LDi(uint16_t instr, struct system* state);

void ADD(uint16_t instr, struct system* state);

void LD(uint16_t instr, struct system* state);

void OR(uint16_t instr, struct system* state);

void AND(uint16_t instr, struct system* state);

void XOR(uint16_t instr, struct system* state);

void ADDi(uint16_t instr, struct system* state);

void SUB(uint16_t instr, struct system* state);

void SHR(uint16_t instr, struct system* state);

void SUBN(uint16_t instr, struct system* state);

void SHL(uint16_t instr, struct system* state);

void SNE(uint16_t instr, struct system* state);

void LDidx(uint16_t instr, struct system* state);

void JPoff(uint16_t instr, struct system* state);

void RND(uint16_t instr, struct system* state);

void DRW(uint16_t instr, struct system* state);

void SKP(uint16_t instr, struct system* state);

void SKNP(uint16_t instr, struct system* state);

void LDdel(uint16_t instr, struct system* state);

void LDkey(uint16_t instr, struct system* state);

void SETdel(uint16_t instr, struct system* state);

void SETsnd(uint16_t instr, struct system* state);

void SETidx(uint16_t instr, struct system* state);

void SETspr(uint16_t instr, struct system* state);

void STdec(uint16_t instr, struct system* state);

void STmem(uint16_t instr, struct system* state);

void LDmem(uint16_t instr, struct system* state);