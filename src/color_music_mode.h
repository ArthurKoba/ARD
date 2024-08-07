#ifndef ARD_COLOR_MUSIC_MODE_H
#define ARD_COLOR_MUSIC_MODE_H

#include "color_modes.h"


uint8_t CRT_correction(uint8_t power, float calibrate = 1.0) {
    return power > 0 ? (1 + uint16_t((calibrate * power * power + 255))) >> 8 : 0;
}

uint8_t calc_bright(uint16_t temp) {
    return CRT_correction(constrain(temp, 0, 255));
}

class ColorMusicMode : public AbstractColorMode {
public:
    ColorMusicMode() {
        show_delay_ms = COLOR_MUSIC_DEF_DELAY_MS;
    }

    bool handle_input_event(input_event_t input) override {return false;}

private:
    void _calculate(LedController &controller) const override {

//        if (ctx.analyzer.need_calibration) {
//                calibrate_audio_analyzer(ctx.analyzer);
//                if (ctx.analyzer.need_calibration) {
//                    blink_mode(ctx);
//                } else {
//                    fill_leds(ctx, CRGB::Black);
//                }
//            } else {
//                readSamples(ctx.analyzer);
////                send_samples(ctx);
//                if (ctx.analyzer.signal_amplitude < SILENCE_AMPLITUDE_LEVEL) {
//                    for (int i = 0; i < FHT_N / 2; ++i) ctx.analyzer.amplitudes[i] = 0;
//                } else {
//                    fht_process(ctx.analyzer);
//                }
////                send_amplitudes(ctx);
//                color_music(ctx);
//            }

//        uint8_t segment_id = 0;
//        uint16_t ampl; int16_t bright;
//
//        if (ctx.analyzer.signal_amplitude < SILENCE_AMPLITUDE_LEVEL) ctx.analyzer.signal_amplitude = 0;
//        put_amplitude_to_history(constrain(ctx.analyzer.signal_amplitude >> 1, 0, 255), ctx.analyzer.history[7]);
//        bright = int16_t(ctx.analyzer.history[7].average);
//        bright = calc_bright(bright);
//        write_color_to_segment(7, ctx, CRGB(bright, bright, bright));
//
//
//        ampl = ctx.analyzer.amplitudes[1];
//        put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
//        bright = 0;
//        for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
//        bright =  constrain((bright >> 1) - 30, 0, 255);
//        write_color_to_segment(segment_id++, ctx, CRGB(calc_bright(bright), 0, 0));
//
//
//        ampl = constrain(ctx.analyzer.amplitudes[3], 0, 255);
//        put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
//        bright = 0;
//        for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
//        bright =  calc_bright(constrain((bright >> 1) - 40, 0, 255));
//        write_color_to_segment(segment_id++, ctx, CRGB(bright, bright >> 1, 0));
//
//        ampl = constrain(ctx.analyzer.amplitudes[8] + ctx.analyzer.amplitudes[9], 0, 255);
//        put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
//        bright = 0;
//        for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
//        bright = calc_bright(constrain((bright >> 1) - 40, 0, 255));
//        write_color_to_segment(segment_id++, ctx, CRGB(0, bright, bright >> 1));
//
//        ampl = constrain(ctx.analyzer.amplitudes[12] + ctx.analyzer.amplitudes[13], 0, 255);
//        put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
//        bright = 0;
//        for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
//        bright =  calc_bright(constrain((bright >> 1) - 40, 0, 255));
//        write_color_to_segment(segment_id++, ctx, CRGB(bright >> 2, bright, bright >> 2));
//
//        ampl = constrain(ctx.analyzer.amplitudes[15] + ctx.analyzer.amplitudes[16], 0, 255);
//        put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
//        bright = 0;
//        for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
//        bright =  calc_bright(constrain((bright >> 1) - 40, 0, 255));
//        write_color_to_segment(segment_id++, ctx, CRGB(bright >> 1, bright, 0));
//
//
//        ampl = 0;
//        for (int i = 45; i < 60; ++i) {
//            if (ctx.analyzer.amplitudes[i] < 32) continue;
//            ampl += ctx.analyzer.amplitudes[i];
//        };
//        put_amplitude_to_history(constrain(ampl, 0, 255), ctx.analyzer.history[segment_id]);
//        bright = ctx.analyzer.history[segment_id].data[0];
//        bright =  calc_bright(constrain(bright - 20, 0, 255));
//        write_color_to_segment(segment_id++, ctx, CRGB(0, bright >> 2, bright));
//
//
//        ampl = 0;
//        for (int i = 100; i < 128; ++i) {
//            if (ctx.analyzer.amplitudes[i] < 20) continue;
//            ampl += ctx.analyzer.amplitudes[i];
//        };
//        put_amplitude_to_history(ampl << 1, ctx.analyzer.history[segment_id]);
//        bright = ctx.analyzer.history[segment_id].data[0];
//        bright =  calc_bright(constrain(bright - 10, 0, 255));
//        write_color_to_segment(segment_id++, ctx, CRGB(bright, 0, bright));

    }
};


#endif //ARD_COLOR_MUSIC_MODE_H
