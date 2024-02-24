#pragma once

#include <JuceHeader.h>
#include"DeckGUI.h"
#include"DjAudioPlayer.h"
#include"DecksContainer.h"



class MainComponent  : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics& g) override;
    void resized() override;


private:
    juce::Label otodecksLabel;
    DecksContainer container;
    std::vector<std::unique_ptr<DjAudioPlayer>> players;
      std::vector<std::unique_ptr<DeckGUI>> deckGUIs;
    juce::MixerAudioSource mixer;
    AppLAF appLAF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
