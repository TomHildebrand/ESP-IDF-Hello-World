#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "app_config.h"     // includes board.h
#include "board_led.h"

void app_main(void)
{
    printf("Hello world from Module %d (%s)!\n", BOARD_MODULE_ID, BOARD_NAME);

    esp_chip_info_t chip_info;
    uint32_t flash_size = 0;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), silicon revision %d\n",
           CONFIG_IDF_TARGET, chip_info.cores, chip_info.revision);

    if (esp_flash_get_size(NULL, &flash_size) == ESP_OK) {
        printf("%" PRIu32 " MB %s flash\n", flash_size / (1024 * 1024),
               (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    }

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    board_led_init();
#if BOARD_PIN_LED < 0
    printf("LED test: this board has no user LED\n");
#elif BOARD_LED_IS_RGB
    printf("LED test: cycling colours on WS2812 RGB LED at GPIO%d\n", BOARD_PIN_LED);
#else
    printf("LED test: blinking LED on GPIO%d\n", BOARD_PIN_LED);
#endif

    // Blink (plain LED) or colour cycle (RGB LED), 0.5 s per step, at low brightness.
    static const struct { uint8_t r, g, b; const char *name; } steps[] = {
        { 32,  0,  0, "red"   }, {  0,  0,  0, "off" },
        {  0, 32,  0, "green" }, {  0,  0,  0, "off" },
        {  0,  0, 32, "blue"  }, {  0,  0,  0, "off" },
        { 24, 24, 24, "white" }, {  0,  0,  0, "off" },
    };
    const int n_steps = sizeof(steps) / sizeof(steps[0]);

    int counter = 0;
    for (int i = 0; ; i = (i + 1) % n_steps) {
        board_led_set_rgb(steps[i].r, steps[i].g, steps[i].b);
#if BOARD_PIN_LED < 0
        if (i % 2 == 0) {   // keep the original 1 line per second without an LED
            printf("Hello from ESP-IDF! (%d)\n", counter++);
        }
#elif BOARD_LED_IS_RGB
        printf("Hello from ESP-IDF! (%d) LED %s\n", counter++, steps[i].name);
#else
        printf("Hello from ESP-IDF! (%d) LED %s\n", counter++, (i % 2) ? "off" : "on");
#endif
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
