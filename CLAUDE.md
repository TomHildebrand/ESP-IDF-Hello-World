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

The project is in OneDrive, so `build_dir` is set to `C:/pio-build/ESP-IDF-Hello-World` (outside OneDrive). Builds inside OneDrive caused locked or vanishing `.o` files and link errors. Don't move the build folder back.

## Modules

| Env | Module | Board | Memory (verified with esptool) | User LED | Port |
|---|---|---|---|---|---|
| `m12_doit_esp32` | 12 | ESP32 DEVKIT V1 **38-pin** clone, WROOM-**32U**, **external antenna** (U.FL) | ESP32-D0WD-V3, 4 MB flash, no PSRAM | **none** (verified; red power LED only) | COM4 (CP2102) |
| `m14_esp32s3` | 14 | ESP32-S3-DevKitC-1 N16R8 **44-pin** clone, on-board antenna | 16 MB quad 3.3 V + 8 MB PSRAM | WS2812 GPIO48 (verified) | COM10 |
| `m15_esp32s3` | 15 | ESP32-S3-DevKitC-1 N16R8 44-pin clone, on-board antenna | same | WS2812 GPIO48 (verified) | COM7 |
| `m16_esp32s3` | 16 | ESP32-S3-DevKitC-1 N16R8 44-pin clone, on-board antenna | same | WS2812 GPIO48 (verified) | COM9 |
| `m17_esp32s3` | 17 | ESP32-S3-DevKitC-1**U** N16R8 44-pin clone, **external antenna** (U.FL) | same | WS2812 GPIO48 (verified) | COM12 |
| `m18_esp32s3` | 18 | ESP32-S3-DevKitC-1**U** N16R8 44-pin clone, **external antenna** (U.FL) | same | WS2812 GPIO48 (verified) | COM13 |
| `m19_esp32s3` | 19 | **Genuine Espressif** ESP32-S3-DevKitC-1 N32R16V, 44-pin, on-board antenna | 32 MB octal flash + 16 MB octal PSRAM (1.8 V) | WS2812 GPIO38 (verified) | COM16 (native USB) |
| `m20_lilygo_sim7080g` | 20 | LILYGO T-SIM7080G-S3, variant **without PMU** (WROOM-1 N16R8, SIM7080G) | 16 MB quad + 8 MB PSRAM | none | not set yet |

Modules 14–18 are clone boards with CH343 USB-UART chips. The LED test in `main.c` (blink / RGB colour cycle) is used to verify `BOARD_PIN_LED` on each board; update the User LED column once it is confirmed.

## How the configuration fits together

- `platformio.ini`
  - `[env]` holds settings shared by every board.
  - Each `[hw_*]` section is a hardware variant: `board`, flash mode and size, `-DBOARD_xxx`, and `board_build.cmake_extra_args = -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.<variant>[;...]"`.
  - `[hw_s3_devkitc_n16r8u]` extends `hw_s3_devkitc_n16r8` and only adds `-DBOARD_ANTENNA_EXTERNAL=1`.
  - `[hw_esp32_devkit38_wroom32u]` (Module 12) reuses `hw_doit_esp32`'s board and memory settings, but is its own board type in `board.h`. It has an external antenna and **no user LED** (checked on hardware).
  - No module uses `[hw_doit_esp32]` directly any more. It stays as the base for genuine 30-pin DOIT DevKit V1 boards (LED on GPIO2).
  - Each `[env:mNN_*]` is one physical module: `extends = hw_*`, adds `-DBOARD_MODULE_ID=NN`, and sets `upload_port`/`monitor_port`.
  - To add a module, add a new `[env:mNN_*]` section and reuse an existing `hw_*`.
- `sdkconfig.defaults` holds settings shared by all boards. `sdkconfig.defaults.<variant>` holds per-variant settings (`esp32_4mb`, `s3_n16r8`, `s3_n32r16v`). `sdkconfig.defaults.usb_console` puts the primary console on USB-Serial-JTAG (used by m19 and m20).
- `sdkconfig.<env>` files are generated and git-ignored. Defaults are applied **only when that file is regenerated**: after changing a defaults file or `SDKCONFIG_DEFAULTS`, delete `sdkconfig.<env>` and rebuild.
- `include/board.h` maps `BOARD_xxx` to `BOARD_NAME`, pins and constants. It raises `#error` if no board is selected. Code should use these macros, not hard-coded pins.

## Hardware gotchas

- **Verify a board's flash type with `esptool flash_id` before adding an octal config.** Octal flash needs `board_build.flash_mode = dout` plus `CONFIG_ESPTOOLPY_OCT_FLASH=y`. A wrong flash mode builds fine but won't boot.
- With QIO selected, `CONFIG_ESPTOOLPY_FLASHMODE` shows `"dio"`. That's normal: the bootloader switches the flash to QIO at startup.
- **Module 19** is used through its native USB port. It shows up as USB-Serial-JTAG (VID 303A, PID 1001) on COM16, and uploads auto-reset it.
  - If it's ever running firmware that exposes its own USB CDC port instead (e.g. PID 4001), esptool can't reset it, and fails with "Invalid head of packet (0x1B)". Fix: hold BOOT, tap RESET, release BOOT, then upload.
  - After a manual download-mode entry, esptool's hard reset over USB-Serial-JTAG may leave the chip in download mode. Press RESET once.
  - When reading a USB-Serial-JTAG port from a script, open it with DTR and RTS deasserted (set `dtr`/`rts` to False **before** `open()`). Asserting them resets the chip into download mode.
- The S3 RGB LED (WS2812) is on GPIO48 for the N16R8 clone boards and GPIO38 for the octal / v1.1 boards. Not yet tested on hardware.
- **LILYGO T-SIM7080G-S3 (Module 20):** this unit is the "Standard" variant **without** the AXP2101 power management unit (PMU), so there is no PMU init.
  - Pins in `board.h` come from LILYGO's "T-SIM7080G-S3 Standard PINOUT" diagram (a copy the user has is at `OneDrive\Desktop\temp\`).
  - LilyGO's GitHub examples are for the PMU variant and use **different** modem pins (PWR 41 / DTR 42 / RI 3). Don't copy pins from them.
  - The camera uses GPIO35/36/37, which octal PSRAM also needs, so the two can't be used together. Its PSRAM type and size are **not yet verified**; run esptool `flash_id` once the board is connected.
- Don't probe COM3: it's an LG device, not an ESP board.

## Repo

- GitHub: `TomHildebrand/ESP-IDF-Hello-World`, branch `main`.
- `README.md` still describes the original single-board setup and is out of date.
