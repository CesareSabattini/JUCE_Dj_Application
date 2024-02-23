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
    g.fillAll(juce::Colours::black);
   
}

void DecksContainer::resized()
{
    int borderGap = 10; 
    int x = borderGap;
    const int height = 300;
    for (auto& deckGUI : deckGUIs)
    {
        deckGUI->setBounds(x, borderGap, (getWidth()-2*borderGap)/2, (getHeight() - 140));
        x += (getWidth() - 2 * borderGap) / 2 + borderGap;
    }
}
