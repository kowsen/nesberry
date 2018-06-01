#define PC_START_OVERRIDE 0xC000

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
		case 0x00:
			instruction_brk();
			return 0x07;

		case 0x06:
			instruction_asl(operand_address_zero_page());
			return 0x05;

		case 0x0A:
			instruction_asl_accumulator();
			return 0x02;

		case 0x0E:
			instruction_asl(operand_address_absolute());
			return 0x06;

		case 0x10:
			return 0x02 + instruction_bpl(operand());

		case 0x16:
			instruction_asl(operand_address_zero_page_x());
			return 0x06;

		case 0x18:
			instruction_clc();
			return 0x02;

		case 0x1E:
			instruction_asl(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x20:
			instruction_jsr(operand_address_absolute());
			return 0x06;

		case 0x21:
			instruction_and(operand_value_indirect_x());
			return 0x06;

		case 0x24:
			instruction_bit(operand_value_zero_page());
			return 0x03;

		case 0x25:
			instruction_and(operand_value_zero_page());
			return 0x03;

		case 0x29:
			instruction_and(operand());
			return 0x02;

		case 0x2C:
			instruction_bit(operand_value_absolute());
			return 0x04;

		case 0x2D:
			instruction_and(operand_value_absolute());
			return 0x04;

		case 0x30:
			return 0x02 + instruction_bmi(operand());

		case 0x31:
			instruction_and(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0x35:
			instruction_and(operand_value_zero_page_x());
			return 0x04;

		case 0x39:
			instruction_and(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x3D:
			instruction_and(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;
			
		case 0x41:
			instruction_eor(operand_value_indirect_x());
			return 0x06;

		case 0x45:
			instruction_eor(operand_value_zero_page());
			return 0x03;

		case 0x49:
			instruction_eor(operand());
			return 0x02;

		case 0x4C:
			instruction_jmp(operand_address_absolute());
			return 0x03;

		case 0x4D:
			instruction_eor(operand_address_absolute());
			return 0x04;

		case 0x50:
			return 0x02 + instruction_bvc(operand());

		case 0x51:
			instruction_eor(operand_address_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0x55:
			instruction_eor(operand_value_zero_page_x());
			return 0x04;

		case 0x58:
			instruction_cli();
			return 0x02;

		case 0x59:
			instruction_eor(operand_address_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x5D:
			instruction_eor(operand_address_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

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

		case 0x70:
			return 0x02 + instruction_bvs(operand());

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

		case 0x88:
			instruction_dey();
			return 0x02;

		case 0xA1:
			instruction_lda(operand_value_indirect_x());
			return 0x06;

		case 0xA5:
			instruction_lda(operand_value_zero_page());
			return 0x03;

		case 0xA9:
			instruction_lda(operand());
			return 0x02;

		case 0xAD:
			instruction_lda(operand_address_absolute());
			return 0x04;

		case 0xB0:
			return 0x02 + instruction_bcc(operand());

		case 0xB1:
			instruction_lda(operand_address_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0xB5:
			instruction_lda(operand_value_zero_page_x());
			return 0x04;

		case 0xB8:
			instruction_clv();
			return 0x02;

		case 0xB9:
			instruction_lda(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xBD:
			instruction_lda(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xC0:
			instruction_cpy(operand());
			return 0x02;

		case 0xC1:
			instruction_cmp(operand_value_indirect_x());
			return 0x06;

		case 0xC4:
			instruction_cpy(operand_value_zero_page());
			return 0x03;

		case 0xC5:
			instruction_cmp(operand_value_zero_page());
			return 0x03;

		case 0xC6:
			instruction_dec(operand_address_zero_page());
			return 0x05;

		case 0xC8:
			instruction_iny();
			return 0x02;

		case 0xC9:
			instruction_cmp(operand());
			return 0x02;

		case 0xCA:
			instruction_dex();
			return 0x02;

		case 0xCC:
			instruction_cpy(operand_value_absolute());
			return 0x04;

		case 0xCD:
			instruction_cmp(operand_value_absolute());
			return 0x04;

		case 0xCE:
			instruction_dec(operand_address_absolute());
			return 0x06;

		case 0xD0:
			return 0x02 + instruction_bne(operand());

		case 0xD1:
			instruction_cmp(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0xD5:
			instruction_cmp(operand_value_zero_page_x());
			return 0x04;

		case 0xD6:
			instruction_dec(operand_address_zero_page_x());
			return 0x06;

		case 0xD9:
			instruction_cmp(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xDD:
			instruction_cmp(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xDE:
			instruction_dec(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0xE0:
			instruction_cpx(operand());
			return 0x02;

		case 0xE4:
			instruction_cpx(operand_value_zero_page());
			return 0x03;

		case 0xE6:
			instruction_inc(operand_address_zero_page());
			return 0x05;

		case 0xE8:
			instruction_inx();
			return 0x02;

		case 0xEC:
			instruction_cpx(operand_value_absolute());
			return 0x04;

		case 0xEE:
			instruction_inc(operand_address_absolute());
			return 0x06;

		case 0xF0:
			return 0x02 + instruction_beq(operand());

		case 0xF6:
			instruction_inc(operand_address_zero_page_x());
			return 0x06;

		case 0xFE:
			instruction_inc(operand_address_absolute_x(&extra_cycle));
			return 0x07;

	}

	// Unimplemented opcode
	return 0;
}
