/*
  ==============================================================================

    Playlist.h
    Created: 25 Feb 2024 10:56:58am
    Author:  cesar

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include"DjAudioPlayer.h"
#include"AppStyle.h"

//==============================================================================
/*
*/

struct Track {
    juce::String name;
    juce::String artist; // Potresti dover estrarre questo dai metadati, se disponibili
    double lengthInSeconds;
    juce::String path;

    Track(const juce::String& name, const juce::String& artist, double lengthInSeconds, const juce::String& path)
        : name(name), artist(artist), lengthInSeconds(lengthInSeconds), path(path) {}
};



class Playlist  : public juce::Component,
    public juce::TableListBoxModel{
public:
    Playlist(DjAudioPlayer* audio);
    ~Playlist() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    void addTrack();
    void selectedRowsChanged(int lastRowSelected);
    void loadTracksFromDirectory();

private:
    std::unique_ptr<AppLAF> appLAF;
    DjAudioPlayer* player;
    juce::TableListBox tableComponent;
    std::vector<Track> tracks;
    juce::TextButton addTrackButton{ "Add Track" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Playlist)
};


