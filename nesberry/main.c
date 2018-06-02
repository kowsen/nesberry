#include "cartridge.h"
#include "cpu.h"

int main()
{
	int success = cart_init("C:\\dev\\nesberry-support\\nestest.nes");
	cpu_init();

	uint8 test1 = 0b11111111;
	uint8 test2 = 0b10000000;

	uint8 test3 = test2 - test1;

	uint32 cycles = 0;

	uint32 counter = 0;

	while (counter < 8995)
	{
		cycles += cpu_step();
		counter += 1;
	}
	return 0;
}