# ESP-IDF Hello World

A minimal ESP32 / ESP32-S3 "Hello World" project using the [PlatformIO](https://platformio.org/) build system and the Espressif **ESP-IDF** framework. The same code builds for several different dev boards.

On boot it prints which module and board it is running on, plus chip information: core count, silicon revision, flash size and free heap. After that it prints a counter once per second over the serial port.

## Supported boards

Each physical board ("module") has its own PlatformIO environment:

| Environment | Board | Flash / PSRAM |
|---|---|---|
| `m12_doit_esp32` | DOIT ESP32 DEVKIT V1 (ESP32-WROOM-32) | 4 MB / – |
| `m14_esp32s3` … `m16_esp32s3` | ESP32-S3-DevKitC-1 N16R8 (on-board antenna) | 16 MB quad / 8 MB |
| `m17_esp32s3`, `m18_esp32s3` | ESP32-S3-DevKitC-1U N16R8 (external antenna) | 16 MB quad / 8 MB |
| `m19_esp32s3` | ESP32-S3-DevKitC-1 N32R16V | 32 MB octal / 16 MB |
| `m20_lilygo_sim7080g` | LILYGO T-SIM7080G-S3 | 16 MB quad / 8 MB |

Each environment's serial port is set in [platformio.ini](platformio.ini).

## Build, flash, monitor

To choose a board, use the environment picker in the VS Code PlatformIO status bar, or pass `-e <environment>` on the command line:

```sh
pio run -e m15_esp32s3                        # build
pio run -e m15_esp32s3 -t upload              # flash
pio run -e m15_esp32s3 -t upload -t monitor   # flash, then open the serial monitor (115200 baud)
```

Plain `pio run` builds only the environments listed in `default_envs`.

## How the board configuration works

- **[platformio.ini](platformio.ini)** has three kinds of sections:
  - `[env]` holds settings shared by every board.
  - Each `[hw_*]` section describes one hardware variant: board definition, flash/PSRAM setup, and which ESP-IDF defaults files to use.
  - Each `[env:mNN_*]` section is one physical module. It extends a `[hw_*]` section and adds its module number and COM port.
- **`sdkconfig.defaults`** holds ESP-IDF settings shared by all boards. **`sdkconfig.defaults.<variant>`** holds the settings for one hardware variant (flash size and mode, PSRAM, console).
- **[include/board.h](include/board.h)** defines the board name, pins and other constants for each variant. Application code should use these macros instead of hard-coded pin numbers.

### Adding a board

1. If the board's hardware matches an existing `[hw_*]` section, add a new `[env:mNN_*]` that extends it and sets the COM port.
2. Otherwise:
   - add a new `[hw_*]` section in [platformio.ini](platformio.ini)
   - add a `sdkconfig.defaults.<variant>` file
   - add a matching `#elif` block in [include/board.h](include/board.h)
3. To check a board's actual flash type and PSRAM, run `esptool.py --port COMx flash_id`. The markings on boards are not always accurate.

> **Note:** ESP-IDF applies the defaults files only when it generates `sdkconfig.<environment>`. After changing a defaults file, delete the matching `sdkconfig.<environment>` file and rebuild.

## Project layout

| Path | Purpose |
|------|---------|
| `platformio.ini` | PlatformIO environments: hardware variants and physical modules |
| `sdkconfig.defaults*` | ESP-IDF configuration defaults (shared and per hardware variant) |
| `include/board.h` | Per-board pins and constants |
| `CMakeLists.txt` | ESP-IDF top-level CMake project |
| `src/main.c` | Application entry point (`app_main`) |
| `src/CMakeLists.txt` | Registers the source files as an ESP-IDF component |
| `CLAUDE.md` | Project notes for Claude Code |
| `lib/`, `test/` | Private libraries and unit tests |
