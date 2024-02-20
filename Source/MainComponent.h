#pragma once

#include <JuceHeader.h>
#include"DeckGUI.h"
#include"DjAudioPlayer.h"

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
    DeckGUI deckGUI;
    juce::MixerAudioSource mixer;
    DjAudioPlayer player1;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
