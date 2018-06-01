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
	is_extra_cycle = address > 0xFF;
	address += (operand() << 8);
	return address;
}

uint16 operand_address_absolute_y(bool* is_extra_cycle)
{
	uint16 address = operand() + Y;
	is_extra_cycle = address > 0xFF;
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
	return mmu_read(address) + (mmu_read((address & 0xFF00) + ((address + 1) & 0xFF)) << 8);
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
	is_extra_cycle = address > 0xFF;
	address += mmu_read((operand_value + 1) % 0x100) << 8;
	return address;
}

uint8 operand_value_zero_page()
{
	return mmu_read(operand_address_zero_page());
}

uint8 operand_value_zero_page_x()
{
	return mmu_read(operand_address_zero_page_x());
}

uint8 operand_value_zero_page_y()
{
	return mmu_read(operand_address_zero_page_y());
}

uint8 operand_value_absolute()
{
	return mmu_read(operand_address_absolute());
}

uint8 operand_value_absolute_x(bool* is_extra_cycle)
{
	return mmu_read(operand_address_absolute_x(is_extra_cycle));
}

uint8 operand_value_absolute_y(bool* is_extra_cycle)
{
	return mmu_read(operand_address_absolute_y(is_extra_cycle));
}

uint8 operand_value_relative()
{
	return mmu_read(operand_address_relative());
}

uint8 operand_value_indirect()
{
	return mmu_read(operand_address_indirect());
}

uint8 operand_value_indirect_x()
{
	return mmu_read(operand_address_indirect_x());
}

uint8 operand_value_indirect_y(bool* is_extra_cycle)
{
	return mmu_read(operand_address_indirect_y(is_extra_cycle));
}
