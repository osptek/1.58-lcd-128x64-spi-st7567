# 1.58" 128×64 LCD SPI module (ST7567) — documentation & samples

**简体中文：** [`README.md`](README.md)

---

> This repository provides **sample projects** for this module, together with datasheets, specifications, and interface documentation for selection reference and integration.

## Product overview

| Item | Description |
|:--|:--|
| Module | 1.58-inch **LCD** (monochrome dot matrix), **128×64** resolution |
| Interface | **SPI** |
| Driver IC | **ST7567** |
| Spec ID | **`1.58-lcd-128x64-spi-st7567`** is the common product designation in documentation |

---

## Repository layout

### Top-level

| Path | Contents |
|:--|:--|
| `assets/` | Demo images / videos for sample projects (when available) |
| `docs/` | Datasheets, specifications, initialization references |
| `examples/` | **Sample projects** |

### `examples/` layout

| Location | Description (internal package folder) |
|:--|:--|
| `examples/` root | **ESP-IDF代码** (ST7567 SPI display) |

### Sample project paths

| Description | Path |
|:--|:--|
| ST7567 SPI display | `examples/esp32s3-idf5_st7567-spi/` |

#### Sample demo

<p align="center">
  <video src="assets/demo_1.mp4" controls width="480"></video>
</p>
