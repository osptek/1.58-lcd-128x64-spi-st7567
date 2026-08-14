#pragma once

#include "esp_lcd_panel_vendor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建 ST7567 液晶显示屏驱动句柄
 * * @param[in] io SPI IO 句柄（由 esp_lcd_new_panel_io_spi 创建）
 * @param[in] panel_dev_config 屏幕通用配置（包含复位引脚、颜色深度等）
 * @param[out] ret_panel 返回的屏幕面板句柄
 * @return esp_err_t ESP_OK 表示成功
 */
esp_err_t esp_lcd_new_panel_st7567(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel);

#ifdef __cplusplus
}
#endif
