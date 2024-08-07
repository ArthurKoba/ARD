#ifndef ARD_DEFAULT_DEFS_H
#define ARD_DEFAULT_DEFS_H

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

#define FFT_MODE 1

#if (FFT_MODE == 0)
#define LOG_OUT 1 // Тип амплитуд преобразования Хартли.
#elif (FFT_MODE == 1)
#define LIN_OUT 1
#endif

#define FHT_N 256 // Количество измерений сигнала для преобразования Хартли.

#if not defined(SILENCE_AMPLITUDE_LEVEL)
#error SILENCE_AMPLITUDE_LEVEL not defined!
#endif

#if not defined(AUDIO_PIN)
#error AUDIO_PIN not defined!!!
#endif

#if not defined(INCREASE_PIN) or not defined(DECREASE_PIN) or not defined(CHANGE_MODE_PIN)
#error INCREASE_PIN or DECREASE_PIN or CHANGE_MODE_PIN not defined!!!
#endif

#if not defined(BUTTONS_DELAY_MS)
#define BUTTONS_DELAY_MS 100
#endif

#if not defined(SECTOR_1_START) or not defined(SECTOR_1_END) or not defined(SECTOR_2_START) or not defined(SECTOR_2_END) or \
    not defined(SECTOR_3_START) or not defined(SECTOR_3_END) or not defined(SECTOR_4_START) or not defined(SECTOR_4_END) or \
    not defined(SECTOR_5_START) or not defined(SECTOR_5_END) or not defined(SECTOR_6_START) or not defined(SECTOR_6_END) or \
    not defined(SECTOR_7_START) or not defined(SECTOR_7_END) or not defined(SECTOR_8_START) or not defined(SECTOR_8_END)
#error There is an error when declaring sectors

#endif

#endif //ARD_DEFAULT_DEFS_H
