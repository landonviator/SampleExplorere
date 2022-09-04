/*
  ==============================================================================

    HeaderComp.h
    Created: 4 Sep 2022 12:48:36am
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../PluginProcessor.h"

//==============================================================================
/*
*/
class HeaderComp  : public juce::Component
{
public:
    HeaderComp(SampleExplorerAudioProcessor&);
    ~HeaderComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    SampleExplorerAudioProcessor& audioProcessor;
    
    juce::TextButton settingsBtn;
    std::unique_ptr<juce::FileChooser> chooser;
    std::unique_ptr<juce::AlertWindow> settingsDialog;
    void setSettingsBtnProps();
    void initButtons(juce::TextButton &btn, const juce::String btnText);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComp)
};
