# ESP-IDF Hello World: project blueprint

This is the **blueprint for new ESP32 projects**. New projects are created from it (GitHub template),
and `app_main()` in `src/main.c` (Hello World + LED test) is replaced with the real application.

- PlatformIO, framework **ESP-IDF 6.0.1** (`espressif32` platform), C.
- Host is Windows; `pio` is not on PATH.

## Board layer: `boards/` submodule

All board data lives in the shared repo **`TomHildebrand/esp32-boards`**, included as a git submodule at `boards/`:
- hardware types and the module inventory (`boards/boards.ini`)
- pins and capabilities (`boards/include/board.h`)
- the LED driver
- ESP-IDF defaults per hardware type
- hardware gotchas

**Read [boards/CLAUDE.md](boards/CLAUDE.md) for the module table, COM ports and hardware gotchas.**

- **Don't edit files under `boards/` as part of a project change.** Board changes are made and pushed in the esp32-boards repo. Then run `git submodule update --remote boards` here, and commit the new submodule pointer.
- After cloning: `git submodule update --init`.

## What belongs to the app (this repo)

| Path | Purpose |
|---|---|
| `platformio.ini` | `extra_configs = boards/boards.ini`, `[boards] dir = boards`, `default_envs` (modules this project targets), `[env]` with `framework`, app-wide `build_flags`, `lib_deps`, and `build_dir` outside OneDrive |
| `sdkconfig.defaults` | App ESP-IDF settings. Applied **after** the board defaults, so it can override them. Must exist (may be empty). |
| `include/app_config.h` | App requirements (`#if BOARD_PSRAM_MB < 2` → `#error`) and wiring of external parts per hardware type. Includes `board.h`. |
| `src/main.c` | `app_main()`: replace in real projects |
| `src/CMakeLists.txt` | Registers `src/` as the app component. The board sources in `boards/src/` are added by `build_src_filter` in `boards.ini`. Don't also add them here: they would be compiled twice and fail to link with "multiple definition". |

Code uses `board.h` macros (`BOARD_PIN_LED`, `BOARD_FLASH_MB`, …) and `app_config.h` pins, never hard-coded GPIO numbers.

## Commands

```sh
PIO=~/.platformio/penv/Scripts/pio.exe
$PIO run -e m15_esp32s3                       # build one module
$PIO run -e m15_esp32s3 -t upload -t monitor  # flash + serial monitor
$PIO run                                      # builds default_envs only
```

- A first S3 build takes ~3–5 min; run multi-env builds in the background.
- `sdkconfig.<env>` files are generated and git-ignored. Defaults apply **only when that file is regenerated**. After changing any defaults file, delete `sdkconfig.<env>` and rebuild.
- The project is in OneDrive, so `build_dir` is `C:/pio-build/ESP-IDF-Hello-World`. Builds inside OneDrive caused locked or vanishing `.o` files and link errors. Every project created from this blueprint needs its own `build_dir` outside OneDrive.

## Plan: porting legacy Arduino projects

Existing Arduino projects (hard-coded pins) will be ported to the board layer.
- Steps: see the "Porting an existing Arduino project" section in [boards/README.md](boards/README.md).
- The Arduino starting point is `boards/examples/arduino-blink`.
- Phase 1 only makes them build per module, on Arduino core 2.0.17.
- Clean-up, refactoring and linting come later, possibly together with a move to Arduino core 3.x.

## Repo

GitHub: `TomHildebrand/ESP-IDF-Hello-World`, branch `main`.
