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

#include "stdint.h"

#if !defined(FALSE) && !defined(TRUE)
typedef enum bool_type {
    FALSE = 0,
    TRUE
} bool_type_t;
//#else
#endif
typedef int bool_t;

#ifdef __XTENSA__

#include "esp_err.h"

#define PDP11_CHECK_RESULT(res) ESP_ERROR_CHECK(res)
#define PDP11_CHECK_ERROR(res) ESP_ERROR_CHECK(res)
#define pdp11_result_t esp_err_t

#define PDP11_RESULT_OK            ESP_OK
#define PDP11_RESULT_FAIL          ESP_ERR_FAIL
#define PDP11_RESULT_INVALID_ARG   ESP_ERR_INVALID_ARG
#define PDP11_RESULT_INVALID_STATE ESP_ERR_INVALID_STATE
#define PDP11_RESULT_NO_MEM        ESP_ERR_NO_MEM

#else

#define PDP11_CHECK_RESULT(res) do { pdp11_result_t __r = (res);  if (__r != PDP11_RESULT_OK) return __r; } while(0);
#define PDP11_CHECK_ERROR(res) do { pdp11_result_t __r = (res);  if (__r != PDP11_RESULT_OK) { printf("Error %d at %s:%d\n", __r, __FILE__, __LINE__); exit(-1); } } while(0);

typedef enum {
    PDP11_RESULT_OK = 0,
    PDP11_RESULT_FAIL,
    PDP11_RESULT_INVALID_ARG,
    PDP11_RESULT_INVALID_STATE,
    PDP11_RESULT_NO_MEM
} pdp11_result_t;

#endif

#ifdef __cplusplus
}
#endif

