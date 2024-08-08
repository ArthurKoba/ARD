#ifndef ARD_APP_H
#define ARD_APP_H

#include "config.h"

#include <Vector.h>
#include <BDSP.h>
//#include "eeprom_saves.h"
#include "led_controller.h"
#include "input_controller.h"
#include "color_modes.h"
#include "color_music_mode.h"
#include "audio_analyzer.h"


class Application {
public:
    InputController input_controller;
    LedController led_controller;
    AbstractColorMode* color_modes_p[NUMBER_OF_MODES]; // 119 bytes
    ColorMode current_mode = COLOR_MUSIC;
    Analyzer analyzer;
    BDSPTransmitter transmitter;

    void init() {
        Serial.println(F("\nStart ARD Project. Code: https://github.com/ArthurKoba/ARD"));

        led_controller.init();
        init_modes();

        input_controller.init([] (input_event_t event, void *context) {
            auto &t = *reinterpret_cast<Application*>(context);
            switch (event) {
                case CHANGE_BUTTON:
                    t.current_mode = t.current_mode >= NUMBER_OF_MODES ? WHITE_MODE :
                            static_cast<ColorMode>(t.current_mode + 1);
                    break;
                case DECREASE_BUTTON:
                case INCREASE_BUTTON:
                    t.color_modes_p[t.current_mode]->handle_input_event(event);
                    break;
            }
        },this);

        COBS::config_t config = {.delimiter = '\n', .depth = 64};

        transmitter.set_config(config, [] (uint8_t *data_p, size_t size, void *context) {
            Serial.write(data_p, size);
            Serial.flush();
        });

//        is_need_calibration = false;
//        init_eeprom(context);

    }

    void loop() {
        input_controller.check();
        show_modes();
    }

    ~Application() {
        for (auto & color_mode : color_modes_p) delete color_mode;
    }

private:

    void init_modes () {
        for (int i = 0; i < ColorMode::NUMBER_OF_MODES; ++i) {
            switch (static_cast<ColorMode>(i)) {
                case WHITE_MODE:
                    color_modes_p[i] = new WhiteMode;
                    break;
                case CREATIVE_MODE:
                    color_modes_p[i] = new CreativeMode(led_controller.number_of_segments());
                    break;
                case FILL_WHITE_MODE:
                    color_modes_p[i] = new FillWhiteMode;
                    break;
                case MOVE_TO_CENTER_MODE:
                    color_modes_p[i] = new ToCenterMode;
                    break;
                case FADE_MODE:
                    color_modes_p[i] = new FadeMode;
                    break;
                case RAINBOW_MODE:
                    color_modes_p[i] = new RainbowMode;
                    break;
                case RAINBOW2_MODE:
                    color_modes_p[i] = new Rainbow2Mode;
                    break;
                case RAINBOW3_MODE:
                    color_modes_p[i] = new Rainbow3Mode;
                    break;
                case FIRE_MODE:
                    color_modes_p[i] = new FireMode;
                    break;
                case COLOR_MUSIC:
                    color_modes_p[i] = new ColorMusicMode;
                case NUMBER_OF_MODES:
                    break;
            }
        }
    }

    void show_modes() {
        if (not color_modes_p[current_mode]) return;
        bool is_need_show = color_modes_p[current_mode]->calculate(led_controller);
        if (is_need_show) led_controller.show();
    }
};

#endif //ARD_APP_H