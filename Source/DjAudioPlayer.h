#pragma once

#include <JuceHeader.h>

class DjAudioPlayer  : public juce::AudioSource
{
public:
    DjAudioPlayer();
    ~DjAudioPlayer() override;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DjAudioPlayer)
};
