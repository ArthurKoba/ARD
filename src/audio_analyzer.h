#ifndef ARD_AUDIO_ANALYZER_H
#define ARD_AUDIO_ANALYZER_H

#include "config.h"
#include "stdint.h"
#include <AvrFHT.h>
#include <fht_reorder.h>
#include <fht_window.h>
#include <fht_mag_lin8_no_asm.h>
#include <Vector.h>
#include <BDSP.h>

enum AudioInformationIndexes {KICK = 0, FREQ1, FREQ2, FREQ3, FREQ4, FREQ5, HIGH_F, HI_HATS};

struct AudioInformation {
    uint8_t kick = 0;
    uint8_t freq1 = 0;
    uint8_t freq2 = 0;
    uint8_t freq3 = 0;
    uint8_t freq4 = 0;
    uint8_t freq5 = 0;
    uint8_t high_f = 0;
    uint8_t hi_hats = 0;
};

template<typename Type>
class ValueHistory {
public:
    Type min = 0;
    Type max = 0;
    Type avg = 0;
    Type delta = 0;
    Type data[AMPLITUDES_HISTORY_LENGTH] = {0};

    ValueHistory() = default;

    void put_to_history(Type value) {
        min = value;
        max = value;
        float average = value;
        for (uint8_t i = AMPLITUDES_HISTORY_LENGTH - 1; i > 0; i--) {
            data[i] = data[i-1];
            average += data[i];
            if (min > data[i]) min = data[i];
            if (max < data[i]) max = data[i];
        }
        avg = average / AMPLITUDES_HISTORY_LENGTH;
        delta = max - min;
        data[0] = value;
    }
};


class Analyzer {
public:
    ValueHistory<uint8_t> history[NUMBER_OF_SEGMENTS];

    int16_t *samples = fht_input;
    uint8_t *amplitudes = reinterpret_cast<uint8_t *>(fht_lin_out8);
    uint8_t smoothing_amplitudes[LENGTH_OF_SMOOTHED_AMPLITUDES] = {0};

    int16_t delay_reading_samples_us = DELAY_BETWEEN_READ_SAMPLES_US;
    int16_t sample_offset = DEFAULT_SAMPLE_OFFSET;

    uint16_t sampling_frequency = 0;
    int16_t signal_median_amplitude = 0;
    int16_t signal_amplitude = 0;
    BDSPTransmitter *transmitter = nullptr;
public:

    bool is_need_calibration = true;
    bool send_samples = false;

    void init() {

        sample_offset = DEFAULT_SAMPLE_OFFSET;
        pinMode(AR_MIC_PIN, OUTPUT);
        digitalWrite(AR_MIC_PIN, false);

        pinMode(GAIN_MIC_PIN, OUTPUT);
        digitalWrite(GAIN_MIC_PIN, false);

#if not defined(KOBA_CONF)
        analogReference(INTERNAL2V56);
#else
        analogReference(EXTERNAL);
#endif
        _SFR_BYTE(ADCSRA) &= ~_BV(ADPS0);

    };

    void set_transmitter(BDSPTransmitter &transmitter_) {
        transmitter = &transmitter_;
    }

    uint16_t calc_sample_offset(uint32_t start_read_time_us) {
        start_read_time_us = micros() - start_read_time_us;
        start_read_time_us = start_read_time_us / FHT_SAMPLES_N;
        return sampling_frequency = 1000000 / start_read_time_us;
    }

    void read_samples() {
        samples[0] = analogRead(AUDIO_PIN) - sample_offset;
        int16_t min_value = samples[0];
        int16_t max_value = samples[0];
        for (uint16_t i = 1; i < FHT_SAMPLES_N; i++) {
            delayMicroseconds(delay_reading_samples_us);
            samples[i] = analogRead(AUDIO_PIN) - sample_offset;
            if (samples[i] > max_value) max_value = samples[i];
            if (samples[i] < min_value) min_value = samples[i];
        }
        signal_amplitude = (max_value - min_value) >> 1;
        signal_median_amplitude = max_value - signal_amplitude;
        if (not send_samples or not transmitter or is_need_calibration) return;
        transmitter->send_data(1, reinterpret_cast<uint8_t*>(samples), FHT_SAMPLES_N * 2);
    };

    bool calibrate_audio_analyzer() {
        if (not is_need_calibration) return false;
        uint32_t timer = micros();
        read_samples();
        calc_sample_offset(timer);
        if (signal_amplitude > SILENCE_AMPLITUDE_LEVEL) {
            Serial.print(F("High amplitude input audio signal: "));
            Serial.print(signal_amplitude);
            Serial.println(F(". Need silence for calibration!"));
        } else if (abs(signal_median_amplitude) > 20) {
            sample_offset += signal_median_amplitude >> 1;
        } else if (signal_median_amplitude > 0) {
            sample_offset += 1;
        } else if (signal_median_amplitude < 0) {
            sample_offset -= 1;
        } else {
            Serial.print(F("Calibration successful!"));
            Serial.print(F("Sampling Freq: "));
            Serial.println(sampling_frequency);
            Serial.print(F("Samples offset: "));
            Serial.println(sample_offset);
            return is_need_calibration = false;
        }
        return true;
    }

    void analyze_the_signal() {

        read_samples();

        uint32_t timer = millis();
//        fht_window();
        custom_window();
        fht_reorder();
        fht_transform();
        fht_mag_lin8_no_asm();
        timer = millis() - timer;
        uint16_t sum = 0;
        for (int i = 0; i < LENGTH_OF_SMOOTHED_AMPLITUDES; i++) {
            if (i < 16) amplitudes[i] >>= 1;
            amplitudes[i] = (amplitudes[i] + smoothing_amplitudes[i]) >> 1;
            smoothing_amplitudes[i] = amplitudes[i];
        }
        for (int i = LENGTH_OF_SMOOTHED_AMPLITUDES; i < FHT_AMPLITUDES_N; ++i) {
            if (amplitudes[i] > 6) amplitudes[i] <<= 2;
        }

        for (int i = 0; i < FHT_AMPLITUDES_N; ++i) {
            amplitudes[i] <<= 2;
        }

        amplitudes[FHT_AMPLITUDES_N - 2] = freeMemory() / 10;
        amplitudes[FHT_AMPLITUDES_N - 1] = timer * 10;
        transmitter->send_data(2, amplitudes, FHT_AMPLITUDES_N);
    }

    AudioInformation calculate_colors() {
        AudioInformation info_;
        auto *info = reinterpret_cast<uint8_t*>(&info_);

        uint16_t ampl = 0;
        int16_t bright = 0;

        history[KICK].put_to_history(amplitudes[2]);
        for (int i = 0; i < 3; ++i) bright += history[KICK].data[i];
        bright =  constrain((bright) - 15, 0, 255);
        info[KICK] = bright;

        bright = 0;
        history[FREQ1].put_to_history(amplitudes[8]);
        for (int i = 0; i < 3; ++i) bright += history[FREQ1].data[i];
        bright =  constrain((bright) - 15, 0, 255);
        info[FREQ1] = bright;

        bright = 0;
        history[FREQ2].put_to_history(amplitudes[11]);
        for (int i = 0; i < 3; ++i) bright += history[FREQ2].data[i];
        bright = constrain((bright) - 15, 0, 255);
        info[FREQ2] = bright;

        bright = 0;
        history[FREQ3].put_to_history(amplitudes[14]);
        for (int i = 0; i < 3; ++i) bright += history[FREQ3].data[i];
        bright = constrain((bright) - 15, 0, 255);
        info[FREQ3] = bright;

        bright = 0;
        history[FREQ4].put_to_history(amplitudes[17]);
        for (int i = 0; i < 3; ++i) bright += history[FREQ4].data[i];
        bright = constrain((bright) - 15, 0, 255);
        info[FREQ4] = bright;


        bright = 0;
        ampl = 0;
        for (int i = 45; i < 55; ++i) {
            if (amplitudes[i] < 20 or amplitudes[i] < ampl) continue;
            ampl = amplitudes[i];
        };
        history[FREQ5].put_to_history(constrain(ampl, 0, 255));
        for (int i = 0; i < 5; ++i) bright += history[FREQ5].data[i];
        info[FREQ5] = constrain(bright, 0, 255);

        bright = 0;
        ampl = 0;
        for (int i = 60; i < 80; ++i) {
            if (amplitudes[i] < 30 or amplitudes[i] < ampl) continue;
            ampl = amplitudes[i];
        };
        history[HIGH_F].put_to_history(constrain(ampl << 1, 0, 255));
        for (int i = 0; i < 2; ++i) bright += history[HIGH_F].data[i];
        info[HIGH_F] = constrain(bright, 0, 255);

        bright = 0;
        ampl = 0;
        for (int i = 90; i < 126; ++i) {
            if (amplitudes[i] < 20 or amplitudes[i] < ampl) continue;
            ampl = amplitudes[i];
        };
        history[HI_HATS].put_to_history(constrain(ampl << 1, 0, 255));
        for (int i = 0; i < 2; ++i) bright += history[HI_HATS].data[i];
        info[HI_HATS] = constrain(bright, 0, 255);

        return info_;
    }

private:


    void custom_window() {
        samples[0] = 0;
        samples[FHT_SAMPLES_N - 1] = 0;
        for (int i = 1; i < NUMBER_OF_SAMPLES_WINDOW; ++i) {
            samples[i] *= i * (1.0/NUMBER_OF_SAMPLES_WINDOW);
            samples[FHT_SAMPLES_N - 1 - i] *= i * (1.0/NUMBER_OF_SAMPLES_WINDOW);
        }
    }

};

#endif //ARD_AUDIO_ANALYZER_H
