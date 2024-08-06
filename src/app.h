#ifndef ARD_APP_H
#define ARD_APP_H

#include "config.h"
#include "default_defs.h"

#include <Arduino.h>
#include <FastLED.h>

#include "types.h"
#include "buttons.h"
#include "eeprom_saves.h"
#include "color_modes.h"
#include "audio_analyzer.h"
#include "bdsp_sender.h"
#include "utils.h"


class Application {
public:
    void init() {
        Serial.begin(SERIAL_SPEED);
        Serial.println(F("\nStart ARD Project. Code: https://github.com/ArthurKoba/ARD"));
        CFastLED::addLeds<LED_TYPE, STRIP_PIN, LED_COLOR_ORDER>(context.leds, NUM_LEDS);
        FastLED.setCorrection(TypicalLEDStrip);
        init_segments(context);
//        init_eeprom(context);
        init_audio_analyzer(context.analyzer);
        init_buttons();
        init_bdsp(context.transmitter);
        context.analyzer.sample_offset = 570;
        context.analyzer.need_calibration = false;
        set_mode(context, COLOR_MUSIC);
    }

    void loop() {
        check_buttons(context);
//        check_need_update_saves(context);
        show_color_modes(context);
    }
private:
    Context context;
};

#endif //ARD_APP_H
