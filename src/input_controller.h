#ifndef ARD_INPUT_CONTROLLER_H
#define ARD_INPUT_CONTROLLER_H

#include "config.h"
#include "stdint.h"

#define _exit_timer(delay_ms) \
    static uint32_t last_update = 0; \
    if (millis() - last_update < (delay_ms)) return; \
    last_update = millis()



enum input_event_t {DECREASE_BUTTON = -1, CHANGE_BUTTON = 0, INCREASE_BUTTON = 1};
typedef void (*input_controller_callback_t)(input_event_t, void *);


class InputController {
public:
    void init(input_controller_callback_t callback, void *context) {
        _callback_p = callback;
        _callback_context_p = context;

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

    void check() {
        if (not _callback_p) return;
        check_change_mode_button();
        check_decrease_button();
        check_increase_button();
    }

private:
    input_controller_callback_t _callback_p = nullptr;
    void *_callback_context_p = nullptr;

    void check_change_mode_button() {
        static bool is_wait_release = false;
        _exit_timer(BUTTONS_DELAY_MS);
        bool is_pressed = digitalRead(CHANGE_MODE_PIN);
#if defined(BUTTONS_REVERSE_LEVELS)
        is_pressed = not is_pressed;
#endif
        if (is_pressed and not is_wait_release) {
            is_wait_release = true;
            _callback_p(CHANGE_BUTTON, _callback_context_p);
        } else if (not is_pressed) {
            is_wait_release = false;
        }
    }

    void check_increase_button() {
        _exit_timer(BUTTONS_DELAY_MS);
        bool is_pressed = digitalRead(INCREASE_PIN);
#if defined(BUTTONS_REVERSE_LEVELS)
        is_pressed = not is_pressed;
#endif
        if (not is_pressed) return;
        _callback_p(INCREASE_BUTTON, _callback_context_p);
    }

    void check_decrease_button() {
        _exit_timer(BUTTONS_DELAY_MS);
        bool is_pressed = digitalRead(DECREASE_PIN);
#if defined(BUTTONS_REVERSE_LEVELS)
        is_pressed = not is_pressed;
#endif
        if (not is_pressed) return;
        _callback_p(DECREASE_BUTTON, _callback_context_p);
    }
};

#endif //ARD_INPUT_CONTROLLER_H
