#include "cartridge.h"
#include "cpu.h"

int main()
{
	int success = cart_init("C:\\dev\\nesberry-support\\nestest.nes");
	cpu_init();
	while (1)
	{
		cpu_step();
	}
	return 0;
}