#include <JuceHeader.h>
#include "DecksContainer.h"

//==============================================================================
DecksContainer::DecksContainer()
{
 
}

DecksContainer::~DecksContainer()
{
}

void DecksContainer::paint (juce::Graphics& g)
{   
}

void DecksContainer::resized()
{
    int borderGap = 10; 
    int x = borderGap;
    for (auto& deckGUI : deckGUIs)
    {
        deckGUI->setBounds(x, 0, (getWidth()-2*borderGap)/2, getHeight());
        x += (getWidth() - 2 * borderGap) / 2 + borderGap;
    }
}
