#include "registers.h"

void registers_init()
{
	A = 0;
	X = 0;
	Y = 0;
	S = 0xFD;
	registers_set_status(0b00110100);
}

uint8 registers_get_status(bool is_instruction)
{
	return P.carry | (P.zero << 1) | (P.interrupt << 2) | (is_instruction << 4) | (TRUE << 5) | (P.overflow << 6) | (P.negative << 7);
}

void registers_set_status(uint8 status)
{
	P.carry = (status & 0b00000001) ? TRUE : FALSE;
	P.zero = (status & 0b00000010) ? TRUE : FALSE;
	P.interrupt = (status & 0b00000100) ? TRUE : FALSE;
	P.overflow = (status & 0b01000000) ? TRUE : FALSE;
	P.negative = (status & 0b10000000) ? TRUE : FALSE;
}