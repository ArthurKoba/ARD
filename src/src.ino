#if not defined(PLATFORMIO_ENV)

#include "entry.h"

void setup() {
    main_setup();
}

void loop() {
    main_loop();
}

#endif