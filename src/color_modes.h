#ifndef ARD_COLOR_MODES_H
#define ARD_COLOR_MODES_H

#include "led_controller.h"
#include "input_controller.h"
#include "eeprom_momory.h"


class AbstractColorMode {
protected:
    uint32_t last_update = 0;
    EEPROMMemory &mem;

    virtual void _calculate(LedController &controller) = 0;
    virtual uint16_t _get_show_delay() const = 0;
public:
    explicit AbstractColorMode(EEPROMMemory &mem) : mem(mem) {};
    virtual ~AbstractColorMode() = default;

    virtual void handle_input_event(input_event_t input) = 0;

    virtual bool calculate(LedController &controller) {
        if (millis() - last_update < _get_show_delay()) return false;
        last_update = millis();
        _calculate(controller);
        return true;
    }
};


class WhiteMode final: public AbstractColorMode  {
private:
    uint8_t &bright = mem.storage.white_mode_bright;

public:
    explicit WhiteMode(EEPROMMemory &mem) : AbstractColorMode(mem) {};

    void handle_input_event(input_event_t input) override {
        uint16_t new_white_mode_bright = constrain(
            int(bright) + input * 10,
            WHITE_MODE_MIN_BRIGHT,
            WHITE_MODE_MAX_BRIGHT
        );
        if (new_white_mode_bright == mem.storage.white_mode_bright) return;
        mem.storage.white_mode_bright = new_white_mode_bright;
        mem.update();
    }

private:
    uint16_t _get_show_delay() const override { return WHITE_MODE_DEF_DELAY_MS; }

    void _calculate(LedController &controller) override {
        controller.fill_leds(CRGB_f(
                mem.storage.white_mode_bright,
                mem.storage.white_mode_bright,
                mem.storage.white_mode_bright
        ));
    }
};


class CreativeMode final: public AbstractColorMode {
    // 7 сегментов меняют цвет, 255 оттенков, кроме последнего сегмента 8-го, состоящего из центральной точки.
    // Она всегда горит белым цветом, у неё просто меняется яркость если пытаешься менять цвет в этом режиме.
private:
    LedController &controller;
    uint8_t *hues = mem.storage.creative_mode_segments_hues;
    size_t current_hue = 0;

public:
    explicit CreativeMode(EEPROMMemory &mem, LedController &ctrl) : AbstractColorMode(mem), controller(ctrl) {};

    static void init_colors(LedController &controller, uint8_t *hues) {
        for (size_t i = 0; i < controller.number_of_segments() - 1; ++i) hues[i] = 42 * i;
        hues[controller.number_of_segments() - 1] = 255;
    }

    void handle_input_event(input_event_t input) override {
        if (input == input_event_t::INCREASE_BUTTON) {
            current_hue = current_hue >= controller.number_of_segments() ? 0 : current_hue + 1;
        } else if (input == input_event_t::DECREASE_BUTTON) {
            hues[current_hue] += 10;
            mem.update();
        }
    }

private:
    uint16_t _get_show_delay() const override { return CREATIVE_MODE_DEF_DELAY_MS; }

    void _calculate(LedController &ctrl_) override {
        for (size_t i = 0; i < controller.number_of_segments() - 1; ++i) {
            controller.set_color_to_segment(i, CHSV(hues[i],255, 255));
        }
        size_t last = controller.number_of_segments() - 1;
        controller.set_color_to_segment(last, CRGB(hues[last], hues[last], hues[last]));
    }
};


class FillWhiteMode final: public AbstractColorMode {
    //  Поочередно зажигаются диоды начиная от внешнего периметра к центральной точке.
    //  Потом немого поработали включенными все диоды, затухают и цикл начинается заново.
    //  Кнопками меняется скорость работы режима + -.

    uint8_t count_wait = FILL_WHITE_MODE_COUNT_WAIT;

public:
    explicit FillWhiteMode(EEPROMMemory &mem) : AbstractColorMode(mem) {};

    void handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(mem.storage.fill_white_mode_show_delay_ms) + input,
            FILL_WHITE_MODE_MIN_DELAY_MS,
            FILL_WHITE_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == mem.storage.fill_white_mode_show_delay_ms) return;
        mem.storage.fill_white_mode_show_delay_ms = new_show_delay_ms;
        mem.update();
    }

private:
    uint16_t _get_show_delay() const override { return mem.storage.fill_white_mode_show_delay_ms; }

    void _calculate(LedController &controller) override {
        static uint16_t fullness = 0;
        static bool is_fill = true;
        static uint16_t count_waits_ = 0;

        if (is_fill and fullness not_eq controller.number_of_leds()) {
            fullness++;
            controller.fill_leds(CRGB::White, 0, fullness);
            controller.fill_leds(CRGB::Black, fullness + 1);
        } else if (is_fill and count_waits_ < count_wait) {
            controller.fill_leds(CRGB::White);
            count_waits_++;
        } else if (is_fill) {
            count_waits_ = 0;
            fullness = 0;
            is_fill = false;
        } else if (fullness not_eq controller.number_of_leds()) {
            fullness++;
            controller.fill_leds(CRGB::Black, 0, fullness);
            controller.fill_leds(CRGB::White, fullness + 1);
        } else if (count_waits_ < count_wait){
            count_waits_++;
        } else {
            count_waits_ = 0;
            fullness = 0;
            is_fill = true;
        }
    }
};


class ToCenterMode final: public AbstractColorMode {

    uint16_t &show_delay_ms = mem.storage.to_center_mode_show_delay_ms;

public:
    explicit ToCenterMode(EEPROMMemory &mem) : AbstractColorMode(mem) {}

    void handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(show_delay_ms) + input * 100,
            TO_CENTER_MODE_MIN_DELAY_MS,
            TO_CENTER_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == show_delay_ms) return;
        show_delay_ms = new_show_delay_ms;
        mem.update();
    }

private:
    uint16_t _get_show_delay() const override { return show_delay_ms; }

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


class FadeMode final: public AbstractColorMode {
public:
    explicit FadeMode(EEPROMMemory &mem) : AbstractColorMode(mem) {}

    void handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(mem.storage.fade_mode_show_delay_ms) + input,
            FADE_MODE_MIN_DELAY_MS,
            FADE_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == mem.storage.fade_mode_show_delay_ms) return;
        mem.storage.fade_mode_show_delay_ms = new_show_delay_ms;
        mem.update();
    }

private:
    uint16_t _get_show_delay() const override { return mem.storage.fade_mode_show_delay_ms; }

    void _calculate(LedController &controller) override {

        static uint8_t bright = 254;
        static bool is_fade = false;

        bright += is_fade ? - 1 : 1;
        if (bright < 1) is_fade = false;
        if (bright > 254) is_fade = true;

        controller.fill_leds(CRGB_f(bright, bright, bright));
    }
};


class Rainbow1Mode final: public AbstractColorMode {
public:
    explicit Rainbow1Mode(EEPROMMemory &mem) : AbstractColorMode(mem) {}

    void handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(mem.storage.rainbow1_mode_show_delay_ms) + input,
            RAINBOW_MODE_MIN_DELAY_MS,
            RAINBOW_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == mem.storage.rainbow1_mode_show_delay_ms) return;
        mem.storage.rainbow1_mode_show_delay_ms = new_show_delay_ms;
        mem.update();
    }

private:
    uint16_t _get_show_delay() const override { return mem.storage.rainbow1_mode_show_delay_ms; }

    void _calculate(LedController &controller) override {

        static uint8_t current_index = 0;

        for (size_t segment_id = 0; segment_id < controller.number_of_segments(); ++segment_id) {
            uint8_t hue = current_index + RAINBOW_MODE_HUE_OFFSET * segment_id;
            controller.set_color_to_segment(segment_id,CHSV(hue,255,255));
        }
        current_index++;
    }
};


class Rainbow2Mode final: public AbstractColorMode {
public:
    explicit Rainbow2Mode(EEPROMMemory &mem) : AbstractColorMode(mem) {}

    void handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(mem.storage.rainbow2_mode_show_delay_ms) + input,
            RAINBOW2_MODE_MIN_DELAY_MS,
            RAINBOW2_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == mem.storage.rainbow2_mode_show_delay_ms) return;
        mem.storage.rainbow2_mode_show_delay_ms = new_show_delay_ms;
        mem.update();
    }

private:
    uint16_t _get_show_delay() const override { return mem.storage.rainbow2_mode_show_delay_ms; }

    void _calculate(LedController &controller) override {

        static uint8_t current_index = 0;
        for (size_t i = 0; i < controller.number_of_leds(); ++i) {
            controller.set_color_to_led(i, CHSV(current_index + i * RAINBOW2_MODE_HUE_MUL, 255, 255));
        }
        current_index++;
    }
};


class Rainbow3Mode final: public AbstractColorMode {
public:
    explicit Rainbow3Mode(EEPROMMemory &mem) : AbstractColorMode(mem) {}

    void handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
            int(mem.storage.rainbow3_mode_show_delay_ms) + input,
            RAINBOW3_MODE_MIN_DELAY_MS,
            RAINBOW3_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == mem.storage.rainbow3_mode_show_delay_ms) return;
        mem.storage.rainbow3_mode_show_delay_ms = new_show_delay_ms;
        mem.update();
    }

private:
    uint16_t _get_show_delay() const override { return mem.storage.rainbow3_mode_show_delay_ms; }

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


class FireMode final: public AbstractColorMode {


public:
    explicit FireMode(EEPROMMemory &mem) : AbstractColorMode(mem) {}

    void handle_input_event(input_event_t input) override {

        static change_t parameter = HUE_START;


        switch (input) {
            case DECREASE_BUTTON:
                switch (parameter) {
                    case HUE_START: mem.storage.fire_mode_hue_start++;    break;
                    case HUE_GAP:   mem.storage.fire_mode_hue_gap++;      break;
                    case FIRE_STEP: mem.storage.fire_mode_step++;         break;
                }
                break;
            case INCREASE_BUTTON:
                parameter = parameter >= FIRE_STEP ? HUE_START : static_cast<change_t>(parameter + 1);
            case CHANGE_BUTTON:
                break;
        }
        if (input == DECREASE_BUTTON) mem.update();
    }

private:
    enum change_t {HUE_START, HUE_GAP, FIRE_STEP};

    uint16_t _get_show_delay() const override { return FIRE_MODE_DEF_DELAY_MS; }

    void _calculate(LedController &controller) override {

        static uint16_t counter = 0;

        for(size_t i = 0; i < controller.number_of_leds(); i++) {
            uint8_t value = inoise8(i * mem.storage.fire_mode_step, counter);
            controller.set_color_to_led(i, CHSV(
                    mem.storage.fire_mode_hue_start + map(value, 0, 255, 0, mem.storage.fire_mode_hue_gap),
                    constrain(map(value, 0, 255, FIRE_MODE_MAX_SAT, FIRE_MODE_MIN_SAT), 0, 255),
                    constrain(map(value, 0, 255, FIRE_MODE_MIN_BRIGHT, FIRE_MODE_MAX_BRIGHT), 0, 255)
            ));
        }
        counter += 1;
    }
};

class BlinkMode final: public AbstractColorMode {
public:
    explicit BlinkMode(EEPROMMemory &mem) : AbstractColorMode(mem) {}

    void handle_input_event(input_event_t input) override {
        uint16_t new_show_delay_ms = constrain(
                int(mem.storage.blink_mode_show_delay_ms) + input,
                BLINK_MODE_MIN_DELAY_MS,
                BLINK_MODE_MAX_DELAY_MS
        );
        if (new_show_delay_ms == mem.storage.blink_mode_show_delay_ms) return;
        mem.storage.blink_mode_show_delay_ms = new_show_delay_ms;
        mem.update();
    }

protected:
    uint16_t _get_show_delay() const override { return mem.storage.blink_mode_show_delay_ms; }

    void _calculate(LedController &controller) override {
        static bool is_white = true;
        controller.fill_leds(is_white? CRGB::White : CRGB::Black);
        is_white = not is_white;
    }
};

#endif //ARD_COLOR_MODES_H