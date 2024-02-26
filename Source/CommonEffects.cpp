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

    roomSlider.addListener(this);
    dampingSlider.addListener(this);
    wetLevelSlider.addListener(this);
    dryLevelSlider.addListener(this);
    widthSlider.addListener(this);

    roomSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);

}

CommonEffects::~CommonEffects()
{
}

void CommonEffects::paint (juce::Graphics& g)
{

}

void CommonEffects::resized()
{
    int heightFraction = (getHeight() - 120) / 5;
    const int offSet = 50;

    const int borderGap = 20;
    reverbLabel.setBounds(20, 0, getWidth() / 2-40, offSet);
    effectsLabel.setBounds(getWidth() / 2+20, 0, getWidth() / 2-40, 50);

    roomSlider.setBounds(borderGap, heightFraction, (getWidth()-2* borderGap) / 2, heightFraction);
    dampingSlider.setBounds(borderGap, heightFraction * 2, (getWidth() - 2 * borderGap) / 2, heightFraction);
    wetLevelSlider.setBounds(borderGap, heightFraction * 3, (getWidth() - 2 * borderGap) / 2, heightFraction);
    dryLevelSlider.setBounds(borderGap, heightFraction * 4, (getWidth() - 2 * borderGap) / 2, heightFraction);
    widthSlider.setBounds(borderGap, heightFraction * 5, (getWidth() - 2 * borderGap) / 2, heightFraction);



}
