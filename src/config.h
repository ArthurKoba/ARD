#ifndef ARD_CONFIG_H
#define ARD_CONFIG_H

#include <Arduino.h>

#define BUTTONS_DELAY_MS 200 // Задержка между действиями кнопок.

#define WHITE_MODE_DEF_DELAY_MS 100
#define WHITE_MODE_MIN_BRIGHT 10
#define WHITE_MODE_MAX_BRIGHT 255

#define CREATIVE_MODE_DEF_DELAY_MS 30

#define FILL_WHITE_MODE_DEF_DELAY_MS 20
#define FILL_WHITE_MODE_MIN_DELAY_MS 1
#define FILL_WHITE_MODE_MAX_DELAY_MS 50
#define FILL_WHITE_MODE_COUNT_WAIT 100

#define TO_CENTER_MODE_DEF_DELAY_MS 750
#define TO_CENTER_MODE_MIN_DELAY_MS 100
#define TO_CENTER_MODE_MAX_DELAY_MS 10000

#define FADE_MODE_DEF_DELAY_MS 10
#define FADE_MODE_MIN_DELAY_MS 0
#define FADE_MODE_MAX_DELAY_MS 100

#define RAINBOW_MODE_DEF_DELAY_MS 30
#define RAINBOW_MODE_MIN_DELAY_MS 1
#define RAINBOW_MODE_MAX_DELAY_MS 50
#define RAINBOW_MODE_HUE_OFFSET 31

#define RAINBOW2_MODE_DEF_DELAY_MS 30
#define RAINBOW2_MODE_MIN_DELAY_MS 1
#define RAINBOW2_MODE_MAX_DELAY_MS 50
#define RAINBOW2_MODE_HUE_MUL 1

#define RAINBOW3_MODE_DEF_DELAY_MS 5
#define RAINBOW3_MODE_MIN_DELAY_MS 1
#define RAINBOW3_MODE_MAX_DELAY_MS 50
#define RAINBOW3_MODE_HUE_MUL 1
#define RAINBOW3_MODE_SEGMENTS_OFFSET 31

#define FIRE_MODE_DEF_DELAY_MS 30
#define FIRE_MODE_MIN_DELAY_MS 1
#define FIRE_MODE_MAX_DELAY_MS 100
#define FIRE_MODE_HUE_GAP 21      // заброс по оттенку
#define FIRE_MODE_FIRE_STEP 30    // шаг огня
#define FIRE_MODE_HUE_START 0     // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
#define FIRE_MODE_MIN_BRIGHT 70   // минимальная яркость огня
#define FIRE_MODE_MAX_BRIGHT 255  // макс. яркость огня
#define FIRE_MODE_MIN_SAT 245     // максимальная насыщенность
#define FIRE_MODE_MAX_SAT 255     // максимальная насыщенность

#define COLOR_MUSIC_DEF_DELAY_MS 10

#if not defined(KOBA_CONF)
#define SERIAL_SPEED 115200 // Скорость последовательного порта

#define AUDIO_PIN 54 //  Пин аудио-сигнала (микрофона). Для меги: A0 - 54; A1 - 55, ..., A15 - 69. В общем смотрим pinout платы.
#define GAIN_MIC_PIN 55 // A1
#define AR_MIC_PIN 56 // A2
#define SILENCE_AMPLITUDE_LEVEL 50 // Верхний порог амплитуды сигнала во время тишины. Необходимо для калибровки анализатора частот.
#define DEFAULT_SAMPLE_OFFSET 500

#define STRIP_PIN 2 // Пин к которому подключена адресная лента (Вывод Data)
#define CHANGE_MODE_PIN 4 // Пин кнопки смены режима
#define DECREASE_PIN 5 // Пин кнопки -
#define INCREASE_PIN 6 // Пин кнопки +
//#define BUTTONS_REVERSE_LEVELS // Инверсия кнопок. Напряжение на пине 0 - нажата, 5В - отпущена.

#define LED_TYPE WS2813  // Тип адресной ленты
#define LED_COLOR_ORDER GRB // Порядок цветов ленты.

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

#define DEFAULT_SAMPLE_OFFSET 570
#define SILENCE_AMPLITUDE_LEVEL 45
#define AUDIO_PIN 16 // Пин аудио-сигнала (микрофона). Для Nano: A0 - 14; A1 - 15, ..., A7 - 21.

#define GAIN_MIC_PIN 18 // A4
#define AR_MIC_PIN 19 // A5

#define STRIP_PIN 4
#define CHANGE_MODE_PIN 6
#define DECREASE_PIN 8
#define INCREASE_PIN 7
//#define STRIP_PIN 16 // 16-17 left right
//#define CHANGE_MODE_PIN 3
//#define DECREASE_PIN 12
//#define INCREASE_PIN 11

#define BUTTONS_REVERSE_LEVELS

#define LED_TYPE WS2812
#define LED_COLOR_ORDER GRB

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

//#define LENGTH_OF_SMOOTHED_AMPLITUDES 16 // Массив сглаживания амплитуд.

// Параметры ниже данной строки изменять нельзя.
#define NUM_LEDS (SECTOR_8_END + 1)

#if not defined(SERIAL_SPEED)
#define SERIAL_SPEED 115200
#endif

#if not defined(STRIP_PIN)
#error STRIP_PIN not defined!!!
#endif

#if not defined(LED_TYPE)
#error LED_TYPE not defined!!!
#endif

#if not defined(LED_COLOR_ORDER)
#error LED_COLOR_ORDER not defined!!!
#endif

#endif //ARD_CONFIG_H
