#pragma once

#include "typedefs.h"

struct Status {
	bool carry;
	bool zero;
	bool interrupt;
	bool overflow;
	bool negative;
};

struct Registers {
	uint8 a;
	uint8 x;
	uint8 y;
	uint16 pc;
	uint8 s;
	struct Status p;
} registers;

#define A registers.a
#define X registers.x
#define Y registers.y
#define PC registers.pc
#define S registers.s
#define P registers.p

void registers_init();
uint8 registers_get_status(bool is_instruction);
void registers_set_status(uint8 status);