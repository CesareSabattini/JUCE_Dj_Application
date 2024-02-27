#include "AppStyle.h"

/*
AppColours static member variables definition.
*/
const juce::Colour AppColours::customGrey = juce::Colour(30, 30, 30);
const juce::Colour AppColours::customDarkGrey = juce::Colour(54, 54, 54);
const juce::Colour AppColours::customTransparentBlack = juce::Colours::transparentBlack;
const juce::Colour AppColours::customBlack = juce::Colours::black;




AppLAF::AppLAF() {
    
    juce::Typeface::Ptr tface = juce::Typeface::createSystemTypefaceFor(BinaryData::AntaRegular_ttf, BinaryData::AntaRegular_ttfSize);
    antaRegular = juce::Font(tface);

    juce::File discFile("C:/Users/cesar/Desktop/disc.png");
    discImage = juce::ImageFileFormat::loadFrom(discFile);

    juce::File armFile("C:/Users/cesar/Desktop/needle_arm.png");
    armImage = juce::ImageFileFormat::loadFrom(armFile);

}


//inherited method to customize a juce::Label component.
void AppLAF::drawLabel(juce::Graphics& g, juce::Label& label) {

    //definitions of the local variables.
    auto bounds = label.getLocalBounds().toFloat();
    auto cornerSize = 4.0f;
    auto textArea = bounds.reduced(4);
    juce::ColourGradient silverGradient(juce::Colours::white.withAlpha(0.8f), bounds.getX(), bounds.getY(),
        juce::Colours::grey.withAlpha(0.5f), bounds.getX(), bounds.getBottom(), false);


    //fill the background.
    g.fillAll(AppColours::customTransparentBlack);
    //draw the border.
    g.setGradientFill(silverGradient);
    g.fillRoundedRectangle(bounds, cornerSize);

    //depth effect.
    juce::Path depthEffect;
    depthEffect.addRoundedRectangle(bounds.reduced(1), cornerSize);
    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.strokePath(depthEffect, juce::PathStrokeType(2.0f));

    //customize the text.
    g.setColour(AppColours::customGrey);
    g.setFont(label.getFont());

    //draw the text in the center
    g.drawFittedText(label.getText(), textArea.toType<int>(), juce::Justification::centred,
        juce::jmax(1, (int)(textArea.getHeight() / g.getCurrentFont().getHeight())),
        label.getMinimumHorizontalScale());
}

//inherited method to customize a juce::Button component's background.

void AppLAF::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour& backgroundColour, bool isMouseOverButton,
    bool isButtonDown) {
    auto buttonBounds = button.getLocalBounds().toFloat();
    auto cornerRadius = 6.0f; // Raggio degli angoli arrotondati del pulsante

    // Scegli il colore del pulsante in base allo stato
    auto baseColour = isButtonDown ? juce::Colour(30, 30, 30) : juce::Colours::darkgrey;
    g.setColour(baseColour);

    // Disegna il fondo del pulsante con angoli arrotondati
    g.fillRoundedRectangle(buttonBounds, cornerRadius);

    // Aggiungi un effetto di luce per dare l'impressione che il pulsante sporga
    if (!isButtonDown) {
        juce::ColourGradient gradient(juce::Colour(54, 54, 54), buttonBounds.getX(), buttonBounds.getY(),
            juce::Colour(30, 30, 30), buttonBounds.getX(), buttonBounds.getBottom(), false);
        g.setGradientFill(gradient);
        g.fillRect(buttonBounds);
    }

    // Disegna un bordo argentato lucente
    juce::ColourGradient silverGradient(juce::Colours::white.withAlpha(0.8f), buttonBounds.getX(), buttonBounds.getY(),
        juce::Colours::grey.withAlpha(0.5f), buttonBounds.getX(), buttonBounds.getBottom(), false);
    g.setGradientFill(silverGradient);
    g.drawRect(buttonBounds.reduced(1.0f), 2.0f); // Usa uno spessore maggiore per un bordo più evidente

    // Per un effetto extra lucente, aggiungi un sottile bordo chiaro nella parte superiore del pulsante
    if (!isButtonDown) {
        auto highlightBounds = buttonBounds.reduced(2.0f);
        highlightBounds.removeFromBottom(buttonBounds.getHeight() / 2);
        juce::ColourGradient highlightGradient(juce::Colours::white.withAlpha(0.5f), highlightBounds.getX(), highlightBounds.getY(),
            juce::Colours::transparentWhite, highlightBounds.getX(), highlightBounds.getBottom(), false);
        g.setGradientFill(highlightGradient);
        g.drawRect(highlightBounds, 1.0f); // Sottile bordo superiore per un effetto lucente
    }

  
}

//inherited method to customize a juce::Button component's text.

void AppLAF::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Font buttonFont = antaRegular;
    const float fontHeight = 20.0f;

    buttonFont.setHeight(fontHeight);
    buttonFont.setBold(true);
    g.setFont(buttonFont);
    g.setColour(juce::Colours::gold);
    g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
}

/*
inherited method to customize a linear juce::Slider component.
It will differently render it based on its orientation.
*/

void AppLAF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider) {


    if (style == juce::Slider::LinearVertical) {

        //definitions of the local variables.
        juce::Rectangle<float> track(x + width / 2 - 2, y, 4, height);
        const int numMarks = 10;
        const int knobWidth = 20;
        const int knobHeight = 10;
        juce::Rectangle<float> knob(x + (width - knobWidth) / 2, sliderPos - (knobHeight / 2), knobWidth, knobHeight);

        //fill the track
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(track); 

      

        //draw marks and texts
        g.setColour(juce::Colours::gold);
        for (int i = 0; i <= numMarks; ++i) {
            float pos = y + (i * height / numMarks);
            g.drawHorizontalLine(pos,static_cast<float>( x + width )/ 2 - 5, static_cast<float>(x + width) / 2 + 5);
            juce::String valueString = juce::String((numMarks - i) * (slider.getMaximum() / numMarks), 0);
            g.drawText(valueString, x + width / 2 + 10, pos - 10, 30, 20, juce::Justification::centredLeft);
        }

        // Draw the knob
        g.setColour(juce::Colours::grey);
        g.fillRect(knob);
    }


    if (style == juce::Slider::LinearHorizontal) {
        //definitions of the local variables.
        juce::Rectangle<float> track(x, y + height / 2 - 2, width, 4);
        const int numMarks = 10;
        const int knobWidth = 10; // Altezza del knob
        const int knobHeight = 20; // Larghezza del knob, maggiore per visibilità orizzontale
        juce::Rectangle<float> knob(sliderPos - (knobWidth / 2), y + (height - knobHeight) / 2, knobWidth, knobHeight);



        //draw the track
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(track);

        // draw marks and texts
        g.setColour(juce::Colours::gold);
        for (int i = 0; i <= numMarks; ++i) {
            float pos = x + (i * width / numMarks);
            g.drawVerticalLine(pos, y + height / 2 - 5, y + height / 2 + 5);          
        }

      //draw the knob
        g.setColour(juce::Colours::grey);
        g.fillRect(knob);
    }
}

/*
inherited method to customize a juce::TableHeader component.
*/


void AppLAF::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header) {
    //definitions of the local variables.
    juce::ColourGradient gradient(juce::Colours::silver, 0, 0, juce::Colours::grey, header.getWidth(), header.getHeight(), false);
    auto r = header.getLocalBounds();

    //fill the background.
    g.setGradientFill(gradient);
    g.fillRect(r);
    g.setColour(juce::Colours::gold);
    g.drawRect(r);
}




juce::Image* AppLAF::getDiscImage() {
    return &discImage;
}
juce::Image* AppLAF::getArmImage() {
    return &armImage;

}