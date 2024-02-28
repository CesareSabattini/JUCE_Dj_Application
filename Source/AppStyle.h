#pragma once
#include<JuceHeader.h>

/*
Definition of AppColours class, containing all the user defined colours, utilized in the application.
*/

struct AppColours {
    static const juce::Colour customGrey;
    static const juce::Colour customTransparentBlack;
    static const juce::Colour customBlack;
    static const juce::Colour customDarkGrey;
    static const juce::Colour customDarkerGrey;
};

/*
Definition of AppLAF class, which inherits from juce::LookAndFeel_V4, meaning that it serves as a specific implementation
of its virtual member functions. Each of them is meant to custom the style of a built-in JUCE component.
*/
class AppLAF : public juce::LookAndFeel_V4{
public:
    AppLAF();
    void drawLabel(juce::Graphics& g, juce::Label& label) override;
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour, bool isMouseOverButton,
        bool isButtonDown) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    void drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header) override;

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

  juce::Image* getDiscImage();
    juce::Image* getArmImage();
private:

    juce::Font antaRegular;
    juce::Image discImage;
    juce::Image armImage;
};
