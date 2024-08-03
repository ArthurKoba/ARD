#ifndef ARD_CONFIG_H
#define ARD_CONFIG_H

#define IR_PIN 7

#if not defined(KOBA_CONF)
#define STRIP_PIN 2 // For led chips like Neopixels, which have a data line, ground, and power, you just need to define DATA_PIN
#define CHANGE_MODE_PIN 4
#define DECREASE_PIN 5
#define INCREASE_PIN 6

#define LED_TYPE WS2813
#define COLOR_ORDER RGB

#define SECTOR_1_START 0  //232
#define SECTOR_1_END 231

#define SECTOR_2_START 232 //69
#define SECTOR_2_END 300

#define SECTOR_3_START 301  //32
#define SECTOR_3_END 332

#define SECTOR_4_START 333  //24
#define SECTOR_4_END 356

#define SECTOR_5_START 357  //22
#define SECTOR_5_END 378

#define SECTOR_6_START 379 //12
#define SECTOR_6_END 390

#define SECTOR_7_START 391 //7
#define SECTOR_7_END 397

#define SECTOR_8_START 398 //1
#define SECTOR_8_END 399

#else
#define STRIP_PIN 16 // 16-17 left right

#define CHANGE_MODE_PIN 3
#define DECREASE_PIN 12
#define INCREASE_PIN 11

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

#endif //ARD_CONFIG_H
