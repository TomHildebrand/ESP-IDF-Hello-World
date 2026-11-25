#pragma once
/*
 * App-level configuration: what this app requires from the board, and how its
 * external parts are wired. Board-intrinsic pins/capabilities come from board.h
 * (esp32-boards submodule); don't hard-code them here.
 */
#include "board.h"

/* ---- Requirements -----------------------------------------------------------
 * Fail the build for boards that can't run this app, e.g.:
 *
 *   #if BOARD_PSRAM_MB < 2
 *   #error "This app needs at least 2 MB PSRAM"
 *   #endif
 *   #if !BOARD_HAS_MODEM
 *   #error "This app needs the cellular modem"
 *   #endif
 *
 * Hello World runs on every board, so there are no checks here.
 */

/* ---- External wiring ----------------------------------------------------------
 * Pins of parts wired to the board by you (sensors, displays, relays). Use one
 * block per hardware type where they differ, e.g.:
 *
 *   #if defined(BOARD_S3_DEVKITC1_N16R8) || defined(BOARD_S3_DEVKITC1_N32R16V)
 *       #define APP_PIN_SENSOR   4
 *   #elif defined(BOARD_ESP32_DEVKIT_V1_38PIN)
 *       #define APP_PIN_SENSOR   15
 *   #else
 *       #error "Wiring for this board is not defined"
 *   #endif
 */
