#include "DeckGUI.h"


DeckGUI::DeckGUI(DjAudioPlayer* audioPlayer) : djAudioPlayer{ audioPlayer }, playlist(std::make_unique<Playlist>(audioPlayer))
{
    

    //Initialize ptrs to the images defined in AppStyle class.
     
    discImage = std::make_unique<juce::Image>(appLAF.getDiscImage()->rescaled(appLAF.getDiscImage()->getWidth() / 4, appLAF.getDiscImage()->getHeight() / 4));
    armImage = std::make_unique<juce::Image>(appLAF.getArmImage()->rescaled(appLAF.getArmImage()->getWidth() / 5, appLAF.getArmImage()->getHeight() / 5));


     //define buttons properties.
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


    //define slider properties
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

    //add playlist component to the UI and set it invisible by default.
    addAndMakeVisible(playlist.get());
    playlist->setVisible(false);

    //start the timer and set the callback each 60 ms.
    startTimerHz(60);
}

//Override buttonClicked function, to set various callbacks every time a button is clicked.
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

//Override sliderValueChanged function, to set various callbacks every time a slider gets scrolled.
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
       //definitions of local variables.
        auto bounds = getLocalBounds().toFloat();
        auto discCentreX = 200;
        auto discCentreY = bounds.getCentreY() + 50;
        auto imageCentre = juce::Point<int>(discCentreX, discCentreY);
        float radius = discImage->getHeight() / 2.0f;
        juce::Point<float> centre(discCentreX, discCentreY);
        float outerRadius = discImage->getHeight() / 2.0f + 20.0f;
        float innerRadius = outerRadius - 20.0f;
        juce::Path ringPath;
        ringPath.addEllipse(centre.x - outerRadius, centre.y - outerRadius, 2 * outerRadius, 2 * outerRadius);
        ringPath.addEllipse(centre.x - innerRadius, centre.y - innerRadius, 2 * innerRadius, 2 * innerRadius);
        juce::Path goldRingPath;
        goldRingPath.addEllipse(centre.x - radius, centre.y - radius, 2 * radius, 2 * radius);
        float desiredCentreX = 400.0f;
        float desiredCentreY = 250.0f;
        float translateX = desiredCentreX - (armImage->getWidth() / 2.0f) - 10;
        float translateY = desiredCentreY - (armImage->getHeight() / 2.0f) + 130;
        auto thumbnailBounds = juce::Rectangle<int>(10, 40, bounds.getWidth() - 20, 200);
        auto trackSpecsBounds = juce::Rectangle<int>(10, 10, bounds.getWidth() - 20, 30);
        auto& thumbnail = djAudioPlayer->getThumbnail();
        auto& trackName = playlist->getChosenTrackSpecs();
        auto audioPosition = (float)djAudioPlayer->getTransportSource()->getCurrentPosition();
        auto proportionPlayed = audioPosition / thumbnail.getTotalLength();
        auto playedWidth = proportionPlayed * thumbnailBounds.getWidth();


        juce::ColourGradient gradient(
            AppColours::customDarkGrey,
            bounds.getTopLeft(),
            AppColours::customDarkerGrey,
            bounds.getBottomRight(),
            false);
        juce::ColourGradient greyGradient(juce::Colours::grey, centre.withY(centre.y - outerRadius),
            juce::Colours::lightgrey, centre.withY(centre.y + outerRadius), false);
        greyGradient.addColour(0.5, juce::Colours::darkgrey);
        juce::Colour goldStart(255, 215, 0);
        juce::Colour goldEnd(178, 145, 0);
        juce::ColourGradient goldGradient(goldStart, centre, goldEnd, centre.withY(centre.y - outerRadius), false);
        juce::ColourGradient thumbnailGradient(AppColours::customDarkGrey, thumbnailBounds.getX(), thumbnailBounds.getY(),
            AppColours::customDarkerGrey, thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);
        auto lineX = thumbnailBounds.getX() + playedWidth;
        juce::ColourGradient silverGradient(juce::Colours::white.withAlpha(0.8f), thumbnailBounds.getX(), thumbnailBounds.getY(),
            juce::Colours::grey.withAlpha(0.5f), thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);


        //fill the background with the custom gradient.
        g.setGradientFill(gradient);
        g.fillRect(bounds);
        g.setColour(juce::Colours::black);

        if (!discImage->isNull()) {
            //save the state, so that it will be reset to default after the transform process.
            g.saveState();

            /*
            these are the steps to achieve disc rotation:
            1) define float rotationAngle member variable, to store its active status. It's incremented in the timer cb.
            2) Draw both the silvered and the golden ring.
            3) Draw the shadowed ovals and modify the shadow in relation to the angle, to simulate a light effect.
            perform the rotation at each repaint.
            4) Lastly, draw the disc image.
            */
            g.addTransform(juce::AffineTransform::rotation(rotationAngle, imageCentre.getX(), imageCentre.getY()));

            g.setGradientFill(greyGradient);
            g.fillPath(ringPath);

            g.setGradientFill(goldGradient);
            g.strokePath(goldRingPath, juce::PathStrokeType(4.0f));

            for (int i = 0; i < 40; ++i) {
                float angle = juce::MathConstants<float>::twoPi * i / 40;
                juce::Point<float> ovalCentre = centre + juce::Point<float>(std::cos(angle), std::sin(angle)) * (innerRadius + 10.0f);

                juce::ColourGradient ovalGradient(juce::Colours::darkgrey, ovalCentre.translated(-2, -2), juce::Colours::lightgrey, ovalCentre.translated(2, 2), false);
                g.setGradientFill(ovalGradient);
                g.fillEllipse(ovalCentre.x - 5, ovalCentre.y - 5, 10, 10);
            }
            
            g.drawImageAt(*discImage.get(), discCentreX - discImage->getWidth() / 2, discCentreY - static_cast<float>(discImage->getHeight()) / 2);
            g.restoreState();

        }
        /*
        Define a juce::AffineTransform combining a translation and a rotation, to draw the arm image.
        */
        auto transform = juce::AffineTransform::translation(translateX, translateY)
            .rotated(juce::degreesToRadians(20.0f), desiredCentreX, desiredCentreY);
        g.drawImageTransformed(*armImage.get(), transform, false);

        /*
       Draw the thumbnail rectangle.
       1) If its source is initialized to one having
       at least one active channel:
       1.1) Draw the thumbnail's track.
       1.2) Draw the specifications rectangle above and insert the file name inside it,
       with a juce::JustificationType::centred.
       1.3) Dynamically draw the thumbnail with a golden pen, only for the already played section.
       This will return a pleasant looking effect and separate the two parts of the track.
       1.4) Draw the red marker, which follows the track.

       2) If, instead, there's no available channel:
       2.1) Draw a background gradient, similiar to the GUI's background shade.
       2.2) Draw a fitted text, which serves as an instruction, inducing the track selection.
        
        */
        g.setGradientFill(thumbnailGradient); 
        g.fillRect(thumbnailBounds);
       
        if (thumbnail.getNumChannels() > 0) {
          
            g.setColour(juce::Colours::darkgrey);
            thumbnail.drawChannel(g, thumbnailBounds, 0, thumbnail.getTotalLength(), 0, 1.0f);
            g.setColour(juce::Colours::silver);
            g.fillRect(trackSpecsBounds);
            g.setColour(juce::Colours::black);
            g.drawFittedText(trackName.name,trackSpecsBounds,juce::Justification::centred,1);
            

            g.setColour(juce::Colour(246, 207, 0));
            thumbnail.drawChannel(g, juce::Rectangle<int>(thumbnailBounds.getX(), thumbnailBounds.getY(), playedWidth, thumbnailBounds.getHeight()), 0, audioPosition, 0, 1.0f);

            g.setColour(juce::Colours::red);
            g.drawLine(lineX, thumbnailBounds.getY(), lineX, thumbnailBounds.getBottom(), 2.0f);

            g.setGradientFill(silverGradient);
            g.drawRect(10.0f, 10.0f, (float)bounds.getWidth() - 20.0f, 230.0f, 5.0f);

        }
        else {
    
            juce::ColourGradient gradient(AppColours::customDarkGrey, thumbnailBounds.getX(), thumbnailBounds.getY(),
                AppColours::customGrey, thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);
            juce::ColourGradient silverGradient(juce::Colours::white.withAlpha(0.8f), thumbnailBounds.getX(), thumbnailBounds.getY(),
                juce::Colours::grey.withAlpha(0.5f), thumbnailBounds.getX(), thumbnailBounds.getBottom(), false);


            g.setGradientFill(gradient);
            g.fillRect(thumbnailBounds);
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

/*
In the timer cb method the rotation parameters are used to increment the rotation angle at each repaint:
the angular speed is set to 2*pi rad/s. The rotation angle is periodically decremented, according to the sine and cosine domains.
*/
void DeckGUI::timerCallback() {
    if (!playlist->isVisible()) {

        if (djAudioPlayer->getTransportSource()->isPlaying()) {
            rotationAngle += rotationSpeed * frameTime;
            if (rotationAngle >= juce::MathConstants<float>::twoPi)
                rotationAngle -= juce::MathConstants<float>::twoPi;
            repaint();
        }
        else {
            repaint();
        }
    }
}


void DeckGUI::resized()
{
    //local variables definitions
    const float labelWidth = 110.0f;
    const float labelHeight = 30.0f;
    const float buttonHeight = 60.0f;
    const float buttonGap = 20.0f;
    float buttonWidth = (getWidth()-5*buttonGap) / 4;
    const float sliderWidth = 80.0f;
    float sliderHeight = 250.0f;
    const float sliderLabelWidth = 30.0f;
    const float sliderLabelHeight = 20.0f;
    float sliderGap = 10.0f;
    float buttonY = static_cast<float>(getHeight()) - 100.0f;

   
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

/*
method that handles the Playlist component display.
*/
void DeckGUI::showPlaylist() {
    playlist->setVisible(true);
    playlist->toFront(true);
    resized();
}