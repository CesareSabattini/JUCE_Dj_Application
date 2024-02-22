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
    g.fillAll(juce::Colours::red);
   
}

void DecksContainer::resized()
{
    int x = 0; 
    const int width = 130;
    const int height = 300;
    for (auto& deckGUI : deckGUIs)
    {
        deckGUI->setBounds(x, 10, width, height); 
        x += width + 10;
    }
}
