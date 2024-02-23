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

    auto myTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::AntaRegular_ttf, BinaryData::AntaRegular_ttfSize);
    juce::Font myFont(myTypeface);
    myFont.setHeight(35);
    otodecksLabel.setText("Otodecks", juce::dontSendNotification);
    otodecksLabel.setFont(myFont);
    otodecksLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    otodecksLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(otodecksLabel);

    addDeckButton.setButtonText("+");
    addDeckButton.onClick = [this] {
        addDeck();
    };
    addAndMakeVisible(addDeckButton);
    resized();

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


void shadeRect(juce::Graphics& g, juce::Rectangle<float> rect, juce::Colour color) {
    juce::Colour shadowColor3 = color.darker(0.8);
    g.setColour(shadowColor3);
    g.fillRect(rect.reduced(-6));
    juce::Colour shadowColor2 = color.darker(0.5);
    g.setColour(shadowColor2);
    g.fillRect(rect.reduced(-4));
    juce::Colour shadowColor1 = color.darker(0.2);
    g.setColour(shadowColor1);
    g.fillRect(rect.reduced(-2));
    g.setColour(color);
    g.fillRect(rect);
}

void MainComponent::paint(juce::Graphics& g)
{
    juce::Colour rectangleColor = juce::Colour(0, 76, 153);
    juce::Rectangle<float> upperRect(0, 0, getWidth(), 50);
    shadeRect(g, upperRect, rectangleColor);

}

void MainComponent::resized()
{
    otodecksLabel.setBounds(getLocalBounds().removeFromTop(50));
    viewPort.setBounds(0, 100, getWidth(), getHeight());
    container.setBounds(0, 100,getWidth(), getHeight());
    addDeckButton.setBounds(550, viewPort.getBounds().getCentreY()-15, 30, 30);


}


void MainComponent::addDeck() {
    players.push_back(std::make_unique<DjAudioPlayer>());
    deckGUIs.push_back(std::make_unique<DeckGUI>(players[players.size()-1].get()));
    mixer.addInputSource(players[players.size() - 1].get(), true);
    container.addDeckGUI(std::move(deckGUIs[deckGUIs.size()-1]));
    resized();
}