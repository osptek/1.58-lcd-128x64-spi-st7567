<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 1.58″ LCD 128×64 (ST7567 · SPI)</h1>

<p align="center"><b>Monochrome LCD module · SPI · ST7567</b></p>

<p align="center"><a href="./README.md">简体中文</a> | English</p>

<p align="center">
  <img alt="Size: 1.58 inch" src="https://img.shields.io/badge/Size-1.58%22-3498DB?style=flat-square" />
  <img alt="Resolution: 128x64" src="https://img.shields.io/badge/Resolution-128%C3%9764-8E44AD?style=flat-square" />
  <img alt="Interface: SPI" src="https://img.shields.io/badge/Interface-SPI-27AE60?style=flat-square" />
  <img alt="Driver: ST7567" src="https://img.shields.io/badge/Driver-ST7567-E7352C?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 1.58″ LCD 128×64 module (ST7567) product image" src="./images/product.png" width="640" /></p>

## Contents

- [Overview](#overview)
- [Specifications](#specifications)
- [Sample projects](#sample-projects)
- [Repository layout](#repository-layout)
- [Resources](#resources)
- [Buy](#buy)
- [Support](#support)

---

## Overview

OSPTEK **1.58″ 128×64 LCD** is a **SPI** monochrome dot-matrix module driven by **ST7567** (FSTN). Suited to status bars, menus, meters, and low-power information displays.

Spec ID (repository name): `1.58-lcd-128x64-spi-st7567`

Current module version: **LCD12864-1937**. Electrical and mechanical details follow [`docs/LCD12864-1937.pdf`](./docs/LCD12864-1937.pdf).

## Specifications

| Item | Spec |
| ---- | ---- |
| Size | 1.58 inch |
| Type | LCD / FSTN (monochrome) |
| Resolution | 128×64 |
| Interface | SPI (4-wire) |
| Driver IC | ST7567 |

> Full outline, FPC definition, power, and timing follow the product datasheet / driver IC datasheet.

## Sample projects

| Description | Path |
| ---- | ---- |
| ESP32-S3 · ST7567 SPI display | [`examples/esp32s3-idf5_st7567-spi/`](./examples/esp32s3-idf5_st7567-spi/) |

## Repository layout

```text
1.58-lcd-128x64-spi-st7567/
├── README.md
├── README_EN.md
├── MODULE_VERSION.md
├── LICENSE
├── images/          # README assets
├── docs/            # datasheets, init
└── examples/        # sample projects
```

## Resources

### Product files

| Resource | Link |
| ---- | ---- |
| Product datasheet (LCD12864-1937) | [`docs/LCD12864-1937.pdf`](./docs/LCD12864-1937.pdf) |
| Driver IC datasheet (ST7567) | [`docs/ST7567_V1.5_100806.pdf`](./docs/ST7567_V1.5_100806.pdf) |
| Init code | [`docs/ST7567.C`](./docs/ST7567.C) |

### Samples

- [ESP32-S3 ST7567 SPI display](./examples/esp32s3-idf5_st7567-spi/)

## Buy

<p align="center">
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="AliExpress store" src="https://img.shields.io/badge/AliExpress-Official_Store-FF6A00?style=for-the-badge" /></a>
  &nbsp;&nbsp;
  <a href="https://shop110742373.taobao.com/"><img alt="Taobao store" src="https://img.shields.io/badge/Taobao-Official_Store-FF6A00?style=for-the-badge" /></a>
</p>

**Overseas (AliExpress)**

- Store: [OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

**China (Taobao)**

- Store: [鱼鹰光电工厂店](https://shop110742373.taobao.com/)

## Support

- Technical support / product inquiry: <luyu@osptek.com>
- QQ group (China): **985881096**
- Website: <https://osptek.com/>
- Feel free to open an Issue in this repository if you have any questions

---

<p align="center"><sub>© 2026 OSPTEK · Materials in this repository are licensed under CC BY 4.0</sub></p>
