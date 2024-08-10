#ifndef ARD_UTILS_H
#define ARD_UTILS_H

#define exit_timer(delay_ms) \
    static uint32_t last_update = 0; \
    if (millis() - last_update < (delay_ms)) return; \
    last_update = millis()

#endif //ARD_UTILS_H
