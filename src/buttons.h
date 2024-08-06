#ifndef ARD_BUTTONS_H
#define ARD_BUTTONS_H

#include "config.h"
#include "types.h"
#include "color_modes.h"
#include "utils.h"

#if not defined(INCREASE_PIN) or not defined(DECREASE_PIN) or not defined(CHANGE_MODE_PIN)
#error INCREASE_PIN or DECREASE_PIN or CHANGE_MODE_PIN not defined!!!
#endif

void init_buttons() {
#if defined(BUTTONS_REVERSE_LEVELS)
    pinMode(INCREASE_PIN, INPUT_PULLUP);
    pinMode(DECREASE_PIN, INPUT_PULLUP);
    pinMode(CHANGE_MODE_PIN, INPUT_PULLUP);
#else
    pinMode(INCREASE_PIN, INPUT);
    pinMode(DECREASE_PIN, INPUT);
    pinMode(CHANGE_MODE_PIN, INPUT);
#endif
}

void check_change_mode_button(Context &ctx) {
    static bool is_wait_release = false;
    exit_timer(BUTTONS_DELAY_MS);
    bool is_pressed = digitalRead(CHANGE_MODE_PIN);
#if defined(BUTTONS_REVERSE_LEVELS)
    is_pressed = not is_pressed;
#endif
    if (is_pressed and not is_wait_release) {
        is_wait_release = true;
        increase_mode(ctx);
    } else if (not is_pressed) {
        is_wait_release = false;
    }
}

void check_increase_button(Context &ctx) {
    exit_timer(BUTTONS_DELAY_MS);
    bool is_pressed = digitalRead(INCREASE_PIN);
#if defined(BUTTONS_REVERSE_LEVELS)
    is_pressed = not is_pressed;
#endif
    if (not is_pressed) return;
    exec_increase_or_decrease(ctx, 1);
}

void check_decrease_button(Context &ctx) {
    exit_timer(BUTTONS_DELAY_MS);
    bool is_pressed = digitalRead(DECREASE_PIN);
#if defined(BUTTONS_REVERSE_LEVELS)
    is_pressed = not is_pressed;
#endif
    if (not is_pressed) return;
    exec_increase_or_decrease(ctx, -1);
}

void check_buttons(Context &ctx) {
    check_change_mode_button(ctx);
    check_increase_button(ctx);
    check_decrease_button(ctx);
}

#endif //ARD_BUTTONS_H
