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
	A += value;
	P.carry = A < oldA;
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
	P.negative = (value & SIGN_MASK) != 0;
	P.zero = A == 0;
}

void instruction_asl_accumulator()
{
	P.carry = (A & SIGN_MASK) != 0;
	A = A << 1;
	P.negative = (A & SIGN_MASK) != 0;
	P.zero = A == 0;
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
	uint8 value = mmu_read((uint16)0x0100 + S);
	S += 1;
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
