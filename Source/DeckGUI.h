#pragma once

#include "AppStyle.h"
#include "DjAudioPlayer.h"
#include "Playlist.h"
#include <JuceHeader.h>

class DeckGUI : public juce::Component,
                juce::Slider::Listener,
                juce::TextEditor::Listener,
                juce::Button::Listener,
                private juce::Timer {
  public:
    DeckGUI(std::shared_ptr<DjAudioPlayer> audioPlayer);
    ~DeckGUI() override;
    void buttonClicked(juce::Button *button) override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider *slider) override;
    void showPlaylist();
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    /*
    Pay attention to the initialization order.
    */
    float rotationAngle = 0.0f;
    const float rotationSpeed = 2 * juce::MathConstants<float>::twoPi;
    const float frameTime = 1.0f / 60.0f;
    bool isPlaying = false;

    std::shared_ptr<DjAudioPlayer> djAudioPlayer;

    AppLAF appLAF;

    std::unique_ptr<Playlist> playlist;

    std::unique_ptr<juce::Image> discImage;
    std::unique_ptr<juce::Image> armImage;

    juce::TextButton playButton;
    juce::TextButton pauseButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Label volLabel;
    juce::Label speedLabel;
    juce::Label posLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
