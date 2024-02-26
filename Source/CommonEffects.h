
#pragma once

#include <JuceHeader.h>
#include"AppStyle.h"
#include"DjAudioPlayer.h"

class CommonEffects  : public juce::Component, juce::Slider::Listener
{
public:
    CommonEffects();
    ~CommonEffects() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    std::function<void(const juce::Reverb::Parameters&)> onParametersChanged;

    void sliderValueChanged(juce::Slider* slider) override {
        if (onParametersChanged) {
            juce::Reverb::Parameters params;
            params.roomSize = roomSlider.getValue();
            params.damping = dampingSlider.getValue();
            params.wetLevel = wetLevelSlider.getValue();
            params.dryLevel = dryLevelSlider.getValue();
            onParametersChanged(params);
        }
    }

private:
    DjAudioPlayer* djAudioPlayer;
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
