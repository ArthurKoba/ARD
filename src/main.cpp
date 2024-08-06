#if defined(PLATFORMIO_ENV)

#include "app.h"

Application app;

void setup() {
    app.init();
}

void loop() {
    app.loop();
}

#endif