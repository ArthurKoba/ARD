#ifndef ARD_AUDIO_ANALYZER_H
#define ARD_AUDIO_ANALYZER_H

#include "config.h"
#include "stdint.h"
#include <AvrFHT.h>
#include <fht_reorder.h>
#include <fht_window.h>
#include <fht_mag_lin8_no_asm.h>
#include <BDSP.h>

enum FreqIndex {
    LOW_FREQ, FREQ_G, FREQ_C, FREQ_D, FREQ_E, FREQ_H, FREQ_Cd, FREQ_Dd,
    FREQ_F, FREQ_Fd, FREQ_Gd, FREQ_A, FREQ_B, HIGH_F, HI_HATS, NUMBER_OF_FREQ_INDEXES
};

struct FreqInformation final {
    uint8_t low = 0;
    uint8_t freq_g = 0;
    uint8_t freq_c = 0;
    uint8_t freq_d = 0;
    uint8_t freq_e = 0;
    uint8_t freq_h = 0;
    uint8_t freq_cd = 0;
    uint8_t freq_dd = 0;
    uint8_t freq_f = 0;
    uint8_t freq_fd = 0;
    uint8_t freq_gd = 0;
    uint8_t freq_a = 0;
    uint8_t freq_b = 0;
    uint8_t high = 0;
    uint8_t hi_hats = 0;
};


template<typename Type>
class ValueHistory final {
public:
    Type data[AMPLITUDES_HISTORY_LENGTH] = {0};

    ValueHistory() = default;

    void put_to_history(Type value) {
        float average = value;
        for (uint8_t i = AMPLITUDES_HISTORY_LENGTH - 1; i > 0; i--) {
            data[i] = data[i-1];
            average += data[i];
        }
        data[0] = value;
    }
    Type get_min() {
        uint8_t min = data[0];
        for (int i = 1; i < AMPLITUDES_HISTORY_LENGTH; ++i) if (data[i] < min) min = data[i];
        return min;
    }

    Type get_max() {
        uint8_t max = data[0];
        for (int i = 1; i < AMPLITUDES_HISTORY_LENGTH; ++i) if (data[i] > max) max = data[i];
        return max;
    }

    Type get_avg() {
        uint64_t avg = 0;
        for (auto & i : data) avg += i;
        return avg / AMPLITUDES_HISTORY_LENGTH;
    }

    Type get_delta() {
        uint8_t min = data[0];
        uint8_t max = data[0];
        for (int i = 1; i < AMPLITUDES_HISTORY_LENGTH; ++i) {
            if (data[i] < min) min - data[i];
            if (data[i] > max) max - data[i];
        };
        return max - min;
    }
};


class Analyzer final {
public:
    ValueHistory<uint8_t> history[NUMBER_OF_FREQ_INDEXES];

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
    bool is_send_samples = false;
    bool is_send_amplitudes = false;
    bool is_send_freq = false;

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
        if (not is_send_samples or not transmitter or is_need_calibration) return;
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
            analyze_the_signal();
            Serial.print(F("Calibration successful!"));
            Serial.print(F("Sampling Freq: "));
            Serial.println(sampling_frequency);
            Serial.print(F("Samples offset: "));
            Serial.println(sample_offset);
            return is_need_calibration = false;
        }
        return true;
    }

    void amplitudes_transform() {
        fht_window();
        fht_reorder();
        fht_transform();
        fht_mag_lin8_no_asm();
        for (int i = 0; i < LENGTH_OF_SMOOTHED_AMPLITUDES; i++) {
            if (i < 16) amplitudes[i] >>= 1;
            amplitudes[i] = (amplitudes[i] + smoothing_amplitudes[i]) >> 1;
            smoothing_amplitudes[i] = amplitudes[i];
        }
    }

    void analyze_the_signal() {
        read_samples();
        amplitudes_transform();

        for (int i = 0; i < LENGTH_OF_SMOOTHED_AMPLITUDES; ++i) {
            amplitudes[i] <<= (i > 4) ? 3 : 2;
        }

        for (int i = LENGTH_OF_SMOOTHED_AMPLITUDES; i < FHT_AMPLITUDES_N; ++i) {
            if (amplitudes[i] > 5) amplitudes[i] = constrain(amplitudes[i] << 3, 0, 255);
        }

//        amplitudes[FHT_AMPLITUDES_N - 2] = freeMemory() / 10;
//        amplitudes[FHT_AMPLITUDES_N - 1] = timer * 10;
        if (is_send_amplitudes and transmitter) {
            transmitter->send_data(2, amplitudes, FHT_AMPLITUDES_N);
        }
    }

    FreqInformation calculate_colors() {
        FreqInformation info;

        info.low = calc_magnitude(LOW_FREQ, (amplitudes[1] + amplitudes[2]) >> 1, 30, COLOR_MUSIC_BRIGHT_MUL, 2);

        info.freq_g = calc_magnitude(FREQ_G, amplitudes[6], 10, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_c = calc_magnitude(FREQ_C, amplitudes[8], 10, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_d = calc_magnitude(FREQ_D, amplitudes[9], 10, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_e = calc_magnitude(FREQ_E, amplitudes[10], 10, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_h = calc_magnitude(FREQ_H, amplitudes[15], 10, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_cd = calc_magnitude(FREQ_Cd, amplitudes[17], 30, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_dd = calc_magnitude(FREQ_Dd, amplitudes[19], 30, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_f = calc_magnitude(FREQ_F, amplitudes[21], 30, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_fd = calc_magnitude(FREQ_Fd, amplitudes[23], 30, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_gd = calc_magnitude(FREQ_Gd, amplitudes[25], 30, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_a = calc_magnitude(FREQ_A, amplitudes[27], 30, COLOR_MUSIC_BRIGHT_MUL, 4);
        info.freq_b = calc_magnitude(FREQ_B, amplitudes[29], 30, COLOR_MUSIC_BRIGHT_MUL, 4);

        uint16_t ampl = 0;
        for (int i = LENGTH_OF_SMOOTHED_AMPLITUDES; i < 80; ++i) if (amplitudes[i] > ampl) ampl = amplitudes[i];
        info.high = (ampl <= 30) ? 0 : constrain(calc_magnitude(HIGH_F, ampl, 0, 1, 2) << (COLOR_MUSIC_BRIGHT_MUL + 1), 0, 255);

        ampl = 0;
        for (int i = 90; i < 126; ++i) if (amplitudes[i] > ampl) ampl = amplitudes[i];
        info.hi_hats = (ampl <= 30) ? 0 : constrain(calc_magnitude(HI_HATS, ampl, 0, 1, 2) << (COLOR_MUSIC_BRIGHT_MUL + 1), 0, 255);

        if (is_send_freq and not is_send_amplitudes and transmitter) {
            transmitter->send_data(2, reinterpret_cast<uint8_t*>(&info), NUMBER_OF_FREQ_INDEXES);
        }
        return info;
    }

private:
    uint8_t calc_magnitude(FreqIndex f_i, uint8_t amplitude, uint8_t sub, uint8_t mv, uint8_t smooth) {
        int16_t bright = 0;
        if (smooth > AMPLITUDES_HISTORY_LENGTH) smooth = AMPLITUDES_HISTORY_LENGTH;
        history[f_i].put_to_history(constrain((amplitude - sub) << mv, 0, 255));
        for (int i = 0; i < smooth; ++i) bright += history[f_i].data[i];
        return constrain(bright - history[f_i].get_min(), 0, 255);
    }

    uint16_t calc_sample_offset(uint32_t start_read_time_us) {
        start_read_time_us = micros() - start_read_time_us;
        start_read_time_us = start_read_time_us / FHT_SAMPLES_N;
        return sampling_frequency = 1000000 / start_read_time_us;
    }
};

#endif //ARD_AUDIO_ANALYZER_H
