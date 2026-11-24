# ESP-IDF Hello World (multi-board)

PlatformIO project, framework **ESP-IDF 6.0.1** (`espressif32` platform), C. One code base built for several
physical ESP32 / ESP32-S3 boards ("modules"). Host is Windows; `pio` is not on PATH.

## Commands

```sh
PIO=~/.platformio/penv/Scripts/pio.exe
$PIO run -e m15_esp32s3                       # build one module
$PIO run -e m15_esp32s3 -t upload -t monitor  # flash + serial monitor
$PIO run                                      # builds default_envs only (m12)

# Identify a board's chip/flash/PSRAM (read-only, resets the board):
~/.platformio/penv/Scripts/python.exe ~/.platformio/packages/tool-esptoolpy/esptool.py --port COMx flash_id
```

A first S3 build takes ~3–5 min; run multi-env builds in the background.

## Modules

| Env | Module | Board | Memory (verified with esptool) | Port |
|---|---|---|---|---|
| `m12_doit_esp32` | 12 | DOIT ESP32 DEVKIT V1 (WROOM-32) | 4 MB flash, no PSRAM | COM4 |
| `m14_esp32s3` | 14 | ESP32-S3-DevKitC-1 N16R8, on-board antenna | 16 MB quad 3.3 V + 8 MB PSRAM | COM10 |
| `m15_esp32s3` | 15 | ESP32-S3-DevKitC-1 N16R8, on-board antenna | 16 MB quad 3.3 V + 8 MB PSRAM | COM7 |
| `m16_esp32s3` | 16 | ESP32-S3-DevKitC-1 N16R8, on-board antenna | same | COM9 |
| `m17_esp32s3` | 17 | ESP32-S3-DevKitC-1**U** N16R8, **external antenna** (U.FL) | same | COM12 |
| `m18_esp32s3` | 18 | ESP32-S3-DevKitC-1**U** N16R8, **external antenna** (U.FL) | same | COM13 |
| `m19_esp32s3` | 19 | ESP32-S3-DevKitC-1 N32R16V | 32 MB octal 1.8 V + 16 MB octal PSRAM — **not yet verified** | COM11 (native USB) |
| `m20_lilygo_sim7080g` | 20 | LILYGO T-SIM7080G-S3, variant **without PMU** (WROOM-1 N16R8, SIM7080G) | 16 MB quad + 8 MB PSRAM | not set yet |

Modules 14–18 have CH343 USB-UART chips (clone boards).

## How the configuration fits together

- `platformio.ini`
  - `[env]` holds settings shared by every board.
  - Each `[hw_*]` section is a hardware variant: `board`, flash mode and size, `-DBOARD_xxx`, and `board_build.cmake_extra_args = -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.<variant>[;...]"`.
  - `[hw_s3_devkitc_n16r8u]` extends `hw_s3_devkitc_n16r8` and only adds `-DBOARD_ANTENNA_EXTERNAL=1`.
  - Each `[env:mNN_*]` is one physical module: `extends = hw_*`, adds `-DBOARD_MODULE_ID=NN`, and sets `upload_port`/`monitor_port`.
  - To add a module, add a new `[env:mNN_*]` section and reuse an existing `hw_*`.
- `sdkconfig.defaults` holds settings shared by all boards. `sdkconfig.defaults.<variant>` holds per-variant settings (`esp32_4mb`, `s3_n16r8`, `s3_n32r16v`). `sdkconfig.defaults.usb_console` puts the primary console on USB-Serial-JTAG (used by m19 and m20).
- `sdkconfig.<env>` files are generated and git-ignored. Defaults are applied **only when that file is regenerated**: after changing a defaults file or `SDKCONFIG_DEFAULTS`, delete `sdkconfig.<env>` and rebuild.
- `include/board.h` maps `BOARD_xxx` to `BOARD_NAME`, pins and constants. It raises `#error` if no board is selected. Code should use these macros, not hard-coded pins.

## Hardware gotchas

- **Verify a board's flash type with `esptool flash_id` before adding an octal config.** Octal flash needs `board_build.flash_mode = dout` plus `CONFIG_ESPTOOLPY_OCT_FLASH=y`. A wrong flash mode builds fine but won't boot.
- With QIO selected, `CONFIG_ESPTOOLPY_FLASHMODE` shows `"dio"`. That's normal: the bootloader switches the flash to QIO at startup.
- **Module 19** is used through its native USB port. The firmware currently on it exposes TinyUSB CDC (VID 303A, PID 4001), so esptool can't auto-reset it. The first flash needs manual download mode: hold BOOT, tap RESET, release BOOT. The COM number may change once it enumerates as USB-Serial-JTAG.
- The S3 RGB LED (WS2812) is on GPIO48 for the N16R8 clone boards and GPIO38 for the octal / v1.1 boards. Not yet tested on hardware.
- **LILYGO T-SIM7080G-S3 (Module 20):** this unit is the "Standard" variant **without** the AXP2101 power management unit (PMU), so there is no PMU init.
  - Pins in `board.h` come from LILYGO's "T-SIM7080G-S3 Standard PINOUT" diagram (a copy the user has is at `OneDrive\Desktop\temp\`).
  - LilyGO's GitHub examples are for the PMU variant and use **different** modem pins (PWR 41 / DTR 42 / RI 3). Don't copy pins from them.
  - The camera uses GPIO35/36/37, which octal PSRAM also needs, so the two can't be used together. Its PSRAM type and size are **not yet verified**; run esptool `flash_id` once the board is connected.
- Don't probe COM3: it's an LG device, not an ESP board.

## Repo

- GitHub: `TomHildebrand/ESP-IDF-Hello-World`, branch `main`.
- `README.md` still describes the original single-board setup and is out of date.
