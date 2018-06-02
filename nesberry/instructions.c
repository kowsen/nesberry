#include "instructions.h"
#include "registers.h"
#include "mmu.h"
#include "operand.h"

#define SIGN_MASK 0b10000000
#define OVERFLOW_MASK 0b01000000

void update_zn_flags(uint8 value)
{
	P.zero = value == 0;
	P.negative = (value & SIGN_MASK) != 0;
}

void instruction_adc(uint8 value)
{
	uint8 oldA = A;
	A = A + value + P.carry;
	P.carry = A < oldA || (A == oldA && value == 0xFF);
	P.overflow = ((oldA & SIGN_MASK) == (value & SIGN_MASK)) && ((oldA & SIGN_MASK) != (A & SIGN_MASK));
	update_zn_flags(A);
}

void instruction_and(uint8 value)
{
	A &= value;
	update_zn_flags(A);
}

void instruction_asl(uint16 address)
{
	uint8 value = mmu_read(address);
	P.carry = (value & SIGN_MASK) != 0;
	value = value << 1;
	mmu_write(address, value);
	//P.negative = (value & SIGN_MASK) != 0;
	//P.zero = A == 0;
	update_zn_flags(value);
}

void instruction_asl_accumulator()
{
	P.carry = (A & SIGN_MASK) != 0;
	A = A << 1;
	update_zn_flags(A);
}

uint8 branch_shared(int8 displacement)
{
	uint16 oldPC = PC;
	PC += displacement;
	if (((oldPC & 0x00FF) + displacement) & 0xFF00)
	{
		return 0x02;
	}
	return 0x01;
}

uint8 instruction_bcc(int8 displacement)
{
	if (!P.carry)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

uint8 instruction_bcs(int8 displacement)
{
	if (P.carry)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

uint8 instruction_beq(int8 displacement)
{
	if (P.zero)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

void instruction_bit(uint8 value)
{
	P.zero = (A & value) == 0;
	P.negative = (value & SIGN_MASK) != 0;
	P.overflow = (value & OVERFLOW_MASK) != 0;
}

uint8 instruction_bmi(int8 displacement)
{
	if (P.negative)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

uint8 instruction_bne(int8 displacement)
{
	if (!P.zero)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

uint8 instruction_bpl(int8 displacement)
{
	if (!P.negative)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

void push(uint8 value)
{
	mmu_write((uint16)0x0100 + S, value);
	S -= 1;
}

uint8 pull()
{
	S += 1;
	uint8 value = mmu_read((uint16)0x0100 + S);
	return value;
}

void instruction_brk()
{
	if (!P.interrupt)
	{
		push(PC >> 8);
		push(PC & 0xFF);
		push(registers_get_status(TRUE));
		PC = (mmu_read(0xFFFF) << 8) + mmu_read(0xFFFE);
	}
}

uint8 instruction_bvc(int8 displacement)
{
	if (!P.overflow)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

uint8 instruction_bvs(int8 displacement)
{
	if (P.overflow)
	{
		return branch_shared(displacement);
	}
	return 0x00;
}

void instruction_clc()
{
	P.carry = 0;
}

void instruction_cld()
{
	P.decimal = 0;
}

void instruction_cli()
{
	P.interrupt = 0;
}

void instruction_clv()
{
	P.overflow = 0;
}

void compare_shared(uint8 source, uint8 value)
{
	P.carry = source >= value;
	P.zero = source == value;
	P.negative = ((source - value) & SIGN_MASK) != 0;
}

void instruction_cmp(uint8 value)
{
	compare_shared(A, value);
}

void instruction_cpx(uint8 value)
{
	compare_shared(X, value);
}

void instruction_cpy(uint8 value)
{
	compare_shared(Y, value);
}

void instruction_dec(uint16 address)
{
	uint8 value = mmu_read(address) - 1;
	update_zn_flags(value);
	mmu_write(address, value);
}

void instruction_dex()
{
	X -= 1;
	update_zn_flags(X);
}

void instruction_dey()
{
	Y -= 1;
	update_zn_flags(Y);
}

void instruction_eor(uint8 value)
{
	A ^= value;
	update_zn_flags(A);
}

void instruction_inc(uint16 address)
{
	uint8 value = mmu_read(address) + 1;
	update_zn_flags(value);
	mmu_write(address, value);
}

void instruction_inx()
{
	X += 1;
	update_zn_flags(X);
}

void instruction_iny()
{
	Y += 1;
	update_zn_flags(Y);
}

void instruction_jmp(uint16 address)
{
	PC = address;
}

void instruction_jsr(uint16 address)
{
	uint16 return_address = PC - 1;
	push(return_address >> 8);
	push(return_address & 0xFF);
	PC = address;
}

void instruction_lda(uint8 value)
{
	A = value;
	update_zn_flags(A);
}

void instruction_ldx(uint8 value)
{
	X = value;
	update_zn_flags(X);
}

void instruction_ldy(uint8 value)
{
	Y = value;
	update_zn_flags(Y);
}

void instruction_lsr(uint16 address)
{
	uint8 value = mmu_read(address);
	P.carry = (value & 0x01) != 0;
	value = value >> 1;
	mmu_write(address, value);
	update_zn_flags(value);
}

void instruction_lsr_accumulator()
{
	P.carry = (A & 0x01) != 0;
	A = A >> 1;
	update_zn_flags(A);
}

void instruction_nop()
{
}

void instruction_ora(uint8 value)
{
	A |= value;
	update_zn_flags(A);
}

void instruction_pha()
{
	push(A);
}

void instruction_php()
{
	push(registers_get_status(TRUE));
}

void instruction_pla()
{
	A = pull();
	update_zn_flags(A);
}

void instruction_plp()
{
	registers_set_status(pull());
}

void instruction_rol(uint16 address)
{
	uint8 value = mmu_read(address);
	uint8 old_carry = P.carry;
	P.carry = (value & SIGN_MASK) != 0;
	value = (value << 1) + (old_carry ? 1 : 0);
	mmu_write(address, value);
	update_zn_flags(value);
}

void instruction_rol_accumulator()
{
	uint8 old_carry = P.carry;
	P.carry = (A & SIGN_MASK) != 0;
	A = (A << 1) + (old_carry != 0);
	update_zn_flags(A);
}

void instruction_ror(uint16 address)
{
	uint8 value = mmu_read(address);
	uint8 old_carry = P.carry;
	P.carry = (value & 0x01) != 0;
	value = (value >> 1) + (old_carry ? SIGN_MASK : 0);
	mmu_write(address, value);
	update_zn_flags(value);
}

void instruction_ror_accumulator()
{
	uint8 old_carry = P.carry;
	P.carry = (A & 0x01) != 0;
	A = (A >> 1) + (old_carry ? SIGN_MASK : 0);
	update_zn_flags(A);
}

void instruction_rti()
{
	registers_set_status(pull());
	PC = pull() + (pull() << 8);
}

void instruction_rts()
{
	PC = pull() + (pull() << 8);
	PC += 1;
}

void instruction_sbc(uint8 value)
{
	uint8 oldA = A;
	A = A - value - (P.carry ? 0 : 1);
	P.carry = A < oldA || (A == oldA && value == 0);
	P.overflow = ((value & SIGN_MASK) == (A & SIGN_MASK)) && ((oldA & SIGN_MASK) != (A & SIGN_MASK));
	update_zn_flags(A);
}

void instruction_sec()
{
	P.carry = TRUE;
}

void instruction_sed()
{
	P.decimal = TRUE;
}

void instruction_sei()
{
	P.interrupt = TRUE;
}

void instruction_sta(uint16 address)
{
	mmu_write(address, A);
}

void instruction_stx(uint16 address)
{
	mmu_write(address, X);
}

void instruction_sty(uint16 address)
{
	mmu_write(address, Y);
}

void instruction_tax()
{
	X = A;
	update_zn_flags(X);
}

void instruction_tay()
{
	Y = A;
	update_zn_flags(Y);
}

void instruction_tsx()
{
	X = S;
	update_zn_flags(X);
}

void instruction_txa()
{
	A = X;
	update_zn_flags(A);
}

void instruction_txs()
{
	S = X;
}

void instruction_tya()
{
	A = Y;
	update_zn_flags(A);
}
