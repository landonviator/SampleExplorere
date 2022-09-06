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
FileListComp::FileListComp(SamplePlayerComp& s, SampleExplorerAudioProcessor& p) : audioProcessor(p), samplePlayer(s)
{
    juce::File location;
    
    if (!audioProcessor.sampleRoot.exists())
    {
        location = juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory);
    }
   
    else
    {
        location = audioProcessor.sampleRoot.getFullPathName();
    }
    
    fileBrowserComp = std::make_unique<juce::FileBrowserComponent>(5, location.getFullPathName(), &sampleExplorerFilter, nullptr);
    fileBrowserComp->setColour(juce::FileBrowserComponent::ColourIds::filenameBoxBackgroundColourId, juce::Colours::transparentBlack);
    fileBrowserComp->setColour(juce::FileBrowserComponent::ColourIds::currentPathBoxBackgroundColourId, juce::Colours::transparentBlack);
    fileBrowserComp->setColour(juce::FileSearchPathListComponent::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    fileBrowserComp->addListener(this);
    fileBrowserComp->setLookAndFeel(&customFileBrowser);
    addAndMakeVisible(*fileBrowserComp);
    
    addMouseListener(this, true);
}

FileListComp::~FileListComp()
{
    fileBrowserComp->setLookAndFeel(nullptr);
}

void FileListComp::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(39, 60, 117).withAlpha(0.5f));
}

void FileListComp::resized()
{
    fileBrowserComp->setBounds(getLocalBounds());
}

void FileListComp::mouseDrag(const juce::MouseEvent &event)
{
    /**The plugin will now recognize when you're dragging a file*/
    startDragging("File", this);
    
    /**Perform drag operation*/
    performExternalDragDropOfFiles(fileName, true);
}

void FileListComp::fileClicked (const juce::File &file, const juce::MouseEvent &event)
{
    
}

void FileListComp::fileDoubleClicked(const juce::File &file)
{
    juce::StringArray fileArray(file.getFullPathName());
    
    if (file.getFileExtension() == ".wav" || file.getFileExtension() == ".mp3")
    {
        fileName = fileArray;
        samplePlayer.loadFile(file);
    }
    
    audioProcessor.variableTree.setProperty("sampleRoot", file.getParentDirectory().getFullPathName(), nullptr);
}

void FileListComp::browserRootChanged(const juce::File &newRoot)
{
    
}

void FileListComp::selectionChanged()
{

}
