#include "instructions.h"
#include "registers.h"

uint8 instruction_jmp()
{
	PC = mmu_read(PC + 1) | (mmu_read(PC + 2) << 8);
}
