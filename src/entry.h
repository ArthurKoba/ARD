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
    CFastLED::addLeds<LED_TYPE, STRIP_PIN, COLOR_ORDER>(context.leds, NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    init_segments(context);
    init_audio_analyzer(context.analyzer);
    init_buttons();
    set_mode(context, FADE_MODE);
}

void main_loop() {
//    Serial.print(1023);
//    Serial.print(',');
//    Serial.print(0);
//    Serial.print(',');
//    Serial.println(int(analogRead(AUDIO_PIN)));
//    return;
    check_buttons(context);
    show_color_modes(context);
}

#endif //ARD_ENTRY_H
