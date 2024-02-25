#pragma once
#include<JuceHeader.h>

/*Here I define a class containing all the colours used in the application,
beside the existing one in juce::Colours class */

struct AppColours {
    static const juce::Colour internalColour;

};

class AppLAF : public juce::LookAndFeel_V4{
public:
    AppLAF();
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    void drawLabel(juce::Graphics& g, juce::Label& label) override;
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour, bool isMouseOverButton,
        bool isButtonDown) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    void drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header) override;

private:
    juce::Image knobImage;
};
