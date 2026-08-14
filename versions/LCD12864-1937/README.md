<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 1.58″ LCD 128×64（ST7567 · SPI）</h1>

<p align="center"><b>单色 LCD 模组 · SPI · ST7567</b></p>

<p align="center"><a href="./README_EN.md">English</a> | 简体中文 · <a href="../../README.md">规格族索引</a></p>

<p align="center">
  <img alt="Size: 1.58 inch" src="https://img.shields.io/badge/Size-1.58%22-3498DB?style=flat-square" />
  <img alt="Resolution: 128x64" src="https://img.shields.io/badge/Resolution-128%C3%9764-8E44AD?style=flat-square" />
  <img alt="Interface: SPI" src="https://img.shields.io/badge/Interface-SPI-27AE60?style=flat-square" />
  <img alt="Driver: ST7567" src="https://img.shields.io/badge/Driver-ST7567-E7352C?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 1.58 寸 LCD 128×64 模组（ST7567）宣传图" src="./images/product.png" width="640" /></p>

## 目录

- [产品简介](#产品简介)
- [规格参数](#规格参数)
- [示例工程](#示例工程)
- [仓库结构](#仓库结构)
- [相关资料](#相关资料)
- [购买链接](#购买链接)
- [技术支持](#技术支持)

---

## 产品简介

OSPTEK **1.58 寸 128×64 LCD** 是一款 **SPI** 单色点阵显示模组，显示驱动为 **ST7567**（FSTN）。适合状态栏、菜单提示、仪表与低功耗信息显示等场景。

规格标识（仓库名）：`1.58-lcd-128x64-spi-st7567`

当前模组版本：**LCD12864-1937**。电气与外形细节以 [`docs/LCD12864-1937.pdf`](./docs/LCD12864-1937.pdf) 为准。

## 规格参数

| 项目 | 规格 |
| ---- | ---- |
| 尺寸 | 1.58 英寸 |
| 类型 | LCD / FSTN（单色） |
| 分辨率 | 128×64 |
| 接口 | SPI（4-wire） |
| 驱动 IC | ST7567 |

> 完整外形尺寸、FPC 定义、供电与时序以产品规格书 / 驱动手册为准。

## 示例工程

| 说明 | 路径 |
| ---- | ---- |
| ESP32-S3 · ST7567 SPI 显示 | [`examples/esp32s3-idf5_st7567-spi/`](./examples/esp32s3-idf5_st7567-spi/) |

示例效果见 [`assets/demo_1.mp4`](./assets/demo_1.mp4)。

## 仓库结构

```text
1.58-lcd-128x64-spi-st7567/                                # 仓库根（导航见 ../../README.md）
└── versions/
    └── LCD12864-1937/                                # 本料号完整资料
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        └── examples/
```

## 相关资料

### 本产品资料

| 资料 | 链接 |
| ---- | ---- |
| 产品规格书（LCD12864-1937） | [`docs/LCD12864-1937.pdf`](./docs/LCD12864-1937.pdf) |
| 驱动 IC 数据手册（ST7567） | [`docs/ST7567_V1.5_100806.pdf`](./docs/ST7567_V1.5_100806.pdf) |
| 初始化代码 | [`docs/ST7567.C`](./docs/ST7567.C) |

### 示例工程

- [ESP32-S3 ST7567 SPI 显示](./examples/esp32s3-idf5_st7567-spi/)

## 购买链接

<p align="center">
  <a href="https://shop110742373.taobao.com/"><img alt="淘宝官方店铺" src="https://img.shields.io/badge/淘宝-官方店铺-FF6A00?style=for-the-badge" /></a>
  &nbsp;&nbsp;
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="速卖通官方店铺" src="https://img.shields.io/badge/速卖通-官方店铺-FF6A00?style=for-the-badge" /></a>
</p>

**国内（淘宝）**

- 店铺：[鱼鹰光电工厂店](https://shop110742373.taobao.com/)

**海外（AliExpress）**

- 店铺：[OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

## 技术支持

- 技术支持 / 产品咨询：<luyu@osptek.com>
- QQ 技术交流群：**985881096**
- 公司官网：<https://osptek.com/>
- 有任何问题，都可以在本仓库 Issues 中提问

---

<p align="center"><sub>© 2026 OSPTEK 鱼鹰光电 · 本仓库资料采用 CC BY 4.0 许可</sub></p>
