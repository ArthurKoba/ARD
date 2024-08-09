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

//#include "eeprom_saves.h"
#include "led_controller.h"
#include "input_controller.h"
#include "color_modes.h"
#include "color_music_mode.h"
#include "audio_analyzer.h"
#include <BDSP.h>


class Application {
public:
    InputController input_controller;
    LedController led_controller;
    AbstractColorMode* color_mode_p = nullptr;
    ColorMode current_mode = COLOR_MUSIC;
    Analyzer analyzer;
    BDSPTransmitter transmitter;

    void init() {
        Serial.begin(SERIAL_SPEED);
        Serial.println(PSTR("\nStart ARD Project. Code: https://github.com/ArthurKoba/ARD"));


        COBS::config_t config = {.delimiter = '\n', .depth = 255};

        transmitter.set_config(config, [] (uint8_t *data_p, size_t size, void *context) {
            Serial.write(data_p, size);
            Serial.flush();
        });

        analyzer.init();
        analyzer.set_transmitter(transmitter);
        analyzer.is_need_calibration = false;
//        analyzer.send_samples = true;


        led_controller.init();

        input_controller.init([] (input_event_t event, void *context) {
            auto &t = *reinterpret_cast<Application*>(context);
            switch (event) {
                case CHANGE_BUTTON:
                    t.set_mode(static_cast<ColorMode>(t.current_mode + 1));
                    break;
                case DECREASE_BUTTON:
                case INCREASE_BUTTON:
                    t.color_mode_p->handle_input_event(event);
                    break;
            }
        },this);
//        init_eeprom(context);
    }

    void loop() {
        input_controller.check();
        show_current_mode();
    }

    ~Application() {
        delete color_mode_p;
    }

    void set_mode(ColorMode mode) {
        current_mode = mode >= COLOR_MUSIC ? WHITE_MODE : mode;
        delete color_mode_p;
        switch (mode) {
            case WHITE_MODE:
                color_mode_p = new WhiteMode;
                break;
            case CREATIVE_MODE:
                color_mode_p = new FadeMode;
                break;
            case FILL_WHITE_MODE:
                color_mode_p = new FillWhiteMode;
                break;
            case MOVE_TO_CENTER_MODE:
                color_mode_p = new ToCenterMode;
                break;
            case FADE_MODE:
                color_mode_p = new FadeMode;
                break;
            case RAINBOW_MODE:
                color_mode_p = new RainbowMode;
                break;
            case RAINBOW2_MODE:
                color_mode_p = new Rainbow2Mode;
                break;
            case RAINBOW3_MODE:
                color_mode_p = new Rainbow3Mode;
                break;
            case FIRE_MODE:
                color_mode_p = new FireMode;
                break;
            case COLOR_MUSIC:
                color_mode_p = new ColorMusicMode(analyzer);
                break;
        }
    }

    void show_current_mode() {
        if (not color_mode_p) return set_mode(current_mode);
        bool is_need_show = color_mode_p->calculate(led_controller);
        if (is_need_show) led_controller.show();
    }
};

#endif //ARD_APP_H