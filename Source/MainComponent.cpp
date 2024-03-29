#include "MainComponent.h"

/*
The MainComponent serves as an interface to handle the whole application
workflow: two traks can be played simoultaneausly, thanks to the
juce::MixerAudioSource class. The audio effects are applied to the output audio,
which starts from here, with the definition of the callbacks defined as member
object-functions of the CommonEffect component.
*/
MainComponent::MainComponent()
    : leftButton("leftButton", juce::Colours::gold, juce::Colours::white,
                 juce::Colours::grey),
      rightButton("rightButton", juce::Colours::gold, juce::Colours::white,
                  juce::Colours::grey) {

    setSize(600, 400);
    setAudioChannels(2, 2);

    players.push_back(std::make_shared<DjAudioPlayer>());
    players.push_back(std::make_shared<DjAudioPlayer>());
    deckGUIs.push_back(std::make_shared<DeckGUI>(players[0]));
    deckGUIs.push_back(std::make_shared<DeckGUI>(players[1]));

    for (auto &player : players) {
        mixer.addInputSource(player.get(), false);
    }
    for (auto &deckGUI : deckGUIs)

    {
        container.addDeckGUI(deckGUI);
    }
    addAndMakeVisible(container);

    addAndMakeVisible(container);

    auto myTypeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::AntaRegular_ttf, BinaryData::AntaRegular_ttfSize);
    juce::Font myFont(myTypeface);
    myFont.setHeight(35);

    otodecksLabel.setFont(myFont);

    otodecksLabel.setLookAndFeel(&appLAF);
    otodecksLabel.setText("Otodecks", juce::dontSendNotification);
    otodecksLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    otodecksLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(otodecksLabel);

    playButton.setButtonText("Play");
    playButton.onClick = [this] {
        for (auto &player : players) {
            player->start();
        }
    };
    addAndMakeVisible(playButton);
    playButton.setLookAndFeel(&appLAF);

    pauseButton.setButtonText("Pause");
    pauseButton.onClick = [this] {
        for (auto &player : players) {
            player->pause();
        }
    };
    addAndMakeVisible(pauseButton);
    pauseButton.setLookAndFeel(&appLAF);

    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] {
        for (auto &player : players) {
            player->stop();
        }
    };
    addAndMakeVisible(stopButton);
    stopButton.setLookAndFeel(&appLAF);
    resized();

    /*
    This viewport enables the CommonEffect component to be partly
    shown onscreen, to simulate a scrolling effect, mediated by the left and
    right shapeButtons.
    */
    effectsViewport.setScrollBarsShown(false, false);
    addAndMakeVisible(effectsViewport);
    effectsViewport.setViewedComponent(&commonEffects, false);

    /*
   Definitions of the shapeButtons props. They're shaped by a juce::Path
   component. The same process is followed for both the Button components.
    */
    juce::Path leftTrianglePath;
    leftTrianglePath.startNewSubPath(5, 5);
    leftTrianglePath.lineTo(0, 0);
    leftTrianglePath.lineTo(5, -5);
    leftTrianglePath.closeSubPath();

    leftButton.setShape(leftTrianglePath, true, true, true);
    addAndMakeVisible(leftButton);

    /*
   Definition of the callback method.
   */
    leftButton.onClick = [this] {
        effectsViewport.setViewPosition(effectsViewport.getViewPositionX() -
                                            effectsViewport.getWidth(),
                                        effectsViewport.getViewPositionY());
    };

    juce::Path rightTrianglePath;
    rightTrianglePath.startNewSubPath(0, 5);
    rightTrianglePath.lineTo(0, -5);
    rightTrianglePath.lineTo(5, 0);
    rightTrianglePath.closeSubPath();

    rightButton.setShape(rightTrianglePath, true, true, true);
    addAndMakeVisible(rightButton);
    rightButton.onClick = [this] {
        effectsViewport.setViewPosition(effectsViewport.getViewPositionX() +
                                            effectsViewport.getWidth(),
                                        effectsViewport.getViewPositionY());
    };

    /*
    CommonEffects' callbacks definitions.
    */
    commonEffects.onParametersChanged =
        [this](const juce::Reverb::Parameters &params) {
            for (auto &player : players) {
                player->setReverbParameters(params);
            }
        };

    commonEffects.onDelayParametersChanged =
        [this](const DelayParameters &params) {
            for (auto &player : players) {
                player->setDelayParameters(params.delayTime, params.feedback,
                                           params.wetLevel);
            }
        };
}

MainComponent::~MainComponent() {
    mixer.removeAllInputs();
    deckGUIs.clear();
    players.clear();
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate) {

    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {
    mixer.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
    mixer.removeAllInputs();
    for (auto &player : players) {
        player->stop();
    }
    mixer.releaseResources();
}

void MainComponent::paint(juce::Graphics &g) {
    g.fillAll(AppColours::customGrey);
}

void MainComponent::resized() {
    const int topGap = getHeight() / 5;
    const int littleGap = 10;
    const int middleGap = 15;
    const int wideGap = 20;
    const int inBetweenWidth = getWidth() / 6;
    const int labelWidth = inBetweenWidth - 2 * middleGap;
    const int labelHeight = topGap / 2;
    const int buttonHeight = 60;
    const int shapeButtonWidth = 25;
    const int shapeButtonHeight = 25;
    const int viewportY = getBounds().getCentreY() * 5 / 6;

    otodecksLabel.setBounds((getWidth() - inBetweenWidth) / 2 + wideGap,
                            2 * wideGap, labelWidth, labelHeight);
    container.setBounds(0, 0, getWidth(), getHeight());
    playButton.setBounds((getWidth() - inBetweenWidth) / 2 + middleGap, topGap,
                         inBetweenWidth - 2 * middleGap, buttonHeight);
    pauseButton.setBounds((getWidth() - inBetweenWidth) / 2 + middleGap,
                          playButton.getBottom() + middleGap,
                          (inBetweenWidth - 3 * middleGap) / 2, buttonHeight);
    stopButton.setBounds(pauseButton.getRight() + middleGap,
                         playButton.getBottom() + middleGap,
                         (inBetweenWidth - 3 * middleGap) / 2, buttonHeight);

    effectsViewport.setBounds((getWidth() - inBetweenWidth) / 2, viewportY,
                              inBetweenWidth, getHeight() - viewportY);
    commonEffects.setSize(effectsViewport.getWidth() * 3,
                          effectsViewport.getHeight());
    leftButton.setBounds(
        effectsViewport.getBounds().getTopLeft().getX() + wideGap,
        viewportY + littleGap, shapeButtonWidth, shapeButtonHeight);
    rightButton.setBounds(effectsViewport.getBounds().getTopRight().getX() -
                              2 * wideGap - littleGap / 2,
                          viewportY + littleGap, shapeButtonWidth,
                          shapeButtonHeight);
}
