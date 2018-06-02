#include "cartridge.h"
#include "cpu.h"

int main()
{
	int success = cart_init("C:\\dev\\nesberry-support\\all_instrs.nes");
	cpu_init();

	uint32 cycles = 0;

	while (1)
	{
		cycles += cpu_step();
	}
	return 0;
}