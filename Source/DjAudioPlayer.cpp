
#include <JuceHeader.h>
#include "DjAudioPlayer.h"

DjAudioPlayer::DjAudioPlayer(): thumbnailCache(5), // Puoi regolare la dimensione della cache a seconda delle tue esigenze
thumbnail(512, formatManager, thumbnailCache)
{
	
	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);

	reverbParams.roomSize = 0.5; // Dimensione della stanza, range [0, 1]
	reverbParams.damping = 0.5; // Smorzamento, range [0, 1]
	reverbParams.wetLevel = 0.33; // Livello del segnale riverberato, range [0, 1]
	reverbParams.dryLevel = 0.4; // Livello del segnale diretto, range [0, 1]
	reverb.setParameters(reverbParams);
}

DjAudioPlayer::~DjAudioPlayer()
{
}




void DjAudioPlayer::prepareToPlay(int samplesPerBlockExpected,
	double sampleRate)
{
	
	DBG("Preparing to play. Sample rate: " << sampleRate << ", Samples per block: " << samplesPerBlockExpected);
	jassert(sampleRate > 0); // Aggiunge un assert per verificare che sampleRate sia positivo

	currentSampleRate = sampleRate; // Memorizza il sample rate

	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlockExpected;
	spec.numChannels = 2; // Ad esempio, per stereo

	delayLine.reset();
	delayLine.prepare(spec);
	auto maxDelayTimeInSamples = sampleRate * (2000.0f / 1000.0);
	delayLine.setMaximumDelayInSamples(maxDelayTimeInSamples);
	auto delayTimeInSamples = currentSampleRate * (500.0f / 1000.0);
	delayLine.setDelay(delayTimeInSamples);

	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DjAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo&
	bufferToFill) {
	if (transportSource.isPlaying() && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds()) {
		transportSource.setPosition(0.0f);
	}
	resampleSource.getNextAudioBlock(bufferToFill);
	reverb.processStereo(bufferToFill.buffer->getWritePointer(0), bufferToFill.buffer->getWritePointer(1), bufferToFill.numSamples);

	applyDelay(*bufferToFill.buffer, bufferToFill.numSamples);

}

void DjAudioPlayer::releaseResources()
{
	transportSource.releaseResources();
	resampleSource.releaseResources();
	
}


void DjAudioPlayer::loadURL()
{
	chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
		juce::File{}, "*.wav");
	auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
		{
			auto file = fc.getResult();
			if (file != juce::File{}) {
				// Costruisci il percorso della directory "Playlist" nella directory del progetto
				auto playlistDir = juce::File::getCurrentWorkingDirectory().getChildFile("Playlist");
				if (!playlistDir.exists()) {
					playlistDir.createDirectory(); // Crea la directory se non esiste
				}

				// Costruisci il percorso del file di destinazione all'interno della directory "Playlist"
				auto destinationFile = playlistDir.getChildFile(file.getFileName());

				// Copia il file selezionato nella directory "Playlist", sovrascrivendo se esiste
				file.copyFileTo(destinationFile);

				// Utilizza il file nella directory "Playlist" da ora in poi
				auto* reader = formatManager.createReaderFor(destinationFile);
				if (reader != nullptr) {
					auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
					transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
					readerSource.reset(newSource.release());
					thumbnail.setSource(new juce::FileInputSource(destinationFile));
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
		 transportSource.stop();
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
	 transportSource.setPosition(0.0);
	 
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