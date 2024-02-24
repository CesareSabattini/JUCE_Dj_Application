
#include <JuceHeader.h>
#include "DjAudioPlayer.h"

DjAudioPlayer::DjAudioPlayer()
{
	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);
}

DjAudioPlayer::~DjAudioPlayer()
{
}




void DjAudioPlayer::prepareToPlay(int samplesPerBlockExpected,
	double sampleRate)
{
	
	transportSource.prepareToPlay(samplesPerBlockExpected,
		sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected,
		sampleRate);
}

void DjAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo&
	bufferToFill) {

    transportSource.getNextAudioBlock(bufferToFill);
}

void DjAudioPlayer::releaseResources()
{
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

void DjAudioPlayer::loadURL()
{
	 chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
		juce::File{},
		"*.wav");                    
	auto chooserFlags = juce::FileBrowserComponent::openMode
		| juce::FileBrowserComponent::canSelectFiles;

	chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)     
		{

	
			auto file = fc.getResult();

			if (file != juce::File{})                                               
			{
				DBG("File selected: " << file.getFullPathName());
				auto* reader = formatManager.createReaderFor(file);           

				if (reader != nullptr)
				{
					auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);  
					transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);      
					                                    
					readerSource.reset(newSource.release());
				}
			}
			else {
				DBG("No file was selected.");
			}
		});
}

void DjAudioPlayer::setGain(double gain)
{
	if (gain < 0.0 || gain>1.0) {
std::cerr << "Gain should be between 0 and 1" << std::endl;
	
	}
	else {
		transportSource.setGain(gain);
		
	}
}

void DjAudioPlayer::setSpeed(double ratio)
 { 
	if (ratio < 0.0 || ratio>100.0) {
		std::cerr << "Speed should be between 0 and 100" << std::endl;
		
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

	 if (pos < 0.0 || pos > 1.0) {
		 std::cerr << "Position should be between 0 and 1" << std::endl;
	 }
	 else {
		 double posInSecs = transportSource.getLengthInSeconds() * pos;
		 setPosition(posInSecs);
	 }

 }

void DjAudioPlayer::start(void)
 {
	if (!transportSource.isPlaying()) {
		DBG("Starting playback");
		transportSource.start();
	}
	else {
		DBG("TransportSource already playing");
	}
	}

 void DjAudioPlayer::stop(void)
 {
	 transportSource.stop();
	}

  void DjAudioPlayer::changeListenerCallback(juce::ChangeBroadcaster* source) {
	 
 }