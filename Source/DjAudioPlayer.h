#pragma once

#include <JuceHeader.h>

class DjAudioPlayer  : public juce::AudioSource, juce::ChangeListener
{
public:
    DjAudioPlayer();
    ~DjAudioPlayer() override;
    void changeListenerCallback(juce::ChangeBroadcaster*);
    void prepareToPlay(int samplesPerBlockExpected,
        double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo&
        bufferToFill) override;
    void releaseResources() override;
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    void start(void);
    void pause(void);
    void stop(void);
    juce::AudioTransportSource* getTransportSource();
    juce::AudioThumbnail& getThumbnail();
    void loadFileAndPlay(const juce::File& file);
    void setReverbParameters(const juce::Reverb::Parameters& params);
    void setDelayParameters(float delayTime, float feedback, float wetLevel);
    void applyDelay(juce::AudioBuffer<float>& buffer, int numSamples);
private:

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource,
false,2};
    juce::AudioThumbnail thumbnail;
    juce::AudioThumbnailCache thumbnailCache;
    std::unique_ptr<juce::FileChooser> chooser;
   
    //Reverb related variables
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;

    //Delay related variables
    juce::dsp::DelayLine<float> delayLine;
    float delayTime = 500.0f;
    float feedback = 0.5f;
    float wetLevel = 0.5f;
    double currentSampleRate = 44100;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DjAudioPlayer)
};
