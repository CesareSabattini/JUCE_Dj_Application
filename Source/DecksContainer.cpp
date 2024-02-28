#include <JuceHeader.h>
#include "DecksContainer.h"

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
    
    int x = 0;
    const int inBetweenWidth = 250;
    const int deckWidth = (getWidth()-inBetweenWidth)/2;

    for (auto& deckGUI : deckGUIs)
    {
        deckGUI->setBounds(x, 0,deckWidth, getHeight());
        x += deckWidth + inBetweenWidth;
    }
}
