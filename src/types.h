#ifndef ARD_TYPES_H
#define ARD_TYPES_H

#include "config.h"
#include "stdint.h"

#define WHITE_MODE_DELTA_MS 10
#define MIN_BRIGHT_WHITE_MODE 10
#define MAX_BRIGHT_WHITE_MODE 255

#define CREATIVE_MODE_DELTA_MS 30

#define FILL_WHITE_MODE_DELAY_MS        20
#define FILL_WHITE_MODE_COUNT_WAIT      100
#define FILL_WHITE_MODE_MIN_DELAY_MS    1
#define FILL_WHITE_MODE_MAX_DELAY_MS    50

#define RAINBOW_MODE_DEFAULT_DELAY_MS 30
#define RAINBOW_MODE_MIN_DELAY_MS 1
#define RAINBOW_MODE_MAX_DELAY_MS 50
#define RAINBOW_MODE_HUE_OFFSET 31

#define MODE_3_DELAY_MS 20

struct WhiteModeCfg {
    uint8_t bright = MAX_BRIGHT_WHITE_MODE;
    uint8_t delay_ms = WHITE_MODE_DELTA_MS;
};

struct CreativeModeCfg {
    uint8_t delay_ms = CREATIVE_MODE_DELTA_MS;
    uint8_t current_segment = 7;
    uint8_t segment_hues[8];
};

struct RainbowModeCfg {
    uint8_t delay_ms = RAINBOW_MODE_DEFAULT_DELAY_MS;
    uint8_t hue_offset = RAINBOW_MODE_HUE_OFFSET;
};

struct FillWhiteModeCfg {
    uint8_t delay_ms = FILL_WHITE_MODE_DELAY_MS;
    uint8_t count_wait = FILL_WHITE_MODE_COUNT_WAIT;
};

struct ColorModesConfigs {
    WhiteModeCfg white;
    CreativeModeCfg creative;
    FillWhiteModeCfg fill_white;
    RainbowModeCfg rainbow;



    uint8_t mode_3_delay_ms = MODE_3_DELAY_MS;

};

enum ColorMode {
    WHITE_MODE = 0, CREATIVE_MODE, FILL_WHITE_MODE,
//    MODE_4, MODE_5, MODE_6
    MODE_3, RAINBOW_MODE
};

struct Segment {
    uint16_t start;
    uint16_t end;
};

struct App {
    ColorMode mode = WHITE_MODE;
    ColorModesConfigs cfg_modes;
    CRGB leds[NUM_LEDS]; // Define the array of leds
    Segment segments[8];
};

#endif //ARD_TYPES_H
