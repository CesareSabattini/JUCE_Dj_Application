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
    void pause(void);
    void stop(void);
    juce::AudioTransportSource* getTransportSource();
    juce::AudioThumbnail& getThumbnail() { return thumbnail; }
    void loadFileAndPlay(const juce::File& file);
    void setReverbParameters(const juce::Reverb::Parameters& params) {
        reverb.setParameters(params);
    }
    void setDelayParameters(float delayTime, float feedback, float wetLevel) {
        this->delayTime = delayTime;
        this->feedback = feedback;
        this->wetLevel = wetLevel;
    }
    void applyDelay(juce::AudioBuffer<float>& buffer, int numSamples);
private:

    std::unique_ptr<juce::FileChooser> chooser;
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;

    juce::dsp::DelayLine<float> delayLine;
    float delayTime = 500.0f; // Tempo di delay in millisecondi
    float feedback = 0.5f;
    float wetLevel = 0.5f;
    double currentSampleRate = 44100;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource,
false,2};
    juce::AudioThumbnail thumbnail;
    juce::AudioThumbnailCache thumbnailCache;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DjAudioPlayer)
};
