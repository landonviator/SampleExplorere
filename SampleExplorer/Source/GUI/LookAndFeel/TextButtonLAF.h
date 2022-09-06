/*
  ==============================================================================

    TextButtonLAF.h
    Created: 5 Sep 2022 7:12:07pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace juce
{
    class CustomTextButton : public LookAndFeel_V4
    {
        public:
        CustomTextButton();
        
        void drawButtonBackground (Graphics& g,
                                   Button& button,
                                   const Colour& backgroundColour,
                                   bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown) override;
    };
}
