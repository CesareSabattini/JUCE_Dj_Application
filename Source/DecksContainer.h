#pragma once

#include "AppStyle.h"
#include "DeckGUI.h"
#include <JuceHeader.h>

class DecksContainer : public juce::Component {
  public:
    DecksContainer();
    ~DecksContainer() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void addDeckGUI(std::shared_ptr<DeckGUI> deckGUI) {
        deckGUIs.push_back(deckGUI);
        addAndMakeVisible(deckGUIs.back().get());
        resized();
    }

  private:
    std::vector<std::shared_ptr<DeckGUI>> deckGUIs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DecksContainer)
};
