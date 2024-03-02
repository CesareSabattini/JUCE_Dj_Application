#pragma once

#include "AppStyle.h"
#include "DjAudioPlayer.h"
#include <JuceHeader.h>

struct Track {
    juce::String name;
    juce::String artist;
    double lengthInSeconds;
    juce::String path;

    Track(const juce::String &name, const juce::String &artist,
          double lengthInSeconds, const juce::String &path)
        : name(name), artist(artist), lengthInSeconds(lengthInSeconds),
          path(path) {}
};

class Playlist : public juce::Component,
                 public juce::TableListBoxModel,
                 public juce::FileDragAndDropTarget

{
  public:
    Playlist(std::shared_ptr<DjAudioPlayer> audioPlayer);
    ~Playlist() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics &, int rowNumber, int width,
                            int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics &, int rowNumber, int columnId, int width,
                   int height, bool rowIsSelected) override;
    void addTrack();
    void selectedRowsChanged(int lastRowSelected);
    void loadTracksFromDirectory();

    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    Track &getChosenTrackSpecs();

  private:
    std::shared_ptr<DjAudioPlayer> player;
    std::unique_ptr<juce::FileChooser> chooser;
    std::unique_ptr<AppLAF> appLAF;
    juce::TableListBox tableComponent;
    std::vector<Track> tracks;
    Track chosenTrack;
    juce::TextButton addTrackButton;
    juce::TextButton closeButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Playlist)
};
