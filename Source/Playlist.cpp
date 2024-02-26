#include <JuceHeader.h>
#include "Playlist.h"


Playlist::Playlist(DjAudioPlayer* audioPlayer) : player{audioPlayer}, chosenTrack("","",0.0,""){
    loadTracksFromDirectory();
        // Configura il pulsante "Add Track"
        addTrackButton.setButtonText("+");
        addTrackButton.onClick = [this] { addTrack(); };
        addAndMakeVisible(addTrackButton);
        addTrackButton.setLookAndFeel(appLAF.get());

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

                auto name = destinationFile.getFileNameWithoutExtension();
                auto lengthInSeconds = destinationFile.getSize() / 1024.0; // Esempio di placeholder
                auto path = destinationFile.getFullPathName();
                tracks = std::vector<Track>();
                loadTracksFromDirectory();
                // Potrebbe essere necessario aggiornare l'UI per riflettere i nuovi file aggiunti
                tableComponent.updateContent();
            }

        }
    );
    setVisible(false);
}
        
   


void Playlist::resized() {
    auto area = getLocalBounds();
    addTrackButton.setBounds(getRight()-130, 5, 80, 20);
    tableComponent.setBounds(area.reduced(40));
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
            chosenTrack = selectedTrack;
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

bool Playlist::isInterestedInFileDrag(const juce::StringArray& files){
    // Qui puoi verificare se i file trascinati sono di un tipo che la tua app può gestire
    // Per semplicità, restituiamo true per indicare interesse per qualsiasi file
    return true;
}

void Playlist::filesDropped(const juce::StringArray& files, int x, int y){
    for (const auto& file : files) {
        juce::File audioFile(file);
        if (audioFile.existsAsFile()) {
            // Aggiungi il file alla tua playlist
            auto name = audioFile.getFileNameWithoutExtension();
            auto lengthInSeconds = audioFile.getSize() / 1024.0; // Esempio di placeholder
            auto path = audioFile.getFullPathName();
            tracks.emplace_back(name, "Unknown", lengthInSeconds, path);

            // Potrebbe essere necessario aggiornare l'UI per riflettere i nuovi file aggiunti
            tableComponent.updateContent();
        }
    }
}