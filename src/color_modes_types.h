//
// Created by Koba on 10.08.2024.
//

#ifndef ARD_COLOR_MODES_TYPES_H
#define ARD_COLOR_MODES_TYPES_H

#include "config.h"
#include "stdint.h"

enum ColorMode : uint8_t {
    WHITE_MODE = 0, CREATIVE_MODE, FILL_WHITE_MODE, MOVE_TO_CENTER_MODE, FADE_MODE,
    RAINBOW_MODE, RAINBOW2_MODE, RAINBOW3_MODE, FIRE_MODE,
    COLOR_MUSIC
};

struct ColorModesMemory final {
    uint8_t white_mode_bright = WHITE_MODE_MAX_BRIGHT;
    uint8_t creative_mode_segments_hues[NUMBER_OF_SEGMENTS]{};
    uint8_t fill_white_mode_show_delay_ms = FILL_WHITE_MODE_DEF_DELAY_MS;
    uint16_t to_center_mode_show_delay_ms = TO_CENTER_MODE_DEF_DELAY_MS;
    uint8_t fade_mode_show_delay_ms = FADE_MODE_DEF_DELAY_MS;
    uint8_t rainbow1_mode_show_delay_ms = RAINBOW1_MODE_DEF_DELAY_MS;
    uint8_t rainbow2_mode_show_delay_ms = RAINBOW2_MODE_DEF_DELAY_MS;
    uint8_t rainbow3_mode_show_delay_ms = RAINBOW3_MODE_DEF_DELAY_MS;
    uint8_t fire_mode_hue_start = FIRE_MODE_HUE_START;
    uint8_t fire_mode_hue_gap = FIRE_MODE_HUE_GAP;
    uint8_t fire_mode_step = FIRE_MODE_FIRE_STEP;
    uint8_t blink_mode_show_delay_ms = BLINK_MODE_DEF_DELAY_MS;
    ColorMode current_mode = WHITE_MODE;
};



#endif //ARD_COLOR_MODES_TYPES_H
