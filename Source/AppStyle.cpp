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