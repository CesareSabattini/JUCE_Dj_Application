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
    addAndMakeVisible(delayLabel);
    delayLabel.setText("DELAY", juce::dontSendNotification);
    delayLabel.setLookAndFeel(&appLAF);
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


    delayTime.setRange(1.0f, 2000.0f, 1.0f);
    delayFeedback.setRange(0.0f, 0.95f, 0.05f);
    delayWet.setRange(0.0f, 1.0f, 0.05f);

    delayTime.setValue(1000);
    delayFeedback.setValue(0.5);
    delayWet.setValue(0.5);

    addAndMakeVisible(delayTime);
    addAndMakeVisible(delayFeedback);
    addAndMakeVisible(delayWet);

    delayTime.setSliderStyle(juce::Slider::LinearHorizontal);
    delayFeedback.setSliderStyle(juce::Slider::LinearHorizontal);
    delayWet.setSliderStyle(juce::Slider::LinearHorizontal);

    delayTime.setLookAndFeel(&appLAF);
    delayFeedback.setLookAndFeel(&appLAF);
    delayWet.setLookAndFeel(&appLAF);

    delayTime.addListener(this);
    delayFeedback.addListener(this);
    delayWet.addListener(this);

    delayTime.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);
    delayFeedback.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);
    delayWet.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 45, 30);


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
    int widthFraction = getWidth() / 3;
    const int offSet = 50;

    const int borderGap = 20;
    reverbLabel.setBounds(20, 0, widthFraction -40, offSet);
    delayLabel.setBounds(widthFraction +20, 0, widthFraction -40, 50);

    roomSlider.setBounds(borderGap, heightFraction, (widthFraction - 2 * borderGap), heightFraction);
    dampingSlider.setBounds(borderGap, heightFraction * 2, (widthFraction - 2 * borderGap), heightFraction);
    wetLevelSlider.setBounds(borderGap, heightFraction * 3, (widthFraction - 2 * borderGap), heightFraction);
    dryLevelSlider.setBounds(borderGap, heightFraction * 4, (widthFraction - 2 * borderGap), heightFraction);
    widthSlider.setBounds(borderGap, heightFraction * 5, (widthFraction - 2 * borderGap), heightFraction);

    delayTime.setBounds(widthFraction+borderGap, heightFraction, (widthFraction - 2 * borderGap), heightFraction);
    delayFeedback.setBounds(widthFraction + borderGap, heightFraction*2, (widthFraction - 2 * borderGap), heightFraction);
    delayWet.setBounds(widthFraction + borderGap, heightFraction*3, (widthFraction - 2 * borderGap), heightFraction);


}
