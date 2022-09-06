/*
  ==============================================================================

    EffectsComp.h
    Created: 5 Sep 2022 7:04:41pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class EffectsComp  : public juce::Component
{
public:
    EffectsComp();
    ~EffectsComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectsComp)
};
