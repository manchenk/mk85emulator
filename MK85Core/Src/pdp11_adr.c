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


//static uint16_t pdp11_cpu_mode_0(struct pdp11_cpu* cpu)     // Direct
//{
//    return 0;
//}

static uint16_t pdp11_cpu_mode_1_reg_0_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 0) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_1_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 1) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_2_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 2) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_3_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 3) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_4_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 4) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_5_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 5) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_6_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 6) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_7_word(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 7) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_0_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 0);
}

static uint16_t pdp11_cpu_mode_1_reg_1_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 1);
}

static uint16_t pdp11_cpu_mode_1_reg_2_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 2);
}

static uint16_t pdp11_cpu_mode_1_reg_3_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 3);
}

static uint16_t pdp11_cpu_mode_1_reg_4_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 4);
}

static uint16_t pdp11_cpu_mode_1_reg_5_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 5);
}

static uint16_t pdp11_cpu_mode_1_reg_6_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 6) & ~1;
}

static uint16_t pdp11_cpu_mode_1_reg_7_byte(struct pdp11_cpu* cpu)     // Indirect
{
    return PDP11_CPU_R(cpu, 7) & ~1;
}

static uint16_t pdp11_cpu_mode_2_reg_0_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 0) & ~1;
    PDP11_CPU_R(cpu, 0) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_1_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 1) & ~1;
    PDP11_CPU_R(cpu, 1) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_2_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 2) & ~1;
    PDP11_CPU_R(cpu, 2) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_3_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 3) & ~1;
    PDP11_CPU_R(cpu, 3) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_4_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 4) & ~1;
    PDP11_CPU_R(cpu, 4) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_5_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 5) & ~1;
    PDP11_CPU_R(cpu, 5) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_6_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 6) & ~1;
    PDP11_CPU_R(cpu, 6) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_7_word(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 7) & ~1;
    PDP11_CPU_R(cpu, 7) += 2;
    cpu->pc_ofs += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_0_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 0);
    PDP11_CPU_R(cpu, 0) += 1;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_1_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 1);
    PDP11_CPU_R(cpu, 1) += 1;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_2_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 2);
    PDP11_CPU_R(cpu, 2) += 1;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_3_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 3);
    PDP11_CPU_R(cpu, 3) += 1;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_4_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 4);
    PDP11_CPU_R(cpu, 4) += 1;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_5_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 5);
    PDP11_CPU_R(cpu, 5) += 1;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_6_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 6) & ~1;
    PDP11_CPU_R(cpu, 6) += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_2_reg_7_byte(struct pdp11_cpu * cpu)     // AutoIncrement
{
    uint16_t adr = PDP11_CPU_R(cpu, 7) & ~1;
    PDP11_CPU_R(cpu, 7) += 2;
    cpu->pc_ofs += 2;
    return adr;
}

static uint16_t pdp11_cpu_mode_3_reg_0(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    uint16_t addr = PDP11_CPU_R(cpu, 0);
    PDP11_CPU_R(cpu, 0) += 2;
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_3_reg_1(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    uint16_t addr = PDP11_CPU_R(cpu, 1);
    PDP11_CPU_R(cpu, 1) += 2;
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_3_reg_2(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    uint16_t addr = PDP11_CPU_R(cpu, 2);
    PDP11_CPU_R(cpu, 2) += 2;
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_3_reg_3(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    uint16_t addr = PDP11_CPU_R(cpu, 3);
    PDP11_CPU_R(cpu, 3) += 2;
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_3_reg_4(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    uint16_t addr = PDP11_CPU_R(cpu, 4);
    PDP11_CPU_R(cpu, 4) += 2;
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_3_reg_5(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    uint16_t addr = PDP11_CPU_R(cpu, 5);
    PDP11_CPU_R(cpu, 5) += 2;
    // return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_3_reg_6(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    uint16_t addr = PDP11_CPU_R(cpu, 6);
    PDP11_CPU_R(cpu, 6) += 2;
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_3_reg_7(struct pdp11_cpu* cpu)     // AutoIncrementIndirect
{
    //uint16_t adr_ptr = PDP11_CPU_R(cpu, 7);
    //PDP11_CPU_R(cpu, 7) += 2;
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    PDP11_CPU_R(cpu, 7) += 2;
    uint16_t addr = cpu->pc_ofs;
    cpu->pc_ofs += 2;
    return cpu->pc_dev->read_word(cpu->pc_dev, addr);
}

static uint16_t pdp11_cpu_mode_4_reg_0_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 0) -= 2;
    return PDP11_CPU_R(cpu, 0) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_1_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 1) -= 2;
    return PDP11_CPU_R(cpu, 1) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_2_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 2) -= 2;
    return PDP11_CPU_R(cpu, 2) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_3_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 3) -= 2;
    return PDP11_CPU_R(cpu, 3) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_4_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 4) -= 2;
    return PDP11_CPU_R(cpu, 4) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_5_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 5) -= 2;
    return PDP11_CPU_R(cpu, 5) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_6_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 6) -= 2;
    return PDP11_CPU_R(cpu, 6) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_7_word(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 7) -= 2;
    cpu->pc_ofs -= 2;
    return PDP11_CPU_R(cpu, 7) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_0_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 0) -= 1;
    return PDP11_CPU_R(cpu, 0);
}

static uint16_t pdp11_cpu_mode_4_reg_1_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 1) -= 1;
    return PDP11_CPU_R(cpu, 1);
}

static uint16_t pdp11_cpu_mode_4_reg_2_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 2) -= 1;
    return PDP11_CPU_R(cpu, 2);
}

static uint16_t pdp11_cpu_mode_4_reg_3_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 3) -= 1;
    return PDP11_CPU_R(cpu, 3);
}

static uint16_t pdp11_cpu_mode_4_reg_4_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 4) -= 1;
    return PDP11_CPU_R(cpu, 4);
}

static uint16_t pdp11_cpu_mode_4_reg_5_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 5) -= 1;
    return PDP11_CPU_R(cpu, 5);
}

static uint16_t pdp11_cpu_mode_4_reg_6_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 6) -= 2;
    return PDP11_CPU_R(cpu, 6) & ~1;
}

static uint16_t pdp11_cpu_mode_4_reg_7_byte(struct pdp11_cpu* cpu)     // AutoDecrement
{
    PDP11_CPU_R(cpu, 7) -= 2;
    cpu->pc_ofs -= 2;
    return PDP11_CPU_R(cpu, 7) & ~1;
}

static uint16_t pdp11_cpu_mode_5_reg_0(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, 0) -= 2;
    uint16_t addr = PDP11_CPU_R(cpu, 0);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_5_reg_1(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, 1) -= 2;
    uint16_t addr = PDP11_CPU_R(cpu, 1);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_5_reg_2(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, 2) -= 2;
    uint16_t addr = PDP11_CPU_R(cpu, 2);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_5_reg_3(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, 3) -= 2;
    uint16_t addr = PDP11_CPU_R(cpu, 3);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_5_reg_4(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, 4) -= 2;
    uint16_t addr = PDP11_CPU_R(cpu, 4);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_5_reg_5(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, 5) -= 2;
    uint16_t addr = PDP11_CPU_R(cpu, 5);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_5_reg_6(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    PDP11_CPU_R(cpu, 6) -= 2;
    uint16_t addr = PDP11_CPU_R(cpu, 6);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, addr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, ofs);
}

static uint16_t pdp11_cpu_mode_5_reg_7(struct pdp11_cpu* cpu)     // AutoDecrementIndirect:
{
    //PDP11_CPU_R(cpu, 7) -= 2;
    //uint16_t adr_ptr = PDP11_CPU_R(cpu, 7);
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);
    PDP11_CPU_R(cpu, 7) -= 2;
    cpu->pc_ofs -= 2;
    return cpu->pc_dev->read_word(cpu->pc_dev, cpu->pc_ofs);
}

static uint16_t pdp11_cpu_mode_6_reg_0(struct pdp11_cpu* cpu)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
    //uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs = dev->read_word(dev, ofs);
    return ofs + PDP11_CPU_R(cpu, 0);
}

static uint16_t pdp11_cpu_mode_6_reg_1(struct pdp11_cpu* cpu)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs = dev->read_word(dev, ofs);
    return ofs + PDP11_CPU_R(cpu, 1);
}

static uint16_t pdp11_cpu_mode_6_reg_2(struct pdp11_cpu* cpu)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs = dev->read_word(dev, ofs);
    return ofs + PDP11_CPU_R(cpu, 2);
}

static uint16_t pdp11_cpu_mode_6_reg_3(struct pdp11_cpu* cpu)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs = dev->read_word(dev, ofs);
    return ofs + PDP11_CPU_R(cpu, 3);
}

static uint16_t pdp11_cpu_mode_6_reg_4(struct pdp11_cpu* cpu)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs = dev->read_word(dev, ofs);
    return ofs + PDP11_CPU_R(cpu, 4);
}

static uint16_t pdp11_cpu_mode_6_reg_5(struct pdp11_cpu* cpu)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs = dev->read_word(dev, ofs);
    return ofs + PDP11_CPU_R(cpu, 5);
}

static uint16_t pdp11_cpu_mode_6_reg_6(struct pdp11_cpu* cpu)     // Index
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    uint16_t ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs = dev->read_word(dev, ofs);
    return ofs + PDP11_CPU_R(cpu, 6);
}

static uint16_t pdp11_cpu_mode_6_reg_7(struct pdp11_cpu* cpu)     // Index
{
    //uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    //PDP11_CPU_PC(cpu) += 2;
    //uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    //return ofs + PDP11_CPU_R(cpu, 7);

    uint16_t ofs_ptr = cpu->pc_ofs;
    cpu->pc_ofs += 2;
    PDP11_CPU_PC(cpu) += 2;
    uint16_t ofs = cpu->pc_dev->read_word(cpu->pc_dev, ofs_ptr);
    return ofs + PDP11_CPU_R(cpu, 7);
}

static uint16_t pdp11_cpu_mode_7_reg_0(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
//    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 0));
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs_ptr = dev->read_word(dev, dev_ofs);
    ofs_ptr += PDP11_CPU_R(cpu, 0);
    dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

static uint16_t pdp11_cpu_mode_7_reg_1(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
//    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 1));
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs_ptr = dev->read_word(dev, dev_ofs);
    ofs_ptr += PDP11_CPU_R(cpu, 1);
    dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

static uint16_t pdp11_cpu_mode_7_reg_2(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
//    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 2));
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs_ptr = dev->read_word(dev, dev_ofs);
    ofs_ptr += PDP11_CPU_R(cpu, 2);
    dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

static uint16_t pdp11_cpu_mode_7_reg_3(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
//    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 3));
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs_ptr = dev->read_word(dev, dev_ofs);
    ofs_ptr += PDP11_CPU_R(cpu, 3);
    dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

static uint16_t pdp11_cpu_mode_7_reg_4(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
//    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 4));
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs_ptr = dev->read_word(dev, dev_ofs);
    ofs_ptr += PDP11_CPU_R(cpu, 4);
    dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

static uint16_t pdp11_cpu_mode_7_reg_5(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
//    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 5));
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs_ptr = dev->read_word(dev, dev_ofs);
    ofs_ptr += PDP11_CPU_R(cpu, 5);
    dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

static uint16_t pdp11_cpu_mode_7_reg_6(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    PDP11_CPU_PC(cpu) += 2;
//    uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
//    uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 6));
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    ofs_ptr = dev->read_word(dev, dev_ofs);
    ofs_ptr += PDP11_CPU_R(cpu, 6);
    dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, ofs_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

static uint16_t pdp11_cpu_mode_7_reg_7(struct pdp11_cpu* cpu)     // IndexIndirect:
{
    //uint16_t ofs_ptr = PDP11_CPU_PC(cpu);
    //PDP11_CPU_PC(cpu) += 2;
    //uint16_t ofs = pdp11_bus_read_word(cpu->bus, ofs_ptr);
    //uint16_t adr_ptr = (ofs + PDP11_CPU_R(cpu, 7));
    //return pdp11_bus_read_word(cpu->bus, adr_ptr);

    uint16_t ofs_ptr = cpu->pc_ofs;
    cpu->pc_ofs += 2;
    PDP11_CPU_PC(cpu) += 2;
    uint16_t ofs = cpu->pc_dev->read_word(cpu->pc_dev, ofs_ptr);
    uint16_t adr_ptr = ofs + PDP11_CPU_R(cpu, 7);
//    return pdp11_bus_read_word(cpu->bus, adr_ptr);
    uint16_t dev_ofs;
    pdp11_device_t* dev = pdp11_bus_get_device(cpu->bus, PDP11_OP_READ, adr_ptr, &dev_ofs);
    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
    return dev->read_word(dev, dev_ofs);
}

//static pdp11_cpu_get_address_t pdp11_cpu_address_modes_word[] = {
//    pdp11_cpu_mode_0,
//    pdp11_cpu_mode_1_word,
//    pdp11_cpu_mode_2_word,
//    pdp11_cpu_mode_3,
//    pdp11_cpu_mode_4_word,
//    pdp11_cpu_mode_5,
//    pdp11_cpu_mode_6,
//    pdp11_cpu_mode_7
//};

//static pdp11_cpu_get_address_t pdp11_cpu_address_modes_byte[] = {
//    pdp11_cpu_mode_0,
//    pdp11_cpu_mode_1_byte,
//    pdp11_cpu_mode_2_byte,
//    pdp11_cpu_mode_3,
//    pdp11_cpu_mode_4_byte,
//    pdp11_cpu_mode_5,
//    pdp11_cpu_mode_6,
//    pdp11_cpu_mode_7
//};

pdp11_cpu_get_address_t pdp11_cpu_address_mode_reg_word[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    pdp11_cpu_mode_1_reg_0_word,
    pdp11_cpu_mode_1_reg_1_word,
    pdp11_cpu_mode_1_reg_2_word,
    pdp11_cpu_mode_1_reg_3_word,
    pdp11_cpu_mode_1_reg_4_word,
    pdp11_cpu_mode_1_reg_5_word,
    pdp11_cpu_mode_1_reg_6_word,
    pdp11_cpu_mode_1_reg_7_word,
    pdp11_cpu_mode_2_reg_0_word,
    pdp11_cpu_mode_2_reg_1_word,
    pdp11_cpu_mode_2_reg_2_word,
    pdp11_cpu_mode_2_reg_3_word,
    pdp11_cpu_mode_2_reg_4_word,
    pdp11_cpu_mode_2_reg_5_word,
    pdp11_cpu_mode_2_reg_6_word,
    pdp11_cpu_mode_2_reg_7_word,
    pdp11_cpu_mode_3_reg_0,
    pdp11_cpu_mode_3_reg_1,
    pdp11_cpu_mode_3_reg_2,
    pdp11_cpu_mode_3_reg_3,
    pdp11_cpu_mode_3_reg_4,
    pdp11_cpu_mode_3_reg_5,
    pdp11_cpu_mode_3_reg_6,
    pdp11_cpu_mode_3_reg_7,
    pdp11_cpu_mode_4_reg_0_word,
    pdp11_cpu_mode_4_reg_1_word,
    pdp11_cpu_mode_4_reg_2_word,
    pdp11_cpu_mode_4_reg_3_word,
    pdp11_cpu_mode_4_reg_4_word,
    pdp11_cpu_mode_4_reg_5_word,
    pdp11_cpu_mode_4_reg_6_word,
    pdp11_cpu_mode_4_reg_7_word,
    pdp11_cpu_mode_5_reg_0,
    pdp11_cpu_mode_5_reg_1,
    pdp11_cpu_mode_5_reg_2,
    pdp11_cpu_mode_5_reg_3,
    pdp11_cpu_mode_5_reg_4,
    pdp11_cpu_mode_5_reg_5,
    pdp11_cpu_mode_5_reg_6,
    pdp11_cpu_mode_5_reg_7,
    pdp11_cpu_mode_6_reg_0,
    pdp11_cpu_mode_6_reg_1,
    pdp11_cpu_mode_6_reg_2,
    pdp11_cpu_mode_6_reg_3,
    pdp11_cpu_mode_6_reg_4,
    pdp11_cpu_mode_6_reg_5,
    pdp11_cpu_mode_6_reg_6,
    pdp11_cpu_mode_6_reg_7,
    pdp11_cpu_mode_7_reg_0,
    pdp11_cpu_mode_7_reg_1,
    pdp11_cpu_mode_7_reg_2,
    pdp11_cpu_mode_7_reg_3,
    pdp11_cpu_mode_7_reg_4,
    pdp11_cpu_mode_7_reg_5,
    pdp11_cpu_mode_7_reg_6,
    pdp11_cpu_mode_7_reg_7
};

pdp11_cpu_get_address_t pdp11_cpu_address_mode_reg_byte[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    pdp11_cpu_mode_1_reg_0_byte,
    pdp11_cpu_mode_1_reg_1_byte,
    pdp11_cpu_mode_1_reg_2_byte,
    pdp11_cpu_mode_1_reg_3_byte,
    pdp11_cpu_mode_1_reg_4_byte,
    pdp11_cpu_mode_1_reg_5_byte,
    pdp11_cpu_mode_1_reg_6_byte,
    pdp11_cpu_mode_1_reg_7_byte,
    pdp11_cpu_mode_2_reg_0_byte,
    pdp11_cpu_mode_2_reg_1_byte,
    pdp11_cpu_mode_2_reg_2_byte,
    pdp11_cpu_mode_2_reg_3_byte,
    pdp11_cpu_mode_2_reg_4_byte,
    pdp11_cpu_mode_2_reg_5_byte,
    pdp11_cpu_mode_2_reg_6_byte,
    pdp11_cpu_mode_2_reg_7_byte,
    pdp11_cpu_mode_3_reg_0,
    pdp11_cpu_mode_3_reg_1,
    pdp11_cpu_mode_3_reg_2,
    pdp11_cpu_mode_3_reg_3,
    pdp11_cpu_mode_3_reg_4,
    pdp11_cpu_mode_3_reg_5,
    pdp11_cpu_mode_3_reg_6,
    pdp11_cpu_mode_3_reg_7,
    pdp11_cpu_mode_4_reg_0_byte,
    pdp11_cpu_mode_4_reg_1_byte,
    pdp11_cpu_mode_4_reg_2_byte,
    pdp11_cpu_mode_4_reg_3_byte,
    pdp11_cpu_mode_4_reg_4_byte,
    pdp11_cpu_mode_4_reg_5_byte,
    pdp11_cpu_mode_4_reg_6_byte,
    pdp11_cpu_mode_4_reg_7_byte,
    pdp11_cpu_mode_5_reg_0,
    pdp11_cpu_mode_5_reg_1,
    pdp11_cpu_mode_5_reg_2,
    pdp11_cpu_mode_5_reg_3,
    pdp11_cpu_mode_5_reg_4,
    pdp11_cpu_mode_5_reg_5,
    pdp11_cpu_mode_5_reg_6,
    pdp11_cpu_mode_5_reg_7,
    pdp11_cpu_mode_6_reg_0,
    pdp11_cpu_mode_6_reg_1,
    pdp11_cpu_mode_6_reg_2,
    pdp11_cpu_mode_6_reg_3,
    pdp11_cpu_mode_6_reg_4,
    pdp11_cpu_mode_6_reg_5,
    pdp11_cpu_mode_6_reg_6,
    pdp11_cpu_mode_6_reg_7,
    pdp11_cpu_mode_7_reg_0,
    pdp11_cpu_mode_7_reg_1,
    pdp11_cpu_mode_7_reg_2,
    pdp11_cpu_mode_7_reg_3,
    pdp11_cpu_mode_7_reg_4,
    pdp11_cpu_mode_7_reg_5,
    pdp11_cpu_mode_7_reg_6,
    pdp11_cpu_mode_7_reg_7
};

//uint16_t pdp11_cpu_get_address_word(struct pdp11_cpu* cpu, int mode_reg)
//{
//    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[mode_reg];
//    return func(cpu);
//}

//uint16_t pdp11_cpu_get_address_byte(struct pdp11_cpu* cpu, int mode_reg)
//{
//    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_byte[mode_reg];
//    return func(cpu);
//}

//uint16_t pdp11_cpu_read_word(pdp11_cpu_t* cpu, int mode_reg, uint16_t *addr)
//{
//    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[mode_reg];
//    if (func) {
//        *addr = func(cpu);
//        return pdp11_bus_read_word(cpu->bus, *addr);
//    }
//    else
//        return PDP11_CPU_R(cpu, mode_reg);
//}

//uint8_t pdp11_cpu_read_byte(pdp11_cpu_t* cpu, int mode_reg, uint16_t *addr)
//{
//    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_byte[mode_reg];
//    if (func) {
//        *addr = func(cpu);
//        return pdp11_bus_read_byte(cpu->bus, *addr);
//    }
//    else
//        return PDP11_CPU_R(cpu, mode_reg);
//}

//void pdp11_cpu_write_word(pdp11_cpu_t* cpu, int mode_reg, uint16_t *addr, uint16_t val)
//{
//    pdp11_cpu_get_address_t func = NULL;
//    if (!*addr) {
//        func = pdp11_cpu_address_mode_reg_word[mode_reg];
//        if (func) {
//            *addr = func(cpu);
//            pdp11_bus_write_word(cpu->bus, *addr, val);
//        }
//        else
//            PDP11_CPU_R(cpu, mode_reg) = val;
//    }
//    else {
//        pdp11_bus_write_word(cpu->bus, *addr, val);
//    }
//}

//void pdp11_cpu_write_byte(pdp11_cpu_t* cpu, int mode_reg, uint16_t *addr, uint8_t val)
//{
//    pdp11_cpu_get_address_t func = NULL;
//    if (!*addr) {
//        func = pdp11_cpu_address_mode_reg_byte[mode_reg];
//        if (func) {
//            *addr = func(cpu);
//            pdp11_bus_write_byte(cpu->bus, *addr, val);
//        }
//        else
//            PDP11_CPU_R(cpu, mode_reg) = (PDP11_CPU_R(cpu, mode_reg) & 0xff00) | val;
//    }
//    else {
//        pdp11_bus_write_byte(cpu->bus, *addr, val);
//    }
//}

