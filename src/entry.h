#ifndef ARD_ENTRY_H
#define ARD_ENTRY_H

#include "config.h"

#include <Arduino.h>
#include <FastLED.h>

#include "types.h"
#include "buttons.h"
//#include "ir_remote.h"
//#include "eeprom_saves.h"
#include "color_modes.h"
#include "audio_analyzer.h"
#include "utils.h"


App context;


void main_setup() {
    Serial.begin(115200);
//     EEPROM.begin(1024);
    //    read_eeprom();
    init_audio_analyzer(context.analyzer);
    init_buttons();
    CFastLED::addLeds<LED_TYPE, STRIP_PIN, COLOR_ORDER>(context.leds, NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    init_segments(context);
    set_mode(context, MOVE_TO_CENTER_MODE);
}

void main_loop() {
//    Serial.print(1023);
//    Serial.print(',');
//    Serial.print(0);
//    Serial.print(',');
//    Serial.println(int(analogRead(AUDIO_PIN)));
//    return;
    check_buttons(context);
    switch (context.mode) {
        case WHITE_MODE:
            white_mode(context);
            break;
        case CREATIVE_MODE:
            creative_mode(context);
            break;
        case FILL_WHITE_MODE:
            fill_white_mode(context);
            break;
        case MOVE_TO_CENTER_MODE:
            move_to_center_mode(context);
            break;
        case MODE_3:
            mode_3(context);
            break;
//        case MODE_4:
//            break;
//        case MODE_5:
//            break;
//        case MODE_6:
//            break;
        case COLOR_MUSIC:
            if (context.analyzer.need_calibration) {
                calibrate_audio_analyzer(context.analyzer);
                if (context.analyzer.need_calibration) {
                    blink_mode(context);
                } else {
                    fill_leds(context, CRGB::Black);
                }
            } else {
                readSamples(context.analyzer);
                fht_process(context.analyzer);
                color_music(context);
            }
            break;
        case RAINBOW_MODE:
            rainbow_mode(context);
            break;
    }
    FastLED.show();
}

#endif //ARD_ENTRY_H
