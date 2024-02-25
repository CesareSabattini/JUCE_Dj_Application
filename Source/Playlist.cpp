/*
  ==============================================================================

    Playlist.cpp
    Created: 25 Feb 2024 10:56:58am
    Author:  cesar

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Playlist.h"


Playlist::Playlist(DjAudioPlayer* audioPlayer) : player{audioPlayer} {
    loadTracksFromDirectory();
        // Configura il pulsante "Add Track"
        addTrackButton.setButtonText("+");
        addTrackButton.onClick = [this] { addTrack(); };
        addAndMakeVisible(addTrackButton);

        // Inizializza e configura la tabella
        tableComponent.setModel(this);
        addAndMakeVisible(tableComponent);

        // Imposta le colonne
        tableComponent.getHeader().addColumn("Track Name", 1, 100);
        tableComponent.getHeader().addColumn("Artist", 2, 100);
        tableComponent.getHeader().addColumn("Length", 3, 100);
        tableComponent.getHeader().addColumn("Path", 4, 100);
        setLookAndFeel(appLAF.get());
        
    }

Playlist::~Playlist()
{
    setLookAndFeel(nullptr);
}

void Playlist::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void Playlist::addTrack() {
    // Implementa la logica per aggiungere una traccia
    DBG("Add Track Button Clicked");
    player->loadURL();
    setVisible(false);
   
}

void Playlist::resized() {
    auto area = getLocalBounds();
    addTrackButton.setBounds(area.removeFromTop(30).reduced(8));
    tableComponent.setBounds(area.reduced(8));
}


int Playlist::getNumRows()
{
    return tracks.size();
}

void Playlist::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)  {
    if (rowIsSelected){ g.fillAll(juce::Colours::lightblue); }
      
    else{ g.fillAll(juce::Colours::darkgrey); }
      

    g.setColour(juce::Colours::gold); // Imposta il colore della penna a dorato
    g.drawLine(0, height - 1, width, height - 1, 1);
}

void Playlist::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)  {
    if (rowNumber < 0 || rowNumber >= tracks.size()) return;

    auto& track = tracks[rowNumber];
    juce::String text;
    switch (columnId) {
    case 1: text = track.name; break;
    case 2: text = track.artist; break;
    case 3: text = juce::String(track.lengthInSeconds, 2) + "s"; break; // Formato "xx.yy s"
    case 4: text = track.path; break;
    default: break;
    }

    g.setColour(juce::Colours::black);
    g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);

}

void Playlist::loadTracksFromDirectory() {
    auto playlistDir = juce::File::getCurrentWorkingDirectory().getChildFile("Playlist");
    if (playlistDir.exists() && playlistDir.isDirectory()) {
        juce::DirectoryIterator iter(playlistDir, false, "*.wav");
        while (iter.next()) {
            auto file = iter.getFile();
            auto name = file.getFileNameWithoutExtension();
            // Qui assumiamo "Unknown" come artista, e usiamo il file size come placeholder per la lunghezza
            auto lengthInSeconds = file.getSize() / 1024.0; // Esempio di placeholder, sostituisci con la lunghezza reale se possibile
            auto path = file.getFullPathName();

            tracks.emplace_back(name, "Unknown", lengthInSeconds, path);
        }
    }
}

void Playlist::selectedRowsChanged(int lastRowSelected) {
    if (lastRowSelected >= 0 && lastRowSelected < tracks.size()) {
        auto& selectedTrack = tracks[lastRowSelected];
        juce::File audioFile(selectedTrack.path);

        if (audioFile.existsAsFile()) {
            // Chiama un metodo sul tuo DjAudioPlayer per caricare e riprodurre il file
            player->loadFileAndPlay(audioFile);
            setVisible(false);
        }
        else {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                "File Not Found",
                "The selected audio file could not be found.");
        }
    }
}