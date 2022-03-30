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

#include "pdp11_types.h"
#include "pdp11_bus.h"
#include "pdp11_adr.h"

#define PDP11_CPU_CTRL_BASE 0x104
#define PDP11_CPU_CTRL_SIZE 2


#define PDP11_CPU_REGISTER_NUM 8
#define PDP11_CPU_SP_INDEX 6
#define PDP11_CPU_PC_INDEX 7

#define PDP11_CPU_PC(cpu) ((cpu)->r[PDP11_CPU_PC_INDEX])
#define PDP11_CPU_SP(cpu) ((cpu)->r[PDP11_CPU_SP_INDEX])
#define PDP11_CPU_R(cpu,idx) ((cpu)->r[idx])
#define PDP11_CPU_SEL(cpu) ((cpu)->sel)
//#define PDP11_CPU_FLAG(cpu,flg) ((cpu)->flags[flg])
#define PDP11_CPU_FLAG_C(cpu) ((cpu)->flag_c)
#define PDP11_CPU_FLAG_V(cpu) ((cpu)->flag_v)
#define PDP11_CPU_FLAG_Z(cpu) ((cpu)->flag_z)
#define PDP11_CPU_FLAG_N(cpu) ((cpu)->flag_n)
#define PDP11_CPU_FLAG_T(cpu) ((cpu)->flag_t)
#define PDP11_CPU_FLAG_I(cpu) ((cpu)->flag_i)
#define PDP11_CPU_FLAG_H(cpu) ((cpu)->flag_h)

#define PDP11_CPU_CPC(cpu) ((cpu)->cpc)
#define PDP11_CPU_CPS(cpu) ((cpu)->cps)

#define PDP11_CPU_VECTOR(cpu) ((cpu)->vector)

#define PDP11_CPU_WAIT(cpu) ((cpu)->wait)
#define PDP11_CPU_RESET(cpu) ((cpu)->reset)
#define PDP11_CPU_RTT(cpu) ((cpu)->rtt)
#define PDP11_CPU_STEP(cpu) ((cpu)->step)

typedef enum pdp11_cpu_command {
    PDP11_COMMAND_UNKNOWN,
    PDP11_COMMAND_HALT,       // 000000 
    PDP11_COMMAND_WAIT,       // 000001
    PDP11_COMMAND_RTI,        // 000002
    PDP11_COMMAND_BPT,        // 000003
    PDP11_COMMAND_IOT,        // 000004
    PDP11_COMMAND_RESET,      // 000005
    PDP11_COMMAND_RTT,        // 000006
    PDP11_COMMAND_JMP,        // 0001DD
    PDP11_COMMAND_RTS,        // 00020R
    PDP11_COMMAND_CLC,        // 000240
    PDP11_COMMAND_SEC,        // 000260
    PDP11_COMMAND_SWAB,       // 0003DD
    PDP11_COMMAND_BR,         // 0004XX 
    PDP11_COMMAND_BNE,        // 0010XX
    PDP11_COMMAND_BEQ,        // 0014XX
    PDP11_COMMAND_BGE,        // 0020XX
    PDP11_COMMAND_BLT,        // 0024XX
    PDP11_COMMAND_BGT,        // 0030XX
    PDP11_COMMAND_BLE,        // 0034XX
    PDP11_COMMAND_JSR,        // 004RDD
    PDP11_COMMAND_CLR,        // 0050DD
    PDP11_COMMAND_COM,        // 0051DD
    PDP11_COMMAND_INC,        // 0052DD
    PDP11_COMMAND_DEC,        // 0053DD
    PDP11_COMMAND_NEG,        // 0054DD
    PDP11_COMMAND_ADC,        // 0055DD
    PDP11_COMMAND_SBC,        // 0056DD
    PDP11_COMMAND_TST,        // 0057DD
    PDP11_COMMAND_ROR,        // 0060DD
    PDP11_COMMAND_ROL,        // 0061DD
    PDP11_COMMAND_ASR,        // 0062DD
    PDP11_COMMAND_ASL,        // 0063DD
    PDP11_COMMAND_MARK,       // 006400
    PDP11_COMMAND_SXT,        // 0067DD
    PDP11_COMMAND_MOV,        // 01SSDD
    PDP11_COMMAND_CMP,        // 02SSDD 
    PDP11_COMMAND_BIT,        // 03SSDD
    PDP11_COMMAND_BIC,        // 04SSDD
    PDP11_COMMAND_BIS,        // 05SSDD
    PDP11_COMMAND_ADD,        // 06SSDD
    PDP11_COMMAND_MUL,        // 070RSS
    PDP11_COMMAND_DIV,        // 071RSS
    PDP11_COMMAND_ASH,        // 072RSS
    PDP11_COMMAND_ASHC,       // 073RSS
    PDP11_COMMAND_XOR,        // 074RDD
    PDP11_COMMAND_FADD,       // 07500R
    PDP11_COMMAND_FSUB,       // 07501R
    PDP11_COMMAND_FMUL,       // 07502R
    PDP11_COMMAND_FDIV,       // 07503R
    PDP11_COMMAND_SOB,        // 077R00
    PDP11_COMMAND_BPL,        // 1000XX
    PDP11_COMMAND_BMI,        // 1004XX
    PDP11_COMMAND_BHI,        // 1010XX
    PDP11_COMMAND_BLOS,       // 1014XX
    PDP11_COMMAND_BVC,        // 1020XX
    PDP11_COMMAND_BVS,        // 1024XX
    PDP11_COMMAND_BCC,        // 1030XX
    PDP11_COMMAND_BCS,        // 1034XX
    PDP11_COMMAND_EMT,        // 1040XX
    PDP11_COMMAND_TRAP,       // 1044XX
    PDP11_COMMAND_CLRB,       // 1050DD
    PDP11_COMMAND_COMB,       // 1051DD
    PDP11_COMMAND_INCB,       // 1052DD
    PDP11_COMMAND_DECB,       // 1053DD
    PDP11_COMMAND_NEGB,       // 1054DD
    PDP11_COMMAND_ADCB,       // 1055DD
    PDP11_COMMAND_SBCB,       // 1056DD
    PDP11_COMMAND_TSTB,       // 1057DD
    PDP11_COMMAND_RORB,       // 1060DD
    PDP11_COMMAND_ROLB,       // 1061DD
    PDP11_COMMAND_ASRB,       // 1062DD
    PDP11_COMMAND_ASLB,       // 1063DD
    PDP11_COMMAND_MTPS,       // 1064SS
    PDP11_COMMAND_MFPS,       // 1067DD
    PDP11_COMMAND_MOVB,       // 11SSDD
    PDP11_COMMAND_CMPB,       // 12SSDD
    PDP11_COMMAND_BITB,       // 13SSDD
    PDP11_COMMAND_BICB,       // 14SSDD
    PDP11_COMMAND_BISB,       // 15SSDD
    PDP11_COMMAND_SUB,        // 16SSDD
    PDP11_COMMAND_TRAP10
} pdp11_cpu_command_t;

typedef enum {
    PDP11_STATUS_C = 0x001,
    PDP11_STATUS_V = 0x002,
    PDP11_STATUS_Z = 0x004,
    PDP11_STATUS_N = 0x008,
    PDP11_STATUS_T = 0x010,
    PDP11_STATUS_I = 0x080,
    PDP11_STATUS_H = 0x100
} pdp11_cpu_status_t;

typedef enum {
    PDP11_FLAG_C = 0,
    PDP11_FLAG_V = 1,
    PDP11_FLAG_Z = 2,
    PDP11_FLAG_N = 3,
    PDP11_FLAG_T = 4,
    PDP11_FLAG_I = 7,
    PDP11_FLAG_H = 8,
    PDP11_FLAG_MAX = 9
} pdp11_cpu_flags_t;

typedef enum {
    PDP11_COMMAND_CLASS_ZERO,
    PDP11_COMMAND_CLASS_ONE,
    PDP11_COMMAND_CLASS_ONE_AND_HALF,
    PDP11_COMMAND_CLASS_TWO,
    PDP11_COMMAND_CLASS_BRANCH,
    PDP11_COMMAND_CLASS_OTHER
} pdp11_cpu_command_class_t;

#define PDP11_RAW_FLAG_N 0x80000000
#define PDP11_RAW_FLAG_Z 0x40000000
#define PDP11_RAW_FLAG_C 0x20000000

typedef struct pdp11_cpu {
    bool_t rtt;
    bool_t wait;
    bool_t reset;
    bool_t step;
    bool_t halt;

    uint32_t raw_flags;
    bool_t flag_c;  //PDP11_FLAG_C = 0,
    bool_t flag_v;  //PDP11_FLAG_V = 1,
    bool_t flag_z;  //PDP11_FLAG_Z = 2,
    bool_t flag_n;  //PDP11_FLAG_N = 3,
    bool_t flag_t;  //PDP11_FLAG_T = 4,
    bool_t flag_i;  //PDP11_FLAG_I = 7,
    bool_t flag_h;  //PDP11_FLAG_H = 8,

    uint16_t cpc;
    uint16_t cps;
    uint16_t sel;
    uint16_t r[PDP11_CPU_REGISTER_NUM];

    uint16_t vector;

    pdp11_bus_t* bus;
    pdp11_device_t* pc_dev;
    uint16_t pc_ofs;
} pdp11_cpu_t;


pdp11_result_t pdp11_cpu_create(pdp11_cpu_t **cpu);
pdp11_result_t pdp11_cpu_init(pdp11_cpu_t *cpu);
pdp11_result_t pdp11_cpu_done(pdp11_cpu_t *cpu);
pdp11_result_t pdp11_cpu_reset(pdp11_cpu_t* cpu);

uint16_t pdp11_cpu_get_psw(pdp11_cpu_t* cpu);
void pdp11_cpu_set_psw(pdp11_cpu_t* cpu, uint16_t psw);

pdp11_result_t pdp11_cpu_step(pdp11_cpu_t* cpu);


#ifdef __cplusplus
}
#endif

