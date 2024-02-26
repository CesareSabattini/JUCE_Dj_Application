#include "MainComponent.h"

MainComponent::MainComponent(): leftButton("leftButton", juce::Colours::gold, juce::Colours::white,juce::Colours::grey),
rightButton("rightButton", juce::Colours::gold, juce::Colours::white, juce::Colours::grey)
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

    playButton.setButtonText("Play");
    playButton.onClick = [this] {
        for (auto& player : players) {
            player->start();
        }
    };
    addAndMakeVisible(playButton);
    playButton.setLookAndFeel(&appLAF);

    pauseButton.setButtonText("Pause");
    pauseButton.onClick = [this] {
        for (auto& player : players) {
            player->pause();
        }
    };
    addAndMakeVisible(pauseButton);
    pauseButton.setLookAndFeel(&appLAF);

    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] {
        for (auto& player : players) {
            player->stop();
            
        }
    };
    addAndMakeVisible(stopButton);
    stopButton.setLookAndFeel(&appLAF);



    resized();
    effectsViewport.setScrollBarsShown(false, false);
    addAndMakeVisible(effectsViewport);
    effectsViewport.setViewedComponent(&commonEffects, false);
    // Dimensioni di esempio



            // Crea il triangolo per il pulsante sinistro
    juce::Path leftTrianglePath;
    leftTrianglePath.startNewSubPath(5, 5); // Punto in alto
    leftTrianglePath.lineTo(0, 0); // Punto in basso a sinistra
    leftTrianglePath.lineTo(5, -5); // Punto in basso a destra
    leftTrianglePath.closeSubPath(); // Chiudi il percorso

    leftButton.setShape(leftTrianglePath, true, true, true);
    addAndMakeVisible(leftButton);

    leftButton.onClick = [this] {
        effectsViewport.setViewPosition(effectsViewport.getViewPositionX()-effectsViewport.getWidth(), effectsViewport.getViewPositionY());
    };



    juce::Path rightTrianglePath;
    rightTrianglePath.startNewSubPath(0, 5);  // Punto in alto a sinistra
    rightTrianglePath.lineTo(0, -5); // Punto in alto a destra
    rightTrianglePath.lineTo(5, 0);  // Punto in basso a sinistra
    rightTrianglePath.closeSubPath(); // Chiudi il percorso


    rightButton.setShape(rightTrianglePath, true, true, true);
    addAndMakeVisible(rightButton);
    rightButton.onClick = [this] {
        effectsViewport.setViewPosition(effectsViewport.getViewPositionX() + effectsViewport.getWidth(), effectsViewport.getViewPositionY());
    };
  

    commonEffects.onParametersChanged = [this](const juce::Reverb::Parameters& params) {
        for (auto& player : players) {
            player->setReverbParameters(params);

        }
    };

    commonEffects.onDelayParametersChanged = [this](const DelayParameters& params) {
        for (auto& player : players) {
            player->setDelayParameters(params.delayTime, params.feedback, params.wetLevel);

        }
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
    g.fillAll(juce::Colour(30,30,30));

}

void MainComponent::resized()
{

    const int topGap = 40;
    const int middleGap = 15;
    otodecksLabel.setBounds((getWidth()-250)/2+20, 20, 210,80);
    container.setBounds(0, 0,getWidth(), getHeight());
    syncButton.setBounds((getWidth() - 250) / 2 + middleGap, 100 + topGap, (250 - 3 * middleGap)/2, 60);
    playButton.setBounds(syncButton.getRight() + middleGap, 100 + topGap, (250 - 3 * middleGap) / 2, 60);
    pauseButton.setBounds((getWidth() - 250) / 2 + middleGap, playButton.getBottom() + middleGap, (250 - 3 * middleGap) / 2, 60);
    stopButton.setBounds(pauseButton.getRight() + middleGap, playButton.getBottom() + middleGap, (250 - 3 * middleGap) / 2, 60);

    effectsViewport.setBounds((getWidth() - 250) / 2, 340, 250, getHeight() - 310);
    commonEffects.setSize(effectsViewport.getWidth()*3, effectsViewport.getHeight());
    leftButton.setBounds((getWidth() - 250) / 2+20, 350, 25, 25);
    rightButton.setBounds((getWidth()-250) / 2+ 200, 350, 25, 25);
}


