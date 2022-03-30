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
#include "pdp11_cpu.h"
#include "pdp11_cmds.h"
#include "pdp11_adr.h"

static int16_t pdp11_cpu_get_displacement(uint16_t op_code) {
    int16_t disp = (op_code & 0xff);
    if (disp > 127)
        return (disp - 256) << 1;
    return disp << 1;
}

static uint8_t pdp11_cpu_get_offset(uint16_t op_code) {
    return (op_code << 1) & 0x7e;
}

//static uint8_t __attribute__((unused)) pdp11_cpu_get_trap(uint16_t op_code) {
//    return op_code & 0xff;
//}

static uint8_t pdp11_cpu_get_immediate(uint16_t op_code) {
    return op_code & 0x0f;
}

void pdp11_cpu_command_halt(struct pdp11_cpu *cpu, uint16_t op_code)   // 000001
{
    uint16_t addr = 0x0078;
    PDP11_CPU_CPC(cpu) = PDP11_CPU_PC(cpu);
    PDP11_CPU_CPS(cpu) = pdp11_cpu_get_psw(cpu);
    if (PDP11_CPU_FLAG_H(cpu))
        addr += PDP11_CPU_SEL(cpu) & 0xff00;
    PDP11_CPU_PC(cpu) = pdp11_bus_read_word(cpu->bus, addr) & 0xfffe;
    addr += 2;
    pdp11_cpu_set_psw(cpu, pdp11_bus_read_word(cpu->bus, addr));
    PDP11_CPU_FLAG_H(cpu) = TRUE;
}

void pdp11_cpu_command_wait(pdp11_cpu_t *cpu, uint16_t op_code)      // 000001
{
    PDP11_CPU_WAIT(cpu) = TRUE;
}

void pdp11_cpu_command_rti(pdp11_cpu_t *cpu, uint16_t op_code)       // 000002
{
    uint16_t pc = pdp11_bus_read_word(cpu->bus, PDP11_CPU_SP(cpu));
    PDP11_CPU_SP(cpu) += 2;
    pdp11_cpu_set_psw(cpu, PDP11_CPU_SP(cpu));
    PDP11_CPU_SP(cpu) += 2;
    if (pc >= 0xe000)
        PDP11_CPU_FLAG_H(cpu) = TRUE;
}

void pdp11_cpu_command_bpt(pdp11_cpu_t *cpu, uint16_t op_code)       // 000003
{
    PDP11_CPU_VECTOR(cpu) = 0x000c;
}

void pdp11_cpu_command_iot(pdp11_cpu_t *cpu, uint16_t op_code)       // 000004
{
    PDP11_CPU_VECTOR(cpu) = 0x0010;
}

void pdp11_cpu_command_reset(pdp11_cpu_t *cpu, uint16_t op_code)     // 000005
{
    PDP11_CPU_RESET(cpu) = TRUE;
}

void pdp11_cpu_command_rtt(pdp11_cpu_t *cpu, uint16_t op_code)       // 000006
{
    PDP11_CPU_RTT(cpu) = TRUE;
    pdp11_cpu_command_rti(cpu, op_code);
}

void pdp11_cpu_command_jmp(pdp11_cpu_t *cpu, uint16_t op_code)       // 0001DD
{
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[op_code & 077];

    if (func) {
        PDP11_CPU_PC(cpu) = func(cpu);
    }
    else
        PDP11_CPU_VECTOR(cpu) = 0x0004;
}

void pdp11_cpu_command_rts(pdp11_cpu_t *cpu, uint16_t op_code)       // 00020R
{
    int dst_reg = op_code & 7;
    PDP11_CPU_PC(cpu) = PDP11_CPU_R(cpu, dst_reg);
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[(PDP11_ADDRESS_AUTOINCREMENT << 3) | PDP11_CPU_SP_INDEX];
    PDP11_CPU_R(cpu, dst_reg) = pdp11_bus_read_word(cpu->bus, func(cpu));
}

void pdp11_cpu_command_clc(pdp11_cpu_t *cpu, uint16_t op_code)       // 00024I
{
    pdp11_cpu_set_psw(cpu, pdp11_cpu_get_psw(cpu) & ~pdp11_cpu_get_immediate(op_code));
}

void pdp11_cpu_command_sec(pdp11_cpu_t *cpu, uint16_t op_code)       // 000260
{
    pdp11_cpu_set_psw(cpu, pdp11_cpu_get_psw(cpu) | pdp11_cpu_get_immediate(op_code));
}

void pdp11_cpu_command_swab(pdp11_cpu_t *cpu, uint16_t op_code)      // 0003DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t lo = d & 0xff;
    uint8_t hi = (d >> 8) & 0xff;
    uint16_t r = (lo << 8) | hi;
    PDP11_CPU_FLAG_C(cpu) = FALSE;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !(r & 0xff);
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_br(pdp11_cpu_t *cpu, uint16_t op_code)        // 0004XX
{
    PDP11_CPU_PC(cpu) += pdp11_cpu_get_displacement(op_code);
}

void pdp11_cpu_command_bne(pdp11_cpu_t *cpu, uint16_t op_code)       // 0010XX
{
    if (!(PDP11_CPU_FLAG_Z(cpu)))
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_beq(pdp11_cpu_t *cpu, uint16_t op_code)       // 0014XX
{
    if (PDP11_CPU_FLAG_Z(cpu))
        pdp11_cpu_command_br(cpu, op_code);

}

void pdp11_cpu_command_bge(pdp11_cpu_t *cpu, uint16_t op_code)       // 0020XX
{
    bool_t n = PDP11_CPU_FLAG_N(cpu);
    bool_t v = PDP11_CPU_FLAG_V(cpu);
    if (v == n)
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_blt(pdp11_cpu_t *cpu, uint16_t op_code)       // 0024XX
{
    bool_t n = PDP11_CPU_FLAG_N(cpu);
    bool_t v = PDP11_CPU_FLAG_V(cpu);
    if (v != n)
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_bgt(pdp11_cpu_t *cpu, uint16_t op_code)       // 0030XX
{
    if (!(PDP11_CPU_FLAG_Z(cpu)))
        pdp11_cpu_command_bge(cpu, op_code);

}

void pdp11_cpu_command_ble(pdp11_cpu_t *cpu, uint16_t op_code)       // 0034XX
{
    if (PDP11_CPU_FLAG_Z(cpu))
        pdp11_cpu_command_br(cpu, op_code);
    else
        pdp11_cpu_command_blt(cpu, op_code);

}

void pdp11_cpu_command_jsr(pdp11_cpu_t *cpu, uint16_t op_code)       // 004RDD
{
    int dst = op_code & 077;
    int src_reg = (op_code >> 6) & 7;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    if (dfunc) {
        uint16_t sub_addr = dfunc(cpu);
        pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[(PDP11_ADDRESS_AUTODECREMENT << 3) | PDP11_CPU_SP_INDEX];
        pdp11_bus_write_word(cpu->bus, sfunc(cpu), PDP11_CPU_R(cpu, src_reg));
        PDP11_CPU_R(cpu, src_reg) = PDP11_CPU_PC(cpu);
        PDP11_CPU_PC(cpu) = sub_addr;
    }
    else
        PDP11_CPU_VECTOR(cpu) = 0x0004;
}

void pdp11_cpu_command_clr(pdp11_cpu_t *cpu, uint16_t op_code)       // 0050DD
{
    int dst = op_code & 077;

    PDP11_CPU_FLAG_C(cpu) = FALSE;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = TRUE;
    PDP11_CPU_FLAG_N(cpu) = FALSE;

    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    if (func)
        pdp11_bus_write_word(cpu->bus, func(cpu), 0);
    else
        PDP11_CPU_R(cpu, dst) = 0;
}

void pdp11_cpu_command_com(pdp11_cpu_t *cpu, uint16_t op_code)       // 0051DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = ~d;
    PDP11_CPU_FLAG_C(cpu) = TRUE;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_inc(pdp11_cpu_t *cpu, uint16_t op_code)       // 0052DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = d + 1;
    PDP11_CPU_FLAG_V(cpu) = d == 0x7fff;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_dec(pdp11_cpu_t *cpu, uint16_t op_code)       // 0053DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = d - 1;
    PDP11_CPU_FLAG_V(cpu) = d == 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_neg(pdp11_cpu_t *cpu, uint16_t op_code)       // 0054DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = -d;
    PDP11_CPU_FLAG_C(cpu) = r;
    PDP11_CPU_FLAG_V(cpu) = d == 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_adc(pdp11_cpu_t *cpu, uint16_t op_code)       // 0055DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r;
    if (PDP11_CPU_FLAG_C(cpu)) {
        r = d + 1;
        PDP11_CPU_FLAG_C(cpu) = d == 0xffff;
        PDP11_CPU_FLAG_V(cpu) = d == 0x7fff;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x8000;
    }
    else {
        r = d;
        PDP11_CPU_FLAG_C(cpu) = FALSE;
        PDP11_CPU_FLAG_V(cpu) = FALSE;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x8000;
    }

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_sbc(pdp11_cpu_t *cpu, uint16_t op_code)       // 0056DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r;
    if (PDP11_CPU_FLAG_C(cpu)) {
        r = d - 1;
        PDP11_CPU_FLAG_C(cpu) = d;
        PDP11_CPU_FLAG_V(cpu) = d == 0x8000;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x8000;
    }
    else {
        r = d;
        PDP11_CPU_FLAG_C(cpu) = FALSE;
        PDP11_CPU_FLAG_V(cpu) = d = 0x8000;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x8000;
    }

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_tst(pdp11_cpu_t *cpu, uint16_t op_code)       // 0057DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    PDP11_CPU_FLAG_C(cpu) = FALSE;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !d;
    PDP11_CPU_FLAG_N(cpu) = d & 0x8000;
}

void pdp11_cpu_command_ror(pdp11_cpu_t *cpu, uint16_t op_code)       // 0060DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = d >> 1;
    if (PDP11_CPU_FLAG_C(cpu))
        r |= 0x8000;
    else
        r &= ~0x8000;
    PDP11_CPU_FLAG_C(cpu) = d & 1;
    PDP11_CPU_FLAG_V(cpu) = (r & 0x8000) ^ (d & 1 ? 0x8000 : 0);
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_rol(pdp11_cpu_t *cpu, uint16_t op_code)       // 0061DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = d << 1;
    if (PDP11_CPU_FLAG_C(cpu))
        r |= 1;
    PDP11_CPU_FLAG_C(cpu) = d & 0x8000;
    PDP11_CPU_FLAG_V(cpu) = (r ^ d) & 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_asr(pdp11_cpu_t *cpu, uint16_t op_code)       // 0062DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = d >> 1;
    if (d & 0x8000)
        r |= 0x8000;
    PDP11_CPU_FLAG_C(cpu) = d & 1;
    PDP11_CPU_FLAG_V(cpu) = (r & 0x8000) ^ (d & 1 ? 0x8000 : 0);
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_asl(pdp11_cpu_t *cpu, uint16_t op_code)       // 0063DD
{
    int dst = op_code & 077;
    uint16_t addr = 0;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = func ? pdp11_bus_read_word(cpu->bus, addr = func(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = d << 1;
    PDP11_CPU_FLAG_C(cpu) = d & 0x8000;
    PDP11_CPU_FLAG_V(cpu) = (r ^ d) & 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (func)
        pdp11_bus_write_word(cpu->bus, addr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_mark(pdp11_cpu_t *cpu, uint16_t op_code)      // 006400
{
    PDP11_CPU_SP(cpu) = PDP11_CPU_PC(cpu) + pdp11_cpu_get_offset(op_code);
    PDP11_CPU_PC(cpu) = PDP11_CPU_R(cpu, 5) & ~1;
    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[(PDP11_ADDRESS_AUTOINCREMENT << 3) | PDP11_CPU_SP_INDEX];
    PDP11_CPU_R(cpu, 5) = pdp11_bus_read_word(cpu->bus, func(cpu));
}

void pdp11_cpu_command_sxt(pdp11_cpu_t *cpu, uint16_t op_code)       // 0067DD
{
    int dst = op_code & 077;
    uint16_t r = PDP11_CPU_FLAG_N(cpu) ? 0xffff : 0x0000;

    pdp11_cpu_get_address_t func = pdp11_cpu_address_mode_reg_word[dst];
    if (func)
        pdp11_bus_write_word(cpu->bus, func(cpu), r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_mov(pdp11_cpu_t *cpu, uint16_t op_code)       // 01SSDD
{
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);

    uint16_t r = s;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    if (dfunc)
        pdp11_bus_write_word(cpu->bus, dfunc(cpu), r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_cmp(pdp11_cpu_t *cpu, uint16_t op_code)       // 02SSDD
{
    int dst = op_code & 077;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = s - d;
    PDP11_CPU_FLAG_C(cpu) = s < d;
    PDP11_CPU_FLAG_V(cpu) = (s ^ d) & (r ^ ~d) & 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;
}

void pdp11_cpu_command_bit(pdp11_cpu_t *cpu, uint16_t op_code)       // 03SSDD
{
    int dst = op_code & 077;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = s & d;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;
}

void pdp11_cpu_command_bic(pdp11_cpu_t *cpu, uint16_t op_code)       // 04SSDD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = ~s & d;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (dfunc)
        pdp11_bus_write_word(cpu->bus, daddr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_bis(pdp11_cpu_t *cpu, uint16_t op_code)       // 05SSDD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = s | d;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (dfunc)
        pdp11_bus_write_word(cpu->bus, daddr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_add(pdp11_cpu_t *cpu, uint16_t op_code)       // 06SSDD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint32_t r = s + d;
    PDP11_CPU_FLAG_C(cpu) = r > 0xffff;
    PDP11_CPU_FLAG_V(cpu) = (~s ^ d) & (r ^ s) & 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !(r & 0xffff);
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (dfunc)
        pdp11_bus_write_word(cpu->bus, daddr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_mul(pdp11_cpu_t *cpu, uint16_t op_code)       // 070RSS
{
    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);
    int src_reg = (op_code >> 6) & 7;
    uint16_t s = PDP11_CPU_R(cpu, src_reg);

    uint32_t r = s * d;
    PDP11_CPU_FLAG_C(cpu) = !((r > 0x7fff) || (r <= -0x8000));
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (src_reg & 1) {
        PDP11_CPU_R(cpu, src_reg) = r;
    }
    else {
        PDP11_CPU_R(cpu, src_reg) = r >> 16;
        PDP11_CPU_R(cpu, src_reg | 1) = r;
    }
}

void pdp11_cpu_command_div(pdp11_cpu_t *cpu, uint16_t op_code)       // 071RSS
{
    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    int32_t d = dfunc ? pdp11_bus_read_word(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    int src_reg = (op_code >> 6) & 7;
    int32_t s = (PDP11_CPU_R(cpu, src_reg) << 16)
            | PDP11_CPU_R(cpu, src_reg | 1);

    if (d != 0) {
        int32_t r0 = s / d;
        int32_t r1 = s % d;
        PDP11_CPU_FLAG_V(cpu) = (r0 > 0x7fff) || (r0 <= -0x8000);
        PDP11_CPU_FLAG_Z(cpu) = !r0;
        PDP11_CPU_FLAG_N(cpu) = r0 & 0x8000;
        PDP11_CPU_R(cpu, src_reg) = r0;
        PDP11_CPU_R(cpu, src_reg | 1) = r1;
    }
    else {
        PDP11_CPU_FLAG_C(cpu) = TRUE;
        PDP11_CPU_FLAG_V(cpu) = TRUE;
    }
}

//static uint16_t __attribute__((unused)) shift_left_word(pdp11_cpu_t *cpu,
//        uint16_t arg, uint8_t n) {
//    uint32_t tmp = arg;
//    int i;
//    for (i = 0; i < n; i++) {
//        tmp <<= 1;
//    }
//    PDP11_CPU_FLAG_C(cpu) = tmp & 0x10000;
//    PDP11_CPU_FLAG_V(cpu) = (tmp ^ arg) & 0x8000;
//    PDP11_CPU_FLAG_Z(cpu) = !(tmp & 0xffff);
//    PDP11_CPU_FLAG_N(cpu) = tmp & 0x8000;
//    return arg;
//}
//
//static uint16_t __attribute__((unused)) shift_right_word(pdp11_cpu_t *cpu,
//        uint16_t arg, uint8_t n) {
//    bool_t c = FALSE;
//    uint16_t tmp = arg;
//    int i;
//    for (i = 0; i < n; i++) {
//        c = tmp & 1;
//        tmp >>= 1;
//    }
//    PDP11_CPU_FLAG_C(cpu) = c;
//    PDP11_CPU_FLAG_V(cpu) = (tmp ^ arg) & 0x8000;
//    PDP11_CPU_FLAG_Z(cpu) = !tmp;
//    PDP11_CPU_FLAG_N(cpu) = tmp & 0x8000;
//    return arg;
//}

static uint32_t shift_left_dword(pdp11_cpu_t *cpu, uint32_t arg, uint8_t n) {
    bool_t c = FALSE;
    uint32_t tmp = arg;
    int i;
    for (i = 0; i < n; i++) {
        c = tmp & 0x80000000;
        tmp <<= 1;
    }
    PDP11_CPU_FLAG_C(cpu) = c;
    PDP11_CPU_FLAG_V(cpu) = (tmp ^ arg) & 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !tmp;
    PDP11_CPU_FLAG_N(cpu) = tmp & 0x8000;
    return arg;
}

static uint32_t shift_right_dword(pdp11_cpu_t *cpu, uint32_t arg, uint8_t n) {
    bool_t c = FALSE;
    uint32_t tmp = arg;
    int i;
    for (i = 0; i < n; i++) {
        c = tmp & 1;
        tmp >>= 1;
    }
    PDP11_CPU_FLAG_C(cpu) = c;
    PDP11_CPU_FLAG_V(cpu) = (tmp ^ arg) & 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !tmp;
    PDP11_CPU_FLAG_N(cpu) = tmp & 0x8000;
    return arg;
}

void pdp11_cpu_command_ash(pdp11_cpu_t *cpu, uint16_t op_code)       // 072RSS
{
    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint32_t d = dfunc ? pdp11_bus_read_word(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    int src_reg = (op_code >> 6) & 7;
    uint32_t s = (PDP11_CPU_R(cpu, src_reg) << 16) | PDP11_CPU_R(cpu, src_reg | 1);

    uint32_t r;
    if (d >= 0x20) {
        r = shift_right_dword(cpu, s, 0x40 - d);
    }
    else {
        r = shift_left_dword(cpu, s, d);
    }
    PDP11_CPU_R(cpu, src_reg) = r >> 16;
    PDP11_CPU_R(cpu, src_reg | 1) = r;
}

void pdp11_cpu_command_ashc(pdp11_cpu_t *cpu, uint16_t op_code)      // 073RSS
{
    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint32_t d = dfunc ? pdp11_bus_read_word(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    int src_reg = (op_code >> 6) & 7;
    uint32_t s = (PDP11_CPU_R(cpu, src_reg) << 16) | PDP11_CPU_R(cpu, src_reg | 1);

    uint32_t r;
    if (d >= 0x20) {
        r = shift_right_dword(cpu, s, 0x40 - d);
    }
    else {
        r = shift_left_dword(cpu, s, d);
    }
    PDP11_CPU_R(cpu, src_reg) = r >> 16;
    PDP11_CPU_R(cpu, src_reg | 1) = r;
}

//uint16_t xor(pdp11_cpu_t *c, uint16_t s, uint16_t d)
//{
//  uint16_t res = 0;
//  asm volatile (
//      "EOR %[result], %[src], %[dst];\n"
//      "MRS r1, APSR;\n"
//      "STR r1, [%[cpu], #20];\n"
//      : [result] "=r" (res)
//      : [cpu] "r" (c), [src] "r" (s), [dst] "r" (d)
//      : "r1"
//  );
//  return res;
//}

void pdp11_cpu_command_xor(pdp11_cpu_t *cpu, uint16_t op_code)       // 074RDD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    int src = (op_code >> 6) & 7;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r;
    asm volatile (
        "EORS %[result], %[src], %[dst];\n"
        "MRS r1, APSR;\n"
        "STR r1, [%[cpu], #20];\n"
        : [result] "=r" (r)
        : [cpu] "r" (cpu), [src] "r" (s), [dst] "r" (d)
        : "r1"
    );

    //uint16_t r = xor(cpu, s, d);
    //uint16_t r = s ^ d;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (dfunc)
        pdp11_bus_write_word(cpu->bus, daddr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_fadd(pdp11_cpu_t *cpu, uint16_t op_code)       // 07500R
{
    if ((PDP11_CPU_SEL(cpu) & 0x0080) == 0)
        PDP11_CPU_FLAG_H(cpu) = TRUE;
    else
        PDP11_CPU_VECTOR(cpu) = 0x0008;
}

void pdp11_cpu_command_fsub(pdp11_cpu_t *cpu, uint16_t op_code)       // 07501R
{
    if ((PDP11_CPU_SEL(cpu) & 0x0080) == 0)
        PDP11_CPU_FLAG_H(cpu) = TRUE;
    else
        PDP11_CPU_VECTOR(cpu) = 0x0008;
}

void pdp11_cpu_command_fmul(pdp11_cpu_t *cpu, uint16_t op_code)       // 07502R
{
    if ((PDP11_CPU_SEL(cpu) & 0x0080) == 0)
        PDP11_CPU_FLAG_H(cpu) = TRUE;
    else
        PDP11_CPU_VECTOR(cpu) = 0x0008;
}

void pdp11_cpu_command_fdiv(pdp11_cpu_t *cpu, uint16_t op_code)       // 07503R
{
    if ((PDP11_CPU_SEL(cpu) & 0x0080) == 0)
        PDP11_CPU_FLAG_H(cpu) = TRUE;
    else
        PDP11_CPU_VECTOR(cpu) = 0x0008;
}

void pdp11_cpu_command_sob(pdp11_cpu_t *cpu, uint16_t op_code)       // 077R00
{
    int src_reg = (op_code >> 6) & 7;
    PDP11_CPU_R(cpu, src_reg) -= 1;
    if (PDP11_CPU_R(cpu, src_reg) != 0) {
        PDP11_CPU_PC(cpu) -= pdp11_cpu_get_offset(op_code);
    }
}

void pdp11_cpu_command_bpl(pdp11_cpu_t *cpu, uint16_t op_code)       // 1000XX
{
    if (!(PDP11_CPU_FLAG_N(cpu)))
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_bmi(pdp11_cpu_t *cpu, uint16_t op_code)       // 1004XX
{
    if (PDP11_CPU_FLAG_N(cpu))
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_bhi(pdp11_cpu_t *cpu, uint16_t op_code)       // 1010XX
{
    if (!(PDP11_CPU_FLAG_Z(cpu)))
        pdp11_cpu_command_bcc(cpu, op_code);
}

void pdp11_cpu_command_blos(pdp11_cpu_t *cpu, uint16_t op_code)      // 1014XX
{
    if (PDP11_CPU_FLAG_Z(cpu))
        pdp11_cpu_command_br(cpu, op_code);
    else
        pdp11_cpu_command_bcs(cpu, op_code);
}

void pdp11_cpu_command_bvc(pdp11_cpu_t *cpu, uint16_t op_code)       // 1020XX
{
    if (!(PDP11_CPU_FLAG_V(cpu)))
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_bvs(pdp11_cpu_t *cpu, uint16_t op_code)       // 1024XX
{
    if (PDP11_CPU_FLAG_V(cpu))
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_bcc(pdp11_cpu_t *cpu, uint16_t op_code)       // 1030XX
{
    if (!(PDP11_CPU_FLAG_C(cpu)))
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_bcs(pdp11_cpu_t *cpu, uint16_t op_code)       // 1034XX
{
    if (PDP11_CPU_FLAG_C(cpu))
        pdp11_cpu_command_br(cpu, op_code);
}

void pdp11_cpu_command_emt(pdp11_cpu_t *cpu, uint16_t op_code)       // 1040XX
{
    PDP11_CPU_VECTOR(cpu) = 0x0018;
}

void pdp11_cpu_command_trap(pdp11_cpu_t *cpu, uint16_t op_code)      // 1044XX
{
    PDP11_CPU_VECTOR(cpu) = 0x001c;
}

void pdp11_cpu_command_clrb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1050DD
{
    int dst = op_code & 077;
    PDP11_CPU_FLAG_C(cpu) = FALSE;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = TRUE;
    PDP11_CPU_FLAG_N(cpu) = FALSE;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, dfunc(cpu), 0);
    else
        PDP11_CPU_R(cpu, dst) = PDP11_CPU_R(cpu, dst) & 0xff00;
}

void pdp11_cpu_command_comb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1051DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = ~d;

    PDP11_CPU_FLAG_C(cpu) = TRUE;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_incb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1052DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = (d + 1);
    PDP11_CPU_FLAG_V(cpu) = d == 0x7f;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_decb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1053DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = d - 1;
    PDP11_CPU_FLAG_V(cpu) = d == 0x80;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_negb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1054DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = -d;
    PDP11_CPU_FLAG_C(cpu) = r;
    PDP11_CPU_FLAG_V(cpu) = d == 0x80;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_adcb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1055DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r;
    if (PDP11_CPU_FLAG_C(cpu)) {
        r = d + 1;
        PDP11_CPU_FLAG_C(cpu) = d == 0xff;
        PDP11_CPU_FLAG_V(cpu) = d == 0x7f;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x80;
    }
    else {
        r = (uint8_t) d;
        PDP11_CPU_FLAG_C(cpu) = FALSE;
        PDP11_CPU_FLAG_V(cpu) = FALSE;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x80;
    }

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_sbcb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1056DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r;
    if (PDP11_CPU_FLAG_C(cpu)) {
        r = d - 1;
        PDP11_CPU_FLAG_C(cpu) = d;
        PDP11_CPU_FLAG_V(cpu) = d == 0x80;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x80;
    }
    else {
        r = (uint8_t) d;
        PDP11_CPU_FLAG_V(cpu) = d == 0x80;
        PDP11_CPU_FLAG_Z(cpu) = !r;
        PDP11_CPU_FLAG_N(cpu) = r & 0x80;
    }

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_tstb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1057DD
{
    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint8_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);
    PDP11_CPU_FLAG_C(cpu) = FALSE;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !d;
    PDP11_CPU_FLAG_N(cpu) = d & 0x80;
}

void pdp11_cpu_command_rorb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1060DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = (d >> 1) | (PDP11_CPU_FLAG_C(cpu) ? 0x80 : 0);
    PDP11_CPU_FLAG_C(cpu) = d & 1;
    PDP11_CPU_FLAG_V(cpu) = (r & 0x80) ^ (d & 1 ? 0x80 : 0);
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_rolb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1061DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = (d << 1) | (PDP11_CPU_FLAG_C(cpu) ? 1 : 0);
    PDP11_CPU_FLAG_C(cpu) = d & 0x80;
    PDP11_CPU_FLAG_V(cpu) = (r ^ d) & 0x80;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_asrb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1062DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = (d >> 1) & 0xff;
    if (d & 0x80)
        r |= 0x80;
    PDP11_CPU_FLAG_C(cpu) = d & 1;
    PDP11_CPU_FLAG_V(cpu) = (r & 0x80) ^ (d & 1 ? 0x80 : 0);
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_aslb(pdp11_cpu_t *cpu, uint16_t op_code)      // 1063DD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = (d << 1) & 0xff;
    PDP11_CPU_FLAG_C(cpu) = d & 0x80;
    PDP11_CPU_FLAG_V(cpu) = (r ^ d) & 0x80;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_mtps(pdp11_cpu_t *cpu, uint16_t op_code)      // 1064SS
{
    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    const uint16_t mask = 0x00ef;
    pdp11_cpu_set_psw(cpu, pdp11_cpu_get_psw(cpu) & ~mask);
    pdp11_cpu_set_psw(cpu, d & mask);
}

void pdp11_cpu_command_mfps(pdp11_cpu_t *cpu, uint16_t op_code)      // 1067DD
{
    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    if (dfunc)
        pdp11_bus_write_word(cpu->bus, dfunc(cpu), pdp11_cpu_get_psw(cpu));
    else
        PDP11_CPU_R(cpu, dst) = pdp11_cpu_get_psw(cpu);
}

void pdp11_cpu_command_movb(pdp11_cpu_t *cpu, uint16_t op_code)      // 11SSDD
{
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_byte[src];
    uint16_t s = sfunc ? pdp11_bus_read_byte(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);

    uint16_t r = s & 0x80 ? s | 0xff00 : s & ~0xff00;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !(r & 0xff);
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    int dst = op_code & 077;
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, dfunc(cpu), r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_cmpb(pdp11_cpu_t *cpu, uint16_t op_code)      // 12SSDD
{
    int dst = op_code & 077;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_byte[src];
    uint8_t s = sfunc ? pdp11_bus_read_byte(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint8_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = s - d;
    PDP11_CPU_FLAG_C(cpu) = s < d;
    PDP11_CPU_FLAG_V(cpu) = (s ^ d) & (r & ~d) & 0x80;
    PDP11_CPU_FLAG_Z(cpu) = !(r & 0xff);
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;
}

void pdp11_cpu_command_bitb(pdp11_cpu_t *cpu, uint16_t op_code)     // 13SSDD
{
    int dst = op_code & 077;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_byte[src];
    uint8_t s = sfunc ? pdp11_bus_read_byte(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint8_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = d & s;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !(r & 0xff);
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;
}

void pdp11_cpu_command_bicb(pdp11_cpu_t *cpu, uint16_t op_code)      // 14SSDD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_byte[src];
    uint8_t s = sfunc ? pdp11_bus_read_byte(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = d & ~s;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_bisb(pdp11_cpu_t *cpu, uint16_t op_code)      // 15SSDD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_byte[src];
    uint8_t s = sfunc ? pdp11_bus_read_byte(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_byte[dst];
    uint16_t d = dfunc ? pdp11_bus_read_byte(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint8_t r = d | s;
    PDP11_CPU_FLAG_V(cpu) = FALSE;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x80;

    if (dfunc)
        pdp11_bus_write_byte(cpu->bus, daddr, r);
    else
        *(uint8_t *)&PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_sub(pdp11_cpu_t *cpu, uint16_t op_code)       // 16SSDD
{
    int dst = op_code & 077;
    uint16_t daddr = 0;
    int src = (op_code >> 6) & 077;
    pdp11_cpu_get_address_t sfunc = pdp11_cpu_address_mode_reg_word[src];
    uint16_t s = sfunc ? pdp11_bus_read_word(cpu->bus, sfunc(cpu)) : PDP11_CPU_R(cpu, src);
    pdp11_cpu_get_address_t dfunc = pdp11_cpu_address_mode_reg_word[dst];
    uint16_t d = dfunc ? pdp11_bus_read_word(cpu->bus, daddr = dfunc(cpu)) : PDP11_CPU_R(cpu, dst);

    uint16_t r = d - s;
    PDP11_CPU_FLAG_C(cpu) = d < s;
    PDP11_CPU_FLAG_V(cpu) = (s ^ d) & (r ^ ~s) & 0x8000;
    PDP11_CPU_FLAG_Z(cpu) = !r;
    PDP11_CPU_FLAG_N(cpu) = r & 0x8000;

    if (dfunc)
        pdp11_bus_write_word(cpu->bus, daddr, r);
    else
        PDP11_CPU_R(cpu, dst) = r;
}

void pdp11_cpu_command_trap10(pdp11_cpu_t *cpu, uint16_t op_code) {
    PDP11_CPU_VECTOR(cpu) = 0x0008;
}

