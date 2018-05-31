#include "instructions.h"
#include "registers.h"
#include "mmu.h"
#include "operand.h"

uint8 instruction_jmp_absolute()
{
	PC = operand_address_absolute();
	return 0x03;
}

uint8 instruction_jmp_indirect()
{
	PC = operand_address_indirect();
	return 0x05;
}
