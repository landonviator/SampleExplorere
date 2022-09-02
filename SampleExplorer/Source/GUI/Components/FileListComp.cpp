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
    fileBrowserComp->setColour(juce::FileBrowserComponent::ColourIds::filenameBoxBackgroundColourId, juce::Colours::transparentBlack);
    fileBrowserComp->setColour(juce::FileBrowserComponent::ColourIds::currentPathBoxBackgroundColourId, juce::Colours::transparentBlack);
    fileBrowserComp->setColour(juce::FileSearchPathListComponent::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    fileBrowserComp->setLookAndFeel(&customFileBrowser);
    addAndMakeVisible(*fileBrowserComp);
}

FileListComp::~FileListComp()
{
    fileBrowserComp->setLookAndFeel(nullptr);
}

void FileListComp::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::whitesmoke.withAlpha(0.25f));   // clear the background
}

void FileListComp::resized()
{
    fileBrowserComp->setBounds(getLocalBounds());
}
