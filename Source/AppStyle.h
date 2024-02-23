#pragma once
#include<JuceHeader.h>

/*Here I define a class containing all the colours used in the application,
beside the existing one in juce::Colours class */

struct AppColours {
    static const juce::Colour internalColour;

};

class AppLAF : public juce::LookAndFeel_V4{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    void drawLabel(juce::Graphics& g, juce::Label& label) override;
private:

};
