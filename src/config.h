#ifndef ARD_CONFIG_H
#define ARD_CONFIG_H

#include <Arduino.h>

#define IR_PIN 7


#if not defined(KOBA_CONF)

#define SILENCE_AMPLITUDE_LEVEL 50

#define AUDIO_PIN A0
#define STRIP_PIN 2 // For led chips like Neopixels, which have a data line, ground, and power, you just need to define DATA_PIN
#define CHANGE_MODE_PIN 4
#define DECREASE_PIN 5
#define INCREASE_PIN 6

#define LED_TYPE WS2813
#define COLOR_ORDER RGB

#define SECTOR_1_START 0
#define SECTOR_1_END 215

#define SECTOR_2_START 216
#define SECTOR_2_END 267

#define SECTOR_3_START 268
#define SECTOR_3_END 299

#define SECTOR_4_START 300
#define SECTOR_4_END 323

#define SECTOR_5_START 324
#define SECTOR_5_END 350

#define SECTOR_6_START 351
#define SECTOR_6_END 362

#define SECTOR_7_START 363
#define SECTOR_7_END 374

#define SECTOR_8_START 375
#define SECTOR_8_END 376

#else

#define SILENCE_AMPLITUDE_LEVEL 30
#define AUDIO_PIN A2
#define STRIP_PIN 4 // 16-17 left right

#define CHANGE_MODE_PIN 9
#define DECREASE_PIN 8
#define INCREASE_PIN 7

//#define STRIP_PIN 16 // 16-17 left right
//#define CHANGE_MODE_PIN 3
//#define DECREASE_PIN 12
//#define INCREASE_PIN 11

#define BUTTONS_REVERSE_LEVELS

#define LED_TYPE WS2812 // указываем тип ленты
#define COLOR_ORDER GRB // указываем палитру

#define SECTOR_1_START 0
#define SECTOR_1_END 3

#define SECTOR_2_START 4
#define SECTOR_2_END 7

#define SECTOR_3_START 8
#define SECTOR_3_END 11

#define SECTOR_4_START 12
#define SECTOR_4_END 15

#define SECTOR_5_START 16
#define SECTOR_5_END 19

#define SECTOR_6_START 20
#define SECTOR_6_END 23

#define SECTOR_7_START 24
#define SECTOR_7_END 27

#define SECTOR_8_START 28
#define SECTOR_8_END 31

#endif

#define NUM_LEDS (SECTOR_8_END + 1)
#define BUTTONS_DELAY_MS 200

#define LOG_OUT 1
#define FHT_N 256
#define AMPLITUDES_N (FHT_N / 2)
#define LENGTH_OF_SMOOTHED_AMPLITUDES 16

#endif //ARD_CONFIG_H
