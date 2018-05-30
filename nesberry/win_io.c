#include "win_io.h"

#include <stdio.h>

int io_load_rom(const char* filename, uint8* data)
{
	int size = 0;
	FILE *f = fopen(filename, "rb");
	if (f == NULL)
	{
		return -1;
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (size != fread(data, sizeof(uint8), size, f))
	{
		return -2;
	}
	fclose(f);
	data[size] = 0;
	return size;
}