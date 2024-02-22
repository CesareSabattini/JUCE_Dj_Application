#include "MainComponent.h"

MainComponent::MainComponent()
{
   setSize(600, 400);
   
    setAudioChannels(2, 2);

    players.push_back(std::make_unique<DjAudioPlayer>());
    deckGUIs.push_back(std::make_unique<DeckGUI>(players[0].get()));
    mixer.addInputSource(players[0].get(), true);
    for (auto& deckGUI : deckGUIs)
    {

        container.addDeckGUI(std::move(deckGUI));
    }
    addAndMakeVisible(container);

    viewPort.setViewedComponent(&container);
    viewPort.setScrollBarsShown(false, true, false, false);
    addAndMakeVisible(viewPort);

    otodecksLabel.setText("Otodecks", juce::dontSendNotification);
    otodecksLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(otodecksLabel);

    addDeckButton.setButtonText("Add Deck");
    addDeckButton.onClick = [this] {
        addDeck();
    };
    addAndMakeVisible(addDeckButton);

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    for (int i = 0; i < players.size(); i++) {
        players[i]->prepareToPlay(samplesPerBlockExpected,
            sampleRate);
    }

}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    for (int i = 0; i < players.size(); i++) {
        players[i]->getNextAudioBlock(bufferToFill);
    }

}

void MainComponent::releaseResources()
{
    for (int i = 0; i < players.size(); i++) {
        players[i]->releaseResources();
    }
}


void MainComponent::paint(juce::Graphics& g)
{

}

void MainComponent::resized()
{
    otodecksLabel.setBounds(getLocalBounds().removeFromTop(50));
    addDeckButton.setBounds(10, 350, 100, 50);
    viewPort.setBounds(20, 50, 530, 300);
    container.setBounds(0, 50, getParentWidth(), 300);


}


void MainComponent::addDeck() {
    players.push_back(std::make_unique<DjAudioPlayer>());
    deckGUIs.push_back(std::make_unique<DeckGUI>(players[players.size()-1].get()));
    mixer.addInputSource(players[players.size() - 1].get(), true);
   

        container.addDeckGUI(std::move(deckGUIs[deckGUIs.size()-1]));
    
}