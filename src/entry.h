#ifndef ARD_ENTRY_H
#define ARD_ENTRY_H

#include "config.h"

#include <Arduino.h>
#include <FastLED.h>

#include "types.h"
#include "buttons.h"
//#include "eeprom_saves.h"
#include "color_modes.h"
#include "audio_analyzer.h"
#include "utils.h"


App context;


void main_setup() {
    Serial.begin(SERIAL_SPEED);

//     EEPROM.begin(1024);
    //    read_eeprom();
    CFastLED::addLeds<LED_TYPE, STRIP_PIN, LED_COLOR_ORDER>(context.leds, NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    init_segments(context);
    init_audio_analyzer(context.analyzer);
    init_buttons();
    set_mode(context, CREATIVE_MODE);
    init_default_colors_for_creative_mode(context);
}

void main_loop() {
//    send_audio_signal_for_plotter(context);
    check_buttons(context);
    show_color_modes(context);
}

#endif //ARD_ENTRY_H
