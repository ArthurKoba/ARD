#ifndef ARD_IR_REMOTE_H
#define ARD_IR_REMOTE_H

#include "config.h"
#include <IRremote.h>
#include "types.h"
#include "utils.h"


//int cur_code_command = 0;
//CNec IRLremote;
IRrecv irrecv(IR_PIN);

void ir_init() {
    irrecv.enableIRIn();
    //  if (!IRLremote.begin(pinIR))
    // Serial.println(F("Please choose a valid pin for IR RC"));
}

void ir_exec(App &ctx) { // buttons remote control
    decode_results results;

    if (not irrecv.decode(&results)) return;
    irrecv.resume();

    ColorMode new_mode = ctx.mode;

    switch (results.value) {
        case 0xFFA25D: new_mode = WHITE_MODE; break;
        case 0xFF629D: new_mode = CREATIVE_MODE; break;
        case 0xFFE21D: new_mode = FILL_WHITE_MODE; break;
//        case 0xFF22DD: new_mode = MODE_3; break;
//        case 0xFF02FD: new_mode = MODE_4; break;
//        case 0xFFC23D: new_mode = MODE_5; break;
//        case 0xFFE01F: new_mode = MODE_6; break;
        case 0xFF10EF:
            ctx.mode = static_cast<ColorMode>(static_cast<uint8_t>(ctx.mode) - 1);
            break;
        case 0xFF5AA5:
            ctx.mode = static_cast<ColorMode>(static_cast<uint8_t>(ctx.mode) + 1);
            break;
        case 0xFF6897:
            //Serial.println("*");
            bright_button_minus = true;
            break;
        case 0xFF9867:
            //Serial.println("0");
            break;
        case 0xFFB04F:
            //Serial.println("#");
            bright_button_plus = true;
            break;
        case 0xFF18E7: //
            br_button_plus = true;
            //Serial.println("Up");
            break;
        case 0xFF4AB5:
            br_button_minus = true;
            //Serial.println("Down");
            break;
    }

    if (new_mode == ctx.mode) return;
    Serial.print(F("Select mode to: "));
    Serial.println(new_mode);
    ctx.mode = new_mode;
    prepare_vals_new_mode(ctx);
}

#endif //ARD_IR_REMOTE_H
