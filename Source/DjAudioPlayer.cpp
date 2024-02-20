
#include <JuceHeader.h>
#include "DjAudioPlayer.h"

DjAudioPlayer::DjAudioPlayer()
{

}

DjAudioPlayer::~DjAudioPlayer()
{
}


void DjAudioPlayer::prepareToPlay(int samplesPerBlockExpected,
	double sampleRate)
{
	formatManager.registerBasicFormats();
	transportSource.prepareToPlay(samplesPerBlockExpected,
		sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected,
		sampleRate);
}

void DjAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo&
	bufferToFill) {
	if (readerSource.get() == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}
    transportSource.getNextAudioBlock(bufferToFill);
	resampleSource.getNextAudioBlock(bufferToFill);
}

void DjAudioPlayer::releaseResources()
{
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

void DjAudioPlayer::loadURL()
{
	std::unique_ptr<juce::FileChooser> chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
		juce::File{},
		"*.wav");                    
	auto chooserFlags = juce::FileBrowserComponent::openMode
		| juce::FileBrowserComponent::canSelectFiles;

	chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)     
		{
			auto file = fc.getResult();

			if (file != juce::File{})                                               
			{
				auto* reader = formatManager.createReaderFor(file);           

				if (reader != nullptr)
				{
					auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);  
					transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);      
					                                    
					readerSource.reset(newSource.release());
				}
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
	transportSource.start();
	}

 void DjAudioPlayer::stop(void)
 {
	 transportSource.stop();
	}