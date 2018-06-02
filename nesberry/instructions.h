#pragma once

#include "typedefs.h"

void instruction_adc(uint8 value);
void instruction_and(uint8 value);
void instruction_asl(uint16 address);
void instruction_asl_accumulator();
uint8 instruction_bcc(int8 displacement);
uint8 instruction_bcs(int8 displacement);
uint8 instruction_beq(int8 displacement);
void instruction_bit(uint8 value);
uint8 instruction_bmi(int8 displacement);
uint8 instruction_bne(int8 displacement);
uint8 instruction_bpl(int8 displacement);
void instruction_brk();
uint8 instruction_bvc(int8 displacement);
uint8 instruction_bvs(int8 displacement);
void instruction_clc();
void instruction_cld();
void instruction_cli();
void instruction_clv();
void instruction_cmp(uint8 value);
void instruction_cpx(uint8 value);
void instruction_cpy(uint8 value);
void instruction_dec(uint16 address);
void instruction_dex();
void instruction_dey();
void instruction_eor(uint8 value);
void instruction_inc(uint16 address);
void instruction_inx();
void instruction_iny();
void instruction_jmp(uint16 address);
void instruction_jsr(uint16 address);
void instruction_lda(uint8 value);
void instruction_ldx(uint8 value);
void instruction_ldy(uint8 value);
void instruction_lsr(uint16 address);
void instruction_lsr_accumulator();
void instruction_nop();
void instruction_ora(uint8 value);
void instruction_pha();
void instruction_php();
void instruction_pla();
void instruction_plp();
void instruction_rol(uint16 address);
void instruction_rol_accumulator();
void instruction_ror(uint16 address);
void instruction_ror_accumulator();
void instruction_rti();
void instruction_rts();
void instruction_sbc(uint8 value);
void instruction_sec();
void instruction_sed();
void instruction_sei();
void instruction_sta(uint16 address);
void instruction_stx(uint16 address);
void instruction_sty(uint16 address);
void instruction_tax();
void instruction_tay();
void instruction_tsx();
void instruction_txa();
void instruction_txs();
void instruction_tya();

// illegal opcodes
