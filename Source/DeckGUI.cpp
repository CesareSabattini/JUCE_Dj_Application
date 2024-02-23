
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
    volSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volSlider.setLookAndFeel(&appLAF);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setLookAndFeel(&appLAF);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 1.0);
    speedSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(posSlider);
    posSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    posSlider.setLookAndFeel(&appLAF);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(deckLabel);
    deckLabel.setText("Deck Name", juce::dontSendNotification);
    deckLabel.setEditable(true, true, false);
    deckLabel.setLookAndFeel(&appLAF);
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

    auto bounds = getLocalBounds().toFloat();
    auto cornerSize = 10.0f; // Raggio degli angoli arrotondati

    // Definizione del gradiente
    juce::ColourGradient gradient(juce::Colour(212, 213, 213), bounds.getTopLeft(),
        juce::Colour(247, 248, 248), bounds.getBottomRight(), false);
    g.setGradientFill(gradient);

    // Disegna lo sfondo con angoli arrotondati
    g.fillRoundedRectangle(bounds, cornerSize);

    // Disegna il bordo
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(bounds.reduced(0.5f), cornerSize,3.0f);

    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(sliderContainerBounds.toFloat().reduced(0.5f), cornerSize, 3.0f); // Spessore del bordo


}

void DeckGUI::resized()
{
    const int borderGap = 5;
    const int labelWidth = 110;
    const int labelHeight = 30;
    int buttonWidth = 30;
    int buttonHeight = 30;
    int buttonGap = 10;
    int sliderWidth = 128;
    int sliderHeight = 128;
    const int sliderLabelWidth = 30;
    const int sliderLabelHeight = 20;
    int sliderGap = 10;

   
    deckLabel.setBounds(getBounds().getCentreX()-labelWidth/2, borderGap, labelWidth, labelHeight);
    playButton.setBounds(30, deckLabel.getBottom()+30, buttonWidth, buttonHeight);
    pauseButton.setBounds(playButton.getRight() + buttonGap, deckLabel.getBottom()+30, buttonWidth, buttonHeight);
    stopButton.setBounds(pauseButton.getRight() + buttonGap, deckLabel.getBottom()+30, buttonWidth, buttonHeight);

    loadButton.setBounds(getBounds().getRight()-30-labelWidth, deckLabel.getBottom() + 30, 3 * buttonWidth + 2 * buttonGap, buttonHeight);
   
    sliderContainerBounds = getLocalBounds().reduced(20).removeFromBottom(getLocalBounds().getHeight() / 2);

    int startX = sliderContainerBounds.getX() ; // Un esempio di posizionamento iniziale
    volSlider.setBounds(startX, sliderContainerBounds.getY() +10, sliderWidth, sliderHeight);
    speedSlider.setBounds(startX + sliderWidth, sliderContainerBounds.getY() + 10, sliderWidth, sliderHeight);
    posSlider.setBounds(startX + 2 * sliderWidth, sliderContainerBounds.getY() + 10, sliderWidth, sliderHeight);

    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
}
