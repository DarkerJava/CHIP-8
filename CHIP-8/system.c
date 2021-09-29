#define _CRT_SECURE_NO_DEPRECATE
#include "system.h"
#include "opexecute.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct system* makeSystem() {
	struct system* s;
	if ((s = malloc(sizeof(struct system))) != NULL) {
		memcpy(s->mem, font, 80);
		char path[1001];
		char old;
		scanf("%s %c", &path, &old);
		if (old == 'o') s->old_cpu = true; else s->old_cpu = false;
		//printf(path);
		FILE* fp;
		if ((fp = fopen(path, "rb")) != NULL) {
			fseek(fp, 0L, SEEK_END);
			long long size = ftell(fp);
			rewind(fp);
			//printf("%d\n",size);
			int result = fread(s->mem + 0x0200, 1, size, fp);
			//printf("%d\n", feof(fp));
			//printf("%d\n", result);
			fclose(fp);

			s->reg_pc = 0x0200;
			s->reg_sp = 0;
			s->reg_idx = 0;
			s->draw = false;
			memset(s->display, 0, sizeof(s->display));
			memset(s->reg, 0, sizeof(s->reg));
			memset(s->stack, 0, sizeof(s->stack));
			memset(s->keystates, false, sizeof(s->keystates));

			s->sound = 0;
			s->delay = 0;
		} else {
			perror("File open fail");
		}
	}
	/*
	for (uint16_t i = 0x0200; i < 700U; i++) {
		printf("%x: %x\n", i, s->mem[i]);
	}
	*/
	return s;
}

uint16_t fetch(struct system* state) {
	uint16_t instr = 256U * state->mem[state->reg_pc] + state->mem[state->reg_pc + 1U];
	//printf("%x: %x\n", state->reg_pc, instr);
	state->reg_pc += 2U;
	return instr;
}

//generate opcode 0-34
uint8_t decode(uint16_t instr) {
	switch (instr & 0xF000) {

		case 0x0000:
			if (instr == 0x00E0) {
				return 1U;
			} else if (instr == 0x00EE) {
				return 2U;
			} else {
				return 0U;
			}

		case 0x1000: 
			return 3U;

		case 0x2000: 
			return 4U;

		case 0x3000:
			return 5U;

		case 0x4000:
			return 6U;

		case 0x5000:
			if ((instr & 0x000F) == 0x0000) return 7U;
			break;

		case 0x6000:
			return 8U;

		case 0x7000:
			return 9U;

		case 0x8000:
			switch (instr & 0x000F) {
				case 0x0000:
					return 10U;

				case 0x0001:
					return 11U;

				case 0x0002:
					return 12U;

				case 0x0003:
					return 13U;

				case 0x0004:
					return 14U;

				case 0x0005:
					return 15U;

				case 0x0006:
					return 16U;

				case 0x0007:
					return 17U;

				case 0x000E:
					return 18U;
			}
			break;

		case 0x9000:
			if ((instr & 0x000F) == 0x0000) return 19U;
			break;

		case 0xA000:
			return 20U;

		case 0xB000:
			return 21U;

		case 0xC000:
			return 22U;

		case 0xD000:
			return 23U;

		case 0xE000:
			if ((instr & 0x00FF) == 0x009E) {
				return 24U;
			} else if ((instr & 0x0FF) == 0x00A1) {
				return 25U;
			}
			break;

		case 0xF000:
			switch (instr & 0x00FF) {
				case 0x0007:
					return 26U;
				case 0x000A:
					return 27U;
				case 0x0015:
					return 28U;
				case 0x0018:
					return 29U;
				case 0x001E:
					return 30U;
				case 0x0029:
					return 31U;
				case 0x0033:
					return 32U;
				case 0x0055:
					return 33U;
				case 0x0065:
					return 34U;
			}
			break;
	}

	return 35U;
}

void execute(struct system* state, uint16_t instr, uint8_t op) {
	if (op == 35) {
		printf("Bad instruction: %x\n", instr);
		return;
	} else {
		ops[op](instr, state);
	}
}

//one step - fetch decode execute
void fde(struct system* state) {
	//fetch
	uint16_t instr = fetch(state);

	uint8_t opcode = decode(instr);

	execute(state, instr, opcode);

	return;
}

uint8_t font[80] = {
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};