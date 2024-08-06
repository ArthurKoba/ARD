#ifndef ARD_BDSP_SENDER_H
#define ARD_BDSP_SENDER_H

#include "types.h"
#include "BDSP.h"
#include "stdint.h"

void init_bdsp(BDSPTransmitter &transmitter) {
    COBS::config_t config = {.delimiter = '\n', .depth = 255};
    transmitter.set_config(config, [] (uint8_t *data_p, size_t size, void *context) {
        Serial.write(data_p, size);
        Serial.flush();
    }, nullptr);
}

void send_samples(Context &ctx) {

    ctx.transmitter.send_data(1, reinterpret_cast<uint8_t*>(ctx.analyzer.samples), FHT_N * 2);
}

void send_amplitudes(Context &ctx) {
    ctx.transmitter.send_data(2, ctx.analyzer.amplitudes, FHT_N / 2);
}

#endif //ARD_BDSP_SENDER_H
