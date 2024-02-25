#include "MainComponent.h"

MainComponent::MainComponent()
{
   setSize(600, 400);
   
    setAudioChannels(2, 2);

    players.push_back(std::make_unique<DjAudioPlayer>());
    players.push_back(std::make_unique<DjAudioPlayer>());
    deckGUIs.push_back(std::make_unique<DeckGUI>(players[0].get()));
    deckGUIs.push_back(std::make_unique<DeckGUI>(players[1].get()));

    mixer.addInputSource(players[0].get(), true);
    mixer.addInputSource(players[1].get(), true);
    for (auto& deckGUI : deckGUIs)
    {

        container.addDeckGUI(std::move(deckGUI));
    }
    addAndMakeVisible(container);


    addAndMakeVisible(container);
    
    auto myTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::AntaRegular_ttf, BinaryData::AntaRegular_ttfSize);
    juce::Font myFont(myTypeface);
    myFont.setHeight(35);

  
    otodecksLabel.setFont(myFont);

    otodecksLabel.setLookAndFeel(&appLAF);
    otodecksLabel.setText("Otodecks", juce::dontSendNotification);
    otodecksLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    otodecksLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(otodecksLabel);

    syncButton.setButtonText("Sync");
    addAndMakeVisible(syncButton);
    syncButton.setLookAndFeel(&appLAF);

    resized();
    
    addAndMakeVisible(effectsViewport);
    effectsViewport.setViewedComponent(&commonEffects, false);
    // Dimensioni di esempio

    addAndMakeVisible(leftButton);
    leftButton.setButtonText("Sinistra");
    leftButton.onClick = [this] {
        effectsViewport.setViewPosition(effectsViewport.getViewPositionX() - 300, effectsViewport.getViewPositionY());
    };

    addAndMakeVisible(rightButton);
    rightButton.setButtonText("Destra");
    rightButton.onClick = [this] {
        effectsViewport.setViewPosition(effectsViewport.getViewPositionX() + 300, effectsViewport.getViewPositionY());
    };
  

}

MainComponent::~MainComponent()
{
    mixer.releaseResources();
    shutdownAudio();

}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    
        mixer.prepareToPlay(samplesPerBlockExpected,
            sampleRate);
    

}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixer.getNextAudioBlock(bufferToFill);

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
 

}

void MainComponent::resized()
{
    const int middleGap = 30;
    otodecksLabel.setBounds((getWidth()-250)/2, 0, 250,100);
    container.setBounds(0, 0,getWidth(), getHeight());
    syncButton.setBounds((getWidth() - 250) / 2 + middleGap, 100 + middleGap, 250 - 2 * middleGap, 100);
    effectsViewport.setBounds((getWidth() - 250) / 2, 310, 250, getHeight() - 310);
    commonEffects.setSize(effectsViewport.getWidth()*2, effectsViewport.getHeight());
    leftButton.setBounds((getWidth() - 250) / 2, 410, 50, 50);
    rightButton.setBounds((getWidth()) / 2, 410, 50, 50);
}


