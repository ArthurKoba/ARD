#ifndef ARD_COLOR_MODES_H
#define ARD_COLOR_MODES_H

#include "led_controller.h"
#include "input_controller.h"



enum ColorMode : uint8_t {
    WHITE_MODE = 0, CREATIVE_MODE, FILL_WHITE_MODE, MOVE_TO_CENTER_MODE, FADE_MODE,
    RAINBOW_MODE, RAINBOW2_MODE, RAINBOW3_MODE, FIRE_MODE,
    COLOR_MUSIC, NUMBER_OF_MODES
};


class AbstractColorMode {
private:
    uint32_t last_update = 0;
public:
    uint16_t show_delay_ms = 0;

    bool calculate(LedController &controller) {
        if (millis() - last_update < show_delay_ms) return false;
        last_update = millis();
        _calculate(controller);
        return true;
    }

    virtual bool handle_input_event(input_event_t input) = 0;
    virtual ~AbstractColorMode() = default;
protected:
    virtual void _calculate(LedController &controller) = 0;
};



class WhiteMode : public AbstractColorMode {
public:
    uint8_t white_mode_bright = WHITE_MODE_MAX_BRIGHT;

    WhiteMode() {
        show_delay_ms = WHITE_MODE_DEF_DELAY_MS;
    };

    bool handle_input_event(input_event_t input) override {
        uint16_t new_white_mode_bright = constrain(
            int(white_mode_bright) + input * 10,
            WHITE_MODE_MIN_BRIGHT,
            WHITE_MODE_MAX_BRIGHT
        );
        if (new_white_mode_bright == white_mode_bright) return false;
        white_mode_bright = new_white_mode_bright;
        return true;
    }

private:
    void _calculate(LedController &controller) override {
        controller.fill_leds(CRGB_f(white_mode_bright,white_mode_bright, white_mode_bright));
    }
};




class CreativeMode : public AbstractColorMode {
    // 7 сегментов меняют цвет, 255 оттенков, кроме последнего сегмента 8-го, состоящего из центральной точки.
    // Она всегда горит белым цветом, у неё просто меняется яркость если пытаешься менять цвет в этом режиме.
public:
    uint8_t _memory[NUMBER_OF_SEGMENTS]{};
    Vector<uint8_t> segment_hues;
    size_t current_hue = 0;

    CreativeMode() {
        show_delay_ms = CREATIVE_MODE_DEF_DELAY_MS;
        segment_hues.setStorage(_memory, NUMBER_OF_SEGMENTS, NUMBER_OF_SEGMENTS);
        for (size_t i = 0; i < segment_hues.size() - 1; ++i) {
            segment_hues[i] = 42 * i;
        }
        segment_hues[segment_hues.size()] = 255;
    };

    ~CreativeMode() override {
        delete [] segment_hues.data();
    }

    bool handle_input_event(input_event_t input) override {
        if (input == input_event_t::INCREASE_BUTTON) {
            current_hue = current_hue == segment_hues.max_size() ? 0 : current_hue + 1;
        } else if (input == input_event_t::DECREASE_BUTTON) {
            segment_hues[current_hue] += 10;
        }
        return input not_eq input_event_t::CHANGE_BUTTON;
    }

private:
    void _calculate(LedController &controller) override {

        for (size_t i = 0; i < controller.number_of_segments() - 1; ++i) {
            controller.set_color_to_segment(i, CHSV(segment_hues[i],255, 255));
        }

        size_t last = controller.number_of_segments() - 1;
        CRGB last_color = CHSV(segment_hues[last], segment_hues[last], segment_hues[last]);
        controller.set_color_to_segment(last, last_color);
    }
};

class FillWhiteMode : public AbstractColorMode {
    //  Поочередно зажигаются диоды начиная от внешнего периметра к центральной точке.
    //  Потом немого поработали включенными все диоды, затухают и цикл начинается заново.
    //  Кнопками меняется скорость работы режима + -.
public:
    uint8_t count_wait = FILL_WHITE_MODE_COUNT_WAIT;

    FillWhiteMode() {
        show_delay_ms = FILL_WHITE_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(show_delay_ms) + input,
            FILL_WHITE_MODE_MIN_DELAY_MS,
            FILL_WHITE_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return false;
        show_delay_ms = new_show_delay_ms;
        return true;
    }

private:
    void _calculate(LedController &controller) override {
        static uint16_t fullness = 0;
        static bool is_fill = true;
        static uint16_t count_waits_ = 0;

        if (is_fill and fullness not_eq controller.number_of_leds()) {
            fullness++;
            controller.fill_leds(CRGB::White, 0, fullness);
            controller.fill_leds(CRGB::Black, fullness + 1);
        } else if (is_fill and fullness == controller.number_of_leds() and count_waits_ < count_wait) {
            controller.fill_leds(CRGB::White);
            count_waits_++;
        } else if (is_fill and fullness == controller.number_of_leds() and count_waits_ >= count_wait) {
            count_waits_ = 0;
            is_fill = false;
        } else if (fullness) {
            fullness--;
            controller.fill_leds(CRGB(fullness, fullness, fullness));
        } else if (count_waits_ < count_wait){
            controller.fill_leds(CRGB::Black);
            count_waits_++;
        } else {
            count_waits_ = 0;
            fullness = 0;
            is_fill = true;
        }
    }
};

class ToCenterMode : public AbstractColorMode {
public:
    ToCenterMode() {
        show_delay_ms = TO_CENTER_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(show_delay_ms) + input * 100,
            TO_CENTER_MODE_MIN_DELAY_MS,
            TO_CENTER_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return false;
        show_delay_ms = new_show_delay_ms;
        return true;
    }

private:
    void _calculate(LedController &controller) override {
        static uint8_t fullness = 0;
        static bool is_fill = true;

        fullness++;
        for (uint8_t i = 0; i < fullness; ++i) {
            controller.set_color_to_segment(i, is_fill ? CRGB::White : CRGB::Black);
        }

        for (size_t i = fullness; i < controller.number_of_segments(); ++i) {
            controller.set_color_to_segment(i, is_fill ? CRGB::Black : CRGB::White);
        }

        if (fullness < controller.number_of_segments()) return;
        fullness = 0;
        is_fill = not is_fill;
    }
};

class FadeMode : public AbstractColorMode {
public:
    FadeMode() {
        show_delay_ms = FADE_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(show_delay_ms) + input,
            FADE_MODE_MIN_DELAY_MS,
            FADE_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return false;
        show_delay_ms = new_show_delay_ms;
        return true;
    }


private:
    void _calculate(LedController &controller) override {

        static uint8_t bright = 254;
        static bool is_fade = false;

        bright += is_fade ? - 1 : 1;
        if (bright < 1) is_fade = false;
        if (bright > 254) is_fade = true;

        controller.fill_leds(CRGB_f(bright, bright, bright));
    }
};


class RainbowMode : public AbstractColorMode {
public:
    RainbowMode() {
        show_delay_ms = RAINBOW_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(show_delay_ms) + input,
            RAINBOW_MODE_MIN_DELAY_MS,
            RAINBOW_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return false;
        show_delay_ms = new_show_delay_ms;
        return true;
    }

private:
    void _calculate(LedController &controller) override {

        static uint8_t current_index = 0;

        for (size_t segment_id = 0; segment_id < controller.number_of_segments(); ++segment_id) {
            uint8_t hue = current_index + RAINBOW_MODE_HUE_OFFSET * segment_id;
            controller.set_color_to_segment(segment_id,CHSV(hue,255,255));
        }
        current_index++;
    }
};

class Rainbow2Mode : public AbstractColorMode {
public:
    Rainbow2Mode() {
        show_delay_ms = RAINBOW2_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(show_delay_ms) + input,
            RAINBOW2_MODE_MIN_DELAY_MS,
            RAINBOW2_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return false;
        show_delay_ms = new_show_delay_ms;
        return true;
    }

private:
    void _calculate(LedController &controller) override {

        static uint8_t current_index = 0;
        for (size_t i = 0; i < controller.number_of_leds(); ++i) {
            controller.set_color_to_led(i, CHSV(current_index + i * RAINBOW2_MODE_HUE_MUL, 255, 255));
        }
        current_index++;
    }
};


class Rainbow3Mode : public AbstractColorMode {
public:
    Rainbow3Mode() {
        show_delay_ms = RAINBOW3_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(show_delay_ms) + input,
            RAINBOW3_MODE_MIN_DELAY_MS,
            RAINBOW3_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return false;
        show_delay_ms = new_show_delay_ms;
        return true;
    }

private:
    void _calculate(LedController &controller) override {

        static uint8_t current_index = 0;
        static uint8_t current_index_inverse = 127;

        for (size_t segment_id = 0; segment_id < controller.number_of_segments(); ++segment_id) {
            auto segment = controller.get_segment(segment_id);
            if (not segment) return;
            for (uint16_t i = segment->start; i < segment->end + 1; ++i) {
                uint8_t hue = (segment_id % 2 ? current_index : current_index_inverse) +
                              i * RAINBOW3_MODE_HUE_MUL + segment_id * RAINBOW3_MODE_SEGMENTS_OFFSET;
                controller.set_color_to_led(i, CHSV(hue,255, 255));
            }
        }
        current_index++;
        current_index_inverse--;
    }
};


class FireMode : public AbstractColorMode {
public:

    uint8_t hue_start = FIRE_MODE_HUE_START;
    uint8_t hue_gap = FIRE_MODE_HUE_GAP;
    uint8_t fire_step = FIRE_MODE_FIRE_STEP;


    FireMode() {
        show_delay_ms = FIRE_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {

        static change_t parameter = HUE_START;
        if (input == CHANGE_BUTTON) return false;

        switch (input) {
            case DECREASE_BUTTON:
                switch (parameter) {
                    case HUE_START: hue_start++;    break;
                    case HUE_GAP:   hue_gap++;      break;
                    case FIRE_STEP: fire_step++;    break;
                }
                break;
            case INCREASE_BUTTON:
                parameter = parameter >= FIRE_STEP ? HUE_START : static_cast<change_t>(parameter + 1);
            case CHANGE_BUTTON:
                break;
        }
        return true;
    }

private:
    enum change_t {HUE_START, HUE_GAP, FIRE_STEP} ;

    void _calculate(LedController &controller) override {

        static uint16_t counter = 0;

        for(size_t i = 0; i < controller.number_of_leds(); i++) {
            uint8_t value = inoise8(i * fire_step, counter);
            controller.set_color_to_led(i, CHSV(
                    hue_start + map(value, 0, 255, 0, hue_gap),
                    constrain(map(value, 0, 255, FIRE_MODE_MAX_SAT, FIRE_MODE_MIN_SAT), 0, 255),
                    constrain(map(value, 0, 255, FIRE_MODE_MIN_BRIGHT, FIRE_MODE_MAX_BRIGHT), 0, 255)
            ));
        }
        counter += 1;
    }
};

class BlinkMode : public AbstractColorMode {
public:
    BlinkMode() {
        show_delay_ms = BLINK_MODE_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
                int(show_delay_ms) + input,
                BLINK_MODE_MIN_DELAY_MS,
                BLINK_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return false;
        show_delay_ms = new_show_delay_ms;
        return true;
    }

protected:
    void _calculate(LedController &controller) override {
        static bool is_white = true;
        controller.fill_leds(is_white? CRGB::White : CRGB::Black);
        is_white = not is_white;
    }
};

#endif //ARD_COLOR_MODES_H