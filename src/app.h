#ifndef ARD_APP_H
#define ARD_APP_H

#include "config.h"
#include "Arduino.h"


uint16_t freeMemory() {
    auto heap = static_cast<uint8_t*>(malloc(1));
    if (not heap) return 0;
    uint8_t stack;
    uint16_t free_memory = &stack - heap;
    free(heap);
    return free_memory;
}

#include "led_controller.h"
#include "input_controller.h"
#include "eeprom_momory.h"
#include "color_modes_types.h"
#include "color_modes.h"
#include "color_music_mode.h"
#include "audio_analyzer.h"
#include <BDSP.h>


class Application {
public:
    InputController input_controller;
    LedController led_controller;
    BDSPTransmitter transmitter;
    Analyzer analyzer;
    EEPROMMemory memory;
    AbstractColorMode* color_mode_p = nullptr;

    void init() {
        Serial.begin(SERIAL_SPEED);
        Serial.println(F("\nStart ARD Project. Code: https://github.com/ArthurKoba/ARD"));

        analyzer.init();
        led_controller.init();

#if not defined(KOBA_CONF)
//#if true
        memory.init([] (void *context) {
            auto &t = *reinterpret_cast<Application*>(context);
            CreativeMode::init_colors(t.led_controller, t.memory.storage.creative_mode_segments_hues);
            Serial.println(F("\nStart ARD Project. Code: https://github.com/ArthurKoba/ARD"));
        }, this);
#else
        memory.storage.current_mode = COLOR_MUSIC;

        COBS::config_t config = {.delimiter = '\n', .depth = 255};

        transmitter.set_config(config, [] (uint8_t *data_p, size_t size, void *context) {
            Serial.write(data_p, size);
            Serial.flush();
        }, nullptr);

        analyzer.set_transmitter(transmitter);
        analyzer.is_need_calibration = false;
//        analyzer.is_send_samples = true;
        analyzer.is_send_amplitudes = true;
#endif
        input_controller.init([] (input_event_t event, void *context) {
            auto &t = *reinterpret_cast<Application*>(context);
            switch (event) {
                case CHANGE_BUTTON:
                    t.set_mode(static_cast<ColorMode>(t.memory.storage.current_mode + 1));
                    break;
                case DECREASE_BUTTON:
                case INCREASE_BUTTON:
                    t.color_mode_p->handle_input_event(event);
                    break;
            }
        },this);
    }

    void loop() {
        input_controller.check();
        show_current_mode();
        memory.tick();

    }

    ~Application() {
        delete color_mode_p;
    }

    void set_mode(ColorMode mode) {
        mode = mode > COLOR_MUSIC ? WHITE_MODE : mode;
        if (mode not_eq memory.storage.current_mode) memory.update();
        memory.storage.current_mode = mode;
        delete color_mode_p;
        switch (mode) {
            case WHITE_MODE:
                color_mode_p = new WhiteMode(memory);
                break;
            case CREATIVE_MODE:
                color_mode_p = new CreativeMode(memory, led_controller);
                break;
            case FILL_WHITE_MODE:
                color_mode_p = new FillWhiteMode(memory);
                break;
            case MOVE_TO_CENTER_MODE:
                color_mode_p = new ToCenterMode(memory);
                break;
            case FADE_MODE:
                color_mode_p = new FadeMode(memory);
                break;
            case RAINBOW_MODE:
                color_mode_p = new Rainbow1Mode(memory);
                break;
            case RAINBOW2_MODE:
                color_mode_p = new Rainbow2Mode(memory);
                break;
            case RAINBOW3_MODE:
                color_mode_p = new Rainbow3Mode(memory);
                break;
            case FIRE_MODE:
                color_mode_p = new FireMode(memory);
                break;
            case COLOR_MUSIC:
                color_mode_p = new ColorMusicMode(memory, analyzer);
                break;
        }
    }

    void show_current_mode() {
        if (not color_mode_p) return set_mode(memory.storage.current_mode);
        bool is_need_show = color_mode_p->calculate(led_controller);
        if (is_need_show) led_controller.show();
    }
};

#endif //ARD_APP_H