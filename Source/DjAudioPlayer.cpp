

#include <JuceHeader.h>
#include "DjAudioPlayer.h"

DjAudioPlayer::DjAudioPlayer() : thumbnailCache(5),
thumbnail(512, formatManager, thumbnailCache)
{

	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);
	//default reverb params
	reverbParams.roomSize = 0.5;
	reverbParams.damping = 0.5;
	reverbParams.wetLevel = 0.33;
	reverbParams.dryLevel = 0.4;
	reverb.setParameters(reverbParams);
}


DjAudioPlayer::~DjAudioPlayer()
{
}


void DjAudioPlayer::prepareToPlay(int samplesPerBlockExpected,
	double sampleRate)
{

	jassert(sampleRate > 0);

	currentSampleRate = sampleRate;

	/*
	define the specs that need to be passed to the dsp algorithm.
	*/
	juce::dsp::ProcessSpec spec = juce::dsp::ProcessSpec();
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlockExpected;
	spec.numChannels = 2;

	/*
	ensure the current internal vars of the processor are reset to default.
	*/
	delayLine.reset();

	/*
	Apply the specs to the delay effect.
	*/
	delayLine.prepare(spec);
	auto maxDelayTimeInSamples = sampleRate * (2000.0f / 1000.0f);
	delayLine.setMaximumDelayInSamples(maxDelayTimeInSamples);
	auto delayTimeInSamples = currentSampleRate * (500.0f / 1000.0);
	delayLine.setDelay(delayTimeInSamples);

	/*
	Pass audio parameters to the transportSource and the resampleSource.
	*/
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DjAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo&
	bufferToFill) {

	if (transportSource.isPlaying() && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds()) {
		transportSource.setPosition(0.0f);
	}
	/*
	play the audio block through the resample source. The first time the effects aren't applied, but this method is called
	44100 times per second, so...
	*/
	resampleSource.getNextAudioBlock(bufferToFill);
	/*
	Prevent the buffer to get a write pointer to an unexistant channel iterating through the available ones.
	In each channel apply the reverb to the samples contained into the bufferToFill.
	If the number of available channels is 2, then the appropriate inner method is called.
	*/
	if (bufferToFill.buffer->getNumChannels() != 2) {
		for (int i = 0; i < bufferToFill.buffer->getNumChannels(); i++) {
			reverb.processMono(bufferToFill.buffer->getWritePointer(i), bufferToFill.numSamples);
		}
	}
	else {
		reverb.processStereo(bufferToFill.buffer->getWritePointer(0), bufferToFill.buffer->getWritePointer(1), bufferToFill.numSamples);

	}

	/*
	See the related explaination below.
	*/
	applyDelay(*bufferToFill.buffer, bufferToFill.numSamples);

}

void DjAudioPlayer::releaseResources()
{
	/*
	Be sure to free all the resouces during the distruction.
	*/
	transportSource.releaseResources();
	resampleSource.releaseResources();
}





/*
Handle the boundary conditions setting default values to the prop.
*/
void DjAudioPlayer::setGain(double gain)
{
	if (gain < 0.0) {
		transportSource.setGain(0.0f);
	}
	else if (gain > 1.0) {
		transportSource.setGain(1.0f);

	}
	else {
		transportSource.setGain(gain);
	}
}

/*
Handle the boundary conditions setting default values to the prop.
*/
void DjAudioPlayer::setSpeed(double ratio)
{
	if (ratio < 0.0) {
		resampleSource.setResamplingRatio(0.0f);

	}
	else if (ratio > 100.0) {
		resampleSource.setResamplingRatio(100.0f);

	}
	else {
		resampleSource.setResamplingRatio(ratio);
	}
}


void DjAudioPlayer::setPosition(double posInSecs)
{
	transportSource.setPosition(posInSecs);
}

void DjAudioPlayer::setPositionRelative(double pos) {

	if (pos < 0.0) {
		pos = 0.0f;
		double posInSecs = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
		transportSource.stop();

	}
	else if (pos > 1.0) {
		pos = 1.0f;
		double posInSecs = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
		transportSource.stop();

	}
	else {
		double posInSecs = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
		transportSource.stop();
	}

}

void DjAudioPlayer::start(void)
{
	if (!transportSource.isPlaying()) {
		transportSource.start();
	}
	else {

	}
}

void DjAudioPlayer::stop(void)
{

	transportSource.stop();
	transportSource.setPosition(0.0f);

}
void DjAudioPlayer::pause(void)
{
	transportSource.stop();
}

void DjAudioPlayer::changeListenerCallback(juce::ChangeBroadcaster* source) {

}


juce::AudioTransportSource* DjAudioPlayer::getTransportSource() {
	return &transportSource;
}

void DjAudioPlayer::loadFileAndPlay(const juce::File& file) {
	auto* reader = formatManager.createReaderFor(file);

	if (reader != nullptr) {
		auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
		thumbnail.setSource(new juce::FileInputSource(file));


	}
	else {
		juce::Logger::writeToLog("Failed to create audio format reader.");
	}
}

/*
This method applies the delay effect "in place" to a block of samples contained in a buffer,
iterating through its available channels.
Getting the write ptrs, it creates a feedback effect using thejuce::DelayLine::popSample() method.
Then the output is pushed in the buffer and mixed with the original one, blending it depending on the
wet level.
*/

void DjAudioPlayer::applyDelay(juce::AudioBuffer<float>& buffer, int numSamples) {
	for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
		auto* channelData = buffer.getWritePointer(channel);

		for (int i = 0; i < numSamples; ++i) {
			const float in = channelData[i];
			float out = delayLine.popSample(channel, delayTime * currentSampleRate / 1000.0f, true);
			delayLine.pushSample(channel, in + out * feedback);
			channelData[i] = in * (1.0f - wetLevel) + out * wetLevel;
		}
	}
}


void DjAudioPlayer::setDelayParameters(float delayTime, float feedback, float wetLevel) {
	this->delayTime = delayTime;
	this->feedback = feedback;
	this->wetLevel = wetLevel;
}

void DjAudioPlayer::setReverbParameters(const juce::Reverb::Parameters& params) {
	reverb.setParameters(params);
}

juce::AudioThumbnail& DjAudioPlayer::getThumbnail() { return thumbnail; }
