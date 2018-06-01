#define PC_START_OVERRIDE 0xC91C

#include "cpu.h"
#include "registers.h"
#include "mmu.h"
#include "instructions.h"
#include "operand.h"

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

uint8 extra_cycle;

// Might switch to these later, but I feel like they make things more confusing even though they're more concise.

//#define INSTRUCTION(OP, ADDRESS_OR_VALUE, ADDRESS_TYPE, CYCLES) instruction_##OP##(operand_##ADDRESS_OR_VALUE##_##ADDRESS_TYPE##());\
//                                                                return CYCLES;
//
//#define INSTRUCTION_EXTRA_CYCLE(OP, ADDRESS_OR_VALUE, ADDRESS_TYPE, CYCLES) instruction_##OP##(operand_##ADDRESS_OR_VALUE##_##ADDRESS_TYPE##(&extra_cycle));\
//                                                                            return CYCLES + extra_cycle;


uint8 cpu_step()
{
	uint8 opcode = mmu_read(PC);
	PC += 1;
	switch (opcode)
	{

		case 0x4C:
			instruction_jmp(operand_address_absolute());
			return 0x03;

		case 0x61:
			instruction_adc(operand_value_indirect_x());
			return 0x06;

		case 0x65:
			instruction_adc(operand_value_zero_page());
			return 0x03;

		case 0x69:
			instruction_adc(operand());
			return 0x02;

		case 0x6C:
			instruction_jmp(operand_address_indirect());
			return 0x05;

		case 0x6D:
			instruction_adc(operand_value_absolute());
			return 0x04;

		case 0x71:
			instruction_adc(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0x75:
			instruction_adc(operand_value_zero_page_x());
			return 0x04;

		case 0x79:
			instruction_adc(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x7D:
			instruction_adc(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

	}

	// Unimplemented opcode
	return 0;
}
