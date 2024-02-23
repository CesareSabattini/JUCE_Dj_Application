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


void AppLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {

    float diameter = juce::jmin(width, height) * 0.8f;
    float radius = diameter / 2;
    float centerX = x + width / 2;
    float centerY = y + height / 2;
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
    float rotationOffset = -juce::MathConstants<float>::halfPi;

    juce::Rectangle<float> area(centerX - radius, centerY - radius, diameter, diameter);

    // Background gradient for outer wheel
    juce::ColourGradient outerGradient(juce::Colours::grey.brighter(0.5), area.getCentreX(), area.getCentreY() - radius,
        juce::Colours::grey.darker(0.2), area.getCentreX(), area.getCentreY() + radius, true);
    g.setGradientFill(outerGradient);
    g.fillEllipse(area);

    // Inner wheel gradient
    juce::ColourGradient innerGradient(juce::Colours::silver, centerX, centerY - radius * 0.6f,
        juce::Colours::grey, centerX, centerY + radius * 0.6f, true);
    juce::Rectangle<float> innerWheelArea = area.reduced(area.getWidth() * 0.2f);
    g.setGradientFill(innerGradient);
    g.fillEllipse(innerWheelArea);

    // Blurred separation line
    // Instead of drawing a direct line, let's enhance the gradient around the edge of the inner wheel
    // to create a soft separation effect.
    // This is a subtle artistic effect and doesn't involve direct drawing operations.

    // Draw outer and inner wheel borders
    g.setColour(juce::Colours::black);
    g.drawEllipse(area, 1.0f);
    g.drawEllipse(innerWheelArea, 1.0f);

    // Ticks
    juce::Path tick;
    float stepAngle = (rotaryEndAngle - rotaryStartAngle) / 100.0f;
    for (int i = 0; i <= 100; ++i) {
        float tickAngle = rotaryStartAngle + i * stepAngle;
        float tickRadius = radius * 0.9f;
        float x1 = centerX + tickRadius * std::cos(tickAngle + rotationOffset);
        float y1 = centerY + tickRadius * std::sin(tickAngle + rotationOffset);
        float x2 = centerX + radius * std::cos(tickAngle + rotationOffset);
        float y2 = centerY + radius * std::sin(tickAngle + rotationOffset);
        tick.clear();
        tick.startNewSubPath(x1, y1);
        tick.lineTo(x2, y2);
        // Make every 10th tick more prominent
        float lineThickness = (i % 10 == 0) ? 2.0f : 1.0f;
        g.strokePath(tick, juce::PathStrokeType(lineThickness));
    }

    // Replace thumb with red line
    g.setColour(juce::Colours::red);
    float innerCircleRadius = radius * 0.7f; // Adjust this value to match the inner circle radius
    float outerCircleX = centerX + radius * std::cos(angle + rotationOffset);
    float outerCircleY = centerY + radius * std::sin(angle + rotationOffset);
    float innerCircleX = centerX + innerCircleRadius * std::cos(angle + rotationOffset);
    float innerCircleY = centerY + innerCircleRadius * std::sin(angle + rotationOffset);
    g.drawLine(innerCircleX, innerCircleY,
        outerCircleX, outerCircleY, 2.0f); // Red line as the thumb
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