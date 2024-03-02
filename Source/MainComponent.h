#pragma once

#include "CommonEffects.h"
#include "DeckGUI.h"
#include "DecksContainer.h"
#include "DjAudioPlayer.h"
#include <JuceHeader.h>

class MainComponent : public juce::AudioAppComponent {
  public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(
        const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    AppLAF appLAF;
    juce::Label otodecksLabel;
    DecksContainer container;
    CommonEffects commonEffects;
    std::vector<std::shared_ptr<DjAudioPlayer>> players;
    std::vector<std::shared_ptr<DeckGUI>> deckGUIs;
    juce::MixerAudioSource mixer;

    juce::TextButton playButton;
    juce::TextButton pauseButton;
    juce::TextButton stopButton;

    juce::Viewport effectsViewport;
    juce::ShapeButton leftButton;
    juce::ShapeButton rightButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
