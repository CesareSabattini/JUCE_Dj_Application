
#pragma once

#include <JuceHeader.h>
#include"AppStyle.h"
#include"DjAudioPlayer.h"

/*
Basic structure that unifies the three necessary delay params.
*/
struct DelayParameters {
    float delayTime = 500.0f;
    float feedback = 0.5f;
    float wetLevel = 0.5f;
};

class CommonEffects  : public juce::Component, juce::Slider::Listener
{
public:
    CommonEffects();
    ~CommonEffects() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    std::function<void(const juce::Reverb::Parameters&)> onParametersChanged;
    std::function<void(const DelayParameters&)> onDelayParametersChanged;

    void sliderValueChanged(juce::Slider* slider) override;


private:
    DjAudioPlayer* djAudioPlayer;

    AppLAF appLAF;
    juce::Label reverbLabel;
    juce::Label delayLabel;
    juce::Label comingSoonLabel;
    juce::Viewport viewport;

    juce::Slider roomSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
    juce::Slider widthSlider;

    juce::Slider delayTime;
    juce::Slider delayFeedback;
    juce::Slider delayWet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommonEffects)
};
