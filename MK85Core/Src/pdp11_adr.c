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
#include <stddef.h>

#include "pdp11_adr.h"
#include "pdp11_cpu.h"

typedef uint16_t(*pdp11_cpu_get_address_t)(struct pdp11_cpu* cpu, int idx);

static uint16_t pdp11_cpu_mode_0(struct pdp11_cpu* cpu, int idx)     // Direct
{
    return 0;
}

static uint16_t pdp11_cpu_mode_1_word(struct pdp11_cpu* cpu, int idx)     // Indirect
{
    return PDP11_CPU_R(cpu, idx) & ~1;
}

static uint16_t pdp11_cpu_mode_1_byte(struct pdp11_cpu* cpu, int idx)     // Indirect
{
    return PDP11_CPU_R(cpu, idx);
}

static uint16_t pdp11_cpu_mode_2_word(struct pdp11_cpu * cpu, int idx)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, idx) & ~1;
    PDP11_CPU_R(cpu, idx) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_byte(struct pdp11_cpu * cpu, int idx)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, idx);
    PDP11_CPU_R(cpu, idx) += 1;
    return adr;
}

static uint16_t pdp11_cpu_mode_3(struct pdp11_cpu* cpu, int idx)     // AutoIncrementIndirect
{
    uint16_t adr_ptr = PDP11_CPU_R(cpu, idx);
    PDP11_CPU_R(cpu, idx) += 2;
    return pdp11_bus_read_word(cpu->bus, adr_ptr);
}

static uint16_t pdp11_cpu_mode_4_word(struct pdp11_cpu* cpu, int idx)     // AutoDecrement
{
    PDP11_CPU_R(cpu, idx) -= 2;
    return PDP11_CPU_R(cpu, idx) & ~1;
}

static uint16_t pdp11_cpu_mode_4_byte(struct pdp11_cpu* cpu, int idx)     // AutoDecrement
{
    PDP11_CPU_R(cpu, idx) -= 1;
    return PDP11_CPU_R(cpu, idx);
}

static uint16_t pdp11_cpu_mode_5(struct pdp11_cpu* cpu, int idx)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, idx) -= 2;
    uint16_t adr_ptr = PDP11_CPU_R(cpu, idx);
    return pdp11_bus_read_word(cpu->bus, adr_ptr);
}

static uint16_t pdp11_cpu_mode_6(struct pdp11_cpu* cpu, int idx)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    return ofs + PDP11_CPU_R(cpu, idx);
}

static uint16_t pdp11_cpu_mode_7(struct pdp11_cpu* cpu, int idx)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, idx));
    return pdp11_bus_read_word(cpu->bus, adr_ptr);
}

static pdp11_cpu_get_address_t pdp11_cpu_address_modes_word[] = {
    pdp11_cpu_mode_0,
    pdp11_cpu_mode_1_word,
    pdp11_cpu_mode_2_word,
    pdp11_cpu_mode_3,
    pdp11_cpu_mode_4_word,
    pdp11_cpu_mode_5,
    pdp11_cpu_mode_6,
    pdp11_cpu_mode_7
};

static pdp11_cpu_get_address_t pdp11_cpu_address_modes_byte[] = {
    pdp11_cpu_mode_0,
    pdp11_cpu_mode_1_byte,
    pdp11_cpu_mode_2_byte,
    pdp11_cpu_mode_3,
    pdp11_cpu_mode_4_byte,
    pdp11_cpu_mode_5,
    pdp11_cpu_mode_6,
    pdp11_cpu_mode_7
};

uint16_t pdp11_cpu_get_address_word(struct pdp11_cpu* cpu, int idx, pdp11_cpu_addressing_t mode)
{
    pdp11_cpu_get_address_t mode_function = pdp11_cpu_address_modes_word[mode];
    return mode_function(cpu, idx);
}

uint16_t pdp11_cpu_get_address_byte(struct pdp11_cpu* cpu, int idx, pdp11_cpu_addressing_t mode)
{
    pdp11_cpu_get_address_t mode_function = idx > 5 ? pdp11_cpu_address_modes_word[mode] : pdp11_cpu_address_modes_byte[mode];
    return mode_function(cpu, idx);
}
