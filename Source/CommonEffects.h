
#pragma once

#include <JuceHeader.h>
#include"AppStyle.h"
#include"DjAudioPlayer.h"

struct DelayParameters {
    float delayTime = 500.0f; // In millisecondi
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

    void CommonEffects::sliderValueChanged(juce::Slider* slider) {
        if (slider == &roomSlider || slider == &dampingSlider || slider == &wetLevelSlider || slider == &dryLevelSlider) {
            if (onParametersChanged) {
                juce::Reverb::Parameters params;
                params.roomSize = roomSlider.getValue();
                params.damping = dampingSlider.getValue();
                params.wetLevel = wetLevelSlider.getValue();
                params.dryLevel = dryLevelSlider.getValue();

                onParametersChanged(params);
            }
        }
        else if (slider == &delayTime || slider == &delayFeedback || slider == &delayWet) {
            if (onDelayParametersChanged) {
                DelayParameters params;
                params.delayTime = delayTime.getValue();
                params.feedback = delayFeedback.getValue();
                params.wetLevel = delayWet.getValue();

                onDelayParametersChanged(params);
            }
        }
    }




private:
    DjAudioPlayer* djAudioPlayer;
    AppLAF appLAF;
    juce::Label reverbLabel;
    juce::Label delayLabel;
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
