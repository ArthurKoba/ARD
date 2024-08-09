#ifndef ARD_COLOR_MUSIC_MODE_H
#define ARD_COLOR_MUSIC_MODE_H

#include "color_modes.h"
#include "led_controller.h"
#include "audio_analyzer.h"


class ColorMusicMode : public AbstractColorMode  {
public:
    Analyzer &analyzer;
    BlinkMode blink_mode;

    explicit ColorMusicMode(Analyzer &analyzer) : analyzer(analyzer) {
        show_delay_ms = COLOR_MUSIC_DEF_DELAY_MS;
    }


    bool handle_input_event(input_event_t input) override {return false;}

private:
    void _calculate(LedController &controller) override {
        if (analyzer.calibrate_audio_analyzer()) {
            blink_mode.calculate(controller);
            return;
        };
        analyzer.analyze_the_signal();
        AudioInformation info = analyzer.calculate_colors();

        auto *info_ = reinterpret_cast<uint8_t*>(&info);
        uint8_t max_index = 1;
        for (int i = 2; i < 6; ++i) {
            if (info_[max_index] > info_[i]) max_index = i;
        }
        for (int i = 1; i < 6; ++i) {
            if (i == max_index) info_[i] <<= 2;
            else info_[i] >>= 4;
        }

        controller.set_color_to_segment(0,  CRGB_f(info.kick, 0, 0));
        controller.set_color_to_segment(1,  CRGB(0, info.freq1, 0));
        controller.set_color_to_segment(2,  CRGB(info.freq2 >> 1, 0, info.freq2));
        controller.set_color_to_segment(3,  CRGB(info.freq3, 0, info.freq3 >> 1));
        controller.set_color_to_segment(4,  CRGB(info.freq4 >> 1, info.freq4, 0));
        controller.set_color_to_segment(5,  CRGB(0, info.freq5, info.freq5 >> 1));
        controller.set_color_to_segment(6,  CRGB(0, 0, info.high_f));
        controller.set_color_to_segment(7,  CRGB(info.hi_hats, 0, info.hi_hats));

    }
};


#endif //ARD_COLOR_MUSIC_MODE_H
