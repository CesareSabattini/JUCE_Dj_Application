/*
  ==============================================================================

    CommonEffects.cpp
    Created: 25 Feb 2024 6:11:34pm
    Author:  cesar

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CommonEffects.h"

//==============================================================================
CommonEffects::CommonEffects()
{
    addAndMakeVisible(effectsLabel);
    effectsLabel.setText("EFFECTS", juce::dontSendNotification);
    effectsLabel.setLookAndFeel(&appLAF);
    addAndMakeVisible(reverbLabel);
    reverbLabel.setText("REVERB", juce::dontSendNotification);
    reverbLabel.setLookAndFeel(&appLAF);


    roomSlider.setRange(0.0f, 1.0f, 0.05f);
    dampingSlider.setRange(0.0f, 1.0f, 0.05f);
    wetLevelSlider.setRange(0.0f, 1.0f, 0.05f);
    dryLevelSlider.setRange(0.0f, 1.0f, 0.05f);
    widthSlider.setRange(0.0f, 1.0f, 0.05f);


    addAndMakeVisible(roomSlider);
    addAndMakeVisible(dampingSlider);
    addAndMakeVisible(wetLevelSlider);
    addAndMakeVisible(dryLevelSlider);
    addAndMakeVisible(widthSlider);

    roomSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dampingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    wetLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);

    roomSlider.setLookAndFeel(&appLAF);
    dampingSlider.setLookAndFeel(&appLAF);
    wetLevelSlider.setLookAndFeel(&appLAF);
    dryLevelSlider.setLookAndFeel(&appLAF);
    widthSlider.setLookAndFeel(&appLAF);




}

CommonEffects::~CommonEffects()
{
}

void CommonEffects::paint (juce::Graphics& g)
{

}

void CommonEffects::resized()
{
    int heightFraction = (getHeight() - 60) / 5;
    const int offSet = 50;
    reverbLabel.setBounds(0, 0, getWidth() / 2, offSet);
    effectsLabel.setBounds(getWidth() / 2, 0, getWidth() / 2, 50);

    roomSlider.setBounds(0, heightFraction+ offSet, getWidth() / 2, heightFraction);
    dampingSlider.setBounds(0, heightFraction*2+ offSet, getWidth()/2, heightFraction);
    wetLevelSlider.setBounds(0, heightFraction*3+ offSet, getWidth() / 2, heightFraction);
    dryLevelSlider.setBounds(0, heightFraction*4+ offSet, getWidth() / 2, heightFraction);
    widthSlider.setBounds(0, heightFraction*5+offSet, getWidth() / 2, heightFraction);


}
