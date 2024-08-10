#ifndef ARD_EEPROM_MOMORY_H
#define ARD_EEPROM_MOMORY_H

#include "config.h"
#include <HardwareSerial.h>
#include <EEPROM.h>
#include "color_modes_types.h"
#include "utils.h"


#if not defined(EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS)
#define EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS 10000
#endif



class EEPROMMemory final {
    const int firmware_hash_address = 0;
    bool is_need_update_saves = false;
    uint32_t last_update = 0;
public:
    ColorModesMemory storage;

    void update() {
        last_update = millis();
        is_need_update_saves = true;
    }

    void init(void (*on_update_handler)(void *), void *context = nullptr) {
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
            is_need_update_saves = true;
            if (on_update_handler) on_update_handler(context);

        } else {
            Serial.println(F("Settings have been read successfully."));
            EEPROM.get(firmware_hash_address + 1, storage);
        }
    }

    void tick() {
        if (millis() - last_update < EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS) return;
        last_update = millis();
        if (not is_need_update_saves) return;
        EEPROM.put(firmware_hash_address + 1, storage);
        is_need_update_saves = false;
        Serial.println(F("The settings have been updated successfully."));
    }
};

#endif //ARD_EEPROM_MOMORY_H
