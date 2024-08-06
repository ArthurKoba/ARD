#ifndef ARD_UTILS_H
#define ARD_UTILS_H

#include "config.h"
#include <FastLED.h>
#include "stdint.h"
#include "types.h"

#if not defined(BUTTONS_DELAY_MS)
#define BUTTONS_DELAY_MS 100
#endif

#if not defined(SECTOR_1_START) or not defined(SECTOR_1_END) or not defined(SECTOR_2_START) or not defined(SECTOR_2_END) or \
    not defined(SECTOR_3_START) or not defined(SECTOR_3_END) or not defined(SECTOR_4_START) or not defined(SECTOR_4_END) or \
    not defined(SECTOR_5_START) or not defined(SECTOR_5_END) or not defined(SECTOR_6_START) or not defined(SECTOR_6_END) or \
    not defined(SECTOR_7_START) or not defined(SECTOR_7_END) or not defined(SECTOR_8_START) or not defined(SECTOR_8_END)
#error There is an error when declaring sectors
#endif                                                                                                                             \

void increase_mode(App &ctx) {
    ctx.cfg_modes.current_mode = static_cast<ColorMode>(static_cast<uint8_t>(ctx.cfg_modes.current_mode) + 1);
    if (ctx.cfg_modes.current_mode > COLOR_MUSIC) ctx.cfg_modes.current_mode = WHITE_MODE;
    ctx.need_update_saves = true;
}

void fill_leds(App &ctx, CRGB color, uint16_t start_led = 0, uint16_t end_led = NUM_LEDS) {
    for (uint16_t i = start_led; i < end_led; ++i) {
        ctx.leds[i] = color;
    }
}

void init_default_colors_for_creative_mode(App &ctx) {
    ctx.cfg_modes.creative.segment_hues[0] = 42 * 0;
    ctx.cfg_modes.creative.segment_hues[1] = 42 * 2;
    ctx.cfg_modes.creative.segment_hues[2] = 42 * 4;
    ctx.cfg_modes.creative.segment_hues[3] = 42 * 6;
    ctx.cfg_modes.creative.segment_hues[4] = 42 * 1;
    ctx.cfg_modes.creative.segment_hues[5] = 42 * 3;
    ctx.cfg_modes.creative.segment_hues[6] = 42 * 5;
    ctx.cfg_modes.creative.segment_hues[7] = 255;
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
    last_update = millis()


#endif //ARD_UTILS_H
