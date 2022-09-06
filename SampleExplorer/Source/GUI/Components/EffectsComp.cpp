/*
  ==============================================================================

    EffectsComp.cpp
    Created: 5 Sep 2022 7:04:41pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EffectsComp.h"

//==============================================================================
EffectsComp::EffectsComp()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

EffectsComp::~EffectsComp()
{
}

void EffectsComp::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(39, 60, 117).withAlpha(0.5f));
}

void EffectsComp::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
