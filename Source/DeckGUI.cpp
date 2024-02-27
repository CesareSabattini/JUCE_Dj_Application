#include "DeckGUI.h"


DeckGUI::DeckGUI(DjAudioPlayer* audioPlayer) : djAudioPlayer{ audioPlayer }, playlist(std::make_unique<Playlist>(audioPlayer))
{

    //
     
    discImage = std::make_unique<juce::Image>(appLAF.getDiscImage()->rescaled(appLAF.getDiscImage()->getWidth() / 4, appLAF.getDiscImage()->getHeight() / 4));
    armImage = std::make_unique<juce::Image>(appLAF.getArmImage()->rescaled(appLAF.getArmImage()->getWidth() / 5, appLAF.getArmImage()->getHeight() / 5));


     

      playButton.setLookAndFeel(&appLAF);
      playButton.setButtonText("PLAY");
      playButton.addListener(this);
      addAndMakeVisible(playButton);  

    pauseButton.setLookAndFeel(&appLAF);
    pauseButton.setButtonText("PAUSE");
    pauseButton.addListener(this);
    addAndMakeVisible(pauseButton);

    stopButton.setLookAndFeel(&appLAF);
    stopButton.setButtonText("STOP");
    stopButton.addListener(this);
    addAndMakeVisible(stopButton);
  

    loadButton.setLookAndFeel(&appLAF);
    loadButton.setButtonText("LOAD");
    loadButton.addListener(this);;
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    volSlider.setSliderStyle(juce::Slider::LinearVertical);
    volSlider.setLookAndFeel(&appLAF);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(1);
    volSlider.setValue(1.0);

    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setLookAndFeel(&appLAF);
    speedSlider.addListener(this);
    speedSlider.setRange(0.01, 2.0);
    speedSlider.setNumDecimalPlacesToDisplay(1);
    speedSlider.setValue(1.0);

    addAndMakeVisible(posSlider);
    posSlider.setSliderStyle(juce::Slider::LinearVertical);
    posSlider.setLookAndFeel(&appLAF);
    posSlider.addListener(this);
    posSlider.setRange(0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(1);
    posSlider.setValue(0.0);

    addAndMakeVisible(playlist.get());
    playlist->setVisible(false);
    startTimerHz(60);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("start");
        isPlaying = true;
        djAudioPlayer->start();
      
    }
    if (button==&pauseButton) {
        DBG("pause");
        djAudioPlayer->pause();
        isPlaying = false;
    }
    if (button == &stopButton)
    {
        DBG("stop");
        djAudioPlayer->stop();
        isPlaying = false;

    }
    if (button == &loadButton) {
        DBG("Load");
        
        playlist->loadTracksFromDirectory();
        playlist->resized();
        showPlaylist();
    }

  
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

   if(!playlist->isVisible()){
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

        if (!discImage->isNull()) {
            g.saveState();
            auto discCentreX = 200;
            auto discCentreY = bounds.getCentreY()+50;
            auto imageCentre = juce::Point<int>(discCentreX, discCentreY);

            // Applica una trasformazione per ruotare tutto attorno al suo centro
            g.addTransform(juce::AffineTransform::rotation(rotationAngle, imageCentre.getX(), imageCentre.getY()));

            float radius = discImage->getHeight() / 2.0f;
            juce::Point<float> centre(discCentreX, discCentreY);

            // Creazione del gradiente per il cerchio metallizzato
            float outerRadius = discImage->getHeight() / 2.0f + 20.0f;
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
            g.drawImageAt(*discImage.get(), discCentreX - discImage->getWidth() / 2, discCentreY - static_cast<float>(discImage->getHeight()) / 2);
            // Altre operazioni di disegno...

            // Continua con altre operazioni di disegno...
            g.restoreState();

        }
        // Coordinate desiderate per il centro dell'immagine
        float desiredCentreX = 400.0f;
        float desiredCentreY = 250.0f;

        // Calcola lo spostamento necessario per centrare l'immagine in (desiredCentreX, desiredCentreY)
        float translateX = desiredCentreX - (armImage->getWidth() / 2.0f)-10;
        float translateY = desiredCentreY - (armImage->getHeight() / 2.0f)+130;

        // Crea una trasformazione affine per traslare e poi ruotare l'immagine
        auto transform = juce::AffineTransform::translation(translateX, translateY)
            .rotated(juce::degreesToRadians(20.0f), desiredCentreX, desiredCentreY);

        // Disegna l'immagine con la trasformazione applicata
        g.drawImageTransformed(*armImage.get(), transform, false);


        auto thumbnailBounds = juce::Rectangle<int>(10, 40, bounds.getWidth() - 20, 200); // Adatta questi valori al tuo layout
        auto trackSpecsBounds = juce::Rectangle<int>(10, 10, bounds.getWidth() - 20, 30);
        // Disegna lo sfondo della thumbnail
        juce::ColourGradient thumbnailGradient(juce::Colour(54, 54, 54), thumbnailBounds.getX(), thumbnailBounds.getY(),
            juce::Colour(30, 30, 30), thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);
        g.setGradientFill(thumbnailGradient); 
        g.fillRect(thumbnailBounds);
       
        auto& thumbnail = djAudioPlayer->getThumbnail();
        auto& trackName = playlist->getChosenTrackSpecs();
        if (thumbnail.getNumChannels() > 0) {
            // Disegna la parte della thumbnail relativa all'audio non ancora riprodotto
          
            g.setColour(juce::Colours::darkgrey);
            thumbnail.drawChannel(g, thumbnailBounds, 0, thumbnail.getTotalLength(), 0, 1.0f);
            g.setColour(juce::Colours::silver);
            g.fillRect(trackSpecsBounds);
            g.setColour(juce::Colours::black);
            g.drawFittedText(trackName.name,trackSpecsBounds,juce::Justification::centred,1);
            
            // Calcola la larghezza della parte riprodotta e disegnala in oro
            auto audioPosition = (float)djAudioPlayer->getTransportSource()->getCurrentPosition();
            auto proportionPlayed = audioPosition / thumbnail.getTotalLength();
            auto playedWidth = proportionPlayed * thumbnailBounds.getWidth();

            g.setColour(juce::Colour(246, 207, 0));
            thumbnail.drawChannel(g, juce::Rectangle<int>(thumbnailBounds.getX(), thumbnailBounds.getY(), playedWidth, thumbnailBounds.getHeight()), 0, audioPosition, 0, 1.0f);

            // Disegna l'indicatore della posizione corrente
            g.setColour(juce::Colours::red);
            auto lineX = thumbnailBounds.getX() + playedWidth;
            g.drawLine(lineX, thumbnailBounds.getY(), lineX, thumbnailBounds.getBottom(), 2.0f);

            juce::ColourGradient silverGradient(juce::Colours::white.withAlpha(0.8f), thumbnailBounds.getX(), thumbnailBounds.getY(),
                juce::Colours::grey.withAlpha(0.5f), thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);
            g.setGradientFill(silverGradient);
            g.drawRect(10.0f, 10.0f, (float)bounds.getWidth() - 20.0f, 230.0f, 5.0f);

        }
        else {

            // Nessun file caricato, mostra solo lo sfondo
            juce::ColourGradient gradient(juce::Colour(54, 54, 54), thumbnailBounds.getX(), thumbnailBounds.getY(),
                juce::Colour(30, 30, 30), thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);
            g.setGradientFill(gradient);
            g.fillRect(thumbnailBounds); juce::ColourGradient silverGradient(juce::Colours::white.withAlpha(0.8f), thumbnailBounds.getX(), thumbnailBounds.getY(),
                juce::Colours::grey.withAlpha(0.5f), thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);
            g.setGradientFill(silverGradient);

           g.setGradientFill(silverGradient);
            g.drawRect(thumbnailBounds.reduced(1.0f),5);

         
            g.setColour(juce::Colours::gold);
            g.setFont(20.0f);
            g.drawFittedText("Choose an audio file", thumbnailBounds, juce::Justification::centred, 1);
        }}
        else {
            g.fillAll(juce::Colours::darkgrey);
        }
    
   
}


void DeckGUI::timerCallback() {
    if (!playlist->isVisible()) {
        if (djAudioPlayer->getTransportSource()->isPlaying()) {
            const float rotationSpeed = 2 * juce::MathConstants<float>::twoPi; // Radianti al secondo
            const float frameTime = 1.0f / 60.0f; // Tempo per frame a 60 FPS

            // Aggiorna l'angolo di rotazione
            rotationAngle += rotationSpeed * frameTime;

            // Assicurati che l'angolo rimanga valido
            if (rotationAngle >= juce::MathConstants<float>::twoPi)
                rotationAngle -= juce::MathConstants<float>::twoPi;

            // Richiedi il ridisegno del componente
            repaint();
        }
        else {
            repaint();
        }
    }
}


void DeckGUI::resized()
{
    const int labelWidth = 110;
    const int labelHeight = 30;
    int buttonHeight = 60;
    int buttonGap = 20;
    int buttonWidth = (getWidth()-5*buttonGap) / 4;
    int sliderWidth = 80;
    int sliderHeight = 250;
    const int sliderLabelWidth = 30;
    const int sliderLabelHeight = 20;
    int sliderGap = 10;
    float buttonY = getHeight() - 100;


   
   
    playButton.setBounds(buttonGap, buttonY, buttonWidth, buttonHeight);
    pauseButton.setBounds(playButton.getRight()+buttonGap, buttonY, buttonWidth, buttonHeight);
    stopButton.setBounds(pauseButton.getRight() + buttonGap, buttonY, buttonWidth, buttonHeight);

    loadButton.setBounds(stopButton.getRight() + buttonGap, buttonY, buttonWidth, buttonHeight);
   
   
    
    volSlider.setBounds(getLocalBounds().getCentreX()+sliderWidth, getLocalBounds().getCentreY() - sliderHeight / 2+100, sliderWidth, sliderHeight);
    speedSlider.setBounds(volSlider.getRight(), getLocalBounds().getCentreY() - sliderHeight / 2+100, sliderWidth, sliderHeight);
    posSlider.setBounds(speedSlider.getRight(), getLocalBounds().getCentreY() - sliderHeight / 2+100, sliderWidth, sliderHeight);

    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, sliderLabelWidth, sliderLabelHeight);

    playlist->setBounds(50, 50, getWidth() - 100, getHeight() - 100);
}

void DeckGUI::showPlaylist() {
    playlist->setVisible(true);
    playlist->toFront(true);
    resized();
}