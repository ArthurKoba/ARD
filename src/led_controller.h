#ifndef ARD_LED_CONTROLLER_H
#define ARD_LED_CONTROLLER_H

#include "config.h"
#include <FastLED.h>


uint8_t CRT(uint8_t power, float calibrate = 1.0) {
#if not defined(NO_CRT)
    return power > 0 ? (1 + uint16_t((calibrate * power * power + 255))) >> 8 : 0;
#else
    return power;
#endif
}

struct Segment final {
    uint16_t start;
    uint16_t end;
};


class LedController final {
private:
    CRGB leds[NUM_LEDS]{};
    Segment segments[NUMBER_OF_SEGMENTS]{};
public:
    void init() {
        CFastLED::addLeds<LED_TYPE, STRIP_PIN, LED_COLOR_ORDER>(leds, NUM_LEDS);
//        FastLED.setCorrection(TypicalLEDStrip);
        init_segments();
    }

    void show() {
        FastLED.show();
    }

    const Segment* get_segment(size_t index) {
        if (index >= NUMBER_OF_SEGMENTS) return nullptr;
        return &segments[index];
    }

    void fill_leds(CRGB color, size_t start_led = 0, size_t end_led = NUM_LEDS) {
        if (end_led > number_of_leds()) end_led = number_of_leds();
        for (size_t i = start_led; i < end_led; ++i) leds[i] = color;
    }

    void set_color_to_segment(uint8_t segment_id, CRGB color) {
        if (segment_id >= NUMBER_OF_SEGMENTS) return;
        fill_leds(color, segments[segment_id].start, segments[segment_id].end + 1);
    }

    void move_colors(size_t start_led, size_t end_led) {
        for (size_t i = end_led; i > start_led; --i) leds[i] = leds[i - 1];
        for (size_t i = end_led; i < start_led + 1; ++i) leds[i] = leds[i + 1];
    }

    void set_color_to_detail_segment(uint8_t id, CRGB color) {
#if not defined(KOBA_CONF)
        switch (id) {
            case 0: // весь многоугольник
                move_colors(173, 147);
                leds[173] = color;
                move_colors(174, 200);
                leds[174] = color;
                move_colors(65, 39);
                leds[65] = color;
                move_colors(66, 92);
                leds[66] = color;
                move_colors(215, 201);
                leds[215] = leds[0];
                move_colors(11, 0);
                leds[11] = color;
                move_colors(12, 38);
                leds[12] = color;
                move_colors(119, 93);
                leds[119] = color;
                move_colors(120, 146);
                leds[120] = color;
                break;
            case 1: // левый и правый многоугольник
                move_colors(173, 147);
                leds[173] = color;
                move_colors(174, 200);
                leds[174] = color;
                move_colors(65, 39);
                leds[65] = color;
                move_colors(66, 92);
                leds[66] = color;
                break;
            case 2: // нижний многоугольник
                move_colors(215, 201);
                leds[215] = leds[0];
                move_colors(11, 0);
                leds[11] = color;

                move_colors(12, 38);
                leds[12] = color;
                break;
            case 3: // верхний многоугольник
                move_colors(119, 93);
                leds[119] = color;
                move_colors(120, 146);
                leds[120] = color;
                break;
            case 4: // Blue
                fill_leds(color, 216, 222 + 1);
                fill_leds(color, 261, 267 + 1);
                fill_leds(color, 332, 335 + 1);
                fill_leds(color, 336, 338 + 1);
                break;
            case 5: // Lime
                fill_leds(color, 223, 234 + 1);
                fill_leds(color, 340, 342 + 1);
                fill_leds(color, 344, 346 + 1);
                break;
            case 6: // Red
                fill_leds(color, 235, 247 + 1);
                fill_leds(color, 347, 349 + 1);
                fill_leds(color, 351, 353 + 1);
                break;
            case 7: // Yellow
                fill_leds(color, 248, 260 + 1);
                fill_leds(color, 325, 327 + 1);
                fill_leds(color, 329, 331 + 1);
                break;
            case 8: // FireBrick
                fill_leds(color, 268, 269 + 1);
                fill_leds(color, 276, 277 + 1);
                fill_leds(color, 284, 285 + 1);
                fill_leds(color, 292, 293 + 1);
                leds[357] = color;
                leds[359] = color;
                leds[363] = color;
                leds[365] = color;
                break;
            case 9: // OrangeRed
                fill_leds(color, 270, 271 + 1);
                fill_leds(color, 278, 279 + 1);
                fill_leds(color, 286, 287 + 1);
                fill_leds(color, 294, 295 + 1);
                leds[324] = color;
                leds[339] = color;
                break;
            case 10: // HotPink
                fill_leds(color, 272, 273 + 1);
                fill_leds(color, 280, 281 + 1);
                fill_leds(color, 288, 289 + 1);
                fill_leds(color, 296, 297 + 1);
                leds[356] = color;
                leds[358] = color;
                leds[362] = color;
                leds[364] = color;
                break;
            case 11: // DarkGreen
                fill_leds(color, 274, 275 + 1);
                fill_leds(color, 282, 283 + 1);
                fill_leds(color, 290, 291 + 1);
                fill_leds(color, 298, 299 + 1);
                leds[328] = color;
                leds[335] = color;
                leds[343] = color;
                leds[350] = color;
                break;
            case 12: // Purple
                fill_leds(color, 300, 302 + 1);
                fill_leds(color, 312, 314 + 1);
                leds[367] = color;
                leds[376] = color;
                leds[373] = color;
                break;
            case 13: // Brown
                fill_leds(color, 303, 305 + 1);
                fill_leds(color, 315, 317 + 1);
                fill_leds(color, 354, 355 + 1);
                fill_leds(color, 360, 361 + 1);
                fill_leds(color, 368, 369 + 1);
                fill_leds(color, 371, 372 + 1);
                break;
            case 14: // DarkCyan
                fill_leds(color, 306, 308 + 1);
                fill_leds(color, 318, 320 + 1);
                leds[SECTOR_7_START] = color;
                leds[370] = color;
                leds[SECTOR_8_START] = color;
                break;
            case 15: // Grey
                fill_leds(color, 309, 311 + 1);
                fill_leds(color, 321, 323 + 1);
                leds[374] = color;
                leds[375] = color;
                leds[377] = color;
                break;
            default:
                break;
        }
#else
        switch (id) {
            case 0: // левый и правый многоугольник
                move_colors(3, 0);
                leds[3] = color;
            case 1: // левый и правый многоугольник
                move_colors(2, 0);
                leds[2] = color;
                break;
            case 2: // нижний многоугольник
                move_colors(5, 3);
                leds[5] = color;
                break;
            case 3:// верхний многоугольник
                move_colors(8, 6);
                leds[8] = color;
                break;
            case 4:
                fill_leds(color, 9, 10 + 1);
                break;
            case 5:
                fill_leds(color, 11, 12 + 1);
                break;
            case 6: // лепестки - желтый
                fill_leds(color, 13, 14 + 1);
                break;
            case 7: // лепестки - красный
                fill_leds(color, 15, 16 + 1);
                break;
            case 8: // лепестки - зеленый
                fill_leds(color, 17, 18 + 1);
                break;
            case 9: // вложенные лепестки - красный
                fill_leds(color, 19, 20 + 1);
                break;
            case 10: // вложенные лепестки - синий
                fill_leds(color, 21, 22 + 1);
                break;
            case 11: // вложенные лепестки - зеленый
                fill_leds(color, 23, 24 + 1);
                break;
            case 12: // вложенные лепестки - зеленый
                fill_leds(color, 25, 26 + 1);
                break;
            case 13: // вложенные лепестки - зеленый
                fill_leds(color, 27, 28 + 1);
                break;
            case 14: // вложенные лепестки - зеленый
                fill_leds(color, 29, 30 + 1);
                break;
            case 15: // вложенные лепестки - зеленый
                fill_leds(color, 31, 31 + 1);
                break;
            default:
                break;
        }
#endif
    }

    void set_color_to_led(size_t index, CRGB color) {
        if (index >= number_of_leds()) return;
        leds[index] = color;
    }

    const size_t number_of_leds() {
        return NUM_LEDS;
    }

    const size_t number_of_segments() {
        return NUMBER_OF_SEGMENTS;
    }

private:
    void init_segments() {
        segments[0] = {SECTOR_1_START, SECTOR_1_END};
        segments[1] = {SECTOR_2_START, SECTOR_2_END};
        segments[2] = {SECTOR_3_START, SECTOR_3_END};
        segments[3] = {SECTOR_4_START, SECTOR_4_END};
        segments[4] = {SECTOR_5_START, SECTOR_5_END};
        segments[5] = {SECTOR_6_START, SECTOR_6_END};
        segments[6] = {SECTOR_7_START, SECTOR_7_END};
        segments[7] = {SECTOR_8_START, SECTOR_8_END};
    }
};



#endif //ARD_LED_CONTROLLER_H
