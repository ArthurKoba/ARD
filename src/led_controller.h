#ifndef ARD_LED_CONTROLLER_H
#define ARD_LED_CONTROLLER_H

#include "config.h"
#include <FastLED.h>
#include <Vector.h>

uint8_t CRT(uint8_t power, float calibrate = 1.0) {
    return power > 0 ? (1 + uint16_t((calibrate * power * power + 255))) >> 8 : 0;
}

CRGB CRGB_f(uint8_t r, uint8_t g, uint8_t b) {
    return {CRT(r), CRT(g), CRT(b)};
}

CRGB CRGB_f(CRGB color) {
    return {CRT(color.r), CRT(color.g), CRT(color.b)};
}

struct Segment {
    uint16_t start;
    uint16_t end;
};


class LedController {
private:
    CRGB leds[NUM_LEDS]{};
    Vector<Segment> segments;
    Segment _segment_memory[NUMBER_OF_SEGMENTS]{};
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
        if (index >= segments.size()) return nullptr;
        return &segments[index];
    }

    void fill_leds(CRGB color, uint16_t start_led = 0, uint16_t end_led = NUM_LEDS) {
        if (end_led > number_of_leds()) end_led = number_of_leds();
        for (uint16_t i = start_led; i < end_led; ++i) leds[i] = color;
    }

    void set_color_to_segment(uint8_t segment_id, CRGB color) {
        if (segment_id >= segments.size()) return;
        fill_leds(color, segments[segment_id].start, segments[segment_id].end + 1);
    }

    void set_color_to_led(size_t index, CRGB color) {
        if (index >= number_of_leds()) return;
        leds[index] = color;
    }

    const size_t number_of_leds() {
        return NUM_LEDS;
    }

    size_t number_of_segments() const {
        return segments.size();
    }

private:
    void init_segments() {
        segments.setStorage(_segment_memory, NUMBER_OF_SEGMENTS, NUMBER_OF_SEGMENTS);
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
