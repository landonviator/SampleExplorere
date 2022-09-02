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

//==============================================================================
/*
*/
class FileListComp  : public juce::Component
{
public:
    FileListComp();
    ~FileListComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    std::unique_ptr<juce::FileBrowserComponent> fileBrowserComp;
    juce::WildcardFileFilter sampleExplorerFilter{"*.wav,* *.aif,* *.mp3*", "", "Sample filter"};
    juce::CustomFileBrowserLAF customFileBrowser;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileListComp)
};
