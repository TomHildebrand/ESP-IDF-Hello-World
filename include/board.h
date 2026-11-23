#pragma once
/*
 * Board-specific constants. The hardware variant is selected by a -DBOARD_xxx define
 * and the physical module by -DBOARD_MODULE_ID=nn, both set in platformio.ini.
 */

#ifndef BOARD_MODULE_ID
    #define BOARD_MODULE_ID     0
#endif

/* ---------------------------------------------------------------------------- */
#if defined(BOARD_DOIT_ESP32_DEVKIT_V1)
    #define BOARD_NAME          "DOIT ESP32 DEVKIT V1"
    #define BOARD_PIN_LED       2       // blue on-board LED
    #define BOARD_LED_IS_RGB    0
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

/* ---------------------------------------------------------------------------- */
#elif defined(BOARD_S3_DEVKITC1_N16R8)
    #define BOARD_NAME          "ESP32-S3-DevKitC-1 N16R8"
    #define BOARD_PIN_LED       48      // WS2812 RGB LED (GPIO38 on Espressif DevKitC-1 v1.1)
    #define BOARD_LED_IS_RGB    1
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

/* ---------------------------------------------------------------------------- */
#elif defined(BOARD_S3_DEVKITC1_N16R8V) || defined(BOARD_S3_DEVKITC1_N32R16V)
    #if defined(BOARD_S3_DEVKITC1_N16R8V)
        #define BOARD_NAME      "ESP32-S3-DevKitC-1 N16R8V"
    #else
        #define BOARD_NAME      "ESP32-S3-DevKitC-1 N32R16V"
    #endif
    // Octal-flash (1.8 V) modules: GPIO35-37 are used by the flash/PSRAM bus.
    #define BOARD_PIN_LED       38      // WS2812 RGB LED (GPIO48 on DevKitC-1 v1.0 boards)
    #define BOARD_LED_IS_RGB    1
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

/* ---------------------------------------------------------------------------- */
#elif defined(BOARD_LILYGO_T_SIM7080G_S3)
    // Pin map from LilyGO's T-SIM7080G examples (utilities.h) - verify against
    // the schematic for your board revision.
    #define BOARD_NAME          "LILYGO T-SIM7080G-S3"
    #define BOARD_PIN_LED       (-1)    // no user LED
    #define BOARD_LED_IS_RGB    0
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

    // SIM7080G modem (UART pins from the ESP32-S3's point of view)
    #define BOARD_MODEM_PWR_PIN 41
    #define BOARD_MODEM_DTR_PIN 42
    #define BOARD_MODEM_RI_PIN  3
    #define BOARD_MODEM_RX_PIN  4       // ESP RX  <- modem TX
    #define BOARD_MODEM_TX_PIN  5       // ESP TX  -> modem RX

    // AXP2101 PMU (powers the modem, camera and SD rails - must be enabled first)
    #define BOARD_I2C_SDA       15
    #define BOARD_I2C_SCL       7
    #define BOARD_PMU_IRQ_PIN   6

/* ---------------------------------------------------------------------------- */
#else
    #error "No board selected: add -DBOARD_xxx to build_flags in platformio.ini"
#endif
