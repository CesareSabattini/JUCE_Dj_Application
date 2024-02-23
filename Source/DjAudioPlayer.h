#pragma once

#include <JuceHeader.h>

class DjAudioPlayer  : public juce::AudioSource, juce::ChangeListener
{
public:
    DjAudioPlayer();

    
    ~DjAudioPlayer() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void prepareToPlay(int samplesPerBlockExpected,
        double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo&
        bufferToFill) override;
    void releaseResources() override;
    void loadURL();
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    void start(void);
    void stop(void);
 
private:

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::ResamplingAudioSource resampleSource{ &transportSource,
false, 2 };
    std::unique_ptr<juce::FileChooser> chooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DjAudioPlayer)
};
