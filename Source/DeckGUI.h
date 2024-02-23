#pragma once

#include <JuceHeader.h>
#include"AppStyle.h"
#include"DjAudioPlayer.h"


class DeckGUI  : public juce::Component, juce::Slider::Listener, juce::TextEditor::Listener, juce::Button::Listener,
    private juce::Timer
{
public:
    DeckGUI(DjAudioPlayer* audioPlayer);
    ~DeckGUI() override;
    void buttonClicked(juce::Button* button);
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:


    DjAudioPlayer* djAudioPlayer;
    
    juce::Image disk;
    float rotationAngle = 0.0f; // In radianti

    juce::TextButton playButton;
    juce::ShapeButton pauseButton;
    juce::ShapeButton stopButton;

    juce::TextButton loadButton;

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Rectangle<int> sliderContainerBounds;

    juce::Label deckLabel;

    AppLAF appLAF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
