#pragma once
/*
 * On-board LED abstraction, driven by BOARD_PIN_LED / BOARD_LED_IS_RGB from board.h.
 *   - plain LED:   board_led_set_rgb() turns it on if any colour component is non-zero
 *   - WS2812 RGB:  full colour via the RMT peripheral
 *   - no LED (BOARD_PIN_LED < 0): all calls are no-ops
 */
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

esp_err_t board_led_init(void);
void      board_led_set_rgb(uint8_t r, uint8_t g, uint8_t b);
void      board_led_off(void);
