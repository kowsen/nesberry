#pragma once

#include "typedefs.h"

#define MMU_INTERNAL_RAM_SIZE 2048

uint8 internal_ram[MMU_INTERNAL_RAM_SIZE];

void mmu_init();
uint8 mmu_read(uint16 address);
void mmu_write(uint16 address, uint8 data);