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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleExplorerAudioProcessorEditor)
};
