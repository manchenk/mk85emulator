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
#include <malloc.h>
#include <string.h>
#include "pdp11_cpu.h"
#include "pdp11_cmds.h"

struct pdp11_cpu_selector_item;

typedef struct pdp11_cpu_selector_item {
    pdp11_cpu_command_t command;
    uint16_t mask;
    int shift;
    const struct pdp11_cpu_selector_item *items;
} pdp11_cpu_selector_item_t;

typedef void (*pdp11_cpu_command_func_t)(pdp11_cpu_t* cpu, uint16_t op_code);

typedef struct {
    pdp11_cpu_command_t command;
    char* name;
    bool_t is_byte;
    pdp11_cpu_command_class_t class;
    pdp11_cpu_command_func_t function;
} pdp11_cpu_command_item_t;

static const pdp11_cpu_command_item_t pdp11_cpu_commands[] = {
    { PDP11_COMMAND_UNKNOWN, "UNKNOWN", FALSE, PDP11_COMMAND_CLASS_ZERO,         NULL                     },
    { PDP11_COMMAND_HALT,    "HALT",    FALSE, PDP11_COMMAND_CLASS_ZERO,         pdp11_cpu_command_halt   },      // 000000 
    { PDP11_COMMAND_WAIT,    "WAIT",    FALSE, PDP11_COMMAND_CLASS_ZERO,         pdp11_cpu_command_wait   },      // 000001
    { PDP11_COMMAND_RTI,     "RTI",     FALSE, PDP11_COMMAND_CLASS_ZERO,         pdp11_cpu_command_rti    },      // 000002
    { PDP11_COMMAND_BPT,     "BPT",     FALSE, PDP11_COMMAND_CLASS_ZERO,         pdp11_cpu_command_bpt    },      // 000003
    { PDP11_COMMAND_IOT,     "IOT",     FALSE, PDP11_COMMAND_CLASS_ZERO,         pdp11_cpu_command_iot    },      // 000004
    { PDP11_COMMAND_RESET,   "RESET",   FALSE, PDP11_COMMAND_CLASS_ZERO,         pdp11_cpu_command_reset  },      // 000005
    { PDP11_COMMAND_RTT,     "RTT",     FALSE, PDP11_COMMAND_CLASS_ZERO,         pdp11_cpu_command_rtt    },      // 000006
    { PDP11_COMMAND_JMP,     "JMP",     FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_jmp    },      // 0001DD
    { PDP11_COMMAND_RTS,     "RTS",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_rts    },      // 00020R
    { PDP11_COMMAND_CLC,     "CLC",     FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_clc    },      // 000240
    { PDP11_COMMAND_SEC,     "SEC",     FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_sec    },      // 000260
    { PDP11_COMMAND_SWAB,    "SWAB",    FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_swab   },      // 0003DD
    { PDP11_COMMAND_BR,      "BR",      FALSE, PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_br     },      // 0004XX 
    { PDP11_COMMAND_BNE,     "BNE",     FALSE, PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bne    },      // 0010XX
    { PDP11_COMMAND_BEQ,     "BEQ",     FALSE, PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_beq    },      // 0014XX
    { PDP11_COMMAND_BGE,     "BGE",     FALSE, PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bge    },      // 0020XX
    { PDP11_COMMAND_BLT,     "BLT",     FALSE, PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_blt    },      // 0024XX
    { PDP11_COMMAND_BGT,     "BGT",     FALSE, PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bgt    },      // 0030XX
    { PDP11_COMMAND_BLE,     "BLE",     FALSE, PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_ble    },      // 0034XX
    { PDP11_COMMAND_JSR,     "JSR",     FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_jsr    },      // 004RDD
    { PDP11_COMMAND_CLR,     "CLR",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_clr    },      // 0050DD
    { PDP11_COMMAND_COM,     "COM",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_com    },      // 0051DD
    { PDP11_COMMAND_INC,     "INC",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_inc    },      // 0052DD
    { PDP11_COMMAND_DEC,     "DEC",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_dec    },      // 0053DD
    { PDP11_COMMAND_NEG,     "NEG",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_neg    },      // 0054DD
    { PDP11_COMMAND_ADC,     "ADC",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_adc    },      // 0055DD
    { PDP11_COMMAND_SBC,     "SBC",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_sbc    },      // 0056DD
    { PDP11_COMMAND_TST,     "TST",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_tst    },      // 0057DD
    { PDP11_COMMAND_ROR,     "ROR",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_ror    },      // 0060DD
    { PDP11_COMMAND_ROL,     "ROL",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_rol    },      // 0061DD
    { PDP11_COMMAND_ASR,     "ASR",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_asr    },      // 0062DD
    { PDP11_COMMAND_ASL,     "ASL",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_asl    },      // 0063DD
    { PDP11_COMMAND_MARK,    "MARK",    FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_mark   },      // 006400
    { PDP11_COMMAND_SXT,     "SXT",     FALSE, PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_sxt    },      // 0067DD
    { PDP11_COMMAND_MOV,     "MOV",     FALSE, PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_mov    },      // 01SSDD
    { PDP11_COMMAND_CMP,     "CMP",     FALSE, PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_cmp    },      // 02SSDD 
    { PDP11_COMMAND_BIT,     "BIT",     FALSE, PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_bit    },      // 03SSDD
    { PDP11_COMMAND_BIC,     "BIC",     FALSE, PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_bic    },      // 04SSDD
    { PDP11_COMMAND_BIS,     "BIS",     FALSE, PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_bis    },      // 05SSDD
    { PDP11_COMMAND_ADD,     "ADD",     TRUE,  PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_add    },      // 06SSDD
    { PDP11_COMMAND_MUL,     "MUL",     FALSE, PDP11_COMMAND_CLASS_ONE_AND_HALF, pdp11_cpu_command_mul    },      // 070RSS
    { PDP11_COMMAND_DIV,     "DIV",     FALSE, PDP11_COMMAND_CLASS_ONE_AND_HALF, pdp11_cpu_command_div    },      // 071RSS
    { PDP11_COMMAND_ASH,     "ASH",     FALSE, PDP11_COMMAND_CLASS_ONE_AND_HALF, pdp11_cpu_command_ash    },      // 072RSS
    { PDP11_COMMAND_ASHC,    "ASHC",    FALSE, PDP11_COMMAND_CLASS_ONE_AND_HALF, pdp11_cpu_command_ashc   },      // 073RSS
    { PDP11_COMMAND_XOR,     "XOR",     FALSE, PDP11_COMMAND_CLASS_ONE_AND_HALF, pdp11_cpu_command_xor    },      // 074RDD
    { PDP11_COMMAND_FADD,    "FADD",    FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_fadd   },      // 07500R
    { PDP11_COMMAND_FSUB,    "FSUB",    FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_fsub   },      // 07501R
    { PDP11_COMMAND_FMUL,    "FMUL",    FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_fmul   },      // 07502R
    { PDP11_COMMAND_FDIV,    "FDIV",    FALSE, PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_fdiv   },      // 07503R
    { PDP11_COMMAND_SOB,     "SOB",     TRUE,  PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_sob    },      // 077R00
    { PDP11_COMMAND_BPL,     "BPL",     TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bpl    },      // 1000XX
    { PDP11_COMMAND_BMI,     "BMI",     TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bmi    },      // 1004XX
    { PDP11_COMMAND_BHI,     "BHI",     TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bhi    },      // 1010XX
    { PDP11_COMMAND_BLOS,    "BLOS",    TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_blos   },      // 1014XX
    { PDP11_COMMAND_BVC,     "BVC",     TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bvc    },      // 1020XX
    { PDP11_COMMAND_BVS,     "BVS",     TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bvs    },      // 1024XX
    { PDP11_COMMAND_BCC,     "BCC",     TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bcc    },      // 1030XX
    { PDP11_COMMAND_BCS,     "BCS",     TRUE,  PDP11_COMMAND_CLASS_BRANCH,       pdp11_cpu_command_bcs    },      // 1034XX
    { PDP11_COMMAND_EMT,     "EMT",     TRUE,  PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_emt    },      // 1040XX
    { PDP11_COMMAND_TRAP,    "TRAP",    TRUE,  PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_trap   },      // 1044XX
    { PDP11_COMMAND_CLRB,    "CLRB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_clrb   },      // 1050DD
    { PDP11_COMMAND_COMB,    "COMB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_comb   },      // 1051DD
    { PDP11_COMMAND_INCB,    "INCB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_incb   },      // 1052DD
    { PDP11_COMMAND_DECB,    "DECB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_decb   },      // 1053DD
    { PDP11_COMMAND_NEGB,    "NEGB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_negb   },      // 1054DD
    { PDP11_COMMAND_ADCB,    "ADCB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_adcb   },      // 1055DD
    { PDP11_COMMAND_SBCB,    "SBCB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_sbcb   },      // 1056DD
    { PDP11_COMMAND_TSTB,    "TSTB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_tstb   },      // 1057DD
    { PDP11_COMMAND_RORB,    "RORB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_rorb   },      // 1060DD
    { PDP11_COMMAND_ROLB,    "ROLB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_rolb   },      // 1061DD
    { PDP11_COMMAND_ASRB,    "ASRB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_asrb   },      // 1062DD
    { PDP11_COMMAND_ASLB,    "ASLB",    TRUE,  PDP11_COMMAND_CLASS_ONE,          pdp11_cpu_command_aslb   },      // 1063DD
    { PDP11_COMMAND_MTPS,    "MTPS",    TRUE,  PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_mtps   },      // 1064SS
    { PDP11_COMMAND_MFPS,    "MFPS",    TRUE,  PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_mfps   },      // 1067DD
    { PDP11_COMMAND_MOVB,    "MOVB",    TRUE,  PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_movb   },      // 11SSDD
    { PDP11_COMMAND_CMPB,    "CMPB",    TRUE,  PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_cmpb   },      // 12SSDD
    { PDP11_COMMAND_BITB,    "BITB",    TRUE,  PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_bitb   },      // 13SSDD
    { PDP11_COMMAND_BICB,    "BICB",    TRUE,  PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_bicb   },      // 14SSDD
    { PDP11_COMMAND_BISB,    "BISB",    TRUE,  PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_bisb   },      // 15SSDD
    { PDP11_COMMAND_SUB,     "SUB",     TRUE,  PDP11_COMMAND_CLASS_TWO,          pdp11_cpu_command_sub    },      // 16SSDD
    { PDP11_COMMAND_TRAP10,  "TRAP10",  TRUE,  PDP11_COMMAND_CLASS_OTHER,        pdp11_cpu_command_trap10 }
};


static const pdp11_cpu_selector_item_t pdp11_cpu_selector_000_00_77[] = {
    { PDP11_COMMAND_HALT, 0, 0, NULL },  // 00
    { PDP11_COMMAND_WAIT, 0, 0, NULL },  // 01
    { PDP11_COMMAND_RTI, 0, 0, NULL },  // 02
    { PDP11_COMMAND_BPT, 0, 0, NULL },  // 03
    { PDP11_COMMAND_IOT, 0, 0, NULL },  // 04
    { PDP11_COMMAND_RESET, 0, 0, NULL },  // 05
    { PDP11_COMMAND_RTT, 0, 0, NULL },  // 06
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 07
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 10
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 11
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 12
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 13
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 14
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 15
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 16
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 17
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 20
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 21
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 22
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 23
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 24
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 25
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 26
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 27
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 30
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 31
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 32
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 33
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 34
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 35
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 36
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 37
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 40
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 41
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 42
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 43
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 44
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 45
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 46
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 47
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 50
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 51
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 52
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 53
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 54
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 55
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 56
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 57
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 60
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 61
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 62
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 63
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 64
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 65
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 66
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 67
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 70
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 71
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 72
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 73
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 74
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 75
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 76
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 77
};

static const pdp11_cpu_selector_item_t pdp11_cpu_selector_000_02_60[] = {
    { PDP11_COMMAND_RTS, 0, 0, NULL },    // 00
    { PDP11_COMMAND_TRAP10, 0, 0, NULL }, // 01
    { PDP11_COMMAND_CLC, 0, 0, NULL },    // 02
    { PDP11_COMMAND_SEC, 0, 0, NULL },    // 03
};

static const pdp11_cpu_selector_item_t pdp11_cpu_selector_000_77[] = {
    { PDP11_COMMAND_UNKNOWN, 077, 0, pdp11_cpu_selector_000_00_77 },  // 00
    { PDP11_COMMAND_JMP, 0, 0, NULL },  // 01
    { PDP11_COMMAND_UNKNOWN, 060, 4, pdp11_cpu_selector_000_02_60  },  // 02
    { PDP11_COMMAND_SWAB, 0, 0, NULL },  // 03
    { PDP11_COMMAND_BR, 0, 0, NULL },   // 04
    { PDP11_COMMAND_BR, 0, 0, NULL },   // 05
    { PDP11_COMMAND_BR, 0, 0, NULL },   // 06
    { PDP11_COMMAND_BR, 0, 0, NULL },   // 07
    { PDP11_COMMAND_BNE, 0, 0, NULL },  // 10
    { PDP11_COMMAND_BNE, 0, 0, NULL },  // 11
    { PDP11_COMMAND_BNE, 0, 0, NULL },  // 12
    { PDP11_COMMAND_BNE, 0, 0, NULL },  // 13
    { PDP11_COMMAND_BEQ, 0, 0, NULL },  // 14
    { PDP11_COMMAND_BEQ, 0, 0, NULL },  // 15
    { PDP11_COMMAND_BEQ, 0, 0, NULL },  // 16
    { PDP11_COMMAND_BEQ, 0, 0, NULL },  // 17
    { PDP11_COMMAND_BGE, 0, 0, NULL },  // 20
    { PDP11_COMMAND_BGE, 0, 0, NULL },  // 21
    { PDP11_COMMAND_BGE, 0, 0, NULL },  // 22
    { PDP11_COMMAND_BGE, 0, 0, NULL },  // 23
    { PDP11_COMMAND_BLT, 0, 0, NULL },  // 24
    { PDP11_COMMAND_BLT, 0, 0, NULL },  // 25
    { PDP11_COMMAND_BLT, 0, 0, NULL },  // 26
    { PDP11_COMMAND_BLT, 0, 0, NULL },  // 27
    { PDP11_COMMAND_BGT, 0, 0, NULL },  // 30
    { PDP11_COMMAND_BGT, 0, 0, NULL },  // 31
    { PDP11_COMMAND_BGT, 0, 0, NULL },  // 32
    { PDP11_COMMAND_BGT, 0, 0, NULL },  // 33
    { PDP11_COMMAND_BLE, 0, 0, NULL },  // 34
    { PDP11_COMMAND_BLE, 0, 0, NULL },  // 35
    { PDP11_COMMAND_BLE, 0, 0, NULL },  // 36
    { PDP11_COMMAND_BLE, 0, 0, NULL },  // 37
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 40
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 41
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 42
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 43
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 44
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 45
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 46
    { PDP11_COMMAND_JSR, 0, 0, NULL },  // 47
    { PDP11_COMMAND_CLR, 0, 0, NULL },  // 50
    { PDP11_COMMAND_COM, 0, 0, NULL },  // 51
    { PDP11_COMMAND_INC, 0, 0, NULL },  // 52
    { PDP11_COMMAND_DEC, 0, 0, NULL },  // 53
    { PDP11_COMMAND_NEG, 0, 0, NULL },  // 54
    { PDP11_COMMAND_ADC, 0, 0, NULL },  // 55
    { PDP11_COMMAND_SBC, 0, 0, NULL },  // 56
    { PDP11_COMMAND_TST, 0, 0, NULL },  // 57
    { PDP11_COMMAND_ROR, 0, 0, NULL },  // 60
    { PDP11_COMMAND_ROL, 0, 0, NULL },  // 61
    { PDP11_COMMAND_ASR, 0, 0, NULL },  // 62
    { PDP11_COMMAND_ASL, 0, 0, NULL },  // 63
    { PDP11_COMMAND_MARK, 0, 0, NULL },    // 64
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 65
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 66
    { PDP11_COMMAND_SXT, 0, 0, NULL },     // 67
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 70
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 71
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 72
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 73
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 74
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 75
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 76
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 77
};

static const pdp11_cpu_selector_item_t pdp11_cpu_selector_007_5_07[] = {
    { PDP11_COMMAND_FADD, 0, 0, NULL },  // 00
    { PDP11_COMMAND_FSUB, 0, 0, NULL },  // 01
    { PDP11_COMMAND_FMUL, 0, 0, NULL },  // 02
    { PDP11_COMMAND_FDIV, 0, 0, NULL },  // 03
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 04
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 05
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 06
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 07
};

static const pdp11_cpu_selector_item_t pdp11_cpu_selector_007_57[] = {
    { PDP11_COMMAND_UNKNOWN, 070, 3, pdp11_cpu_selector_007_5_07 },  // 00
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 01
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 02
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 03
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 04
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 05
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 06
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 07
};

static const pdp11_cpu_selector_item_t pdp11_cpu_selector_007_7[] = {
    { PDP11_COMMAND_MUL, 0, 0, NULL },  // 00
    { PDP11_COMMAND_DIV, 0, 0, NULL },  // 01
    { PDP11_COMMAND_ASH, 0, 0, NULL },  // 02
    { PDP11_COMMAND_ASHC, 0, 0, NULL },  // 03
    { PDP11_COMMAND_XOR, 0, 0, NULL },  // 04
    { PDP11_COMMAND_UNKNOWN, 0700, 6,  pdp11_cpu_selector_007_57},  // 05
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 06
    { PDP11_COMMAND_SOB, 0, 0, NULL }   // 07
};

static const pdp11_cpu_selector_item_t pdp11_cpu_selector_010_77[] = {
    { PDP11_COMMAND_BPL, 0, 0, NULL },  // 00
    { PDP11_COMMAND_BPL, 0, 0, NULL },  // 01
    { PDP11_COMMAND_BPL, 0, 0, NULL },  // 02
    { PDP11_COMMAND_BPL, 0, 0, NULL },  // 03
    { PDP11_COMMAND_BMI, 0, 0, NULL },  // 04
    { PDP11_COMMAND_BMI, 0, 0, NULL },  // 05
    { PDP11_COMMAND_BMI, 0, 0, NULL },  // 06
    { PDP11_COMMAND_BMI, 0, 0, NULL },  // 07
    { PDP11_COMMAND_BHI, 0, 0, NULL },  // 10
    { PDP11_COMMAND_BHI, 0, 0, NULL },  // 11
    { PDP11_COMMAND_BHI, 0, 0, NULL },  // 12
    { PDP11_COMMAND_BHI, 0, 0, NULL },  // 13
    { PDP11_COMMAND_BLOS, 0, 0, NULL },  // 14
    { PDP11_COMMAND_BLOS, 0, 0, NULL },  // 15
    { PDP11_COMMAND_BLOS, 0, 0, NULL },  // 16
    { PDP11_COMMAND_BLOS, 0, 0, NULL },  // 17
    { PDP11_COMMAND_BVC, 0, 0, NULL },  // 20
    { PDP11_COMMAND_BVC, 0, 0, NULL },  // 21
    { PDP11_COMMAND_BVC, 0, 0, NULL },  // 22
    { PDP11_COMMAND_BVC, 0, 0, NULL },  // 23
    { PDP11_COMMAND_BVS, 0, 0, NULL },  // 24
    { PDP11_COMMAND_BVS, 0, 0, NULL },  // 25
    { PDP11_COMMAND_BVS, 0, 0, NULL },  // 26
    { PDP11_COMMAND_BVS, 0, 0, NULL },  // 27
    { PDP11_COMMAND_BCC, 0, 0, NULL },  // 30
    { PDP11_COMMAND_BCC, 0, 0, NULL },  // 31
    { PDP11_COMMAND_BCC, 0, 0, NULL },  // 32
    { PDP11_COMMAND_BCC, 0, 0, NULL },  // 33
    { PDP11_COMMAND_BCS, 0, 0, NULL },  // 34
    { PDP11_COMMAND_BCS, 0, 0, NULL },  // 35
    { PDP11_COMMAND_BCS, 0, 0, NULL },  // 36
    { PDP11_COMMAND_BCS, 0, 0, NULL },  // 37
    { PDP11_COMMAND_EMT, 0, 0, NULL },  // 40
    { PDP11_COMMAND_EMT, 0, 0, NULL },  // 41
    { PDP11_COMMAND_EMT, 0, 0, NULL },  // 42
    { PDP11_COMMAND_EMT, 0, 0, NULL },  // 43
    { PDP11_COMMAND_TRAP, 0, 0, NULL },  // 44
    { PDP11_COMMAND_TRAP, 0, 0, NULL },  // 45
    { PDP11_COMMAND_TRAP, 0, 0, NULL },  // 46
    { PDP11_COMMAND_TRAP, 0, 0, NULL },  // 47
    { PDP11_COMMAND_CLRB, 0, 0, NULL },  // 50
    { PDP11_COMMAND_COMB, 0, 0, NULL },  // 51
    { PDP11_COMMAND_INCB, 0, 0, NULL },  // 52
    { PDP11_COMMAND_DECB, 0, 0, NULL },  // 53
    { PDP11_COMMAND_NEGB, 0, 0, NULL },  // 54
    { PDP11_COMMAND_ADCB, 0, 0, NULL },  // 55
    { PDP11_COMMAND_SBCB, 0, 0, NULL },  // 56
    { PDP11_COMMAND_TSTB, 0, 0, NULL },  // 57
    { PDP11_COMMAND_RORB, 0, 0, NULL },  // 60
    { PDP11_COMMAND_ROLB, 0, 0, NULL },  // 61
    { PDP11_COMMAND_ASRB, 0, 0, NULL },  // 62
    { PDP11_COMMAND_ASLB, 0, 0, NULL },  // 63
    { PDP11_COMMAND_MTPS, 0, 0, NULL },  // 64
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 65
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 66
    { PDP11_COMMAND_MFPS, 0, 0, NULL },  // 67
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 70
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 71
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 72
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 73
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 74
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 75
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },  // 76
    { PDP11_COMMAND_TRAP10, 0, 0, NULL }   // 77
};


static const pdp11_cpu_selector_item_t pdp11_cpu_selector_017[] = {
    { PDP11_COMMAND_UNKNOWN, 07700, 6, pdp11_cpu_selector_000_77 },  // 00
    { PDP11_COMMAND_MOV, 0, 0, NULL },                              // 01
    { PDP11_COMMAND_CMP, 0, 0, NULL },                              // 02
    { PDP11_COMMAND_BIT, 0, 0, NULL },                              // 03
    { PDP11_COMMAND_BIC, 0, 0, NULL },                              // 04
    { PDP11_COMMAND_BIS, 0, 0, NULL },                              // 05
    { PDP11_COMMAND_ADD, 0, 0, NULL },                              // 06
    { PDP11_COMMAND_UNKNOWN, 07000, 9, pdp11_cpu_selector_007_7 },  // 07
    { PDP11_COMMAND_UNKNOWN, 07700, 6, pdp11_cpu_selector_010_77 }, // 10
    { PDP11_COMMAND_MOVB, 0, 0, NULL },                             // 11
    { PDP11_COMMAND_CMPB, 0, 0, NULL },                             // 12
    { PDP11_COMMAND_BITB, 0, 0, NULL },                             // 13
    { PDP11_COMMAND_BICB, 0, 0, NULL },                             // 14
    { PDP11_COMMAND_BISB, 0, 0, NULL },                             // 15
    { PDP11_COMMAND_SUB, 0, 0, NULL },                              // 16
    { PDP11_COMMAND_TRAP10, 0, 0, NULL },                           // 17
};

static const pdp11_cpu_selector_item_t pdp11_cpu_selector = {
    PDP11_COMMAND_UNKNOWN, 0170000, 12, pdp11_cpu_selector_017
};


uint16_t pdp11_cpu_get_psw(pdp11_cpu_t* cpu)
{
    uint16_t psw = 0;
    if (PDP11_CPU_FLAG_C(cpu)) psw |= PDP11_STATUS_C;
    if (PDP11_CPU_FLAG_V(cpu)) psw |= PDP11_STATUS_V;
    if (PDP11_CPU_FLAG_Z(cpu)) psw |= PDP11_STATUS_Z;
    if (PDP11_CPU_FLAG_N(cpu)) psw |= PDP11_STATUS_N;
    if (PDP11_CPU_FLAG_T(cpu)) psw |= PDP11_STATUS_T;
    if (PDP11_CPU_FLAG_I(cpu)) psw |= PDP11_STATUS_I;
    if (PDP11_CPU_FLAG_H(cpu)) psw |= PDP11_STATUS_H;
    return psw;
}

void pdp11_cpu_set_psw(pdp11_cpu_t* cpu, uint16_t psw)
{
    PDP11_CPU_FLAG_C(cpu) = psw & PDP11_STATUS_C ? TRUE : FALSE;
    PDP11_CPU_FLAG_V(cpu) = psw & PDP11_STATUS_V ? TRUE : FALSE;
    PDP11_CPU_FLAG_Z(cpu) = psw & PDP11_STATUS_Z ? TRUE : FALSE;
    PDP11_CPU_FLAG_N(cpu) = psw & PDP11_STATUS_N ? TRUE : FALSE;
    PDP11_CPU_FLAG_T(cpu) = psw & PDP11_STATUS_T ? TRUE : FALSE;
    PDP11_CPU_FLAG_I(cpu) = psw & PDP11_STATUS_I ? TRUE : FALSE;
    PDP11_CPU_FLAG_H(cpu) = psw & PDP11_STATUS_H ? TRUE : FALSE;
}



pdp11_result_t pdp11_cpu_create(pdp11_cpu_t** pcpu)
{
    PDP11_CHECK_RESULT(pcpu ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    pdp11_cpu_t* cpu = (pdp11_cpu_t*)malloc(sizeof(pdp11_cpu_t));
    PDP11_CHECK_RESULT(cpu ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);
    
    memset(cpu, 0, sizeof(pdp11_cpu_t));

    *pcpu = cpu;
    return PDP11_RESULT_OK;
}

pdp11_result_t pdp11_cpu_init(pdp11_cpu_t* cpu)
{
    PDP11_CHECK_RESULT(cpu ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(cpu->bus ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);

    cpu->sel = 0;

    return PDP11_RESULT_OK;
}

pdp11_result_t pdp11_cpu_done(pdp11_cpu_t* cpu)
{
    PDP11_CHECK_RESULT(cpu ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    return PDP11_RESULT_OK;
}

pdp11_result_t pdp11_cpu_reset(pdp11_cpu_t* cpu)
{
    PDP11_CHECK_RESULT(cpu ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    uint16_t addr = PDP11_CPU_SEL(cpu) & 0xff00;
    uint16_t init_pc = pdp11_bus_read_word(cpu->bus, addr);
    addr += 2;
    uint16_t init_psw = pdp11_bus_read_word(cpu->bus, addr);
    addr += 2;

    PDP11_CPU_PC(cpu) = init_pc;
    pdp11_cpu_set_psw(cpu, init_psw);

    return PDP11_RESULT_OK;
}


static pdp11_cpu_command_t pdp11_cpu_decode(uint16_t op_code)
{
    const pdp11_cpu_selector_item_t* item = &pdp11_cpu_selector;
    while (item) {
        int index = (op_code & item->mask) >> item->shift;
        item = &item->items[index];
        if (item->command != PDP11_COMMAND_UNKNOWN)
            return item->command;
    }
    return PDP11_COMMAND_UNKNOWN;
}

pdp11_result_t pdp11_cpu_step(pdp11_cpu_t* cpu)
{
    PDP11_CHECK_RESULT(cpu ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    uint16_t op_code;
    if (cpu->halt) {
        op_code = 0;
        cpu->halt = FALSE;
    }
    else {
        op_code = pdp11_bus_read_word(cpu->bus, PDP11_CPU_PC(cpu));
        PDP11_CPU_PC(cpu) += 2;
    }
    pdp11_cpu_command_t command = pdp11_cpu_decode(op_code);
    const pdp11_cpu_command_item_t* item = pdp11_cpu_commands + command;
    item->function(cpu, op_code);
    return PDP11_RESULT_OK;
}
