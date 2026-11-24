#pragma once
/*
 * Board-specific constants. The hardware variant is selected by a -DBOARD_xxx define
 * and the physical module by -DBOARD_MODULE_ID=nn, both set in platformio.ini.
 */

#ifndef BOARD_MODULE_ID
    #define BOARD_MODULE_ID     0
#endif

/* 1 = external antenna via U.FL connector (e.g. WROOM-1U), 0 = on-board PCB antenna */
#ifndef BOARD_ANTENNA_EXTERNAL
    #define BOARD_ANTENNA_EXTERNAL  0
#endif

/* ---------------------------------------------------------------------------- */
#if defined(BOARD_DOIT_ESP32_DEVKIT_V1)
    #define BOARD_NAME          "DOIT ESP32 DEVKIT V1"
    #define BOARD_PIN_LED       2       // blue on-board LED
    #define BOARD_LED_IS_RGB    0
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

/* ---------------------------------------------------------------------------- */
#elif defined(BOARD_S3_DEVKITC1_N16R8)
    #if BOARD_ANTENNA_EXTERNAL
        #define BOARD_NAME      "ESP32-S3-DevKitC-1U N16R8, external antenna"
    #else
        #define BOARD_NAME      "ESP32-S3-DevKitC-1 N16R8, on-board antenna"
    #endif
    #define BOARD_PIN_LED       48      // WS2812 RGB LED (GPIO38 on Espressif DevKitC-1 v1.1)
    #define BOARD_LED_IS_RGB    1
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

/* ---------------------------------------------------------------------------- */
#elif defined(BOARD_S3_DEVKITC1_N32R16V)
    #define BOARD_NAME          "ESP32-S3-DevKitC-1 N32R16V"
    // Octal-flash (1.8 V) modules: GPIO35-37 are used by the flash/PSRAM bus.
    #define BOARD_PIN_LED       38      // WS2812 RGB LED (GPIO48 on DevKitC-1 v1.0 boards)
    #define BOARD_LED_IS_RGB    1
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

/* ---------------------------------------------------------------------------- */
#elif defined(BOARD_LILYGO_T_SIM7080G_S3)
    // "Standard" variant WITHOUT the AXP2101 PMU. Pins from LILYGO's
    // "T-SIM7080G-S3 Standard PINOUT" diagram. (LilyGO's GitHub examples are
    // for the PMU variant and use DIFFERENT modem pins - don't copy them.)
    #define BOARD_NAME          "LILYGO T-SIM7080G-S3 Standard (no PMU)"
    #define BOARD_PIN_LED       (-1)    // no user LED
    #define BOARD_LED_IS_RGB    0
    #define BOARD_PIN_BUTTON    0       // BOOT button (active low)

    // SIM7080G modem (UART pins from the ESP32-S3's point of view)
    #define BOARD_MODEM_PWR_PIN 46      // PWRKEY (GPIO46 is a strapping pin)
    #define BOARD_MODEM_DTR_PIN 7
    #define BOARD_MODEM_RI_PIN  6       // RING
    #define BOARD_MODEM_RX_PIN  4       // ESP RX  <- modem TXD
    #define BOARD_MODEM_TX_PIN  5       // ESP TX  -> modem RXD

    // I2C on the 2x STEMMA QT / Qwiic connectors
    #define BOARD_I2C_SDA       3
    #define BOARD_I2C_SCL       2

    // Analog inputs (voltage dividers)
    #define BOARD_BAT_ADC_PIN   8       // 18650 battery voltage
    #define BOARD_SOLAR_ADC_PIN 18      // solar input (5-6 V) voltage

    // TF (microSD) card, SPI mode
    #define BOARD_SD_SCK        12
    #define BOARD_SD_MISO       13
    #define BOARD_SD_MOSI       11
    #define BOARD_SD_CS         10

    // Optional camera (DVP). NOTE: Y2/Y6/PCLK use GPIO35/36/37, which octal
    // PSRAM also needs - the camera and octal PSRAM cannot be used together.
    #define BOARD_CAM_PWDN      (-1)
    #define BOARD_CAM_RESET     (-1)    // NC
    #define BOARD_CAM_XCLK      21
    #define BOARD_CAM_SIOD      1       // SCCB SDA
    #define BOARD_CAM_SIOC      41      // SCCB SCL
    #define BOARD_CAM_VSYNC     9
    #define BOARD_CAM_HREF      14
    #define BOARD_CAM_PCLK      37
    #define BOARD_CAM_Y9        40
    #define BOARD_CAM_Y8        39
    #define BOARD_CAM_Y7        38
    #define BOARD_CAM_Y6        36
    #define BOARD_CAM_Y5        16
    #define BOARD_CAM_Y4        47
    #define BOARD_CAM_Y3        15
    #define BOARD_CAM_Y2        35

/* ---------------------------------------------------------------------------- */
#else
    #error "No board selected: add -DBOARD_xxx to build_flags in platformio.ini"
#endif
