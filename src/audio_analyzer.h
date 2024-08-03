#ifndef ARD_AUDIO_ANALYZER_H
#define ARD_AUDIO_ANALYZER_H

#include "config.h"
#include <Arduino.h>
#include <FHT.h>
#include "types.h"

#ifndef SILENCE_AMPLITUDE_LEVEL
#define SILENCE_AMPLITUDE_LEVEL 15
#endif


void fht_process(AnalyzerConfigs &cfg) {
    fht_window();
    fht_reorder();
    fht_run();
    fht_mag_log();
    for (uint8_t i = 0; i < LENGTH_OF_SMOOTHED_AMPLITUDES; i++ ) {
        cfg.amplitudes[i] = (cfg.amplitudes[i] + cfg.smoothing_amplitudes[i]) >> 1;
        cfg.smoothing_amplitudes[i] = cfg.amplitudes[i];
    }
}


void readSamples(AnalyzerConfigs &cfg) {
    cfg.sampling_frequency = micros();
    cfg.samples[0] = analogRead(AUDIO_PIN) - cfg.sample_offset;
    int16_t min_value = cfg.samples[0];
    int16_t max_value = cfg.samples[0];
    for (uint16_t i = 1; i < FHT_N; i++) {
        if (cfg.delay_reading_samples_us > 0) delayMicroseconds(cfg.delay_reading_samples_us);
        cfg.samples[i] = analogRead(AUDIO_PIN) - cfg.sample_offset;
        if (cfg.samples[i] > max_value) max_value = cfg.samples[i];
        if (cfg.samples[i] < min_value) min_value = cfg.samples[i];
    }
    cfg.sampling_frequency = micros() - cfg.sampling_frequency;
    cfg.sampling_frequency = cfg.sampling_frequency / FHT_N;
    cfg.sampling_frequency = 1000000 / cfg.sampling_frequency;
    cfg.signal_amplitude = (max_value - min_value) >> 1;
    cfg.signal_median_amplitude = max_value - cfg.signal_amplitude;
    if (cfg.signal_amplitude > SILENCE_AMPLITUDE_LEVEL) {
        cfg.time_of_last_signal = millis();
    }
}

void calibrate_audio_analyzer(AnalyzerConfigs &cfg) {
    readSamples(cfg);
    cfg.time_of_last_signal = 0;
    if (cfg.signal_amplitude > SILENCE_AMPLITUDE_LEVEL) {
        Serial.println("HIGH INPUT AUDIO LEVEL. NEED SILENCE FOR CALIBRATION");
    } else if (abs(cfg.signal_median_amplitude) > 20) {
        cfg.sample_offset += cfg.signal_median_amplitude >> 1;
    } else if (cfg.signal_median_amplitude > 0) {
        cfg.sample_offset += 1;
    } else if (cfg.signal_median_amplitude < 0) {
        cfg.sample_offset -= 1;
    } else {
       cfg.need_calibration = false;
       Serial.print(F("Sampling Freq: "));
       Serial.println(cfg.sampling_frequency);
    }
}


void init_audio_analyzer(AnalyzerConfigs &cfg) {

#if not defined(KOBA_CONF)
    analogReference(INTERNAL2V56);
#endif

    _SFR_BYTE(ADCSRA) &= ~_BV(ADPS1);
    TCCR1A = 0b00000001;
    TCCR1B = 0b00001001;
    TCCR2B = 0b00000001;
    TCCR2A = 0b00000011;

    cfg.samples = fht_input;
    cfg.amplitudes = fht_log_out;
}

#endif //ARD_AUDIO_ANALYZER_H
