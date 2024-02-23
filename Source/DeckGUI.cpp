
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(DjAudioPlayer* audioPlayer) : djAudioPlayer{ audioPlayer },// playButton("playButton", juce::Colours::forestgreen, juce::Colours::lawngreen, juce::Colours::darkolivegreen),
pauseButton("pauseButton", juce::Colours::grey, juce::Colours::darkgrey, juce::Colours::black),
stopButton("stopButton", juce::Colours::indianred, juce::Colours::mediumvioletred, juce::Colours::darkred)
{
    //juce::Path trianglePath;
    //trianglePath.addTriangle(0, 0, 40, 25, 0, 50);
    //playButton.setShape(trianglePath, true, true, true);

        juce::File imageFile("C:/Users/cesar/Desktop/disk.png");     
       disk= juce::ImageFileFormat::loadFrom(imageFile);
      disk= disk.rescaled(disk.getWidth() / 4, disk.getHeight() / 4);
      startTimerHz(60);

      playButton.setLookAndFeel(&appLAF);
    addAndMakeVisible(playButton);
    playButton.addListener(this);
  


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
    loadButton.onClick = [this] {
        DBG("CLICKED");
        djAudioPlayer->loadURL();
    };
    loadButton.setLookAndFeel(&appLAF);




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

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button clicked");
        std::cout << "Play button was clicked " << std::endl;
        djAudioPlayer->start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        djAudioPlayer->stop();

    }

    // if (button == &loadButton)
    // {
    //     FileChooser chooser{"Select a file..."};
    //     if (chooser.browseForFileToOpen())
    //     {
    //         player->loadURL(URL{chooser.getResult()});
    //         waveformDisplay.loadURL(URL{chooser.getResult()});

    //     }

}

DeckGUI::~DeckGUI()
{
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

    // Definizione del gradiente e disegno dello sfondo
    juce::ColourGradient gradient(juce::Colour(51, 153, 255), bounds.getTopLeft(),
        juce::Colour(204, 153, 255), bounds.getBottomRight(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, cornerSize);
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(bounds.reduced(0.5f), cornerSize, 3.0f);

    // Disegno del bordo attorno al giradischi
    g.drawRoundedRectangle(sliderContainerBounds.toFloat().reduced(0.5f), cornerSize, 3.0f);

    if (!disk.isNull()) {
        // Calcola il punto centrale esatto dell'immagine del disco per la rotazione
        auto diskCentreX = getBounds().getCentreX();
        auto diskCentreY = getBounds().getCentreY();
        auto imageCentre = juce::Point<int>(diskCentreX, diskCentreY);

        // Applica una trasformazione per ruotare l'immagine attorno al suo centro
        g.addTransform(juce::AffineTransform::rotation(rotationAngle, imageCentre.getX(), imageCentre.getY()));
        g.drawImageAt(disk, diskCentreX - disk.getWidth() / 2, diskCentreY - disk.getHeight() / 2);

        // Resetta la trasformazione per eventuali ulteriori operazioni di disegno
        
    }
}

void DeckGUI::timerCallback() {
    const float rotationSpeed = 2*juce::MathConstants<float>::twoPi; // Radianti al secondo
    const float frameTime = 1.0f / 60.0f; // Tempo per frame a 60 FPS

    // Aggiorna l'angolo di rotazione
    rotationAngle += rotationSpeed * frameTime;

    // Assicurati che l'angolo rimanga valido
    if (rotationAngle >= juce::MathConstants<float>::twoPi)
        rotationAngle -= juce::MathConstants<float>::twoPi;

    // Richiedi il ridisegno del componente
    repaint();
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
