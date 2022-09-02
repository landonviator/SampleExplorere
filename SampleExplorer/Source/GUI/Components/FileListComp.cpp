/*
  ==============================================================================

    FileListComp.cpp
    Created: 2 Sep 2022 12:34:44am
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FileListComp.h"

//==============================================================================
FileListComp::FileListComp()
{
   auto location = juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory);
    fileBrowserComp = std::make_unique<juce::FileBrowserComponent>(5, location, &sampleExplorerFilter, nullptr);
    addAndMakeVisible(*fileBrowserComp);
}

FileListComp::~FileListComp()
{
}

void FileListComp::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::whitesmoke.withAlpha(0.25f));   // clear the background
}

void FileListComp::resized()
{
    fileBrowserComp->setBounds(getLocalBounds());
}
