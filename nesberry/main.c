#include "cartridge.h"
#include "mmu.h"

int main()
{
	int success = cart_init("C:\\dev\\nesberry\\nestest.nes");
	mmu_init();
	uint8 check = mmu_read(0x8000);
	mmu_write(0x0010, 0x69);
	uint8 check2 = mmu_read(0x0009);
	uint8 check3 = mmu_read(0x0010);
	return 0;
}