
#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DjAudioPlayer* audioPlayer) : player1{audioPlayer}, playButton("playButton", juce::Colours::forestgreen, juce::Colours::lawngreen, juce::Colours::darkolivegreen),
pauseButton("pauseButton", juce::Colours::grey, juce::Colours::darkgrey, juce::Colours::black),
stopButton("stopButton", juce::Colours::indianred, juce::Colours::mediumvioletred, juce::Colours::darkred)
{
    juce::Path trianglePath;
    trianglePath.addTriangle(0, 0, 40, 25, 0, 50);
    playButton.setShape(trianglePath,true,true,true);
    playButton.onClick = [this] {  };
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

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 1.0);

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    


}

DeckGUI::~DeckGUI()
{
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        player1->start();
    }
    if (button == &stopButton) {
        player1->stop();
    }
    if (button == &loadButton) {
        player1->loadURL();
       // juce::FileChooser chooser{ "Select a file..." };
       // if (chooser.browseForFileToOpen()) {
           // player1->loadURL(juce::URL{ chooser.getResult() });
       // }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {

    if (slider == &volSlider) {
        player1->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        player1->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        player1->setPositionRelative(slider->getValue());
    }

}




void DeckGUI::paint(juce::Graphics& g)
{

}

void DeckGUI::resized()
{
    playButton.setBounds(0,0,50,50);
    pauseButton.setBounds(50,0,50,50);
    stopButton.setBounds(100,0,50,50);
    loadButton.setBounds(500, 0, 50, 50);
    volSlider.setBounds(150,0,100,50);
    speedSlider.setBounds(250,0,100,50);
    posSlider.setBounds(350,0,100,50);
}
