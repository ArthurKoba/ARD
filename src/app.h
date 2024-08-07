#ifndef ARD_APP_H
#define ARD_APP_H

#include "config.h"

#include <Vector.h>
//#include "eeprom_saves.h"
#include "input_controller.h"
//#include "led_controller.h"
#include "color_modes.h"
#include "color_music_mode.h"
//#include "types.h"
//#include "audio_analyzer.h"
//#include "bdsp_sender.h"


class Application {
public:
    void init() {
        Serial.begin(SERIAL_SPEED);
        Serial.println(F("\nStart ARD Project. Code: https://github.com/ArthurKoba/ARD"));

//        init_eeprom(context);
//        init_audio_analyzer(analyzer);
//        init_bdsp(transmitter);
//        analyzer.sample_offset = 570;
//        analyzer.need_calibration = false;
        led_controller.init();
        init_modes();

        input_controller.init([] (input_event_t event, void *context) {
            auto &t = *reinterpret_cast<Application*>(context);
            switch (event) {
                case CHANGE_BUTTON:
                    t.current_mode = static_cast<ColorMode>(t.current_mode + 1);
                    if (t.current_mode >= NUMBER_OF_MODES) t.current_mode = WHITE_MODE;
                    break;
                case DECREASE_BUTTON:
                case INCREASE_BUTTON:
                    t.color_modes_p[t.current_mode]->handle_input_event(event);
                    break;
            }
        },this);
    }

    void loop() {
        input_controller.check();
        show_modes();
    }

    ~Application() {
        for (auto & color_mode : color_modes_p) delete color_mode;
        delete color_modes_p.data();
    }

private:
    InputController input_controller;
    LedController led_controller;
    Vector<AbstractColorMode*> color_modes_p;
    ColorMode current_mode = FILL_WHITE_MODE;

    void show_modes() {
        if (not color_modes_p[current_mode]) return;
        bool is_need_show = color_modes_p[current_mode]->calculate(led_controller);
        if (is_need_show) led_controller.show();
    }

    void init_modes () {
        if (not color_modes_p.empty()) return;

        color_modes_p.setStorage(new AbstractColorMode*[ColorMode::NUMBER_OF_MODES], ColorMode::NUMBER_OF_MODES, 0);

        for (int i = 0; i < ColorMode::NUMBER_OF_MODES; ++i) {
            switch (static_cast<ColorMode>(i)) {
                case WHITE_MODE:
                    color_modes_p.push_back(new WhiteMode);
                    break;
                case CREATIVE_MODE:
                    color_modes_p.push_back(
                        new CreativeMode(led_controller.number_of_segments())
                    );
                    break;
                case FILL_WHITE_MODE:
                    color_modes_p.push_back(new FillWhiteMode);
                    break;
                case MOVE_TO_CENTER_MODE:
                    color_modes_p.push_back(new ToCenterMode);
                    break;
                case FADE_MODE:
                    color_modes_p.push_back(new FadeMode);
                    break;
                case RAINBOW_MODE:
                    color_modes_p.push_back(new RainbowMode);
                    break;
                case RAINBOW2_MODE:
                    color_modes_p.push_back(new Rainbow2Mode);
                    break;
                case RAINBOW3_MODE:
                    color_modes_p.push_back(new Rainbow3Mode);
                    break;
                case FIRE_MODE:
                    color_modes_p.push_back(new FireMode);
                    break;
                case COLOR_MUSIC:
                    color_modes_p.push_back(new ColorMusicMode);
                case NUMBER_OF_MODES:
                    break;
            }
        }
    }
};

#endif //ARD_APP_H