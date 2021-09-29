#include <stdint.h>
#include <stdbool.h>
struct system {
	uint8_t reg[16];

	uint16_t stack[16];
	uint8_t reg_sp;

	uint8_t mem[4096];
	uint16_t reg_pc;
	uint16_t reg_idx;

	uint8_t sound;
	uint8_t delay;

	bool display[64][32];
	bool draw;
	bool keystates[16];
	bool old_cpu;
};

extern uint8_t font[80];

struct system* makeSystem();

uint16_t fetch(struct system* state);

uint8_t decode(uint16_t instr);

void execute(struct system* state, uint16_t instr, uint8_t op);

void fde(struct system* state);