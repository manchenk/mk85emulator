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
#include "main.h"
#include "app.h"
#include "mk85back.h"

// SYSCLK - 168MHz
// HCLK   - 168MHz=
static void system_clock_init()
{
    RCC_ClkInitTypeDef clock;
    RCC_OscInitTypeDef osc;

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // Main regulator output voltage

    osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc.HSEState       = RCC_HSE_ON;
    osc.PLL.PLLState   = RCC_PLL_ON;
    osc.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLM       = 8;
    osc.PLL.PLLN       = 336;
    osc.PLL.PLLP       = 2;
    osc.PLL.PLLQ       = 7;
    HAL_RCC_OscConfig(&osc);

    clock.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clock.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    clock.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    clock.APB1CLKDivider = RCC_HCLK_DIV4;
    clock.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&clock, FLASH_LATENCY_5);
}

static void hardware_init()
{
    HAL_Init();

    // system clock - 168 MHz
    system_clock_init();

    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);

    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(0);
    BSP_LCD_DisplayOn();
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    uint8_t status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    if (status != TS_OK) {
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 18, (uint8_t*)"ERROR", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 33, (uint8_t*)"Touchscreen cannot be initialized", CENTER_MODE);
        while (1) { }
    }
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 - 12, (uint8_t*)"You need to calibrate touchscreen.", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 3, (uint8_t*)"Press on the black cross", CENTER_MODE);
}

static void touch_kbd_get_state(touch_kbd_info_t *info)
{
    TS_StateTypeDef state;
    BSP_TS_GetState(&state);

    if (state.TouchDetected) {
        info->pressed = TRUE;
        info->touch.x = state.X;
        info->touch.y = state.Y;
    }
    else {
        info->pressed = FALSE;
    }
}

static void keyboard_background_draw()
{
    int x, y;
    uint32_t *dst = (uint32_t*) LCD_FRAME_BUFFER;
    const uint32_t *src = mk85_background;
    for (y = 0; y < 320; ++y) {
        for (x = 0; x < 240; ++x) {
            *dst++ = *src++ | 0xff000000;
        }
    }

    ili9341_WriteReg(LCD_MAC);
    ili9341_WriteData(0x88);
}

static void  draw_calibrate_point(touch_kbd_point_t *info)
{
    if (info->state) {
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DrawLine(info->point->x + 5, info->point->y, info->point->x - 5, info->point->y);
        BSP_LCD_DrawLine(info->point->x, info->point->y + 5, info->point->x, info->point->y - 5);
    }
    else {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_FillRect(info->point->x - 5, info->point->y - 5, info->point->x + 5, info->point->y + 5);
    }
}

int main(void) {
    hardware_init();

    app_t *app = NULL;
    app_create(&app);
    app->video_memory = (void*) LCD_FRAME_BUFFER;
    app->keyboard->disp_size.width = BSP_LCD_GetXSize();
    app->keyboard->disp_size.height = BSP_LCD_GetYSize();
    app->keyboard->draw_point = draw_calibrate_point;
    app->keyboard->keyboard_screen = keyboard_background_draw;
    app->keyboard->get_state =touch_kbd_get_state;
    app_init(app);
    app_run(app);
    app_done(app);
}

