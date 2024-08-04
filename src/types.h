#ifndef ARD_TYPES_H
#define ARD_TYPES_H

#include "config.h"
#include "stdint.h"

#define WHITE_MODE_DEF_DELAY_MS         100
#define WHITE_MODE_MIN_BRIGHT           10
#define WHITE_MODE_MAX_BRIGHT           255

#define CREATIVE_MODE_DEF_DELAY_MS      30

#define FILL_WHITE_MODE_DEF_DELAY_MS    20
#define FILL_WHITE_MODE_MIN_DELAY_MS    1
#define FILL_WHITE_MODE_MAX_DELAY_MS    50
#define FILL_WHITE_MODE_COUNT_WAIT      100

#define TO_CENTER_MODE_DEF_DELAY_MS     750
#define TO_CENTER_MODE_MAX_DELAY_MS     10000
#define TO_CENTER_MODE_MIN_DELAY_MS     100

#define RAINBOW_MODE_DEF_DELAY_MS       30
#define RAINBOW_MODE_MIN_DELAY_MS       1
#define RAINBOW_MODE_MAX_DELAY_MS       50
#define RAINBOW_MODE_HUE_OFFSET         31

#define MODE_3_DELAY_MS 20

#ifndef LENGTH_OF_SMOOTHED_AMPLITUDES
#define LENGTH_OF_SMOOTHED_AMPLITUDES 16
#endif

#ifndef LENGTH_AMPLITUDES_HISTORY
#define LENGTH_COLOR_AMPLITUDES 16
#endif

struct CreativeModeCfg {
    uint8_t current_segment = 0;
    uint8_t segment_hues[8];
};

struct RainbowModeCfg {
    uint8_t delay_ms = RAINBOW_MODE_DEF_DELAY_MS;
    uint8_t hue_offset = RAINBOW_MODE_HUE_OFFSET;
};

struct FillWhiteModeCfg {
    uint8_t delay_ms = FILL_WHITE_MODE_DEF_DELAY_MS;
    uint8_t count_wait = FILL_WHITE_MODE_COUNT_WAIT;
};

struct ColorModesConfigs {
    uint8_t white_mode_bright = WHITE_MODE_MAX_BRIGHT;
    CreativeModeCfg creative;
    FillWhiteModeCfg fill_white;
    uint16_t move_to_center_delay_ms = TO_CENTER_MODE_DEF_DELAY_MS;
    RainbowModeCfg rainbow;
    uint16_t mode_3_delay_ms = TO_CENTER_MODE_DEF_DELAY_MS;
    uint8_t color_music_delay_ms = 10;
};

enum ColorMode {
    WHITE_MODE = 0, CREATIVE_MODE, FILL_WHITE_MODE,
    MOVE_TO_CENTER_MODE,
//    MODE_4, MODE_5, MODE_6
    MODE_3, COLOR_MUSIC, RAINBOW_MODE
};

struct Segment {
    uint16_t start;
    uint16_t end;
};

struct AmplitudeHistory {
    uint8_t minimum = 0;
    uint8_t maximum = 0;
    uint8_t delta = 0;
    uint8_t average = 0;
    uint8_t data[LENGTH_COLOR_AMPLITUDES] = {0};
};

struct AnalyzerConfigs {
    AmplitudeHistory history[8];
    uint8_t *amplitudes = nullptr;
    uint8_t smoothing_amplitudes[LENGTH_OF_SMOOTHED_AMPLITUDES];
    int16_t *samples = nullptr;

    int16_t delay_reading_samples_us = 30;
    int16_t sample_offset = 511;

    uint16_t sampling_frequency = 0;
    int16_t signal_median_amplitude = 0;
    int16_t signal_amplitude = 0;
    uint32_t time_of_last_signal = 0;
    bool need_calibration = true;
};

struct App {
    ColorMode mode = WHITE_MODE;
    ColorModesConfigs cfg_modes;
    CRGB leds[NUM_LEDS]; // Define the array of leds
    Segment segments[8];
    AnalyzerConfigs analyzer;
};

#endif //ARD_TYPES_H
