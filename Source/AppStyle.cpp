#include "AppStyle.h"

juce::Colour createGradientColour(const juce::Colour& lightColour, const juce::Colour& darkColour, float distanceFromEdge)
{
    float fraction = distanceFromEdge / 10.0f; 
    juce::Colour gradientColour = lightColour.withMultipliedBrightness(1.0f - fraction)
        .withMultipliedSaturation(1.0f - fraction) 
        .interpolatedWith(darkColour, fraction); 
    return gradientColour;
}

const juce::Colour AppColours::internalColour = createGradientColour(
    juce::Colour(0xfff5deb3), 
    juce::Colour(0xff8b4513),  
    10.0f 
);

AppLAF::AppLAF() {
    
    knobImage = juce::ImageFileFormat::loadFrom(juce::File("C:/Users/cesar/Desktop/knob.png"));
    knobImage = knobImage.rescaled(120, 120);
}


void AppLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, const float rotaryStartAngle,
    const float rotaryEndAngle, juce::Slider& slider) 
{
    if (!knobImage.isValid()) {
        return; // Assicurati che l'immagine sia valida
    }

    auto radius = juce::jmin(width / 2, height / 2);
    auto centreX = x + width * 0.5f;
    auto centreY = y + height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto rh = radius * 2.0f;

    // Calcola l'angolo del knob in base al valore attuale dello slider
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Prepara l'immagine per la rotazione attorno al suo centro
    juce::AffineTransform transform = juce::AffineTransform::rotation(angle, knobImage.getWidth() / 2.0f, knobImage.getHeight() / 2.0f)
        .translated(centreX - knobImage.getWidth() / 2.0f,
            centreY - knobImage.getHeight() / 2.0f);

    g.drawImageTransformed(knobImage, transform, false);
}



void AppLAF::drawLabel(juce::Graphics& g, juce::Label& label) {
    g.fillAll(juce::Colours::transparentBlack); // Rendi lo sfondo trasparente o di un altro colore se necessario

    auto bounds = label.getLocalBounds().toFloat();
    auto cornerSize = 4.0f; // Raggio degli angoli arrotondati per il bordo

    // Sfondo scuro della Label
    g.setColour(juce::Colours::darkgrey.withAlpha(0.5f)); // Colore e trasparenza dello sfondo
    g.fillRoundedRectangle(bounds, cornerSize);

    // Effetto di profondità
    juce::Path depthEffect;
    depthEffect.addRoundedRectangle(bounds.reduced(1), cornerSize);
    g.setColour(juce::Colours::black.withAlpha(0.2f)); // Colore dell'effetto di profondità
    g.strokePath(depthEffect, juce::PathStrokeType(2.0f)); // Spessore e stile del bordo

    // Testo della Label
    g.setColour(label.findColour(juce::Label::textColourId)); // Usa il colore del testo definito nella Label
    g.setFont(label.getFont()); // Usa il font definito nella Label

    auto textArea = bounds.reduced(4); // Riduci leggermente l'area del testo per il padding
    g.drawFittedText(label.getText(), textArea.toType<int>(), label.getJustificationType(),
        juce::jmax(1, (int)(textArea.getHeight() / g.getCurrentFont().getHeight())),
        label.getMinimumHorizontalScale()); // Disegna il testo adattandolo all'area disponibile
}

void AppLAF::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour& backgroundColour, bool isMouseOverButton,
    bool isButtonDown) {
    auto buttonBounds = button.getLocalBounds().toFloat();
    auto cornerRadius = 6.0f; // Raggio degli angoli arrotondati del pulsante

    // Scegli il colore del pulsante in base allo stato
    auto baseColour = isButtonDown ? juce::Colours::darkgrey.darker() : juce::Colours::darkgrey;
    g.setColour(baseColour);

    // Disegna il fondo del pulsante con angoli arrotondati
    g.fillRoundedRectangle(buttonBounds, cornerRadius);

    // Aggiungi un effetto di luce per dare l'impressione che il pulsante sporga
    if (!isButtonDown) {
        juce::ColourGradient gradient(juce::Colours::white.withAlpha(0.3f), buttonBounds.getX(), buttonBounds.getY(),
            juce::Colours::transparentBlack, buttonBounds.getX(), buttonBounds.getBottom(), false);
        g.setGradientFill(gradient);
        g.fillRoundedRectangle(buttonBounds, cornerRadius);
    }

    // Aggiungi un bordo scuro per definire meglio il pulsante
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawRoundedRectangle(buttonBounds.reduced(1.0f), cornerRadius, 1.0f); // L'ultimo parametro è lo spessore del bordo

    // Gestisci manualmente il disegno del testo se il pulsante contiene del testo
    if (button.getButtonText().isNotEmpty()) {
      

        g.drawText(button.getButtonText(), button.getLocalBounds(),
            juce::Justification::centred, true);
    }
}
