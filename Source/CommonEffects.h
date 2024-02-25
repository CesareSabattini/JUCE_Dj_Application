
#pragma once

#include <JuceHeader.h>
#include"AppStyle.h"

class CommonEffects  : public juce::Component
{
public:
    CommonEffects();
    ~CommonEffects() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AppLAF appLAF;
    juce::Label reverbLabel;
    juce::Label effectsLabel;
    juce::Viewport viewport;
    juce::Slider roomSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
    juce::Slider widthSlider;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommonEffects)
};
