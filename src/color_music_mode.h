#ifndef ARD_COLOR_MUSIC_MODE_H
#define ARD_COLOR_MUSIC_MODE_H

#include "color_modes.h"
#include "led_controller.h"
#include "audio_analyzer.h"


class ColorMusicMode final: public AbstractColorMode  {
public:
    Analyzer &analyzer;
    BlinkMode blink_mode;

    explicit ColorMusicMode(EEPROMMemory &mem, Analyzer &analyzer) : AbstractColorMode(mem), analyzer(analyzer), blink_mode(mem) {}

    void handle_input_event(input_event_t input) override {}

private:
    uint16_t _get_show_delay() const override { return COLOR_MUSIC_DEF_DELAY_MS; }

    void _calculate(LedController &controller) override {
        if (analyzer.calibrate_audio_analyzer()) {
            blink_mode.calculate(controller);
            return;
        }

        analyzer.analyze_the_signal();
        FreqInformation info = analyzer.calculate_colors();

        auto *info_ = reinterpret_cast<uint8_t*>(&info);
        uint8_t high = info.hi_hats > info.high ? info.hi_hats : info.high;
        for (int i = 0; i < NUMBER_OF_FREQ_INDEXES - 2; ++i) info_[i] = CRT(info_[i]);

        controller.set_color_to_detail_segment(0,  CHSV(200, 175, high));
        controller.set_color_to_segment(1,  CHSV(225, 255, info.low));
        controller.set_color_to_segment(2,  CHSV(180, 220, info.freq1));
        controller.set_color_to_segment(3,  CHSV(140, 255, info.freq4));
        controller.set_color_to_segment(4,  CHSV(90, 255, info.freq5));
        controller.set_color_to_segment(5,  CHSV(50, 255, info.freq8));
        controller.set_color_to_segment(6,  CHSV(20, 255, info.freq9));
        controller.set_color_to_segment(7,  CHSV(0, 255, info.freq12));
    }
};

class ColorMusicDetailMode: public AbstractColorMode  {
public:
    Analyzer &analyzer;
    BlinkMode blink_mode;

    explicit ColorMusicDetailMode(EEPROMMemory &mem, Analyzer &analyzer) : AbstractColorMode(mem), analyzer(analyzer), blink_mode(mem) {}

    void handle_input_event(input_event_t input) override {}

private:
    uint16_t _get_show_delay() const override { return COLOR_MUSIC_DEF_DELAY_MS; }

    void _calculate(LedController &controller) override {
        if (analyzer.calibrate_audio_analyzer()) {
            blink_mode.calculate(controller);
            return;
        }

        analyzer.analyze_the_signal();
        FreqInformation info = analyzer.calculate_colors();

        auto *info_ = reinterpret_cast<uint8_t*>(&info);
        for (int i = 0; i < NUMBER_OF_FREQ_INDEXES - 2; ++i) info_[i] = CRT(info_[i]);

        controller.set_color_to_detail_segment(1, CRGB(info.low, 0, 0));
        controller.set_color_to_detail_segment(2, CRGB(0, 0, info.high));
        controller.set_color_to_detail_segment(3, CRGB(0, info.hi_hats, info.hi_hats));

        for (int i = 1; i < NUMBER_OF_FREQ_INDEXES - 2; ++i) {
            controller.set_color_to_detail_segment(i + 3, CHSV(i * 20, 255, info_[i]));
        }
    }
};


#endif //ARD_COLOR_MUSIC_MODE_H
