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
#include <string.h>
#include <malloc.h>
#include "pdp11_bus.h"


static uint8_t pdp11_unk_read_byte(struct pdp11_device* dev, uint16_t ofs)
{
    return 0xff;
}

static uint16_t pdp11_unk_read_word(struct pdp11_device* dev, uint16_t ofs)
{
    return 0xffff;
}

static void pdp11_unk_write_byte(struct pdp11_device* dev, uint16_t ofs, uint8_t val)
{

}

static void pdp11_unk_write_word(struct pdp11_device* dev, uint16_t ofs, uint16_t val)
{

}
static uint8_t pdp11_mem_read_byte(struct pdp11_device* dev, uint16_t ofs)
{
    uint8_t* data = dev->data;
    return data[ofs];
}

static uint16_t pdp11_mem_read_word(struct pdp11_device* dev, uint16_t ofs)
{
    uint8_t* data = (uint8_t *)dev->data + ofs;
    return *(uint16_t *)data;
}

static void pdp11_mem_write_byte(struct pdp11_device* dev, uint16_t ofs, uint8_t val)
{
    uint8_t* data = dev->data;
    data[ofs] = val;
}

static void pdp11_mem_write_word(struct pdp11_device* dev, uint16_t ofs, uint16_t val)
{
    uint8_t* data = (uint8_t*)dev->data + ofs;
    *(uint16_t*)data = val;
}

pdp11_result_t pdp11_bus_create(pdp11_bus_t** pbus)
{
    PDP11_CHECK_RESULT(pbus ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    pdp11_bus_t* bus = (pdp11_bus_t*)malloc(sizeof(pdp11_bus_t));
    PDP11_CHECK_RESULT(bus ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);

    memset(bus, 0, sizeof(pdp11_bus_t));

    bus->device = (pdp11_device_t**)malloc(PDP11_BUS_MAX_DEVICE * sizeof(pdp11_device_t*));
    PDP11_CHECK_RESULT(bus->device ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);
    memset(bus->device, 0, PDP11_BUS_MAX_DEVICE * sizeof(pdp11_device_t*));

    *pbus = bus;
    return PDP11_RESULT_OK;
}

pdp11_result_t pdp11_bus_init(pdp11_bus_t* bus)
{
    PDP11_CHECK_RESULT(bus ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(bus->ram_size ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);

    if (bus->ram_size == 0)
        bus->ram_size = MK85_DEFAULT_RAM_SIZE;
    if (bus->ram == 0) {
        bus->ram = (uint8_t*)malloc(bus->ram_size);
        PDP11_CHECK_RESULT(bus->ram ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);
        memset(bus->ram, 0, bus->ram_size);
    }

    pdp11_device_t* item;
    bus->rom_device = pdp11_bus_new_device(bus);
    item = *bus->rom_device;
    PDP11_CHECK_RESULT(item ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);
    item->allow_op = PDP11_OP_READ;
    item->base = 0;
    item->size = bus->rom_size;
    item->data = bus->rom;
    item->read_byte = pdp11_mem_read_byte;
    item->read_word = pdp11_mem_read_word;
    item->write_byte = pdp11_mem_write_byte;
    item->write_word = pdp11_mem_write_word;

    item = *pdp11_bus_new_device(bus);
    PDP11_CHECK_RESULT(item ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);
    item->allow_op = PDP11_OP_READ | PDP11_OP_WRITE;
    item->base = bus->rom_size;
    item->size = bus->ram_size;
    item->data = bus->ram;
    item->read_byte = pdp11_mem_read_byte;
    item->read_word = pdp11_mem_read_word;
    item->write_byte = pdp11_mem_write_byte;
    item->write_word = pdp11_mem_write_word;


    bus->unk_device = *pdp11_bus_new_device(bus);
    item = bus->unk_device;
    PDP11_CHECK_RESULT(item ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);
    item->allow_op = PDP11_OP_READ | PDP11_OP_WRITE;
    item->base = 0x0000;
    item->size = 0xffff;
    item->data = NULL;
    item->read_byte = pdp11_unk_read_byte;
    item->read_word = pdp11_unk_read_word;
    item->write_byte = pdp11_unk_write_byte;
    item->write_word = pdp11_unk_write_word;

    return PDP11_RESULT_OK;
}

pdp11_result_t pdp11_bus_done(pdp11_bus_t* bus)
{
    PDP11_CHECK_RESULT(bus ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(bus->device ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);
    PDP11_CHECK_RESULT(bus->ram ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);

    pdp11_device_t** item = bus->device;
    while (*item) {
        free(*item);
        item++;
    }

    free(bus->device);
    free(bus->ram);
    free(bus);

    return PDP11_RESULT_OK;

}

pdp11_device_t** pdp11_bus_new_device(pdp11_bus_t* bus)
{
    pdp11_device_t** item = bus->device;
    while (*item)
        item++;

    *item = (pdp11_device_t*)malloc(sizeof(pdp11_device_t));
    return item;
}

pdp11_result_t pdp11_bus_reset(pdp11_bus_t* bus)
{
    PDP11_CHECK_RESULT(bus ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    return PDP11_RESULT_OK;
}


pdp11_device_t* pdp11_bus_get_device(pdp11_bus_t* bus, pdp11_devop_t op, uint16_t addr, uint16_t* ofs)
{
    //asm volatile("nop");
    pdp11_device_t** item = addr < 0x106 ? bus->device : bus->rom_device;
    while (*item) {
        if ((*item)->allow_op & op) {
            *ofs = addr - (*item)->base;
            if (*ofs < (*item)->size) {
                return *item;
            }
        }
        item++;
    }
    //asm volatile("nop");
    return bus->unk_device;
}

//uint16_t pdp11_bus_read_word(pdp11_bus_t* bus, uint16_t addr)
//{
//    uint16_t ofs;
//    pdp11_device_t* dev = pdp11_bus_get_device(bus, PDP11_OP_READ, addr, &ofs);
//    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
//    return dev->read_word(dev, ofs);
//}

//void pdp11_bus_write_word(pdp11_bus_t* bus, uint16_t addr, uint16_t val)
//{
//    uint16_t ofs;
//    pdp11_device_t* dev = pdp11_bus_get_device(bus, PDP11_OP_WRITE, addr, &ofs);
//    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
//    dev->write_word(dev, ofs, val);
//}

//uint8_t pdp11_bus_read_byte(pdp11_bus_t* bus, uint16_t addr)
//{
//    uint16_t ofs;
//    pdp11_device_t* dev = pdp11_bus_get_device(bus, PDP11_OP_READ, addr, &ofs);
//    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
//    return dev->read_byte(dev, ofs);
//}

//void pdp11_bus_write_byte(pdp11_bus_t* bus, uint16_t addr, uint8_t val)
//{
//    uint16_t ofs;
//    pdp11_device_t* dev = pdp11_bus_get_device(bus, PDP11_OP_WRITE, addr, &ofs);
//    PDP11_CHECK_ERROR(dev ? PDP11_RESULT_OK : PDP11_RESULT_FAIL);
//    dev->write_byte(dev, ofs, val);
//}
