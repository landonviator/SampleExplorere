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
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(40, 42, 53).darker(0.15f), getHeight(), juce::Colour::fromRGB(40, 42, 53).brighter(0.02), getHeight() * 0.4));
    g.fillRect(getLocalBounds());
}

void EffectsComp::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
