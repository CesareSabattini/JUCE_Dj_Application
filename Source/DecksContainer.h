#pragma once

#include <JuceHeader.h>
#include"AppStyle.h"
#include"DeckGUI.h"
class DecksContainer  : public juce::Component
{
public:
    DecksContainer();
    ~DecksContainer() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void addDeckGUI(std::unique_ptr<DeckGUI> deckGUI)
    {
        deckGUIs.push_back(std::move(deckGUI)); 
        addAndMakeVisible(deckGUIs.back().get()); 
        resized();
    }

private:

    std::vector<std::unique_ptr<DeckGUI>> deckGUIs;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecksContainer)
};
