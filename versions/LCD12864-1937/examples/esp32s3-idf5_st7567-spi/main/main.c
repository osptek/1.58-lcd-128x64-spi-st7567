/*
 * SPDX-FileCopyrightText: Copyright 2026 OSPTEK
 * SPDX-License-Identifier: CC-BY-4.0
 *
 * https://github.com/osptek
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "esp_lcd_st7567.h"
#include "image_data.h"

#define LCD_HOST        SPI2_HOST

#define PIN_LCD_SCLK    41
#define PIN_LCD_MOSI    40
#define PIN_LCD_CS      47
#define PIN_LCD_DC      39
#define PIN_LCD_RST     38
#define PIN_LCD_BL      -1 // 背光引脚（若无独立背光引脚控制，保持 -1 即可）

#define LCD_PIXEL_CLOCK_HZ (10 * 1000 * 1000) 
#define LCD_H_RES          128                 
#define LCD_V_RES          64                  
#define LCD_BUFFER_SIZE    (LCD_H_RES * LCD_V_RES / 8) // 1024 字节

// =============================================================================
// 显存缓冲区及显存控制函数
// =============================================================================
static uint8_t g_frame_buffer[LCD_BUFFER_SIZE];

/**
 * @brief 全屏刷新函数
 */
void lcd_full_screen_refresh(esp_lcd_panel_handle_t panel_handle, const uint8_t *data)
{
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, data);
}

/**
 * @brief 全屏清屏
 */
void lcd_clear_screen(esp_lcd_panel_handle_t panel_handle, bool black)
{
    uint8_t fill_val = black ? 0xFF : 0x00;
    memset(g_frame_buffer, fill_val, sizeof(g_frame_buffer));
    lcd_full_screen_refresh(panel_handle, g_frame_buffer);
}

/**
 * @brief 动态测试图案 3 棋盘格生成
 */
void generate_chessboard_pattern(uint8_t *buf)
{
    for (int page = 0; page < 8; page++) {
        for (int col = 0; col < LCD_H_RES; col++) {
            if (((col / 8) + page) % 2 == 0) {
                buf[page * LCD_H_RES + col] = 0xF0;
            } else {
                buf[page * LCD_H_RES + col] = 0x0F;
            }
        }
    }
}

void app_main(void)
{
    printf("开始初始化 ST7567 (128x64 SPI)...\n");

    // 配置 SPI 总线
    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_LCD_SCLK,
        .mosi_io_num = PIN_LCD_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_BUFFER_SIZE,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    // 配置 ESP-LCD SPI IO
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_LCD_DC,
        .cs_gpio_num = PIN_LCD_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0, 
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    // 挂载我们编写的 st7567 组件
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_LCD_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 1,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7567(io_handle, &panel_config, &panel_handle));

    // 硬件复位并执行芯片原厂初始化流
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    printf("ST7567 驱动初始化完成！启动 4 项全屏交替测试流程...\n");

    while (1) {
        // 测试 1：全屏清空（全白/全灭）
        printf("[1/4] 全屏清空 (全白)\n");
        lcd_clear_screen(panel_handle, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 测试 2：全屏点亮（全黑/全亮）
        printf("[2/4] 全屏点亮 (全黑)\n");
        lcd_clear_screen(panel_handle, true);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 测试 3：写入几何棋盘格图案
        printf("[3/4] 刷新棋盘格图案\n");
        generate_chessboard_pattern(g_frame_buffer);
        lcd_full_screen_refresh(panel_handle, g_frame_buffer);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 测试 4：全屏显示自定义图像数组
        printf("[4/4] 刷新全屏自定义测试图片\n");
        lcd_full_screen_refresh(panel_handle, g_test_image_12864);
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}