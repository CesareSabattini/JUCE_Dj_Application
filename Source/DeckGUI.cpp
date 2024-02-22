
#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(DjAudioPlayer* audioPlayer) : djAudioPlayer{ audioPlayer }, playButton("playButton", juce::Colours::forestgreen, juce::Colours::lawngreen, juce::Colours::darkolivegreen),
pauseButton("pauseButton", juce::Colours::grey, juce::Colours::darkgrey, juce::Colours::black),
stopButton("stopButton", juce::Colours::indianred, juce::Colours::mediumvioletred, juce::Colours::darkred)
{
    juce::Path trianglePath;
    trianglePath.addTriangle(0, 0, 40, 25, 0, 50);
    playButton.setShape(trianglePath, true, true, true);
    playButton.onClick = [this] {};
    addAndMakeVisible(playButton);

    juce::Path doubleRectPath;
    doubleRectPath.addRectangle(0, 0, 10, 30);
    doubleRectPath.addRectangle(14, 0, 10, 30);
    pauseButton.setShape(doubleRectPath, true, true, true);
    pauseButton.onClick = [this] {};
    addAndMakeVisible(pauseButton);

    juce::Path squarePath;
    squarePath.addRectangle(0, 0, 40, 40);
    stopButton.setShape(squarePath, true, true, true);
    playButton.onClick = [this] {};
    addAndMakeVisible(stopButton);

    loadButton.setButtonText("Load");
    addAndMakeVisible(loadButton);


    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 1.0);
    speedSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(deckLabel);
    deckLabel.addListener(this);
    deckLabel.setMultiLine(false);
    deckLabel.setReturnKeyStartsNewLine(false);
    deckLabel.setReadOnly(false);
    deckLabel.setScrollbarsShown(false);
    deckLabel.setCaretVisible(true);
    deckLabel.setTextToShowWhenEmpty("insert track name", juce::Colours::grey);



}

DeckGUI::~DeckGUI()
{
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        djAudioPlayer->start();
    }
    if (button == &stopButton) {
        djAudioPlayer->stop();
    }
    if (button == &loadButton) {
        djAudioPlayer->loadURL();
        // juce::FileChooser chooser{ "Select a file..." };
        // if (chooser.browseForFileToOpen()) {
            // djAudioPlayer->loadURL(juce::URL{ chooser.getResult() });
        // }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {

    if (slider == &volSlider) {
        djAudioPlayer->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        djAudioPlayer->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        djAudioPlayer->setPositionRelative(slider->getValue());
    }

}




void DeckGUI::paint(juce::Graphics& g)
{

    g.fillAll(AppColours::internalColour);
}

void DeckGUI::resized()
{
    const int borderGap = 5;
    const int labelWidth = 110;
    const int labelHeight = 30;
    int buttonWidth = 30;
    int buttonHeight = 30;
    int buttonGap = 10;
    int sliderWidth = 110;
    int sliderHeight = 50;
    const int sliderLabelWidth = 30;
    const int sliderLabelHeight = 20;
    int sliderGap = 10;

    deckLabel.setBounds(borderGap, borderGap, labelWidth, labelHeight);
    playButton.setBounds(borderGap, deckLabel.getBottom()+5, buttonWidth, buttonHeight);
    pauseButton.setBounds(playButton.getRight() + buttonGap, deckLabel.getBottom()+5, buttonWidth, buttonHeight);
    stopButton.setBounds(pauseButton.getRight() + buttonGap, deckLabel.getBottom()+5, buttonWidth, buttonHeight);

    loadButton.setBounds(10, playButton.getBottom() + buttonGap, 3*buttonWidth+2*buttonGap, buttonHeight);

    volSlider.setBounds(sliderGap, loadButton.getBottom() + buttonGap, sliderWidth, sliderHeight);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, sliderLabelWidth, sliderLabelHeight);
    speedSlider.setBounds(sliderGap, volSlider.getBottom() + buttonGap, sliderWidth, sliderHeight);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, sliderLabelWidth, sliderLabelHeight);
    posSlider.setBounds(sliderGap, speedSlider.getBottom() + buttonGap, sliderWidth, sliderHeight);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, sliderLabelWidth, sliderLabelHeight);
}
