#include "Playlist.h"
#include <JuceHeader.h>

Playlist::Playlist(std::shared_ptr<DjAudioPlayer> audioPlayer)
    : player{audioPlayer}, chosenTrack("", "", 0.0, "") {
    loadTracksFromDirectory();
    appLAF = std::make_unique<AppLAF>();

    addTrackButton.setButtonText("+");
    addTrackButton.onClick = [this] { addTrack(); };
    addAndMakeVisible(addTrackButton);
    addTrackButton.setLookAndFeel(appLAF.get());

    closeButton.setButtonText("Close");
    closeButton.onClick = [this] { setVisible(false); };
    addAndMakeVisible(closeButton);
    closeButton.setLookAndFeel(appLAF.get());

    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    tableComponent.getHeader().addColumn("Track Name", 1, 120);
    tableComponent.getHeader().addColumn("Artist", 2, 120);
    tableComponent.getHeader().addColumn("Length", 3, 120);
    tableComponent.getHeader().addColumn("Path", 4, 120);
    setLookAndFeel(appLAF.get());
}

Playlist::~Playlist() { setLookAndFeel(nullptr); }

void Playlist::paint(juce::Graphics &) {}

/*
Starting a parallel thread, a window to select audio files from the device is
opened. In the lauchAsync function, a callback passed to the constructor,
defining the behaviour after the selection: a playlist directory is created in
the project folder and the chosen file is copied here, to enable a permanent
local storage. Then,the metadata of the file are extracted and pushed in the
tracks member vector, and are then shown on screen.
*/

void Playlist::addTrack() {

    chooser = std::make_unique<juce::FileChooser>(
        "Select a Wave file to play...", juce::File{}, "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode |
                        juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser &fc) {
        auto file = fc.getResult();
        if (file != juce::File{}) {
            auto playlistDir =
                juce::File::getCurrentWorkingDirectory().getChildFile(
                    "Playlist");
            if (!playlistDir.exists()) {
                playlistDir.createDirectory();
            }

            auto destinationFile = playlistDir.getChildFile(file.getFileName());

            file.copyFileTo(destinationFile);
            tracks = std::vector<Track>();
            loadTracksFromDirectory();
            tableComponent.updateContent();
        }
    });
    setVisible(false);
}

void Playlist::resized() {
    const int borderGap = 20;
    const int buttonWidth = 80;
    const int buttonHeight = 30;
    auto area = getLocalBounds();

    closeButton.setBounds(getBounds().getCentreX() + buttonWidth, borderGap,
                          buttonWidth, buttonHeight);
    addTrackButton.setBounds(getBounds().getCentreX() - 2.5f * buttonWidth,
                             borderGap, buttonWidth, buttonHeight);
    tableComponent.setBounds(area.reduced(3 * borderGap));
}

int Playlist::getNumRows() { return static_cast<int>(tracks.size()); }

void Playlist::paintRowBackground(juce::Graphics &g, int, int width, int height,
                                  bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::lightblue);
    } else {
        g.fillAll(juce::Colours::darkgrey);
    }
    g.setColour(juce::Colours::gold);
    g.drawLine(0.0f, static_cast<float>(height - 1), static_cast<float>(width),
               static_cast<float>(height - 1.0f), 1.0f);
}

void Playlist::paintCell(juce::Graphics &g, int rowNumber, int columnId,
                         int width, int height, bool) {
    if (rowNumber < 0 || rowNumber >= tracks.size())
        return;

    auto &track = tracks[rowNumber];
    juce::String text;
    switch (columnId) {
    case 1:
        text = track.name;
        break;
    case 2:
        text = track.artist;
        break;
    case 3:
        text = juce::String(track.lengthInSeconds, 2) + "s";
        break;
    case 4:
        text = track.path;
        break;
    default:
        break;
    }

    g.setColour(juce::Colours::black);
    g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft,
               true);
}

void Playlist::loadTracksFromDirectory() {
    auto playlistDir =
        juce::File::getCurrentWorkingDirectory().getChildFile("Playlist");
    if (playlistDir.exists() && playlistDir.isDirectory()) {
        juce::DirectoryIterator iter(playlistDir, false, "*.wav");
        while (iter.next()) {
            auto file = iter.getFile();
            auto name = file.getFileNameWithoutExtension();
            auto lengthInSeconds = file.getSize() / 1024.0;
            auto path = file.getFullPathName();

            tracks.emplace_back(name, "Unknown", lengthInSeconds, path);
        }
    }
}

/*
When a row is clicked, the DjAudioPlayer::loadFileAndPlay() is called, and the
transportSource is initialized to the audioFile.Then this component is removed
from the screen viewport. If the audio file isn't found, or is corrupted, the
playlist is only set invisible.
*/
void Playlist::selectedRowsChanged(int lastRowSelected) {
    if (lastRowSelected >= 0 && lastRowSelected < tracks.size()) {
        auto &selectedTrack = tracks[lastRowSelected];
        juce::File audioFile(selectedTrack.path);

        if (audioFile.existsAsFile()) {

            chosenTrack = selectedTrack;
            player->loadFileAndPlay(audioFile);
            setVisible(false);
        } else {
            setVisible(false);
        }
    }
}

/*
Definitions of the pure virtual method of juce::FileDragAndDropTarget class.
*/
bool Playlist::isInterestedInFileDrag(const juce::StringArray &) {
    return true;
}
/*
After a file is dropped a procedure similiar to the one followed in the addTrack
method, is exhecuted.
*/
void Playlist::filesDropped(const juce::StringArray &files, int, int) {
    for (const auto &file : files) {
        juce::File audioFile(file);
        if (audioFile.existsAsFile()) {

            auto playlistDir =
                juce::File::getCurrentWorkingDirectory().getChildFile(
                    "Playlist");
            if (!playlistDir.exists()) {
                playlistDir.createDirectory();
            }

            auto destinationFile =
                playlistDir.getChildFile(audioFile.getFileName());

            audioFile.copyFileTo(destinationFile);
            tracks = std::vector<Track>();
            loadTracksFromDirectory();
            tableComponent.updateContent();
        }
    }
}

Track &Playlist::getChosenTrackSpecs() { return chosenTrack; }