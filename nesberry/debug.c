#include "debug.h"
#include <stdio.h>

#include "typedefs.h"
#include "registers.h"

void print_status(uint8 opcode)
{
	printf("%04X  %02X  A:%02X X:%02X Y:%02X P:%02X SP:%02X\n", registers.pc, opcode, registers.a, registers.x, registers.y, registers_get_status(0), registers.s);
}