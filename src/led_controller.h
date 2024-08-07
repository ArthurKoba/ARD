#ifndef ARD_LED_CONTROLLER_H
#define ARD_LED_CONTROLLER_H

#include "config.h"
#include <FastLED.h>
#include <Vector.h>


struct Segment {
    uint16_t start;
    uint16_t end;
};


class LedController {
public:
    void init() {
        CFastLED::addLeds<LED_TYPE, STRIP_PIN, LED_COLOR_ORDER>(leds, NUM_LEDS);
        FastLED.setCorrection(TypicalLEDStrip);
        init_segments();
    }

    static void show() {
        FastLED.show();
    }

    const Segment* get_segment(size_t index) {
        if (index >= segments.size()) return nullptr;
        return &segments[index];
    }

    void fill_leds(CRGB color, uint16_t start_led = 0, uint16_t end_led = NUM_LEDS) {
        if (end_led >= NUM_LEDS) end_led = NUM_LEDS - 1;
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

    size_t number_of_leds() {
        return NUM_LEDS;
    }

    size_t number_of_segments() const {
        return segments.size();
    }

    ~LedController() {
        delete segments.data();
    }

private:
    CRGB leds[NUM_LEDS];
    Vector<Segment> segments;

    void init_segments() {
        segments.setStorage(new Segment[8], 8, 0);
        segments.push_back({SECTOR_1_START, SECTOR_1_END});
        segments.push_back({SECTOR_2_START, SECTOR_2_END});
        segments.push_back({SECTOR_3_START, SECTOR_3_END});
        segments.push_back({SECTOR_4_START, SECTOR_4_END});
        segments.push_back({SECTOR_5_START, SECTOR_5_END});
        segments.push_back({SECTOR_6_START, SECTOR_6_END});
        segments.push_back({SECTOR_7_START, SECTOR_7_END});
        segments.push_back({SECTOR_8_START, SECTOR_8_END});
    }
};



#endif //ARD_LED_CONTROLLER_H
