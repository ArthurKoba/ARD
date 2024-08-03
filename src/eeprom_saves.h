#ifndef ARD_EEPROM_SAVES_H
#define ARD_EEPROM_SAVES_H

#include "config.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "types.h"


void read_eeprom(App &ctx) {

    // 1 mode
    EEPROM.get((6 * sizeof(float)),br_button);
    if (br_button < MIN_BRIGHTNESS_1_MODE or br_button > MAX_BRIGHTNESS_1_MODE) {
        br_button = 127;
    }
    Serial.print(br_button);
    Serial.print(F(" br_1   "));

    // 2 mode
    EEPROM.get((3 * sizeof(float)), br_button_2);
    if (br_button_2 < MIN_BRIGHTNESS_2_MODE or br_button_2 > MAX_BRIGHTNESS) {
        br_button_2 = 127;
    }
    Serial.print(br_button_2);
    Serial.print(F(" br_2  "));
    Serial.print(F(" 2 mode colors: "));

    //6 7 8 9 10 11
    for( n_sect = 0; n_sect < NUNMBER_SECTORS; n_sect++) {
        EEPROM.get( ((6 + n_sect)*sizeof(float)), segments_colors_2[n_sect + 1]);
        Serial.print(segments_colors_2[n_sect + 1]);
        Serial.print(' ');
    }

    //3 mode
    EEPROM.get( (0*sizeof(float)), serial_on_interval); //eeAddress, customVar
    if (serial_on_interval < MIN_SERIAL_ON_INTERVAL) { serial_on_interval = MIN_SERIAL_ON_INTERVAL; }
    if (serial_on_interval > MAX_SERIAL_ON_INTERVAL) { serial_on_interval = MAX_SERIAL_ON_INTERVAL; }

    EEPROM.get( (1*sizeof(float)), br_button_3);
    if ( (br_button_3 < MIN_BRIGHTNESS_3_MODE) || (br_button_3 > MAX_BRIGHTNESS) ) { br_button_3 = 127; }

    Serial.print(F("3: "));
    Serial.print(serial_on_interval);
    Serial.print(' ');
    Serial.print(br_button_3);
    Serial.print(F("   "));

    //4 mode
    EEPROM.get( (2*sizeof(float)), pulse_interval_sec);
    if (pulse_interval_sec < MIN_PULSE_INTERVAL) { pulse_interval_sec = MIN_PULSE_INTERVAL; }
    if (pulse_interval_sec > MAX_PULSE_INTERVAL) { pulse_interval_sec = MAX_PULSE_INTERVAL; }

    pulse_interval_ms = (int) 1000.0 * pulse_interval_sec / 510.0;

    Serial.print(F("4: "));
    Serial.print(pulse_interval_sec);
    Serial.print(' ');
    Serial.print(pulse_interval_ms);
    Serial.print(' ');

    //5 mode
    EEPROM.get( (4*sizeof(float)), m5_swith_interval);
    if (m5_swith_interval < MIN_SWITCH_INTERVAl) { m5_swith_interval = MIN_SWITCH_INTERVAl; }
    if (m5_swith_interval > MAX_SWITCH_INTERVAl) { m5_swith_interval = MAX_SWITCH_INTERVAl; }

    //m5_swith_interval = ; // 3
    EEPROM.get( (5*sizeof(float)), br_button_5);
    if ( (br_button_5 < MIN_BRIGHTNESS_5_MODE) || (br_button_5 > MAX_BRIGHTNESS_5_MODE) ) { br_button_5 = 127; }
    //br_button = ;

    Serial.print(F("5: "));
    Serial.print(m5_swith_interval);
    Serial.print(' ');
    Serial.println(br_button_5);

}


#endif //ARD_EEPROM_SAVES_H
