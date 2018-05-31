#define PC_START_OVERRIDE 0xC000

#include "cpu.h"
#include "registers.h"
#include "mmu.h"
#include "instructions.h"

void cpu_init()
{
	mmu_init();
	registers_init();
	#ifdef PC_START_OVERRIDE
		PC = PC_START_OVERRIDE;
	#else
		PC = (mmu_read(0xFFFD) << 8) | mmu_read(0xFFFC);
	#endif
}

uint8 cpu_step()
{
	uint8 opcode = mmu_read(PC);
	PC += 1;
	switch (opcode)
	{
		case 0x4C: return instruction_jmp_absolute();
		case 0x6C: return instruction_jmp_indirect();
	}
	return 0;
}
