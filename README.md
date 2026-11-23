# ESP-IDF Hello World

A minimal ESP32 "Hello World" project using the [PlatformIO](https://platformio.org/) build system and the Espressif **ESP-IDF** framework.

On boot it prints chip information (core count, silicon revision, flash size, free heap), then prints a counter once per second over the serial port.

## Hardware

- Board: `esp32dev` (any generic ESP32 dev board). To target a different board, change `board` in [platformio.ini](platformio.ini).

## Build, flash, monitor

```sh
pio run                      # build
pio run -t upload            # flash
pio device monitor           # serial monitor at 115200 baud
```

You can also use the PlatformIO toolbar in VS Code.

## Project layout

| Path | Purpose |
|------|---------|
| `platformio.ini` | PlatformIO environment (board, framework, monitor speed) |
| `CMakeLists.txt` | ESP-IDF top-level CMake project |
| `src/main.c` | Application entry point (`app_main`) |
| `src/CMakeLists.txt` | Registers the source files as an ESP-IDF component |
| `include/`, `lib/`, `test/` | Headers, private libraries, and unit tests |
