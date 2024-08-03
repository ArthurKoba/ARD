#ifndef ARD_COLOR_MODES_H
#define ARD_COLOR_MODES_H

#include "types.h"
#include "utils.h"
#include "pixeltypes.h"
#include "audio_analyzer.h"


void set_mode(App &ctx, ColorMode mode) {
    ctx.mode = mode;
}

void exec_increase_or_decrease(App &ctx, int8_t offset) {
    switch (ctx.mode) {
        case WHITE_MODE:
            ctx.cfg_modes.white.bright = constrain(
                    int(ctx.cfg_modes.white.bright) + offset * 10,
                    MIN_BRIGHT_WHITE_MODE,
                    MAX_BRIGHT_WHITE_MODE
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
        case MODE_3:
            break;
        case RAINBOW_MODE:
            ctx.cfg_modes.rainbow.delay_ms = constrain(
                    int(ctx.cfg_modes.rainbow.delay_ms) + offset,
                    RAINBOW_MODE_MIN_DELAY_MS,
                    RAINBOW_MODE_MAX_DELAY_MS
            );
            break;
        case COLOR_MUSIC:
            if (offset > 0) ctx.analyzer.need_calibration = true;
            break;
    }
}

void increase_mode(App &ctx) {
    ctx.mode = static_cast<ColorMode>(static_cast<uint8_t>(ctx.mode) + 1);
    if (ctx.mode > RAINBOW_MODE) ctx.mode = WHITE_MODE;
}

void white_mode(App &ctx) {
    exit_timer(ctx.cfg_modes.white.delay_ms);

    fill_leds(ctx, CRGB(
            ctx.cfg_modes.white.bright,
            ctx.cfg_modes.white.bright,
            ctx.cfg_modes.white.bright
    ));
}

void creative_mode(App &ctx) {
    // 7 сегментов меняют цвет, 255 оттенков, кроме последнего сегмента 8-го, состоящего из центральной точки.
    // Она всегда горит белым цветом, у неё просто меняется яркость если пытаешься менять цвет в этом режиме.

    exit_timer(ctx.cfg_modes.creative.delay_ms);

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

void fill_white_mode(App &ctx) {
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



#define LOW_START_AMPLITUDE_INDEX 1
#define LOW_END_AMPLITUDE_INDEX 1
#define LOW_MULTIPLICATION_AMPLITUDE 2

#define MIDDLE_START_AMPLITUDE_INDEX 8
#define MIDDLE_END_AMPLITUDE_INDEX 32
#define MIDDLE_MULTIPLICATION_AMPLITUDE 0.1

#define HIGH_START_AMPLITUDE_INDEX 119
#define HIGH_END_AMPLITUDE_INDEX 127
#define HIGH_MULTIPLICATION_AMPLITUDE 0.7

void color_music(App &ctx) {

    exit_timer(ctx.cfg_modes.color_music_delay_ms);

    uint8_t segment_id = 0;
    uint16_t ampl = ctx.analyzer.amplitudes[segment_id + 1];
    put_amplitude_to_history(ampl << 1, ctx.analyzer.history[segment_id]);
    int16_t bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].minimum * 1.2);
    bright = constrain(bright * 1.5, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(bright, 0, 0));

    segment_id++;
    ampl = 0;
    for (int i = 2; i < 5; ++i) ampl += ctx.analyzer.amplitudes[i];
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].average);
    bright = constrain(bright * 1.5, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(bright, bright >> 1, 0));

    segment_id++;
    ampl = 0;
    for (int i = 7; i < 10; ++i) ampl += ctx.analyzer.amplitudes[i];
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].average * 1.4);
    bright = constrain(bright, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(bright, bright, 0));

    segment_id++;
    ampl = 0;
    for (int i = 10; i < 13; ++i) ampl += ctx.analyzer.amplitudes[i];
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].average * 1.2);
    bright = constrain(bright * 1.5, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(bright >> 1, bright, 0));

    segment_id++;
    ampl = 0;
    for (int i = 13; i < 16; ++i) ampl += ctx.analyzer.amplitudes[i];
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].minimum * 1.4);
    bright = constrain(bright, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(0, bright, bright >> 1));

    segment_id++;
    ampl = 0;
    for (int i = 16; i < 19; ++i) {
        if (ctx.analyzer.amplitudes[i] < ctx.analyzer.history[segment_id].average) continue;
        ampl += ctx.analyzer.amplitudes[i];
    }
    put_amplitude_to_history(ampl >> 1, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].average * 1.4);
    bright = constrain(bright, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(0, bright >> 1, bright));


    segment_id++;
    ampl = 0;
    for (int i = 50; i < 55; ++i) {
        if (ctx.analyzer.amplitudes[i] < ctx.analyzer.history[segment_id].minimum) continue;
        ampl += ctx.analyzer.amplitudes[i];
    }
    put_amplitude_to_history(ampl, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].average * 1.2);
    bright = constrain(bright * 1.5, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(0, 0, bright));


    segment_id++;
    ampl = 0;
    for (int i = 100; i < 107; ++i) {
        if (ctx.analyzer.amplitudes[i] < ctx.analyzer.history[segment_id].minimum) continue;
        ampl += ctx.analyzer.amplitudes[i];
    }
    put_amplitude_to_history(ampl >> 1, ctx.analyzer.history[segment_id]);
    bright = ctx.analyzer.history[segment_id].data[0] - uint8_t(ctx.analyzer.history[segment_id].average * 1.2);
    bright = constrain(bright * 1.5, 0, 255);
    write_color_to_segment(segment_id, ctx, CRGB(bright >> 1, 0, bright));

}

void mode_3(App &ctx) {

    exit_timer(ctx.cfg_modes.mode_3_delay_ms);

    write_color_to_segment(0, ctx, CRGB(
            255,
            0,
            0
    ));
    write_color_to_segment(1, ctx, CRGB(
            0,
            255,
            0
    ));
    write_color_to_segment(2, ctx, CRGB(
            0,
            0,
            255
    ));
    write_color_to_segment(3, ctx, CRGB(
            255,
            0,
            0
    ));
    write_color_to_segment(4, ctx, CRGB(
            0,
            255,
            0
    ));
    write_color_to_segment(5, ctx, CRGB(
            0,
            0,
            255
    ));
    write_color_to_segment(6, ctx, CRGB(
            255,
            0,
            0
    ));
    write_color_to_segment(7, ctx, CRGB(
            0,
            255,
            0
    ));
}

void rainbow_mode(App &ctx) {
    // Радуга
    static uint8_t current_index = 0;

    exit_timer(ctx.cfg_modes.rainbow.delay_ms);

    for (int segment_id = 0; segment_id < 8; ++segment_id) {
        uint8_t hue = current_index + ctx.cfg_modes.rainbow.hue_offset * segment_id;
        write_color_to_segment(segment_id,ctx,CHSV(hue,255,255));
    }
    current_index++;
}

void blink_mode(App &ctx) {
    static bool is_white = true;

    exit_timer(200);

    fill_leds(ctx, is_white? CRGB::White : CRGB::Black);
    is_white = not is_white;
}

#endif //ARD_COLOR_MODES_H
