#ifndef ARD_EEPROM_SAVES_H
#define ARD_EEPROM_SAVES_H

#include "config.h"
#include <HardwareSerial.h>
#include <EEPROM.h>


#if not defined(EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS)
#define EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS 10000
#endif

class EEPROMSaves {

public:
    void init() {
        String start_datetime = String(__TIMESTAMP__);
        Serial.print(F("Date of firmware: "));
        Serial.println(start_datetime);
        Serial.println();

        uint8_t hash = 0;
        for (size_t i = 0; i < start_datetime.length(); ++i) {
            hash += start_datetime.charAt(i);
        }

        if (EEPROM.read(firmware_hash_address) not_eq hash) {
            Serial.println(F("The hash of the memory did not match. Reset memory settings."));
            EEPROM.write(firmware_hash_address, hash);
            need_update_saves = true;
        } else {
            Serial.println(F("Settings have been read successfully."));
//            EEPROM.get(firmware_hash_address + 1, );
        }
    }

    void update() {
//        exit_timer(EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS);
//
//        if (not ctx.need_update_saves) return;
//
//        EEPROM.put(ctx.firmware_hash_address + 1, ctx.cfg_modes);
//        ctx.need_update_saves = false;
//        Serial.println(F("The settings have been updated successfully."));
    }

private:
    int firmware_hash_address = 0;
    bool need_update_saves = false;
};

#endif //ARD_EEPROM_SAVES_H
