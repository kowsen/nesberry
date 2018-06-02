#define PC_START_OVERRIDE 0xC000

#include "cpu.h"
#include "registers.h"
#include "mmu.h"
#include "instructions.h"
#include "operand.h"

#include "debug.h"

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
	print_status(opcode);
	PC += 1;
	switch (opcode)
	{
		case 0x00:
			instruction_brk();
			return 0x07;

		case 0x01:
			instruction_ora(operand_value_indirect_x());
			return 0x06;

		case 0x03:
			instruction_slo(operand_address_indirect_x());
			return 0x08;

		case 0x04:
			instruction_dop(operand());
			return 0x03;

		case 0x05:
			instruction_ora(operand_value_zero_page());
			return 0x03;

		case 0x06:
			instruction_asl(operand_address_zero_page());
			return 0x05;

		case 0x07:
			instruction_slo(operand_address_zero_page());
			return 0x05;

		case 0x08:
			instruction_php();
			return 0x03;

		case 0x09:
			instruction_ora(operand());
			return 0x02;

		case 0x0A:
			instruction_asl_accumulator();
			return 0x02;

		case 0x0B:
			instruction_aac(operand());
			return 0x02;

		case 0x0C:
			instruction_top(operand_address_absolute());
			return 0x04;

		case 0x0D:
			instruction_ora(operand_value_absolute());
			return 0x04;

		case 0x0E:
			instruction_asl(operand_address_absolute());
			return 0x06;

		case 0x0F:
			instruction_slo(operand_address_absolute());
			return 0x06;

		case 0x10:
			return 0x02 + instruction_bpl(operand());

		case 0x11:
			instruction_ora(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0x13:
			instruction_slo(operand_address_indirect_y(&extra_cycle));
			return 0x08;

		case 0x14:
			instruction_dop(operand());
			return 0x04;

		case 0x15:
			instruction_ora(operand_value_zero_page_x());
			return 0x04;

		case 0x16:
			instruction_asl(operand_address_zero_page_x());
			return 0x06;

		case 0x17:
			instruction_slo(operand_address_zero_page_x());
			return 0x06;

		case 0x18:
			instruction_clc();
			return 0x02;

		case 0x19:
			instruction_ora(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x1A:
			instruction_nop();
			return 0x02;

		case 0x1B:
			instruction_slo(operand_address_absolute_y(&extra_cycle));
			return 0x07;

		case 0x1C:
			instruction_top(operand_address_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x1D:
			instruction_ora(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x1E:
			instruction_asl(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x1F:
			instruction_slo(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x20:
			instruction_jsr(operand_address_absolute());
			return 0x06;

		case 0x21:
			instruction_and(operand_value_indirect_x());
			return 0x06;

		case 0x23:
			instruction_rla(operand_address_indirect_x());
			return 0x08;

		case 0x24:
			instruction_bit(operand_value_zero_page());
			return 0x03;

		case 0x25:
			instruction_and(operand_value_zero_page());
			return 0x03;

		case 0x26:
			instruction_rol(operand_address_zero_page());
			return 0x05;

		case 0x27:
			instruction_rla(operand_address_zero_page());
			return 0x05;

		case 0x28:
			instruction_plp();
			return 0x04;

		case 0x29:
			instruction_and(operand());
			return 0x02;

		case 0x2A:
			instruction_rol_accumulator();
			return 0x02;

		case 0x2B:
			instruction_aac(operand());
			return 0x02;

		case 0x2C:
			instruction_bit(operand_value_absolute());
			return 0x04;

		case 0x2D:
			instruction_and(operand_value_absolute());
			return 0x04;

		case 0x2E:
			instruction_rol(operand_address_absolute());
			return 0x06;

		case 0x2F:
			instruction_rla(operand_address_absolute());
			return 0x06;

		case 0x30:
			return 0x02 + instruction_bmi(operand());

		case 0x31:
			instruction_and(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0x33:
			instruction_rla(operand_address_indirect_y(&extra_cycle));
			return 0x08;

		case 0x34:
			instruction_dop(operand());
			return 0x04;

		case 0x35:
			instruction_and(operand_value_zero_page_x());
			return 0x04;

		case 0x36:
			instruction_rol(operand_address_zero_page_x());
			return 0x06;

		case 0x37:
			instruction_rla(operand_address_zero_page_x());
			return 0x06;

		case 0x38:
			instruction_sec();
			return 0x02;

		case 0x39:
			instruction_and(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x3A:
			instruction_nop();
			return 0x02;

		case 0x3B:
			instruction_rla(operand_address_absolute_y(&extra_cycle));
			return 0x07;

		case 0x3C:
			instruction_top(operand_address_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x3D:
			instruction_and(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x3E:
			instruction_rol(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x3F:
			instruction_rla(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x40:
			instruction_rti();
			return 0x06;
			
		case 0x41:
			instruction_eor(operand_value_indirect_x());
			return 0x06;

		case 0x43:
			instruction_sre(operand_address_indirect_x());
			return 0x08;

		case 0x44:
			instruction_dop(operand());
			return 0x03;

		case 0x45:
			instruction_eor(operand_value_zero_page());
			return 0x03;

		case 0x46:
			instruction_lsr(operand_address_zero_page());
			return 0x05;

		case 0x47:
			instruction_sre(operand_address_zero_page());
			return 0x05;

		case 0x48:
			instruction_pha();
			return 0x03;

		case 0x49:
			instruction_eor(operand());
			return 0x02;

		case 0x4A:
			instruction_lsr_accumulator();
			return 0x02;

		case 0x4C:
			instruction_jmp(operand_address_absolute());
			return 0x03;

		case 0x4D:
			instruction_eor(operand_value_absolute());
			return 0x04;

		case 0x4E:
			instruction_lsr(operand_address_absolute());
			return 0x06;

		case 0x4F:
			instruction_sre(operand_address_absolute());
			return 0x06;

		case 0x50:
			return 0x02 + instruction_bvc(operand());

		case 0x51:
			instruction_eor(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0x53:
			instruction_sre(operand_address_indirect_y(&extra_cycle));
			return 0x08;

		case 0x54:
			instruction_dop(operand());
			return 0x04;

		case 0x55:
			instruction_eor(operand_value_zero_page_x());
			return 0x04;

		case 0x56:
			instruction_lsr(operand_address_zero_page_x());
			return 0x06;

		case 0x57:
			instruction_sre(operand_address_zero_page_x());
			return 0x06;

		case 0x58:
			instruction_cli();
			return 0x02;

		case 0x59:
			instruction_eor(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x5A:
			instruction_nop();
			return 0x02;

		case 0x5B:
			instruction_sre(operand_address_absolute_y(&extra_cycle));
			return 0x07;

		case 0x5C:
			instruction_top(operand_address_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x5D:
			instruction_eor(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x5E:
			instruction_lsr(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x5F:
			instruction_sre(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x60:
			instruction_rts();
			return 0x06;

		case 0x61:
			instruction_adc(operand_value_indirect_x());
			return 0x06;

		case 0x63:
			instruction_rra(operand_address_indirect_x());
			return 0x08;

		case 0x64:
			instruction_dop(operand());
			return 0x03;

		case 0x65:
			instruction_adc(operand_value_zero_page());
			return 0x03;

		case 0x66:
			instruction_ror(operand_address_zero_page());
			return 0x05;

		case 0x67:
			instruction_rra(operand_address_zero_page());
			return 0x05;

		case 0x68:
			instruction_pla();
			return 0x04;

		case 0x69:
			instruction_adc(operand());
			return 0x02;

		case 0x6A:
			instruction_ror_accumulator();
			return 0x02;

		case 0x6C:
			instruction_jmp(operand_address_indirect());
			return 0x05;

		case 0x6D:
			instruction_adc(operand_value_absolute());
			return 0x04;

		case 0x6E:
			instruction_ror(operand_address_absolute());
			return 0x06;

		case 0x6F:
			instruction_rra(operand_address_absolute());
			return 0x06;

		case 0x70:
			return 0x02 + instruction_bvs(operand());

		case 0x71:
			instruction_adc(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0x73:
			instruction_rra(operand_address_indirect_y(&extra_cycle));
			return 0x08;

		case 0x74:
			instruction_dop(operand());
			return 0x04;

		case 0x75:
			instruction_adc(operand_value_zero_page_x());
			return 0x04;

		case 0x76:
			instruction_ror(operand_address_zero_page_x());
			return 0x06;

		case 0x77:
			instruction_rra(operand_address_zero_page_x());
			return 0x06;

		case 0x78:
			instruction_sei();
			return 0x02;

		case 0x79:
			instruction_adc(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x7A:
			instruction_nop();
			return 0x02;

		case 0x7B:
			instruction_rra(operand_address_absolute_y(&extra_cycle));
			return 0x07;

		case 0x7C:
			instruction_top(operand_address_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x7D:
			instruction_adc(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0x7E:
			instruction_ror(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x7F:
			instruction_rra(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0x80:
			instruction_dop(operand());
			return 0x02;

		case 0x81:
			instruction_sta(operand_address_indirect_x());
			return 0x06;

		case 0x82:
			instruction_dop(operand());
			return 0x02;

		case 0x83:
			instruction_sax(operand_address_indirect_x());
			return 0x06;

		case 0x84:
			instruction_sty(operand_address_zero_page());
			return 0x03;

		case 0x85:
			instruction_sta(operand_address_zero_page());
			return 0x03;

		case 0x86:
			instruction_stx(operand_address_zero_page());
			return 0x03;

		case 0x87:
			instruction_sax(operand_address_zero_page());
			return 0x03;

		case 0x88:
			instruction_dey();
			return 0x02;

		case 0x89:
			instruction_dop(operand());
			return 0x02;

		case 0x8A:
			instruction_txa();
			return 0x02;

		case 0x8C:
			instruction_sty(operand_address_absolute());
			return 0x04;

		case 0x8D:
			instruction_sta(operand_address_absolute());
			return 0x04;

		case 0x8E:
			instruction_stx(operand_address_absolute());
			return 0x04;

		case 0x8F:
			instruction_sax(operand_address_absolute());
			return 0x04;

		case 0x90:
			return 0x02 + instruction_bcc(operand());

		case 0x91:
			instruction_sta(operand_address_indirect_y(&extra_cycle));
			return 0x06;

		case 0x94:
			instruction_sty(operand_address_zero_page_x());
			return 0x04;

		case 0x95:
			instruction_sta(operand_address_zero_page_x());
			return 0x04;

		case 0x96:
			instruction_stx(operand_address_zero_page_y());
			return 0x04;

		case 0x97:
			instruction_sax(operand_address_zero_page_y());
			return 0x04;

		case 0x98:
			instruction_tya();
			return 0x02;

		case 0x99:
			instruction_sta(operand_address_absolute_y(&extra_cycle));
			return 0x05;

		case 0x9A:
			instruction_txs();
			return 0x02;

		case 0x9D:
			instruction_sta(operand_address_absolute_x(&extra_cycle));
			return 0x05;

		case 0xA0:
			instruction_ldy(operand());
			return 0x02;

		case 0xA1:
			instruction_lda(operand_value_indirect_x());
			return 0x06;

		case 0xA2:
			instruction_ldx(operand());
			return 0x02;

		case 0xA3:
			instruction_lax(operand_value_indirect_x());
			return 0x06;

		case 0xA4:
			instruction_ldy(operand_value_zero_page());
			return 0x03;

		case 0xA5:
			instruction_lda(operand_value_zero_page());
			return 0x03;

		case 0xA6:
			instruction_ldx(operand_value_zero_page());
			return 0x03;

		case 0xA7:
			instruction_lax(operand_value_zero_page());
			return 0x03;

		case 0xA8:
			instruction_tay();
			return 0x02;

		case 0xA9:
			instruction_lda(operand());
			return 0x02;

		case 0xAA:
			instruction_tax();
			return 0x02;

		case 0xAC:
			instruction_ldy(operand_value_absolute());
			return 0x04;

		case 0xAD:
			instruction_lda(operand_value_absolute());
			return 0x04;

		case 0xAE:
			instruction_ldx(operand_value_absolute());
			return 0x04;

		case 0xAF:
			instruction_lax(operand_value_absolute());
			return 0x04;

		case 0xB0:
			return 0x02 + instruction_bcs(operand());

		case 0xB1:
			instruction_lda(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0xB3:
			instruction_lax(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0xB4:
			instruction_ldy(operand_value_zero_page_x());
			return 0x04;

		case 0xB5:
			instruction_lda(operand_value_zero_page_x());
			return 0x04;

		case 0xB6:
			instruction_ldx(operand_value_zero_page_y());
			return 0x04;

		case 0xB7:
			instruction_lax(operand_value_zero_page_y());
			return 0x04;

		case 0xB8:
			instruction_clv();
			return 0x02;

		case 0xB9:
			instruction_lda(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xBA:
			instruction_tsx();
			return 0x02;

		case 0xBC:
			instruction_ldy(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xBD:
			instruction_lda(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xBE:
			instruction_ldx(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xBF:
			instruction_lax(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xC0:
			instruction_cpy(operand());
			return 0x02;

		case 0xC1:
			instruction_cmp(operand_value_indirect_x());
			return 0x06;

		case 0xC2:
			instruction_dop(operand());
			return 0x02;

		case 0xC3:
			instruction_dcp(operand_address_indirect_x(&extra_cycle));
			return 0x08;

		case 0xC4:
			instruction_cpy(operand_value_zero_page());
			return 0x03;

		case 0xC5:
			instruction_cmp(operand_value_zero_page());
			return 0x03;

		case 0xC6:
			instruction_dec(operand_address_zero_page());
			return 0x05;

		case 0xC7:
			instruction_dcp(operand_address_zero_page());
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

		case 0xCF:
			instruction_dcp(operand_address_absolute());
			return 0x06;

		case 0xD0:
			return 0x02 + instruction_bne(operand());

		case 0xD1:
			instruction_cmp(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0xD3:
			instruction_dcp(operand_address_indirect_y(&extra_cycle));
			return 0x08;

		case 0xD4:
			instruction_dop(operand());
			return 0x04;

		case 0xD5:
			instruction_cmp(operand_value_zero_page_x());
			return 0x04;

		case 0xD6:
			instruction_dec(operand_address_zero_page_x());
			return 0x06;

		case 0xD7:
			instruction_dcp(operand_address_zero_page_x());
			return 0x06;

		case 0xD8:
			instruction_cld();
			return 0x02;

		case 0xD9:
			instruction_cmp(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xDA:
			instruction_nop();
			return 0x02;

		case 0xDB:
			instruction_dcp(operand_address_absolute_y(&extra_cycle));
			return 0x07;

		case 0xDC:
			instruction_top(operand_address_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xDD:
			instruction_cmp(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xDE:
			instruction_dec(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0xDF:
			instruction_dcp(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0xE0:
			instruction_cpx(operand());
			return 0x02;

		case 0xE1:
			instruction_sbc(operand_value_indirect_x());
			return 0x06;

		case 0xE2:
			instruction_dop(operand());
			return 0x02;

		case 0xE3:
			instruction_isb(operand_address_indirect_x());
			return 0x08;

		case 0xE4:
			instruction_cpx(operand_value_zero_page());
			return 0x03;

		case 0xE5:
			instruction_sbc(operand_value_zero_page());
			return 0x03;

		case 0xE6:
			instruction_inc(operand_address_zero_page());
			return 0x05;

		case 0xE7:
			instruction_isb(operand_address_zero_page());
			return 0x05;

		case 0xE8:
			instruction_inx();
			return 0x02;

		case 0xE9:
			instruction_sbc(operand());
			return 0x02;

		case 0xEA:
			instruction_nop();
			return 0x02;

		case 0xEB:
			instruction_sbc(operand());
			return 0x02;

		case 0xEC:
			instruction_cpx(operand_value_absolute());
			return 0x04;

		case 0xED:
			instruction_sbc(operand_value_absolute());
			return 0x04;

		case 0xEE:
			instruction_inc(operand_address_absolute());
			return 0x06;

		case 0xEF:
			instruction_isb(operand_address_absolute());
			return 0x06;

		case 0xF0:
			return 0x02 + instruction_beq(operand());

		case 0xF1:
			instruction_sbc(operand_value_indirect_y(&extra_cycle));
			return 0x05 + extra_cycle;

		case 0xF3:
			instruction_isb(operand_address_indirect_y(&extra_cycle));
			return 0x08;

		case 0xF4:
			instruction_dop(operand());
			return 0x04;

		case 0xF5:
			instruction_sbc(operand_value_zero_page_x());
			return 0x04;

		case 0xF6:
			instruction_inc(operand_address_zero_page_x());
			return 0x06;

		case 0xF7:
			instruction_isb(operand_address_zero_page_x());
			return 0x06;

		case 0xF8:
			instruction_sed();
			return 0x02;

		case 0xF9:
			instruction_sbc(operand_value_absolute_y(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xFA:
			instruction_nop();
			return 0x02;

		case 0xFB:
			instruction_isb(operand_address_absolute_y(&extra_cycle));
			return 0x07;

		case 0xFC:
			instruction_top(operand_address_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xFD:
			instruction_sbc(operand_value_absolute_x(&extra_cycle));
			return 0x04 + extra_cycle;

		case 0xFE:
			instruction_inc(operand_address_absolute_x(&extra_cycle));
			return 0x07;

		case 0xFF:
			instruction_isb(operand_address_absolute_x(&extra_cycle));
			return 0x07;

	}

	// Unimplemented opcode
	return 0;
}
