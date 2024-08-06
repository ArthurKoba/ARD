#ifndef ARD_TYPES_H
#define ARD_TYPES_H

#include "config.h"
#include "stdint.h"

#if not defined(WHITE_MODE_DEF_DELAY_MS)
#define WHITE_MODE_DEF_DELAY_MS 100
#endif
#if not defined(WHITE_MODE_MIN_BRIGHT)
#define WHITE_MODE_MIN_BRIGHT 10
#endif
#if not defined(WHITE_MODE_MAX_BRIGHT)
#define WHITE_MODE_MAX_BRIGHT 255
#endif

#if not defined(CREATIVE_MODE_DEF_DELAY_MS)
#define CREATIVE_MODE_DEF_DELAY_MS 30
#endif

#if not defined(FILL_WHITE_MODE_DEF_DELAY_MS)
#define FILL_WHITE_MODE_DEF_DELAY_MS 20
#endif
#if not defined(FILL_WHITE_MODE_MIN_DELAY_MS)
#define FILL_WHITE_MODE_MIN_DELAY_MS 1
#endif
#if not defined(FILL_WHITE_MODE_MAX_DELAY_MS)
#define FILL_WHITE_MODE_MAX_DELAY_MS 50
#endif
#if not defined(FILL_WHITE_MODE_COUNT_WAIT)
#define FILL_WHITE_MODE_COUNT_WAIT 100
#endif

#if not defined(TO_CENTER_MODE_DEF_DELAY_MS)
#define TO_CENTER_MODE_DEF_DELAY_MS 750
#endif
#if not defined(TO_CENTER_MODE_MIN_DELAY_MS)
#define TO_CENTER_MODE_MIN_DELAY_MS 100
#endif
#if not defined(TO_CENTER_MODE_MAX_DELAY_MS)
#define TO_CENTER_MODE_MAX_DELAY_MS 10000
#endif

#if not defined(FADE_MODE_DEF_DELAY_MS)
#define FADE_MODE_DEF_DELAY_MS 10
#endif
#if not defined(FADE_MODE_MIN_DELAY_MS)
#define FADE_MODE_MIN_DELAY_MS 0
#endif
#if not defined(FADE_MODE_MAX_DELAY_MS)
#define FADE_MODE_MAX_DELAY_MS 100
#endif

#if not defined(RAINBOW_MODE_DEF_DELAY_MS)
#define RAINBOW_MODE_DEF_DELAY_MS 30
#endif
#if not defined(RAINBOW_MODE_MIN_DELAY_MS)
#define RAINBOW_MODE_MIN_DELAY_MS 1
#endif
#if not defined(RAINBOW_MODE_MAX_DELAY_MS)
#define RAINBOW_MODE_MAX_DELAY_MS 50
#endif
#if not defined(RAINBOW_MODE_HUE_OFFSET)
#define RAINBOW_MODE_HUE_OFFSET 31
#endif

#if not defined(RAINBOW2_MODE_DEF_DELAY_MS)
#define RAINBOW2_MODE_DEF_DELAY_MS 30
#endif
#if not defined(RAINBOW2_MODE_MIN_DELAY_MS)
#define RAINBOW2_MODE_MIN_DELAY_MS 1
#endif
#if not defined(RAINBOW2_MODE_MAX_DELAY_MS)
#define RAINBOW2_MODE_MAX_DELAY_MS 50
#endif
#if not defined(RAINBOW2_MODE_HUE_MUL)
#define RAINBOW2_MODE_HUE_MUL 1
#endif // MUL

#if not defined(RAINBOW3_MODE_DEF_DELAY_MS)
#define RAINBOW3_MODE_DEF_DELAY_MS 5
#endif
#if not defined(RAINBOW3_MODE_MIN_DELAY_MS)
#define RAINBOW3_MODE_MIN_DELAY_MS 1
#endif
#if not defined(RAINBOW3_MODE_MAX_DELAY_MS)
#define RAINBOW3_MODE_MAX_DELAY_MS 50
#endif
#if not defined(RAINBOW3_MODE_HUE_MUL)
#define RAINBOW3_MODE_HUE_MUL 1
#endif
#if not defined(RAINBOW3_MODE_SEGMENTS_OFFSET)
#define RAINBOW3_MODE_SEGMENTS_OFFSET 31
#endif

#if not defined(FIRE_MODE_DEF_DELAY_MS)
#define FIRE_MODE_DEF_DELAY_MS 30
#endif
#if not defined(FIRE_MODE_MIN_DELAY_MS)
#define FIRE_MODE_MIN_DELAY_MS 1
#endif
#if not defined(FIRE_MODE_MAX_DELAY_MS)
#define FIRE_MODE_MAX_DELAY_MS 100
#endif
#if not defined(FIRE_MODE_HUE_GAP)
#define FIRE_MODE_HUE_GAP 21
#endif
#if not defined(FIRE_MODE_FIRE_STEP)
#define FIRE_MODE_FIRE_STEP 30
#endif
#if not defined(FIRE_MODE_HUE_START)
#define FIRE_MODE_HUE_START 0
#endif
#if not defined(FIRE_MODE_MIN_BRIGHT)
#define FIRE_MODE_MIN_BRIGHT 70
#endif
#if not defined(FIRE_MODE_MAX_BRIGHT)
#define FIRE_MODE_MAX_BRIGHT 255
#endif
#if not defined(FIRE_MODE_MIN_SAT)
#define FIRE_MODE_MIN_SAT 245
#endif
#if not defined(FIRE_MODE_MAX_SAT)
#define FIRE_MODE_MAX_SAT 255
#endif

#if not defined(COLOR_MUSIC_DEF_DELAY_MS)
#define COLOR_MUSIC_DEF_DELAY_MS 10
#endif

#if not defined(DELAY_BETWEEN_READ_SAMPLES_US)
#define DELAY_BETWEEN_READ_SAMPLES_US 30
#endif

#if not defined(LENGTH_OF_SMOOTHED_AMPLITUDES)
#define LENGTH_OF_SMOOTHED_AMPLITUDES 16
#endif

#if not defined(AMPLITUDES_HISTORY_LENGTH)
#define AMPLITUDES_HISTORY_LENGTH 16
#endif

#if not defined(DEFAULT_SAMPLE_OFFSET)
#define DEFAULT_SAMPLE_OFFSET 511
#endif

#if not defined(NUM_LEDS)
#error NUM_LEDS not defined!
#endif

#define LOG_OUT 1 // Тип амплитуд преобразования Хартли.
#define FHT_N 256 // Количество измерений сигнала для преобразования Хартли.

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

struct App {
    ColorModesConfigs cfg_modes;
    CRGB leds[NUM_LEDS];
    Segment segments[8];
    AnalyzerConfigs analyzer;
    int firmware_hash_address = 0;
    bool need_update_saves = false;
};

#endif //ARD_TYPES_H
