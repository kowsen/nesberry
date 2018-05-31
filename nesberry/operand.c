#include "operand.h"
#include "mmu.h"
#include "registers.h"

uint8 operand()
{
	return mmu_read(PC++);
}

uint16 operand_address_zero_page()
{
	return operand();
}

uint16 operand_address_zero_page_x()
{
	return (operand() + X) % 0x100;
}

uint16 operand_address_zero_page_y()
{
	return (operand() + Y) % 0x100;
}

uint16 operand_address_absolute()
{
	return operand() + (operand() << 8);
}

uint16 operand_address_absolute_x(bool* is_extra_cycle)
{
	uint16 address = operand() + X;
	if (address > 0xFF && is_extra_cycle != NULL)
	{
		*is_extra_cycle = TRUE;
	}
	address += (operand() << 8);
	return address;
}

uint16 operand_address_absolute_y(bool* is_extra_cycle)
{
	uint16 address = operand() + Y;
	if (address > 0xFF && is_extra_cycle != NULL)
	{
		*is_extra_cycle = TRUE;
	}
	address += (operand() << 8);
	return address;
}

uint16 operand_address_relative()
{
	int8 distance = operand();
	return PC + distance;
}

uint16 operand_address_indirect()
{
	uint16 address = operand_address_absolute();
	return mmu_read(address) + (mmu_read(address + 1) << 8);
}

uint16 operand_address_indirect_x()
{
	uint8 operand_value = operand();
	uint16 address = mmu_read((operand_value + X) % 0x100) + mmu_read(((operand_value + X + 1) % 0x100) << 8);
	return address;
}

uint16 operand_address_indirect_y(bool* is_extra_cycle)
{
	uint8 operand_value = operand();
	uint16 address = mmu_read(operand_value) + Y;
	if (address > 0xFF && is_extra_cycle != NULL)
	{
		*is_extra_cycle = TRUE;
	}
	address += mmu_read((operand_value + 1) % 0x100) << 8;
	return address;
}