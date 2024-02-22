#pragma once

#include <JuceHeader.h>
#include"AppStyle.h"
#include"DjAudioPlayer.h"


class DeckGUI  : public juce::Component, juce::Button::Listener, juce::Slider::Listener, juce::TextEditor::Listener
{
public:
    DeckGUI(DjAudioPlayer* audioPlayer);
    ~DeckGUI() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    DjAudioPlayer* djAudioPlayer;

    juce::ShapeButton playButton;
    juce::ShapeButton pauseButton;
    juce::ShapeButton stopButton;

    juce::TextButton loadButton;

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::TextEditor deckLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
