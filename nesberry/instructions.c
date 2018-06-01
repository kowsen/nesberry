#include "instructions.h"
#include "registers.h"
#include "mmu.h"
#include "operand.h"

#define SIGN_MASK 0b10000000

void update_zn_flags()
{
	P.zero = A == 0;
	P.negative = (A & SIGN_MASK) != 0;
}

void instruction_adc(uint8 value)
{
	uint8 oldA = A;
	A += value;
	P.carry = A < oldA;
	P.overflow = ((oldA & SIGN_MASK) == (value & SIGN_MASK)) && ((oldA & SIGN_MASK) != (A & SIGN_MASK));
	update_zn_flags();
}

void instruction_jmp(uint16 address)
{
	PC = address;
}