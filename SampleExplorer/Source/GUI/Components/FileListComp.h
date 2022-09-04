/*
  ==============================================================================

    FileListComp.h
    Created: 2 Sep 2022 12:34:44am
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel/FileBrowserLAF.h"
#include "SamplePlayerComp.h"
#include "../../PluginProcessor.h"

//==============================================================================
/*
*/
class FileListComp  : public juce::Component
, public juce::DragAndDropContainer
, juce::FileBrowserListener
{
public:
    FileListComp(SamplePlayerComp&, SampleExplorerAudioProcessor&);
    ~FileListComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDrag(const juce::MouseEvent &event) override;
    
    /**Virtual functions for file browser*/
    void fileClicked (const juce::File &file, const juce::MouseEvent &event) override;
    void selectionChanged () override;
    void fileDoubleClicked (const juce::File &file) override;
    void browserRootChanged (const juce::File &newRoot) override;
    
private:
    
    SampleExplorerAudioProcessor& audioProcessor;
    SamplePlayerComp& samplePlayer;
    
    std::unique_ptr<juce::FileBrowserComponent> fileBrowserComp;
    juce::WildcardFileFilter sampleExplorerFilter{"*.wav,* *.aif,* *.mp3*", "", "Sample filter"};
    juce::CustomFileBrowserLAF customFileBrowser;
    juce::StringArray fileName;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileListComp)
};
