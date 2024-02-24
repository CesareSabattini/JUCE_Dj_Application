
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(DjAudioPlayer* audioPlayer) : djAudioPlayer{ audioPlayer }
{


        juce::File imageFile("C:/Users/cesar/Desktop/disc.png");     
       disk= juce::ImageFileFormat::loadFrom(imageFile);
      disk= disk.rescaled(disk.getWidth() / 4, disk.getHeight() / 4);
      startTimerHz(60);

     

      playButton.setLookAndFeel(&appLAF);
      playButton.setButtonText("PLAY");
      playButton.onClick = [this] {
          DBG("PLAY BUTTON HAS BEEN CLICKED");
          djAudioPlayer->start();
      };
      addAndMakeVisible(playButton);  

    pauseButton.setLookAndFeel(&appLAF);
    pauseButton.setButtonText("PAUSE");
    pauseButton.onClick = [this] {};
    addAndMakeVisible(pauseButton);

    stopButton.setLookAndFeel(&appLAF);
    stopButton.setButtonText("STOP");
    stopButton.onClick = [this] {};
    addAndMakeVisible(stopButton);
  

    loadButton.setLookAndFeel(&appLAF);
    loadButton.setButtonText("LOAD");
    loadButton.onClick = [this] {
        djAudioPlayer->loadURL();
    };
    addAndMakeVisible(loadButton);



    addAndMakeVisible(volSlider);
    volSlider.setSliderStyle(juce::Slider::LinearVertical);
    volSlider.setLookAndFeel(&appLAF);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setLookAndFeel(&appLAF);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 1.0);
    speedSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible(posSlider);
    posSlider.setSliderStyle(juce::Slider::LinearVertical);
    posSlider.setLookAndFeel(&appLAF);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(1);


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

    repaint();
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

    // Definizione del gradiente e disegno dello sfondo
    juce::ColourGradient gradient(
        juce::Colour::fromRGB(50, 50, 50), // Grigio scuro come colore di partenza
        bounds.getTopLeft(),
        juce::Colour::fromRGB(25, 25, 25), // Ancora più scuro, vicino al nero, come colore di fine
        bounds.getBottomRight(),
        false);

    g.setGradientFill(gradient);
    g.fillRect(bounds);
    g.setColour(juce::Colours::black);

    if (!disk.isNull()) {
        g.saveState();
        auto diskCentreX = 250;
        auto diskCentreY = bounds.getCentreY();
        auto imageCentre = juce::Point<int>(diskCentreX, diskCentreY);

        // Applica una trasformazione per ruotare tutto attorno al suo centro
        g.addTransform(juce::AffineTransform::rotation(rotationAngle, imageCentre.getX(), imageCentre.getY()));

        float radius = disk.getHeight() / 2.0f;
        juce::Point<float> centre(diskCentreX, diskCentreY);

        // Creazione del gradiente per il cerchio metallizzato
        float outerRadius = disk.getHeight() / 2.0f + 20.0f;
        float innerRadius = outerRadius - 20.0f;

        // Miglioramento del gradiente con una transizione più complessa
        juce::ColourGradient greyGradient(juce::Colours::grey, centre.withY(centre.y - outerRadius),
            juce::Colours::lightgrey, centre.withY(centre.y + outerRadius), false);
        greyGradient.addColour(0.5, juce::Colours::darkgrey); // Transizione centrale più scura

        juce::Path ringPath;
        ringPath.addEllipse(centre.x - outerRadius, centre.y - outerRadius, 2 * outerRadius, 2 * outerRadius);
        ringPath.addEllipse(centre.x - innerRadius, centre.y - innerRadius, 2 * innerRadius, 2 * innerRadius);

        g.setGradientFill(greyGradient);
        g.fillPath(ringPath);

        // Corona circolare dorata
        juce::Colour goldStart(255, 215, 0); // Oro chiaro
        juce::Colour goldEnd(178, 145, 0); // Oro scuro
        juce::ColourGradient goldGradient(goldStart, centre, goldEnd, centre.withY(centre.y - outerRadius), false);
        juce::Path goldRingPath;
        goldRingPath.addEllipse(centre.x - radius, centre.y - radius, 2 * radius, 2 * radius);
        g.setGradientFill(goldGradient);
        g.strokePath(goldRingPath, juce::PathStrokeType(4.0f));

        // Disegno dei piccoli ovali (rientranze)
        for (int i = 0; i < 40; ++i) {
            float angle = juce::MathConstants<float>::twoPi * i / 40;
            juce::Point<float> ovalCentre = centre + juce::Point<float>(std::cos(angle), std::sin(angle)) * (innerRadius + 10.0f);

            juce::ColourGradient ovalGradient(juce::Colours::darkgrey, ovalCentre.translated(-2, -2), juce::Colours::lightgrey, ovalCentre.translated(2, 2), false);
            g.setGradientFill(ovalGradient);
            g.fillEllipse(ovalCentre.x - 5, ovalCentre.y - 5, 10, 10); // Raggio aggiornato a 5.0f
        }

        // Disegno del disco
        g.drawImageAt(disk, diskCentreX - disk.getWidth() / 2, diskCentreY - disk.getHeight() / 2);
        g.restoreState();

    }

    auto thumbnailBounds = juce::Rectangle<int>(0,0,getParentWidth(), 100); // Regola questo per adattarlo al tuo layout
    auto& thumbnail = djAudioPlayer->getThumbnail();
    if (thumbnail.getNumChannels() == 0)
        return; // Nessuna thumbnail da disegnare

    g.setColour(juce::Colours::lightgrey);
    g.fillRect(thumbnailBounds); // Sfondo per la thumbnail

    g.setColour(juce::Colours::darkblue);
    thumbnail.drawChannel(g, thumbnailBounds, 0, thumbnail.getTotalLength(), 0, 1.0f);
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
    int buttonWidth = 150;
    int buttonHeight = 60;
    int buttonGap = 5;
    int sliderWidth = 80;
    int sliderHeight = 250;
    const int sliderLabelWidth = 30;
    const int sliderLabelHeight = 20;
    int sliderGap = 10;
    float buttonX = 40;
    float buttonY = getHeight() - 100;;


   
   
    playButton.setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
    pauseButton.setBounds(playButton.getRight()+buttonGap, buttonY, buttonWidth, buttonHeight);
    stopButton.setBounds(pauseButton.getRight() + buttonGap, buttonY, buttonWidth, buttonHeight);

    loadButton.setBounds(stopButton.getRight() + buttonGap, buttonY, buttonWidth, buttonHeight);
   
   
    
    volSlider.setBounds(getLocalBounds().getCentreX()+sliderWidth, getLocalBounds().getCentreY() - sliderHeight / 2, sliderWidth, sliderHeight);
    speedSlider.setBounds(volSlider.getRight(), getLocalBounds().getCentreY() - sliderHeight / 2, sliderWidth, sliderHeight);
    posSlider.setBounds(speedSlider.getRight(), getLocalBounds().getCentreY() - sliderHeight / 2, sliderWidth, sliderHeight);

    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
   
}
