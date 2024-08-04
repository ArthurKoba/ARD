#ifndef ARD_UTILS_H
#define ARD_UTILS_H

#include "config.h"
#include <FastLED.h>
#include "stdint.h"
#include "types.h"

void increase_mode(App &ctx) {
    ctx.mode = static_cast<ColorMode>(static_cast<uint8_t>(ctx.mode) + 1);
    if (ctx.mode > RAINBOW_MODE) ctx.mode = WHITE_MODE;
}

void fill_leds(App &ctx, CRGB color, uint16_t start_led = 0, uint16_t end_led = NUM_LEDS) {
    for (uint16_t i = start_led; i < end_led; ++i) {
        ctx.leds[i] = color;
    }
}

void init_segments(App &ctx) {
    ctx.segments[0] = {SECTOR_1_START, SECTOR_1_END};
    ctx.segments[1] = {SECTOR_2_START, SECTOR_2_END};
    ctx.segments[2] = {SECTOR_3_START, SECTOR_3_END};
    ctx.segments[3] = {SECTOR_4_START, SECTOR_4_END};
    ctx.segments[4] = {SECTOR_5_START, SECTOR_5_END};
    ctx.segments[5] = {SECTOR_6_START, SECTOR_6_END};
    ctx.segments[6] = {SECTOR_7_START, SECTOR_7_END};
    ctx.segments[7] = {SECTOR_8_START, SECTOR_8_END};
}

void write_color_to_segment(uint8_t segment, App &ctx, CRGB color) {
    if (segment > 7) return;
    for (uint16_t i = ctx.segments[segment].start; i < ctx.segments[segment].end + 1; ++i) {
        ctx.leds[i] = color;
    }
}

#define exit_timer(delay_ms) \
    static uint32_t last_update = 0; \
    if (millis() - last_update < (delay_ms)) return; \
    last_update = millis();


#endif //ARD_UTILS_H
