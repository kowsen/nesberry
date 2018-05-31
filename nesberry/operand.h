#pragma once

#include "typedefs.h"

uint8 operand();
uint16 operand_address_zero_page();
uint16 operand_address_zero_page_x();
uint16 operand_address_zero_page_y();
uint16 operand_address_absolute();
uint16 operand_address_absolute_x(bool* is_extra_cycle);
uint16 operand_address_absolute_y(bool* is_extra_cycle);
uint16 operand_address_relative();
uint16 operand_address_indirect();
uint16 operand_address_indirect_x();
uint16 operand_address_indirect_y(bool* is_extra_cycle);