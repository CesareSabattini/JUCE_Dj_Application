#include "MainComponent.h"

MainComponent::MainComponent() : deckGUI(&player1)
{
    setSize (800, 600);
    setAudioChannels (2, 2);
    mixer.addInputSource(&player1, true);
    addAndMakeVisible(deckGUI);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected,
        sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
}


void MainComponent::paint (juce::Graphics& g)
{

}

void MainComponent::resized()
{
    deckGUI.setBounds(0,0,getWidth(), getHeight());
}
