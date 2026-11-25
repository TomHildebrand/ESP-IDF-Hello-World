# ESP-IDF Hello World: project blueprint

A starting point for ESP32 / ESP32-S3 projects that use [PlatformIO](https://platformio.org/) and the Espressif **ESP-IDF** framework. Projects created from it can be built for any board in a shared inventory.

The board definitions come from [esp32-boards](https://github.com/TomHildebrand/esp32-boards), included as a git submodule at `boards/`. That repo holds the hardware types, the physical modules, their pins and their capabilities.

The example application prints:
- which module and board it runs on
- chip information
- the result of an on-board LED test: a plain LED blinks, a WS2812 RGB LED cycles through colours

## Starting a new project

1. On GitHub, click **Use this template** (or clone and re-push this repo).
2. Clone the new repo with submodules:

   ```sh
   git clone --recurse-submodules <url>
   # or, in an existing clone:
   git submodule update --init
   ```
3. Change these settings in `platformio.ini`:
   - `build_dir` (one folder per project)
   - `default_envs` (the modules this project targets)
4. Replace `app_main()` in `src/main.c` with the application.
5. In `include/app_config.h`, state what the app needs from the board, and where external parts are wired.

## Build, flash, monitor

Pick a module in the VS Code PlatformIO environment picker, or pass `-e <environment>` on the command line:

```sh
pio run -e m15_esp32s3                        # build
pio run -e m15_esp32s3 -t upload -t monitor   # flash, then open the serial monitor (115200 baud)
```

Plain `pio run` builds the modules listed in `default_envs`. For the list of modules, see the [esp32-boards README](https://github.com/TomHildebrand/esp32-boards#modules).

## How it fits together

| Path | Purpose |
|---|---|
| `boards/` | **Submodule** [esp32-boards](https://github.com/TomHildebrand/esp32-boards): board types, module inventory, `board.h`, LED driver, ESP-IDF defaults per board |
| `platformio.ini` | Includes `boards/boards.ini`; app-wide settings (`framework`, `build_flags`, `lib_deps`) |
| `sdkconfig.defaults` | App ESP-IDF settings, applied after the board defaults |
| `include/app_config.h` | What the app needs from the board (checked at compile time), and the pins of external parts |
| `src/main.c` | Application (`app_main`) |
| `src/CMakeLists.txt` | Registers `src/` with ESP-IDF. The board sources in `boards/src/` are added automatically by `boards.ini`. |

Application code uses the `board.h` macros (`BOARD_PIN_LED`, `BOARD_PSRAM_MB`, `BOARD_HAS_MODEM`, …) instead of hard-coded pin numbers. To add a board, or change one, edit the esp32-boards repo, then update the submodule here:

```sh
git submodule update --remote boards
git commit -am "Update boards"
```

> **Note:** ESP-IDF applies the defaults files only when it generates `sdkconfig.<environment>`. After changing a defaults file, delete the matching `sdkconfig.<environment>` file and rebuild.
