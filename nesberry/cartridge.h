#pragma once

#include "typedefs.h";

#define CART_MAX_ROM_SIZE (1024 * 1024)

#define CART_INIT_INVALID_FILE -2
#define CART_INIT_INVALID_ROM -1
#define CART_INIT_SUCCESS 0

struct {
	uint8 prg_num_pages;
	uint16 prg_start;
	uint16 chr_start;
	uint32 data_size;
	uint8 data[CART_MAX_ROM_SIZE];
} cartridge;

int cart_init(char* filename);
extern inline uint8 cart_get_prg(uint16 address);