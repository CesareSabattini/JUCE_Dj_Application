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
    int x = 10; 
    const int width = 420;
    const int height = 300;
    for (auto& deckGUI : deckGUIs)
    {
        deckGUI->setBounds(x, 8, width, height); 
        x += width + 10;
    }
}
