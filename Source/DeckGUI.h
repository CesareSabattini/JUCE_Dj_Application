#pragma once

#include <JuceHeader.h>
#include"DjAudioPlayer.h"

class DeckGUI  : public juce::Component, juce::Button::Listener, juce::Slider::Listener
{
public:
    DeckGUI(DjAudioPlayer* audioPlayer);
    ~DeckGUI() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    DjAudioPlayer* player1;

    juce::ShapeButton playButton;
    juce::ShapeButton pauseButton;
    juce::ShapeButton stopButton;

    juce::TextButton loadButton;

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
