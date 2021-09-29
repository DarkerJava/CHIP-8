#include "opexecute.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "system.h"

//retrieve operation function based on opcode 0-35
void (*ops[35])(uint16_t instr, struct system* state) = {NOP, CLS, RET, JP, CALL, SEi, SNEi, SE, LDi, ADDi, LD, OR, AND, XOR, ADD, SUB, SHR, SUBN, SHL, SNE, LDidx, JPoff, RND, DRW, SKP, SKNP, LDdel, LDkey, SETdel, SETsnd, SETidx, SETspr, STdec, STmem, LDmem };

//operation functions
void NOP(uint16_t instr, struct system* state) {
	return;
}

void CLS(uint16_t instr, struct system* state) {
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++) {
			state->display[i][j] = 0;
		}
	}
	state->draw = true;
	return;
}

void RET(uint16_t instr, struct system* state) {
	--(state->reg_sp);
	state->reg_pc = state->stack[state->reg_sp];
	return;
}

void JP(uint16_t instr, struct system* state) {
	state->reg_pc = instr & 0x0FFF;
	return;
}

void CALL(uint16_t instr, struct system* state) {
	state->stack[state->reg_sp] = state->reg_pc;
	state->reg_pc = instr & 0x0FFF;
	++(state->reg_sp);

	return;
}

void SEi(uint16_t instr, struct system* state) {
	if (state->reg[(instr & 0x0F00) >> 8] == (instr & 0x00FF)) {
		state->reg_pc += 2U;
	}
	return;
}

void SNEi(uint16_t instr, struct system* state) {
	if (state->reg[(instr & 0x0F00) >> 8] != (instr & 0x00FF)) {
		state->reg_pc += 2U;
	}
	return;
}

void SE(uint16_t instr, struct system* state) {
	if (state->reg[(instr & 0x0F00) >> 8] == state->reg[(instr & 0x00F0) >> 4]) {
		state->reg_pc += 2U;
	}
	return;
}

void LDi(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] = instr & 0x00FF;
	return;
}

void ADDi(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] += instr & 0x00FF;
	return;
}

void LD(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] = state->reg[(instr & 0x00F0) >> 4];
	return;
}

void OR(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] |= state->reg[(instr & 0x00F0) >> 4];
	return;
}

void AND(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] &= state->reg[(instr & 0x00F0) >> 4];
	return;
}

void XOR(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] ^= state->reg[(instr & 0x00F0) >> 4];
	return;
}

void ADD(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] += state->reg[(instr & 0x00F0) >> 4];

	if (state->reg[(instr & 0x0F00) >> 8] < state->reg[(instr & 0x00F0) >> 4]) {
		state->reg[0xF] = 0x0001;
	} else {
		state->reg[0xF] = 0x0000;
	}
}

void SUB(uint16_t instr, struct system* state) {
	uint8_t flag;
	if (state->reg[(instr & 0x0F00) >> 8] < state->reg[(instr & 0x00F0) >> 4]) {
		flag = 0x0000;
	} else {
		flag = 0x0001;
	}
	state->reg[(instr & 0x0F00) >> 8] -= state->reg[(instr & 0x00F0) >> 4];
	state->reg[0xF] = flag;
	return;
}

void SHR(uint16_t instr, struct system* state) {
	if (state->old_cpu) {
		state->reg[(instr & 0x0F00) >> 8] = state->reg[(instr & 0x00F0) >> 4];
	}
	uint8_t flag = state->reg[(instr & 0x0F00) >> 8] & 0x01;
	state->reg[(instr & 0x0F00) >> 8] >>= 1;
	state->reg[0xF] = flag;
}

void SUBN(uint16_t instr, struct system* state) {
	uint8_t flag;
	if (state->reg[(instr & 0x0F00) >> 8] > state->reg[(instr & 0x00F0) >> 4]) {
		flag = 0x0000;
	} else {
		flag = 0x0001;
	}
	state->reg[(instr & 0x0F00) >> 8] = state->reg[(instr & 0x00F0) >> 4] - state->reg[(instr & 0x0F00) >> 8];
	state->reg[0xF] = flag;
	return;
}

void SHL(uint16_t instr, struct system* state) {
	if (state->old_cpu) {
		state->reg[(instr & 0x0F00) >> 8] = state->reg[(instr & 0x00F0) >> 4];
	}
	uint8_t flag = (state->reg[(instr & 0x0F00) >> 8] & 0x80) >> 7;
	state->reg[(instr & 0x0F00) >> 8] <<= 1;
	state->reg[0xF] = flag;
}

void SNE(uint16_t instr, struct system* state) {
	if (state->reg[(instr & 0x0F00) >> 8] != state->reg[(instr & 0x00F0) >> 4]) {
		state->reg_pc += 2U;
	}
	return;
}

void LDidx(uint16_t instr, struct system* state) {
	state->reg_idx = instr & 0x0FFF;
	return;
}

void JPoff(uint16_t instr, struct system* state) {
	state->reg_pc = state->reg[0x0] + (instr & 0x0FFF);
}

void RND(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] = (uint8_t)rand() & (instr & 0x00FF);
}

void DRW(uint16_t instr, struct system* state) {
	uint8_t x = state->reg[(instr & 0x0F00) >> 8] % 64;
	uint8_t y = state->reg[(instr & 0x00F0) >> 4] % 32;
	uint8_t bytes = instr & 0x000F;
	state->reg[0xF] = false;

	//printf("%d %d\n", x, y);

	for (int i = 0; i < min(bytes, 32 - y); i++) {
		uint8_t word = state->mem[state->reg_idx + i];
		for (int j = x + 7; j > x; j--) {
			//printf("%d", word & 0x0001);
			if (j < 64) {
				state->reg[0xF] = state->reg[0xF] || (state->display[j][y + i] & (word & 0x0001));
				state->display[j][y + i] ^= (word & 0x0001);
			}
			word >>= 1;
		}
		//printf("%d", word & 0x0001);
		state->reg[0xF] |= state->display[x][y + i] & (word & 0x0001);
		state->display[x][y + i] ^= (word & 0x0001);
		//printf("\n");
	}

	state->draw = true;
	return;
}

void SKP(uint16_t instr, struct system* state) {
	if (state->keystates[state->reg[(instr & 0x0F00) >> 8]]) {
		state->reg_pc += 2;
	}
	return;
}

void SKNP(uint16_t instr, struct system* state) {
	if (!state->keystates[state->reg[(instr & 0x0F00) >> 8]]) {
		state->reg_pc += 2;
	}
}

void LDdel(uint16_t instr, struct system* state) {
	state->reg[(instr & 0x0F00) >> 8] = state->delay;
}

void LDkey(uint16_t instr, struct system* state) {
	bool pressed = false;
	for (uint8_t i = 0; i < 16; i++) {
		if (state->keystates[i]) {
			pressed = true;
			state->reg[(instr & 0x0F00) >> 8] = i;
		}
	}

	if (!pressed) {
		state->reg_pc -= 2;
	}

	return;
}

void SETdel(uint16_t instr, struct system* state) {
	state->delay = state->reg[(instr & 0x0F00) >> 8];
}

void SETsnd(uint16_t instr, struct system* state) {
	state->sound = state->reg[(instr & 0x0F00) >> 8];
}

void SETidx(uint16_t instr, struct system* state) {
	state->reg_idx += state->reg[(instr & 0x0F00) >> 8];
}

void SETspr(uint16_t instr, struct system* state) {
	state->reg_idx = state->reg[(instr & 0x0F00) >> 8] * 5U;
}

void STdec(uint16_t instr, struct system* state) {
	uint8_t num = state->reg[(instr & 0x0F00) >> 8];
	state->mem[state->reg_idx] = num / 100U;
	state->mem[state->reg_idx + 1U] = num % 100U / 10U;
	state->mem[state->reg_idx + 2U] = num % 10U;
}

void STmem(uint16_t instr, struct system* state) {
	for (uint8_t i = 0; i <= ((instr & 0x0F00) >> 8); i++) {
		state->mem[state->reg_idx + i] = state->reg[i];
	}

	if (state->old_cpu) {
		state->reg_idx += ((instr & 0x0F00) >> 8) + 1;
	}
}

void LDmem(uint16_t instr, struct system* state) {
	for (uint8_t i = 0; i <= ((instr & 0x0F00) >> 8); i++) {
		state->reg[i] = state->mem[state->reg_idx + i];
	}

	if (state->old_cpu) {
		state->reg_idx += ((instr & 0x0F00) >> 8) + 1;
	}
}
