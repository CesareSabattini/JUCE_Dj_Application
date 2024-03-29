#include "DeckGUI.h"

DeckGUI::DeckGUI(std::shared_ptr<DjAudioPlayer> audioPlayer)
    : djAudioPlayer{audioPlayer},
      playlist(std::make_unique<Playlist>(audioPlayer)) {

    // Initialize ptrs to the images defined in AppStyle class.

    discImage = std::make_unique<juce::Image>(appLAF.getDiscImage()->rescaled(
        appLAF.getDiscImage()->getWidth() / 4,
        appLAF.getDiscImage()->getHeight() / 4));
    armImage = std::make_unique<juce::Image>(
        appLAF.getArmImage()->rescaled(appLAF.getArmImage()->getWidth() / 5,
                                       appLAF.getArmImage()->getHeight() / 5));

    // define buttons properties.
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
    loadButton.addListener(this);
    ;
    addAndMakeVisible(loadButton);

    // define slider properties
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

    // define labels props
    addAndMakeVisible(volLabel);
    volLabel.setText("Gain", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centred);
    volLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    volLabel.setColour(juce::Label::textColourId, juce::Colours::gold);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    speedLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    speedLabel.setColour(juce::Label::textColourId, juce::Colours::gold);

    addAndMakeVisible(posLabel);
    posLabel.setText("Pos", juce::dontSendNotification);
    posLabel.setJustificationType(juce::Justification::centred);
    posLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    posLabel.setColour(juce::Label::textColourId, juce::Colours::gold);

    // add playlist component to the UI and set it invisible by default.
    addAndMakeVisible(playlist.get());
    playlist->setVisible(false);

    // start the timer and set the callback each 60 ms.
    startTimerHz(60);
}

// Override buttonClicked function, to set various callbacks every time a button
// is clicked.
void DeckGUI::buttonClicked(juce::Button *button) {
    if (button == &playButton) {
        isPlaying = true;
        djAudioPlayer->start();
    }
    if (button == &pauseButton) {
        djAudioPlayer->pause();
        isPlaying = false;
    }
    if (button == &stopButton) {
        djAudioPlayer->stop();
        isPlaying = false;
    }
    if (button == &loadButton) {

        playlist->loadTracksFromDirectory();
        playlist->resized();
        showPlaylist();
    }

    repaint();
}

DeckGUI::~DeckGUI() {}

// Override sliderValueChanged function, to set various callbacks every time a
// slider gets scrolled.
void DeckGUI::sliderValueChanged(juce::Slider *slider) {

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

void DeckGUI::paint(juce::Graphics &g) {

    if (!playlist->isVisible()) {
        // definitions of local variables.
        auto bounds = getLocalBounds().toFloat();
        float discCentreX = static_cast<float>(bounds.getCentreX() / 2 + 50);
        float discCentreY = static_cast<float>(bounds.getCentreY() + 50);
        auto imageCentre = juce::Point<float>(discCentreX, discCentreY);
        float radius = discImage->getHeight() / 2.0f;
        juce::Point<float> centre(discCentreX, discCentreY);
        float outerRadius = discImage->getHeight() / 2.0f + 20.0f;
        float innerRadius = outerRadius - 20.0f;
        juce::Path ringPath;
        ringPath.addEllipse(centre.x - outerRadius, centre.y - outerRadius,
                            2 * outerRadius, 2 * outerRadius);
        ringPath.addEllipse(centre.x - innerRadius, centre.y - innerRadius,
                            2 * innerRadius, 2 * innerRadius);
        juce::Path goldRingPath;
        goldRingPath.addEllipse(centre.x - radius, centre.y - radius,
                                2 * radius, 2 * radius);
        float desiredCentreX = 400.0f;
        float desiredCentreY = 250.0f;
        float translateX = bounds.getCentreX() + 20;
        float translateY = bounds.getCentreY() / 2 + 100;
        auto thumbnailBounds = juce::Rectangle<int>(
            10, 40, static_cast<int>(bounds.getWidth() - 20),
            bounds.getHeight() / 5);
        auto trackSpecsBounds = juce::Rectangle<int>(
            10, 10, static_cast<int>(bounds.getWidth() - 20), 30);
        auto &thumbnail = djAudioPlayer->getThumbnail();
        auto &trackName = playlist->getChosenTrackSpecs();
        auto audioPosition =
            (float)djAudioPlayer->getTransportSource()->getCurrentPosition();
        auto proportionPlayed = audioPosition / thumbnail.getTotalLength();
        auto playedWidth = proportionPlayed * thumbnailBounds.getWidth();

        juce::ColourGradient gradient(
            AppColours::customDarkGrey, bounds.getTopLeft(),
            AppColours::customDarkerGrey, bounds.getBottomRight(), false);
        juce::ColourGradient greyGradient(
            juce::Colours::grey, centre.withY(centre.y - outerRadius),
            juce::Colours::lightgrey, centre.withY(centre.y + outerRadius),
            false);
        greyGradient.addColour(0.5, juce::Colours::darkgrey);
        juce::Colour goldStart(255, 215, 0);
        juce::Colour goldEnd(178, 145, 0);
        juce::ColourGradient goldGradient(goldStart, centre, goldEnd,
                                          centre.withY(centre.y - outerRadius),
                                          false);
        juce::ColourGradient thumbnailGradient(
            AppColours::customDarkGrey,
            static_cast<float>(thumbnailBounds.getX()),
            static_cast<float>(thumbnailBounds.getY()),
            AppColours::customDarkerGrey,
            static_cast<float>(thumbnailBounds.getX()),
            static_cast<float>(thumbnailBounds.getBottom()), false);
        auto lineX = thumbnailBounds.getX() + playedWidth;
        juce::ColourGradient silverGradient(
            juce::Colours::white.withAlpha(0.8f),
            static_cast<float>(thumbnailBounds.getX()),
            static_cast<float>(thumbnailBounds.getY()),
            juce::Colours::grey.withAlpha(0.5f),
            static_cast<float>(thumbnailBounds.getX()),
            static_cast<float>(thumbnailBounds.getBottom()), false);

        // fill the background with the custom gradient.
        g.setGradientFill(gradient);
        g.fillRect(bounds);
        g.setColour(juce::Colours::black);

        if (!discImage->isNull()) {
            // save the state, so that it will be reset to default after the
            // transform process.
            g.saveState();

            /*
            these are the steps to achieve disc rotation:
            1) define float rotationAngle member variable, to store its active
            status. It's incremented in the timer cb. 2) Draw both the silvered
            and the golden ring. 3) Draw the shadowed ovals and modify the
            shadow in relation to the angle, to simulate a light effect. perform
            the rotation at each repaint. 4) Lastly, draw the disc image.
            */
            g.addTransform(juce::AffineTransform::rotation(
                rotationAngle, static_cast<float>(imageCentre.getX()),
                static_cast<float>(imageCentre.getY())));

            g.setGradientFill(greyGradient);
            g.fillPath(ringPath);

            g.setGradientFill(goldGradient);
            g.strokePath(goldRingPath, juce::PathStrokeType(4.0f));

            for (int i = 0; i < 40; ++i) {
                float angle = juce::MathConstants<float>::twoPi * i / 40;
                juce::Point<float> ovalCentre =
                    centre +
                    juce::Point<float>(std::cos(angle), std::sin(angle)) *
                        (innerRadius + 10.0f);

                juce::ColourGradient ovalGradient(
                    juce::Colours::darkgrey, ovalCentre.translated(-2, -2),
                    juce::Colours::lightgrey, ovalCentre.translated(2, 2),
                    false);
                g.setGradientFill(ovalGradient);
                g.fillEllipse(ovalCentre.x - 5, ovalCentre.y - 5, 10, 10);
            }

            g.drawImageAt(
                *discImage.get(),
                static_cast<int>(discCentreX -
                                 static_cast<float>(discImage->getWidth() / 2)),
                static_cast<int>(discCentreY -
                                 static_cast<float>(discImage->getHeight()) /
                                     2));
            g.restoreState();
        }
        /*
        Define a juce::AffineTransform combining a translation and a rotation,
        to draw the arm image.
        */
        auto transform =
            juce::AffineTransform::translation(translateX, translateY)
                .rotated(juce::degreesToRadians(20.0f), desiredCentreX,
                         desiredCentreY);
        g.drawImageTransformed(*armImage.get(), transform, false);

        /*
       Draw the thumbnail rectangle.
       1) If its source is initialized to one having
       at least one active channel:
       1.1) Draw the thumbnail's track.
       1.2) Draw the specifications rectangle above and insert the file name
       inside it, with a juce::JustificationType::centred. 1.3) Dynamically draw
       the thumbnail with a golden pen, only for the already played section.
       This will return a pleasant looking effect and separate the two parts of
       the track. 1.4) Draw the red marker, which follows the track.

       2) If, instead, there's no available channel:
       2.1) Draw a background gradient, similiar to the GUI's background shade.
       2.2) Draw a fitted text, which serves as an instruction, inducing the
       track selection.

        */
        g.setGradientFill(thumbnailGradient);
        g.fillRect(thumbnailBounds);

        if (thumbnail.getNumChannels() > 0) {

            g.setColour(juce::Colours::darkgrey);
            thumbnail.drawChannel(g, thumbnailBounds, 0,
                                  thumbnail.getTotalLength(), 0, 1.0f);
            g.setColour(juce::Colours::silver);
            g.fillRect(trackSpecsBounds);
            g.setColour(juce::Colours::black);
            g.drawFittedText(trackName.name, trackSpecsBounds,
                             juce::Justification::centred, 1);

            g.setColour(juce::Colour(246, 207, 0));
            thumbnail.drawChannel(
                g,
                juce::Rectangle<int>(
                    thumbnailBounds.getX(), thumbnailBounds.getY(),
                    static_cast<int>(playedWidth), thumbnailBounds.getHeight()),
                0.0, static_cast<double>(audioPosition), 0, 1.0f);

            g.setColour(juce::Colours::red);
            g.drawLine(static_cast<float>(lineX),
                       static_cast<float>(thumbnailBounds.getY()),
                       static_cast<float>(lineX),
                       static_cast<float>(thumbnailBounds.getBottom()), 2.0f);

            g.setGradientFill(silverGradient);
            g.drawRect(
                static_cast<juce::Rectangle<int>>(thumbnailBounds.reduced(1)),
                5);

        } else {

            juce::ColourGradient gradient(
                AppColours::customDarkGrey,
                static_cast<float>(thumbnailBounds.getX()),
                static_cast<float>(thumbnailBounds.getY()),
                AppColours::customGrey,
                static_cast<float>(thumbnailBounds.getX()),
                static_cast<float>(thumbnailBounds.getBottom()), false);
            juce::ColourGradient silverGradient(
                juce::Colours::white.withAlpha(0.8f),
                static_cast<float>(thumbnailBounds.getX()),
                static_cast<float>(thumbnailBounds.getY()),
                juce::Colours::grey.withAlpha(0.5f),
                static_cast<float>(thumbnailBounds.getX()),
                static_cast<float>(thumbnailBounds.getBottom()), false);

            g.setGradientFill(gradient);
            g.fillRect(thumbnailBounds);
            g.setGradientFill(silverGradient);
            g.setGradientFill(silverGradient);
            g.drawRect(
                static_cast<juce::Rectangle<int>>(thumbnailBounds.reduced(1)),
                5);
            g.setColour(juce::Colours::gold);
            g.setFont(20.0f);
            g.drawFittedText("Choose an audio file", thumbnailBounds,
                             juce::Justification::centred, 1);
        }
    } else {
    }
}

/*
In the timer cb method the rotation parameters are used to increment the
rotation angle at each repaint: the angular speed is set to 2*pi rad/s. The
rotation angle is periodically decremented, according to the sine and cosine
domains.
*/
void DeckGUI::timerCallback() {
    if (!playlist->isVisible()) {

        if (djAudioPlayer->getTransportSource()->isPlaying()) {
            rotationAngle += rotationSpeed * frameTime;
            if (rotationAngle >= juce::MathConstants<float>::twoPi)
                rotationAngle -= juce::MathConstants<float>::twoPi;
            repaint();
        } else {
            repaint();
        }
    }
}

void DeckGUI::resized() {
    // local variables definitions
    const int buttonHeight = getHeight() / 12;
    const int buttonGap = 20;
    int buttonWidth = (getWidth() - 5 * buttonGap) / 4;
    const int sliderWidth = getWidth() / 10;
    const int sliderHeight = getHeight() / 3;
    const int sliderLabelWidth = sliderWidth - 15;
    const int sliderLabelHeight = 20;
    int buttonY = getHeight() - 100;
    int playlistWidth = getWidth();
    int playlistHeight = getHeight();

    playButton.setBounds(buttonGap, buttonY, buttonWidth, buttonHeight);
    pauseButton.setBounds(playButton.getRight() + buttonGap, buttonY,
                          buttonWidth, buttonHeight);
    stopButton.setBounds(pauseButton.getRight() + buttonGap, buttonY,
                         buttonWidth, buttonHeight);

    loadButton.setBounds(stopButton.getRight() + buttonGap, buttonY,
                         buttonWidth, buttonHeight);

    volSlider.setBounds(getLocalBounds().getRight() - sliderWidth - 10,
                        getLocalBounds().getCentreY() - sliderHeight / 2 + 100,
                        sliderWidth, sliderHeight);
    speedSlider.setBounds(getLocalBounds().getRight() - 2 * sliderWidth - 10,
                          getLocalBounds().getCentreY() - sliderHeight / 2 +
                              100,
                          sliderWidth, sliderHeight);
    posSlider.setBounds(getLocalBounds().getRight() - 3 * sliderWidth - 10,
                        getLocalBounds().getCentreY() - sliderHeight / 2 + 100,
                        sliderWidth, sliderHeight);

    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true,
                              sliderLabelWidth, sliderLabelHeight);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true,
                                sliderLabelWidth, sliderLabelHeight);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true,
                              sliderLabelWidth, sliderLabelHeight);

    playlist->setBounds((getWidth() - playlistWidth) / 2,
                        (getHeight() - playlistHeight) / 2, playlistWidth,
                        playlistHeight);

    volLabel.setBounds(
        volSlider.getBounds().getCentreX() - sliderLabelWidth / 2,
        volSlider.getBounds().getTopRight().getY() - 1.5 * sliderLabelHeight,
        sliderLabelWidth, sliderLabelHeight);
    speedLabel.setBounds(
        speedSlider.getBounds().getCentreX() - sliderLabelWidth / 2,
        volSlider.getBounds().getTopRight().getY() - 1.5 * sliderLabelHeight,
        sliderLabelWidth, sliderLabelHeight);
    posLabel.setBounds(
        posSlider.getBounds().getCentreX() - sliderLabelWidth / 2,
        volSlider.getBounds().getTopRight().getY() - 1.5 * sliderLabelHeight,
        sliderLabelWidth, sliderLabelHeight);
}

/*
method that handles the Playlist component display.
*/
void DeckGUI::showPlaylist() {
    playlist->setVisible(true);
    playlist->toFront(true);
    resized();
}