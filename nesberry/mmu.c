#include "mmu.h"
#include "cartridge.h"

void mmu_init()
{
	for (uint32 i = 0; i < MMU_INTERNAL_RAM_SIZE; i++)
	{
		internal_ram[i] = 0;
	}
}

uint8 mmu_read(uint16 address)
{
	if (address < 0x2000)
	{
		return internal_ram[address % 0x0800];
	}
	else if (address >= 0x4020)
	{
		return cart_get_cpu(address);
	}
	else
	{
		return 0;
	}
}

void mmu_write(uint16 address, uint8 data)
{
	if (address < 0x2000)
	{
		internal_ram[address % 0x0800] = data;
	}
	else if (address >= 0x4020)
	{
		cart_write_cpu(address, data);
	}
}
