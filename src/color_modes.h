#ifndef ARD_COLOR_MODES_H
#define ARD_COLOR_MODES_H

#include "types.h"
#include "utils.h"
#include "pixeltypes.h"
#include "audio_analyzer.h"
#include "bdsp_sender.h"


void set_mode(Context &ctx, ColorMode mode) {
    ctx.cfg_modes.current_mode = mode;
}

void exec_increase_or_decrease(Context &ctx, int8_t offset) {
    switch (ctx.cfg_modes.current_mode) {
        case WHITE_MODE:
            ctx.cfg_modes.white_mode_bright = constrain(
                int(ctx.cfg_modes.white_mode_bright) + offset * 10,
                WHITE_MODE_MIN_BRIGHT,
                WHITE_MODE_MAX_BRIGHT
            );
            break;
        case CREATIVE_MODE:
            if (offset > 0) {
                ctx.cfg_modes.creative.current_segment++;
                if (ctx.cfg_modes.creative.current_segment > 7) {
                    ctx.cfg_modes.creative.current_segment = 0;
                }
            } else if (offset < 0) {
                ctx.cfg_modes.creative.segment_hues[ctx.cfg_modes.creative.current_segment] += 10;
            }
            break;
        case FILL_WHITE_MODE:
            ctx.cfg_modes.fill_white.delay_ms = constrain(
                int(ctx.cfg_modes.fill_white.delay_ms) + offset,
                FILL_WHITE_MODE_MIN_DELAY_MS,
                FILL_WHITE_MODE_MAX_DELAY_MS
            );
            break;
        case MOVE_TO_CENTER_MODE:
            ctx.cfg_modes.move_to_center_delay_ms = constrain(
                int(ctx.cfg_modes.move_to_center_delay_ms) + offset * 100,
                TO_CENTER_MODE_MIN_DELAY_MS,
                TO_CENTER_MODE_MAX_DELAY_MS
            );
            break;
        case FADE_MODE:
            ctx.cfg_modes.fade_mode_delay_ms = constrain(
                    int(ctx.cfg_modes.fade_mode_delay_ms) + offset,
                    FADE_MODE_MIN_DELAY_MS,
                    FADE_MODE_MAX_DELAY_MS
            );
            break;
        case RAINBOW_MODE:
            ctx.cfg_modes.rainbow_mode_delay_ms = constrain(
                    int(ctx.cfg_modes.rainbow_mode_delay_ms) + offset,
                    RAINBOW_MODE_MIN_DELAY_MS,
                    RAINBOW_MODE_MAX_DELAY_MS
            );
            break;
        case RAINBOW2_MODE:
            ctx.cfg_modes.rainbow_mode_delay_ms = constrain(
                    int(ctx.cfg_modes.rainbow2_mode_delay_ms) + offset,
                    RAINBOW2_MODE_MIN_DELAY_MS,
                    RAINBOW2_MODE_MAX_DELAY_MS
            );
            break;
        case RAINBOW3_MODE:
            ctx.cfg_modes.rainbow_mode_delay_ms = constrain(
                    int(ctx.cfg_modes.rainbow3_mode_delay_ms) + offset,
                    RAINBOW3_MODE_MIN_DELAY_MS,
                    RAINBOW3_MODE_MAX_DELAY_MS
            );
            break;
        case FIRE_MODE:
            ctx.cfg_modes.fire_mode_delay_ms = constrain(
                    int(ctx.cfg_modes.fire_mode_delay_ms) + offset,
                    FIRE_MODE_MIN_DELAY_MS,
                    FIRE_MODE_MAX_DELAY_MS
            );
            break;
        case COLOR_MUSIC:
            if (offset > 0) ctx.analyzer.need_calibration = true;
            break;
    }
    if (ctx.cfg_modes.current_mode == COLOR_MUSIC) return;
    ctx.need_update_saves = true;
}



void white_mode(Context &ctx) {
    exit_timer(WHITE_MODE_DEF_DELAY_MS);

    fill_leds(ctx, CRGB(
        ctx.cfg_modes.white_mode_bright,
        ctx.cfg_modes.white_mode_bright,
        ctx.cfg_modes.white_mode_bright
    ));
}

void creative_mode(Context &ctx) {
    // 7 сегментов меняют цвет, 255 оттенков, кроме последнего сегмента 8-го, состоящего из центральной точки.
    // Она всегда горит белым цветом, у неё просто меняется яркость если пытаешься менять цвет в этом режиме.

    exit_timer(CREATIVE_MODE_DEF_DELAY_MS);

    for (int i = 0; i < 7; ++i) {
        write_color_to_segment(i, ctx, CHSV(
                ctx.cfg_modes.creative.segment_hues[i],
                255, 255
        ));
    }
    write_color_to_segment(7, ctx, CRGB(
            ctx.cfg_modes.creative.segment_hues[7],
            ctx.cfg_modes.creative.segment_hues[7],
            ctx.cfg_modes.creative.segment_hues[7]
    ));


}

void fill_white_mode(Context &ctx) {
    //  Поочередно зажигаются диоды начиная от внешнего периметра к центральной точке.
    //  Потом немого поработали включенными все диоды, затухают и цикл начинается заново.
    //  Кнопками меняется скорость работы режима + -.
    static uint16_t fullness = 0;
    static bool is_fill = true;
    static uint16_t count_waits = 0;

    exit_timer(ctx.cfg_modes.fill_white.delay_ms);

    if (is_fill and fullness not_eq NUM_LEDS) {
        fullness++;
        fill_leds(ctx, CRGB::White, 0, fullness);
        fill_leds(ctx, CRGB::Black, fullness + 1);
    } else if (is_fill and fullness == NUM_LEDS and count_waits < ctx.cfg_modes.fill_white.count_wait) {
        fill_leds(ctx, CRGB::White);
        count_waits++;
    } else if (is_fill and fullness == NUM_LEDS and count_waits >= ctx.cfg_modes.fill_white.count_wait) {
        count_waits = 0;
        is_fill = false;
    } else if (fullness) {
        fullness--;
        fill_leds(ctx, CRGB(fullness, fullness, fullness));
    } else if (count_waits < ctx.cfg_modes.fill_white.count_wait){
        fill_leds(ctx, CRGB::Black);
        count_waits++;
    } else {
        count_waits = 0;
        fullness = 0;
        is_fill = true;
    }
}

void move_to_center_mode(Context &ctx) {

    static uint8_t fullness = 0;
    static bool is_fill = true;

    exit_timer(ctx.cfg_modes.move_to_center_delay_ms);

    fullness++;
    for (uint8_t i = 0; i < fullness; ++i) {
        write_color_to_segment(i, ctx, is_fill ? CRGB::White : CRGB::Black);
    }

    for (uint8_t i = fullness; i < 8; ++i) {
        write_color_to_segment(i, ctx, is_fill ? CRGB::Black : CRGB::White);
    }

    if (fullness < 8) return;
    fullness = 0;
    is_fill = not is_fill;
}

void fade_mode(Context &ctx) {

    static uint8_t bright = 254;
    static bool is_fade = false;

    exit_timer(ctx.cfg_modes.fade_mode_delay_ms);

    bright += is_fade ? - 1 : 1;
    if (bright < 1) is_fade = false;
    if (bright > 254) is_fade = true;

    fill_leds(ctx, CRGB(bright, bright, bright));
}

void rainbow_mode(Context &ctx) {
    // Радуга
    static uint8_t current_index = 0;

    exit_timer(ctx.cfg_modes.rainbow_mode_delay_ms);

    for (int segment_id = 0; segment_id < 8; ++segment_id) {
        uint8_t hue = current_index + RAINBOW_MODE_HUE_OFFSET * segment_id;
        write_color_to_segment(segment_id,ctx,CHSV(hue,255,255));
    }
    current_index++;
}

void rainbow2_mode(Context &ctx) {
    // Радуга
    static uint8_t current_index = 0;

    exit_timer(ctx.cfg_modes.rainbow2_mode_delay_ms);

    for (int i = 0; i < NUM_LEDS; ++i) {
        ctx.leds[i] = CHSV(current_index + i * RAINBOW2_MODE_HUE_MUL, 255, 255);
    }
    current_index++;
}

void rainbow3_mode(Context &ctx) {
    // Радуга
    static uint8_t current_index = 0;
    static uint8_t current_index_inverse = 127;

    exit_timer(ctx.cfg_modes.rainbow3_mode_delay_ms);

    for (int segment_id = 0; segment_id < 8; ++segment_id) {
        for (uint16_t i = ctx.segments[segment_id].start; i < ctx.segments[segment_id].end + 1; ++i) {
            uint8_t hue = (segment_id % 2 ? current_index : current_index_inverse) +
                    i * RAINBOW3_MODE_HUE_MUL + segment_id * RAINBOW3_MODE_SEGMENTS_OFFSET;
            ctx.leds[i] = CHSV(hue,255, 255);
        }
    }

    current_index++;
    current_index_inverse--;
}

void fire_mode(Context &ctx) {

    static uint16_t counter = 0;

    exit_timer(ctx.cfg_modes.fire_mode_delay_ms);

    for(int i = 0; i < NUM_LEDS; i++) {
        uint8_t value = inoise8(i * FIRE_MODE_FIRE_STEP, counter);
        ctx.leds[i] = CHSV(
                FIRE_MODE_HUE_START + map(value, 0, 255, 0, FIRE_MODE_HUE_GAP),
            constrain(map(value, 0, 255, FIRE_MODE_MAX_SAT, FIRE_MODE_MIN_SAT), 0, 255),
            constrain(map(value, 0, 255, FIRE_MODE_MIN_BRIGHT, FIRE_MODE_MAX_BRIGHT), 0, 255)
        );
    }
    counter += 20;
}

uint8_t getBrightCRT(uint8_t power, float calibrate = 1.0) {
    return power > 0 ? (1 + uint16_t((calibrate * power * power + 255))) >> 8 : 0;
}

uint8_t calc_bright(uint16_t temp) {
    return getBrightCRT(constrain(temp, 0, 255));
}

void color_music(Context &ctx) {

    exit_timer(ctx.cfg_modes.color_music_delay_ms);

    uint8_t segment_id = 0;
    uint16_t ampl; int16_t bright;

    if (ctx.analyzer.signal_amplitude < SILENCE_AMPLITUDE_LEVEL) ctx.analyzer.signal_amplitude = 0;
    put_amplitude_to_history(constrain(ctx.analyzer.signal_amplitude >> 1, 0, 255), ctx.analyzer.history[7]);
    bright = int16_t(ctx.analyzer.history[7].average);
    bright = calc_bright(bright);
    write_color_to_segment(7, ctx, CRGB(bright, bright, bright));


    ampl = ctx.analyzer.amplitudes[1];
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = 0;
    for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
    bright =  constrain((bright >> 1) - 30, 0, 255);
    write_color_to_segment(segment_id++, ctx, CRGB(calc_bright(bright), 0, 0));


    ampl = constrain(ctx.analyzer.amplitudes[3], 0, 255);
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = 0;
    for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
    bright =  calc_bright(constrain((bright >> 1) - 40, 0, 255));
    write_color_to_segment(segment_id++, ctx, CRGB(bright, bright >> 1, 0));

    ampl = constrain(ctx.analyzer.amplitudes[8] + ctx.analyzer.amplitudes[9], 0, 255);
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = 0;
    for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
    bright = calc_bright(constrain((bright >> 1) - 40, 0, 255));
    write_color_to_segment(segment_id++, ctx, CRGB(0, bright, bright >> 1));

    ampl = constrain(ctx.analyzer.amplitudes[12] + ctx.analyzer.amplitudes[13], 0, 255);
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = 0;
    for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
    bright =  calc_bright(constrain((bright >> 1) - 40, 0, 255));
    write_color_to_segment(segment_id++, ctx, CRGB(bright >> 2, bright, bright >> 2));

    ampl = constrain(ctx.analyzer.amplitudes[15] + ctx.analyzer.amplitudes[16], 0, 255);
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = 0;
    for (int i = 0; i < 3; ++i) bright += ctx.analyzer.history[segment_id].data[i];
    bright =  calc_bright(constrain((bright >> 1) - 40, 0, 255));
    write_color_to_segment(segment_id++, ctx, CRGB(bright >> 1, bright, 0));


    ampl = 0;
    for (int i = 45; i < 60; ++i) {
        if (ctx.analyzer.amplitudes[i] < 32) continue;
        ampl += ctx.analyzer.amplitudes[i];
    };
    put_amplitude_to_history(constrain(ampl, 0, 255), ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0];
    bright =  calc_bright(constrain(bright - 20, 0, 255));
    write_color_to_segment(segment_id++, ctx, CRGB(0, bright >> 2, bright));


    ampl = 0;
    for (int i = 100; i < 128; ++i) {
        if (ctx.analyzer.amplitudes[i] < 20) continue;
        ampl += ctx.analyzer.amplitudes[i];
    };
    put_amplitude_to_history(ampl << 1, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0];
    bright =  calc_bright(constrain(bright - 10, 0, 255));
    write_color_to_segment(segment_id++, ctx, CRGB(bright, 0, bright));

}



void blink_mode(Context &ctx) {
    static bool is_white = true;

    exit_timer(200);

    fill_leds(ctx, is_white? CRGB::White : CRGB::Black);
    is_white = not is_white;
}

void show_color_modes(Context &ctx) {
    switch (ctx.cfg_modes.current_mode) {
        case WHITE_MODE:            white_mode(ctx);            break;
        case CREATIVE_MODE:         creative_mode(ctx);         break;
        case FILL_WHITE_MODE:       fill_white_mode(ctx);       break;
        case MOVE_TO_CENTER_MODE:   move_to_center_mode(ctx);   break;
        case FADE_MODE:             fade_mode(ctx);             break;
        case RAINBOW_MODE:          rainbow_mode(ctx);          break;
        case RAINBOW2_MODE:         rainbow2_mode(ctx);         break;
        case RAINBOW3_MODE:         rainbow3_mode(ctx);         break;
        case FIRE_MODE:             fire_mode(ctx);             break;
        case COLOR_MUSIC:
            if (ctx.analyzer.need_calibration) {
                calibrate_audio_analyzer(ctx.analyzer);
                if (ctx.analyzer.need_calibration) {
                    blink_mode(ctx);
                } else {
                    fill_leds(ctx, CRGB::Black);
                }
            } else {
                readSamples(ctx.analyzer);
//                send_samples(ctx);
                if (ctx.analyzer.signal_amplitude < SILENCE_AMPLITUDE_LEVEL) {
                    for (int i = 0; i < FHT_N / 2; ++i) ctx.analyzer.amplitudes[i] = 0;
                } else {
                    fht_process(ctx.analyzer);
                }
//                send_amplitudes(ctx);
                color_music(ctx);
            }
            break;


    }
    FastLED.show();
}

#endif //ARD_COLOR_MODES_H
