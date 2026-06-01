#include <stdlib.h>
#include <string.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_st7567.h"

static const char *TAG = "st7567";

// ST7567 常用指令集
#define ST7567_CMD_DISPLAY_ON         0xAF
#define ST7567_CMD_DISPLAY_OFF        0xAE
#define ST7567_CMD_START_LINE         0x40
#define ST7567_CMD_PAGE_ADDRESS       0xB0
#define ST7567_CMD_COL_ADDR_MSB       0x10
#define ST7567_CMD_COL_ADDR_LSB       0x00
#define ST7567_CMD_SEG_NORMAL         0xA0
#define ST7567_CMD_SEG_REVERSE        0xA1
#define ST7567_CMD_REG_RES_RATIO      0x20
#define ST7567_CMD_POWER_CTRL         0x2F
#define ST7567_CMD_EV_SET             0x81
#define ST7567_CMD_BIAS_9             0xA2
#define ST7567_CMD_RESET              0xE2
#define ST7567_CMD_SCAN_NORMAL        0xC0
#define ST7567_CMD_SCAN_REVERSE       0xC8

typedef struct {
    esp_lcd_panel_t base;
    esp_lcd_panel_io_handle_t io;
    int reset_gpio_num;
    bool reset_level;
} st7567_panel_t;

static esp_err_t panel_st7567_del(esp_lcd_panel_t *panel);
static esp_err_t panel_st7567_reset(esp_lcd_panel_t *panel);
static esp_err_t panel_st7567_init(esp_lcd_panel_t *panel);
static esp_err_t panel_st7567_draw_bitmap(esp_lcd_panel_t *panel, int x_start, int y_start, int x_end, int y_end, const void *color_data);
static esp_err_t panel_st7567_invert_color(esp_lcd_panel_t *panel, bool invert_color_data);

esp_err_t esp_lcd_new_panel_st7567(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel)
{
    esp_err_t ret = ESP_OK;
    st7567_panel_t *st7567 = NULL;
    ESP_GOTO_ON_FALSE(io && panel_dev_config && ret_panel, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");

    st7567 = calloc(1, sizeof(st7567_panel_t));
    ESP_GOTO_ON_FALSE(st7567, ESP_ERR_NO_MEM, err, TAG, "no mem for st7567 panel");

    if (panel_dev_config->reset_gpio_num >= 0) {
        gpio_config_t io_conf = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << panel_dev_config->reset_gpio_num,
        };
        ESP_GOTO_ON_ERROR(gpio_config(&io_conf), err, TAG, "configure GPIO for RST line failed");
    }

    st7567->io = io;
    st7567->reset_gpio_num = panel_dev_config->reset_gpio_num;
    st7567->reset_level = panel_dev_config->flags.reset_active_high;
    st7567->base.del = panel_st7567_del;
    st7567->base.reset = panel_st7567_reset;
    st7567->base.init = panel_st7567_init;
    st7567->base.draw_bitmap = panel_st7567_draw_bitmap;
    st7567->base.invert_color = panel_st7567_invert_color;
    
    // 暂不实现或单色屏不需要的虚函数接口
    st7567->base.disp_on_off = NULL; 
    st7567->base.mirror = NULL;
    st7567->base.swap_xy = NULL;
    st7567->base.set_gap = NULL;

    *ret_panel = &(st7567->base);
    return ESP_OK;

err:
    if (st7567) {
        free(st7567);
    }
    return ret;
}

static esp_err_t panel_st7567_del(esp_lcd_panel_t *panel)
{
    st7567_panel_t *st7567 = __containerof(panel, st7567_panel_t, base);
    if (st7567->reset_gpio_num >= 0) {
        gpio_reset_pin(st7567->reset_gpio_num);
    }
    free(st7567);
    return ESP_OK;
}

static esp_err_t panel_st7567_reset(esp_lcd_panel_t *panel)
{
    st7567_panel_t *st7567 = __containerof(panel, st7567_panel_t, base);

    if (st7567->reset_gpio_num >= 0) {
        gpio_set_level(st7567->reset_gpio_num, st7567->reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(st7567->reset_gpio_num, !st7567->reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
    } else {
        // 软件复位
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, ST7567_CMD_RESET, NULL, 0), TAG, "send command failed");
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    return ESP_OK;
}

static esp_err_t panel_st7567_init(esp_lcd_panel_t *panel)
{
    st7567_panel_t *st7567 = __containerof(panel, st7567_panel_t, base);
    
    // 1. 软件复位
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0xE2, NULL, 0), TAG, "send CMD 0xE2 failed");
    vTaskDelay(pdMS_TO_TICKS(5));
    
    // 2. 分步升压序列 (Power Control 步进)
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x2C, NULL, 0), TAG, "send CMD 0x2C failed"); /* 升压步骤 1 */
    vTaskDelay(pdMS_TO_TICKS(5));
    
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x2E, NULL, 0), TAG, "send CMD 0x2E failed"); /* 升压步骤 2 */
    vTaskDelay(pdMS_TO_TICKS(5));
    
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x2F, NULL, 0), TAG, "send CMD 0x2F failed"); /* 升压步骤 3 */
    vTaskDelay(pdMS_TO_TICKS(5));
    
    // 3. 对比度调节
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x25, NULL, 0), TAG, "send CMD 0x25 failed"); /* 粗调对比度值 0x20~0x27 */
    
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x81, NULL, 0), TAG, "send CMD 0x81 failed"); /* 对比度微调指令 */
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x1A, NULL, 0), TAG, "send CMD 0x1A failed"); /* 对比度微调值 0x00~0x3f */
    
    // 4. 屏幕显示及扫描方向配置
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0xA2, NULL, 0), TAG, "send CMD 0xA2 failed"); /* 1/9 bias */
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0xC8, NULL, 0), TAG, "send CMD 0xC8 failed"); /* 行扫描 (COM 反向) */
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0xA0, NULL, 0), TAG, "send CMD 0xA0 failed"); /* 列扫描 (SEG 正向) */
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x40, NULL, 0), TAG, "send CMD 0x40 failed"); /* 起始行 */
    
    // 5. 开启显示
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0x40, NULL, 0), TAG, "send CMD 0x40 failed"); /* 重设一次确保起始行在顶端 */
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, 0xAF, NULL, 0), TAG, "send CMD 0xAF failed"); /* 开显示 */

    return ESP_OK;
}

static esp_err_t panel_st7567_draw_bitmap(esp_lcd_panel_t *panel, int x_start, int y_start, int x_end, int y_end, const void *color_data)
{
    st7567_panel_t *st7567 = __containerof(panel, st7567_panel_t, base);
    
    // 注意：ST7567 是 Page 架构（1个Page = 8个纵向像素）
    // 输入的 y_start 和 y_end 是像素坐标。
    int page_start = y_start / 8;
    int page_end = (y_end - 1) / 8;
    int x_len = x_end - x_start;

    const uint8_t *p = (const uint8_t *)color_data;

    for (int page = page_start; page <= page_end; page++) {
        // 1. 设置 Page 地址
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, ST7567_CMD_PAGE_ADDRESS | (page & 0x0F), NULL, 0), TAG, "set page failed");
        
        // 2. 设置列高低位地址 (ST7567 列地址从 x_start 开始)
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, ST7567_CMD_COL_ADDR_MSB | ((x_start >> 4) & 0x0F), NULL, 0), TAG, "set col MSB failed");
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, ST7567_CMD_COL_ADDR_LSB | (x_start & 0x0F), NULL, 0), TAG, "set col LSB failed");
        
        // 3. 写入单页横向像素数据 (长度为宽度像素数)
        // 注意：传入的数据格式必须已经按照 ST7567 的垂直字节排布（即 LVGL 单色格式）处理好
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_color(st7567->io, -1, p, x_len), TAG, "tx columns data failed");
        p += x_len;
    }

    return ESP_OK;
}

static esp_err_t panel_st7567_invert_color(esp_lcd_panel_t *panel, bool invert_color_data)
{
    st7567_panel_t *st7567 = __containerof(panel, st7567_panel_t, base);
    uint8_t cmd = invert_color_data ? 0xA7 : 0xA6; // 0xA7 全显反转, 0xA6 正常显示
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7567->io, cmd, NULL, 0), TAG, "invert color failed");
    return ESP_OK;
}