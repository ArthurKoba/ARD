#ifndef ARD_TYPES_H
#define ARD_TYPES_H

#include "config.h"
#include "default_defs.h"
#include "stdint.h"
#include "BDSP.h"
#include "FastLED.h"

enum ColorMode {
    WHITE_MODE = 0, CREATIVE_MODE, FILL_WHITE_MODE, MOVE_TO_CENTER_MODE, FADE_MODE,
    RAINBOW_MODE, RAINBOW2_MODE, RAINBOW3_MODE, FIRE_MODE,
    COLOR_MUSIC
};

struct CreativeModeCfg {
    uint8_t current_segment = 0;
    uint8_t segment_hues[8];
};

struct FillWhiteModeCfg {
    uint8_t delay_ms = FILL_WHITE_MODE_DEF_DELAY_MS;
    uint8_t count_wait = FILL_WHITE_MODE_COUNT_WAIT;
};

struct ColorModesConfigs {
    ColorMode current_mode = WHITE_MODE;
    uint8_t white_mode_bright = WHITE_MODE_MAX_BRIGHT;
    CreativeModeCfg creative;
    FillWhiteModeCfg fill_white;
    uint16_t move_to_center_delay_ms = TO_CENTER_MODE_DEF_DELAY_MS;
    uint8_t fade_mode_delay_ms = FADE_MODE_DEF_DELAY_MS;
    uint8_t rainbow_mode_delay_ms = RAINBOW_MODE_DEF_DELAY_MS;
    uint8_t rainbow2_mode_delay_ms = RAINBOW2_MODE_DEF_DELAY_MS;
    uint8_t rainbow3_mode_delay_ms = RAINBOW3_MODE_DEF_DELAY_MS;
    uint8_t fire_mode_delay_ms = FIRE_MODE_DEF_DELAY_MS;
    uint8_t color_music_delay_ms = COLOR_MUSIC_DEF_DELAY_MS;
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
    uint8_t data[AMPLITUDES_HISTORY_LENGTH] = {0};
};

struct AnalyzerConfigs {
    AmplitudeHistory history[8];
    uint8_t *amplitudes = nullptr;
    uint8_t smoothing_amplitudes[LENGTH_OF_SMOOTHED_AMPLITUDES];
    int16_t *samples = nullptr;

    int16_t delay_reading_samples_us = DELAY_BETWEEN_READ_SAMPLES_US;
    int16_t sample_offset = DEFAULT_SAMPLE_OFFSET;

    uint16_t sampling_frequency = 0;
    int16_t signal_median_amplitude = 0;
    int16_t signal_amplitude = 0;
    uint32_t time_of_last_signal = 0;
    bool need_calibration = true;
};

struct Context {
    ColorModesConfigs cfg_modes;
    CRGB leds[NUM_LEDS];
    Segment segments[8];
    AnalyzerConfigs analyzer;
    int firmware_hash_address = 0;
    bool need_update_saves = false;
    BDSPTransmitter transmitter;
};

#endif //ARD_TYPES_H
