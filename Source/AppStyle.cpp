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
    const float rotaryEndAngle, juce::Slider& slider) {
    auto size = juce::jmin(width, height);
    auto bounds = juce::Rectangle<float>(x, y, size, size).reduced(10); // Riduci per il padding
    auto centre = bounds.getCentre();
    auto radius = size / 2.0f;

    // Gradiente migliorato della corona esterna
    juce::ColourGradient outerGradient(juce::Colours::darkslategrey, centre, juce::Colours::slategrey, centre, true);
    outerGradient.addColour(0.7, juce::Colours::lightgrey); // Transizione più marcata da scuro a chiaro
    g.setGradientFill(outerGradient);
    g.fillEllipse(bounds);

    // Bordo esterno più scuro e trasparente
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawEllipse(bounds, 1.0f);

    // Disegna le scanalature radiali che congiungono il bordo esterno al knob
    juce::Path scanalaturePath;
    const int numScanalature = 60; // Numero di scanalature ridotto per visibilità
    for (int i = 0; i < numScanalature; ++i) {
        float angle = rotaryStartAngle + i * (rotaryEndAngle - rotaryStartAngle) / numScanalature;
        auto startX = centre.x + std::cos(angle) * (radius - 20);
        auto startY = centre.y + std::sin(angle) * (radius - 20);
        auto endX = centre.x + std::cos(angle) * (radius * 0.75); // Estendi fino al knob centrale
        auto endY = centre.y + std::sin(angle) * (radius * 0.75);
        scanalaturePath.startNewSubPath(startX, startY);
        scanalaturePath.lineTo(endX, endY);
    }
    g.setColour(juce::Colours::whitesmoke.withAlpha(0.5f)); // Rende le scanalature meno invasive
    g.strokePath(scanalaturePath, juce::PathStrokeType(0.5f));

    // Crea e applica il gradiente per l'arco
    juce::ColourGradient arcGradient(juce::Colours::green, centre, juce::Colours::red, centre, true);
    g.setGradientFill(arcGradient);
    juce::Path path;
    path.addPieSegment(bounds.reduced(bounds.getWidth() / 20), rotaryStartAngle, rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * sliderPosProportional, 0.9f);
    g.fillPath(path);

    // Effetto di sporgenza migliorato del knob centrale
    auto knobRadius = radius * 0.25f;
    juce::ColourGradient knobGradient(juce::Colours::grey, centre.x - knobRadius * 0.1f, centre.y - knobRadius * 0.1f,
        juce::Colours::darkgrey, centre.x + knobRadius * 0.25f, centre.y + knobRadius * 0.25f, true);
    knobGradient.addColour(0.5, juce::Colours::black); // Aggiunge profondità
    g.setGradientFill(knobGradient);
    g.fillEllipse(centre.x - knobRadius, centre.y - knobRadius, knobRadius * 2, knobRadius * 2);

    // Bordo chiaro intorno al knob per accentuare la sporgenza
    g.setColour(juce::Colours::whitesmoke.withAlpha(0.8f));
    g.drawEllipse(centre.x - knobRadius, centre.y - knobRadius, knobRadius * 2, knobRadius * 2, 2.0f);
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
    g.setColour(juce::Colours::gold); // Usa il colore del testo definito nella Label
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

void AppLAF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider) {
    // Assicurati che lo stile dello slider sia verticale
    if (style == juce::Slider::LinearVertical) {
        // Colore di sfondo della track
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(x + width / 2 - 2, y, 4, height); // Track verticale

        // Segni di demarcazione e valori
        g.setColour(juce::Colours::gold); // Colore dorato per i dettagli
        const int numMarks = 10;
        for (int i = 0; i <= numMarks; ++i) {
            float pos = y + (i * height / numMarks);
            g.drawHorizontalLine(pos, x + width / 2 - 5, x + width / 2 + 5);

            // Opzionale: aggiungi valori di fianco ai segni
            juce::String valueString = juce::String((numMarks - i) * (slider.getMaximum() / numMarks), 0);
            g.drawText(valueString, x + width / 2 + 10, pos - 10, 30, 20, juce::Justification::centredLeft);
        }

        // Disegna il cursore come un rettangolo (knob)
        const int knobWidth = 20; // Larghezza del knob
        const int knobHeight = 10; // Altezza del knob
        g.setColour(juce::Colours::grey);
        juce::Rectangle<float> knob(x + (width - knobWidth) / 2, sliderPos - (knobHeight / 2), knobWidth, knobHeight);
        g.fillRect(knob);
    }
}


void AppLAF::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header) {
    auto r = header.getLocalBounds();

    // Sfondo metallico/argenteo
    juce::ColourGradient gradient(juce::Colours::silver, 0, 0, juce::Colours::grey, header.getWidth(), header.getHeight(), false);
   
    g.setGradientFill(gradient);
    g.fillRect(r);

    // Linee decorative dorate
    g.setColour(juce::Colours::gold);
    g.drawRect(r);
}