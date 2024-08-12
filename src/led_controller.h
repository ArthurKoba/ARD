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

    void fill_leds(CRGB color, uint16_t start_led = 0, uint16_t end_led = NUM_LEDS) {
        if (end_led > number_of_leds()) end_led = number_of_leds();
        for (uint16_t i = start_led; i < end_led; ++i) leds[i] = color;
    }

    void set_color_to_segment(uint8_t segment_id, CRGB color) {
        if (segment_id >= NUMBER_OF_SEGMENTS) return;
        fill_leds(color, segments[segment_id].start, segments[segment_id].end + 1);
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
