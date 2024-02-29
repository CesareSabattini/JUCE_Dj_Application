#include <JuceHeader.h>
#include "CommonEffects.h"


/*
In the constructure the labels and the slider that compose the interface are initialized.
The sliders' ranges, styles, textBoxStyles are set the CommonEffect class is configured as their listener.
The callbacks are declared in the public scope and defined in the MainComponent.
Like in the other user defined components, the pre-defined ones are styled using the LookAndFeel_V4 inherithed
class AppLAF.
*/
CommonEffects::CommonEffects()
{
    addAndMakeVisible(delayLabel);
    delayLabel.setText("DELAY", juce::dontSendNotification);
    delayLabel.setLookAndFeel(&appLAF);
    addAndMakeVisible(reverbLabel);
    reverbLabel.setText("REVERB", juce::dontSendNotification);
    reverbLabel.setLookAndFeel(&appLAF);

    addAndMakeVisible(comingSoonLabel);
    comingSoonLabel.setText("MORE EFFECTS COMING SOON...", juce::dontSendNotification);
    comingSoonLabel.setLookAndFeel(&appLAF);

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


    addAndMakeVisible(roomLabel);
    roomLabel.setText("Room", juce::dontSendNotification);
    roomLabel.setJustificationType(juce::Justification::centred);
    roomLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    roomLabel.setColour(juce::Label::textColourId, juce::Colours::gold);



    addAndMakeVisible(dampingLabel);
    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.setJustificationType(juce::Justification::centred);
    dampingLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    dampingLabel.setColour(juce::Label::textColourId, juce::Colours::gold);


    addAndMakeVisible(wetLabel);
    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.setJustificationType(juce::Justification::centred);
    wetLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    wetLabel.setColour(juce::Label::textColourId, juce::Colours::gold);


    addAndMakeVisible(dryLabel);
    dryLabel.setText("Dry", juce::dontSendNotification);
    dryLabel.setJustificationType(juce::Justification::centred);
    dryLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    dryLabel.setColour(juce::Label::textColourId, juce::Colours::gold);



    addAndMakeVisible(widthLabel);
    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centred);
    widthLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    widthLabel.setColour(juce::Label::textColourId, juce::Colours::gold);



    addAndMakeVisible(delayTimeLabel);
    delayTimeLabel.setText("Time", juce::dontSendNotification);
    delayTimeLabel.setJustificationType(juce::Justification::centred);
    delayTimeLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    delayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::gold);


    addAndMakeVisible(delayFbLabel);
    delayFbLabel.setText("Feedback", juce::dontSendNotification);
    delayFbLabel.setJustificationType(juce::Justification::centred);
    delayFbLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    delayFbLabel.setColour(juce::Label::textColourId, juce::Colours::gold);



    addAndMakeVisible(delayWetLabel);
    delayWetLabel.setText("Wet", juce::dontSendNotification);
    delayWetLabel.setJustificationType(juce::Justification::centred);
    delayWetLabel.setColour(juce::Label::outlineColourId, juce::Colours::silver);
    delayWetLabel.setColour(juce::Label::textColourId, juce::Colours::gold);



}

CommonEffects::~CommonEffects()
{
}

void CommonEffects::paint (juce::Graphics&)
{

}

void CommonEffects::resized()
{
    int heightFraction = getHeight() / 6;
    int widthFraction = getWidth() / 3;
    const int labelHeight = getHeight()/9;
    const int borderGap = getWidth()/40;


    reverbLabel.setBounds(borderGap, 0, widthFraction -2* borderGap, labelHeight);
    delayLabel.setBounds(widthFraction + borderGap, 0, widthFraction -2* borderGap, labelHeight);
    comingSoonLabel.setBounds(2 * widthFraction + borderGap, 0, widthFraction - 2 * borderGap, labelHeight);

    roomSlider.setBounds(borderGap, heightFraction, (widthFraction - 2 * borderGap), heightFraction);
    dampingSlider.setBounds(borderGap, heightFraction * 2, (widthFraction - 2 * borderGap), heightFraction);
    wetLevelSlider.setBounds(borderGap, heightFraction * 3, (widthFraction - 2 * borderGap), heightFraction);
    dryLevelSlider.setBounds(borderGap, heightFraction * 4, (widthFraction - 2 * borderGap), heightFraction);
    widthSlider.setBounds(borderGap, heightFraction * 5, (widthFraction - 2 * borderGap), heightFraction);

    delayTime.setBounds(widthFraction+borderGap, heightFraction, (widthFraction - 2 * borderGap), heightFraction);
    delayFeedback.setBounds(widthFraction + borderGap, heightFraction*2, (widthFraction - 2 * borderGap), heightFraction);
    delayWet.setBounds(widthFraction + borderGap, heightFraction*3, (widthFraction - 2 * borderGap), heightFraction);

    const int labelWidth = roomSlider.getBounds().getWidth() / 2;

    roomLabel.setBounds(getBounds().getWidth() / 6 -labelWidth/2, heightFraction - 10, labelWidth, heightFraction / 3);
    dampingLabel.setBounds(getBounds().getWidth() / 6 - labelWidth / 2, 2*heightFraction-10, labelWidth, heightFraction / 3);
    wetLabel.setBounds(getBounds().getWidth() / 6 - labelWidth / 2, 3*heightFraction-10, labelWidth, heightFraction / 3);
    dryLabel.setBounds(getBounds().getWidth() / 6 - labelWidth / 2, 4*heightFraction-10, labelWidth, heightFraction / 3);
    widthLabel.setBounds(getBounds().getWidth() / 6 - labelWidth / 2, 5*heightFraction-10, labelWidth, heightFraction / 3);

    delayTimeLabel.setBounds(getBounds().getWidth() / 2 - labelWidth / 2, heightFraction-10, labelWidth, heightFraction / 3);
    delayFbLabel.setBounds(getBounds().getWidth() / 2 - labelWidth / 2, 2 * heightFraction-10, labelWidth, heightFraction / 3);
    delayWetLabel.setBounds(getBounds().getWidth() / 2 - labelWidth / 2, 3 * heightFraction-10, labelWidth, heightFraction / 3);


}

/*
Definition of the slider cb function, called when a slider mutates. It distinguishes the origins of the changes
using references and modifies the related parameters.
*/

void CommonEffects::sliderValueChanged(juce::Slider* slider) {
    if (slider == &roomSlider || slider == &dampingSlider || slider == &wetLevelSlider || slider == &dryLevelSlider) {
        if (onParametersChanged) {
            juce::Reverb::Parameters params;
            params.roomSize = static_cast<float>(roomSlider.getValue());
            params.damping = static_cast<float>(dampingSlider.getValue());
            params.wetLevel = static_cast<float>(wetLevelSlider.getValue());
            params.dryLevel = static_cast<float>(dryLevelSlider.getValue());

            onParametersChanged(params);
        }
    }
    else if (slider == &delayTime || slider == &delayFeedback || slider == &delayWet) {
        if (onDelayParametersChanged) {
            DelayParameters params;
            params.delayTime = static_cast<float>(delayTime.getValue());
            params.feedback = static_cast<float>(delayFeedback.getValue());
            params.wetLevel = static_cast<float>(delayWet.getValue());

            onDelayParametersChanged(params);
        }
    }
}
