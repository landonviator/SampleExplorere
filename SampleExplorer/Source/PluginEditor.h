/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/Components/FileListComp.h"
#include "GUI/Components/SamplePlayerComp.h"
#include "GUI/Components/HeaderComp.h"
#include "GUI/Components/EffectsComp.h"

//==============================================================================
/**
*/
class SampleExplorerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SampleExplorerAudioProcessorEditor (SampleExplorerAudioProcessor&);
    ~SampleExplorerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SampleExplorerAudioProcessor& audioProcessor;
    
    void initWindow();
    
    FileListComp fileListComp;
    SamplePlayerComp samplePlayerComp;
    juce::StringArray fileName;
    HeaderComp headerComp;
    EffectsComp effectsComp;
    
    std::unique_ptr<juce::DropShadower> sampleDropShadower;
    juce::DropShadow sampleDropShadow;
    
    std::unique_ptr<juce::DropShadower> headerDropShadower;
    juce::DropShadow headerDropShadow;
    
    std::unique_ptr<juce::DropShadower> fileDropShadower;
    juce::DropShadow fileDropShadow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleExplorerAudioProcessorEditor)
};
