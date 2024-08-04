#ifndef ARD_EEPROM_SAVES_H
#define ARD_EEPROM_SAVES_H

#include "config.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "types.h"
#include "utils.h"

#if not defined(EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS)
#define EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS 10000
#endif

void check_need_update_saves(App &ctx) {
    exit_timer(EEPROM_DELAY_CHECK_NEED_UPDATE_SAVES_MS);

    if (not ctx.need_update_saves) return;

    EEPROM.put(ctx.firmware_hash_address + 1, ctx.cfg_modes);
    ctx.need_update_saves = false;
    Serial.println(F("The settings have been updated successfully."));
}

void init_eeprom(App &ctx) {
    String start_datetime = String(__TIMESTAMP__);
    Serial.print(F("Date of firmware: "));
    Serial.println(start_datetime);
    Serial.println();

    uint8_t hash = 0;
    for (size_t i = 0; i < start_datetime.length(); ++i) {
        hash += start_datetime.charAt(i);
    }

    if (EEPROM.read(ctx.firmware_hash_address) not_eq hash) {
        Serial.println(F("The hash of the memory did not match. Reset memory settings."));
        EEPROM.write(ctx.firmware_hash_address, hash);
        init_default_colors_for_creative_mode(ctx);
        ctx.need_update_saves = true;
    } else {
        Serial.println(F("Settings have been read successfully."));
        EEPROM.get(ctx.firmware_hash_address + 1, ctx.cfg_modes);
    }
}

#endif //ARD_EEPROM_SAVES_H
