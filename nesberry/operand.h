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

uint8 operand_value_zero_page();
uint8 operand_value_zero_page_x();
uint8 operand_value_zero_page_y();
uint8 operand_value_absolute();
uint8 operand_value_absolute_x(bool* is_extra_cycle);
uint8 operand_value_absolute_y(bool* is_extra_cycle);
uint8 operand_value_relative();
uint8 operand_value_indirect();
uint8 operand_value_indirect_x();
uint8 operand_value_indirect_y(bool* is_extra_cycle);