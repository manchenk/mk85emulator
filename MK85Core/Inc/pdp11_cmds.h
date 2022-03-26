/*
 * This file is part of the MK85Emulator distribution (https://github.com/manchenk/mk85emulator.git).
 * Copyright (c) 2022 Dmitry Romanchenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "pdp11_cpu.h"

void pdp11_cpu_command_halt(pdp11_cpu_t* cpu, uint16_t op_code);      // 000001
void pdp11_cpu_command_wait(pdp11_cpu_t* cpu, uint16_t op_code);      // 000001
void pdp11_cpu_command_rti(pdp11_cpu_t* cpu, uint16_t op_code);       // 000002
void pdp11_cpu_command_bpt(pdp11_cpu_t* cpu, uint16_t op_code);       // 000003
void pdp11_cpu_command_iot(pdp11_cpu_t* cpu, uint16_t op_code);       // 000004
void pdp11_cpu_command_reset(pdp11_cpu_t* cpu, uint16_t op_code);     // 000005
void pdp11_cpu_command_rtt(pdp11_cpu_t* cpu, uint16_t op_code);       // 000006
void pdp11_cpu_command_jmp(pdp11_cpu_t* cpu, uint16_t op_code);       // 0001DD
void pdp11_cpu_command_rts(pdp11_cpu_t* cpu, uint16_t op_code);       // 00020R
void pdp11_cpu_command_clc(pdp11_cpu_t* cpu, uint16_t op_code);       // 000240
void pdp11_cpu_command_sec(pdp11_cpu_t* cpu, uint16_t op_code);       // 000260
void pdp11_cpu_command_swab(pdp11_cpu_t* cpu, uint16_t op_code);      // 0003DD
void pdp11_cpu_command_br(pdp11_cpu_t* cpu, uint16_t op_code);        // 0004XX 
void pdp11_cpu_command_bne(pdp11_cpu_t* cpu, uint16_t op_code);       // 0010XX
void pdp11_cpu_command_beq(pdp11_cpu_t* cpu, uint16_t op_code);       // 0014XX
void pdp11_cpu_command_bge(pdp11_cpu_t* cpu, uint16_t op_code);       // 0020XX
void pdp11_cpu_command_blt(pdp11_cpu_t* cpu, uint16_t op_code);       // 0024XX
void pdp11_cpu_command_bgt(pdp11_cpu_t* cpu, uint16_t op_code);       // 0030XX
void pdp11_cpu_command_ble(pdp11_cpu_t* cpu, uint16_t op_code);       // 0034XX
void pdp11_cpu_command_jsr(pdp11_cpu_t* cpu, uint16_t op_code);       // 004RDD
void pdp11_cpu_command_clr(pdp11_cpu_t* cpu, uint16_t op_code);       // 0050DD
void pdp11_cpu_command_com(pdp11_cpu_t* cpu, uint16_t op_code);       // 0051DD
void pdp11_cpu_command_inc(pdp11_cpu_t* cpu, uint16_t op_code);       // 0052DD
void pdp11_cpu_command_dec(pdp11_cpu_t* cpu, uint16_t op_code);       // 0053DD
void pdp11_cpu_command_neg(pdp11_cpu_t* cpu, uint16_t op_code);       // 0054DD
void pdp11_cpu_command_adc(pdp11_cpu_t* cpu, uint16_t op_code);       // 0055DD
void pdp11_cpu_command_sbc(pdp11_cpu_t* cpu, uint16_t op_code);       // 0056DD
void pdp11_cpu_command_tst(pdp11_cpu_t* cpu, uint16_t op_code);       // 0057DD
void pdp11_cpu_command_ror(pdp11_cpu_t* cpu, uint16_t op_code);       // 0060DD
void pdp11_cpu_command_rol(pdp11_cpu_t* cpu, uint16_t op_code);       // 0061DD
void pdp11_cpu_command_asr(pdp11_cpu_t* cpu, uint16_t op_code);       // 0062DD
void pdp11_cpu_command_asl(pdp11_cpu_t* cpu, uint16_t op_code);       // 0063DD
void pdp11_cpu_command_mark(pdp11_cpu_t* cpu, uint16_t op_code);      // 006400
void pdp11_cpu_command_sxt(pdp11_cpu_t* cpu, uint16_t op_code);       // 0067DD
void pdp11_cpu_command_mov(pdp11_cpu_t* cpu, uint16_t op_code);       // 01SSDD
void pdp11_cpu_command_cmp(pdp11_cpu_t* cpu, uint16_t op_code);       // 02SSDD 
void pdp11_cpu_command_bit(pdp11_cpu_t* cpu, uint16_t op_code);       // 03SSDD
void pdp11_cpu_command_bic(pdp11_cpu_t* cpu, uint16_t op_code);       // 04SSDD
void pdp11_cpu_command_bis(pdp11_cpu_t* cpu, uint16_t op_code);       // 05SSDD
void pdp11_cpu_command_add(pdp11_cpu_t* cpu, uint16_t op_code);       // 06SSDD
void pdp11_cpu_command_mul(pdp11_cpu_t* cpu, uint16_t op_code);       // 070RSS
void pdp11_cpu_command_div(pdp11_cpu_t* cpu, uint16_t op_code);       // 071RSS
void pdp11_cpu_command_ash(pdp11_cpu_t* cpu, uint16_t op_code);       // 072RSS
void pdp11_cpu_command_ashc(pdp11_cpu_t* cpu, uint16_t op_code);      // 073RSS
void pdp11_cpu_command_xor(pdp11_cpu_t* cpu, uint16_t op_code);       // 074RDD
void pdp11_cpu_command_fadd(pdp11_cpu_t* cpu, uint16_t op_code);      // 07500R
void pdp11_cpu_command_fsub(pdp11_cpu_t* cpu, uint16_t op_code);      // 07501R
void pdp11_cpu_command_fmul(pdp11_cpu_t* cpu, uint16_t op_code);      // 07502R
void pdp11_cpu_command_fdiv(pdp11_cpu_t* cpu, uint16_t op_code);      // 07503R
void pdp11_cpu_command_sob(pdp11_cpu_t* cpu, uint16_t op_code);       // 077R00
void pdp11_cpu_command_bpl(pdp11_cpu_t* cpu, uint16_t op_code);       // 1000XX
void pdp11_cpu_command_bmi(pdp11_cpu_t* cpu, uint16_t op_code);       // 1004XX
void pdp11_cpu_command_bhi(pdp11_cpu_t* cpu, uint16_t op_code);       // 1010XX
void pdp11_cpu_command_blos(pdp11_cpu_t* cpu, uint16_t op_code);      // 1014XX
void pdp11_cpu_command_bvc(pdp11_cpu_t* cpu, uint16_t op_code);       // 1020XX
void pdp11_cpu_command_bvs(pdp11_cpu_t* cpu, uint16_t op_code);       // 1024XX
void pdp11_cpu_command_bcc(pdp11_cpu_t* cpu, uint16_t op_code);       // 1030XX
void pdp11_cpu_command_bcs(pdp11_cpu_t* cpu, uint16_t op_code);       // 1034XX
void pdp11_cpu_command_emt(pdp11_cpu_t* cpu, uint16_t op_code);       // 1040XX
void pdp11_cpu_command_trap(pdp11_cpu_t* cpu, uint16_t op_code);      // 1044XX
void pdp11_cpu_command_clrb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1050DD
void pdp11_cpu_command_comb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1051DD
void pdp11_cpu_command_incb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1052DD
void pdp11_cpu_command_decb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1053DD
void pdp11_cpu_command_negb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1054DD
void pdp11_cpu_command_adcb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1055DD
void pdp11_cpu_command_sbcb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1056DD
void pdp11_cpu_command_tstb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1057DD
void pdp11_cpu_command_rorb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1060DD
void pdp11_cpu_command_rolb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1061DD
void pdp11_cpu_command_asrb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1062DD
void pdp11_cpu_command_aslb(pdp11_cpu_t* cpu, uint16_t op_code);      // 1063DD
void pdp11_cpu_command_mtps(pdp11_cpu_t* cpu, uint16_t op_code);      // 1064SS
void pdp11_cpu_command_mfps(pdp11_cpu_t* cpu, uint16_t op_code);      // 1067DD
void pdp11_cpu_command_movb(pdp11_cpu_t* cpu, uint16_t op_code);      // 11SSDD
void pdp11_cpu_command_cmpb(pdp11_cpu_t* cpu, uint16_t op_code);      // 12SSDD
void pdp11_cpu_command_bitb(pdp11_cpu_t* cpu, uint16_t op_code);     // 13SSDD
void pdp11_cpu_command_bicb(pdp11_cpu_t* cpu, uint16_t op_code);      // 14SSDD
void pdp11_cpu_command_bisb(pdp11_cpu_t* cpu, uint16_t op_code);      // 15SSDD
void pdp11_cpu_command_sub(pdp11_cpu_t* cpu, uint16_t op_code);       // 16SSDD
void pdp11_cpu_command_trap10(pdp11_cpu_t* cpu, uint16_t op_code);

#ifdef __cplusplus
}
#endif

