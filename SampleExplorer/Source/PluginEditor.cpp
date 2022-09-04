/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SampleExplorerAudioProcessorEditor::SampleExplorerAudioProcessorEditor (SampleExplorerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
, fileListComp(samplePlayerComp)
, samplePlayerComp(audioProcessor)
{
    addAndMakeVisible(fileListComp);
    
    addAndMakeVisible(samplePlayerComp);
    initWindow();
}

SampleExplorerAudioProcessorEditor::~SampleExplorerAudioProcessorEditor()
{
}

//==============================================================================
void SampleExplorerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(252, 66, 123), getHeight(), juce::Colour::fromRGB(214, 162, 232), getHeight() * 0.3));
    g.fillRect(getLocalBounds());
}

void SampleExplorerAudioProcessorEditor::resized()
{
    const auto fileX = getWidth() * 0.02;
    const auto fileY = getHeight() * 0.03;
    const auto fileWidth = getHeight() * 0.5;
    const auto fileHeight = getHeight() - fileY * 2.0;
    fileListComp.setBounds(fileX, fileY, fileWidth, fileHeight);
    
    const auto leftPad = fileListComp.getX() + fileListComp.getWidth() * 1.1;
    const auto sampleWidth = getWidth() * 0.68;
    samplePlayerComp.setBounds(leftPad, fileY, sampleWidth, fileHeight);
}

void SampleExplorerAudioProcessorEditor::initWindow()
{
    // Grab the window instance and create a rectangle
    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
    
    // Using the width is more useful than the height, because we know the height will always be < than width
    int x = r.getWidth();
    
    auto width = 0;
    
    if (r.getWidth() <= 1920)
    {
        width = x * 0.9;
    }
    
    else
    {
        width = x * 0.35;
    }
    
    auto height = width * 0.35;
    
    //Making the window resizable by aspect ratio and setting size
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(width * 0.75, height * 0.75, width * 1.25, height * 1.25);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
    
    if (audioProcessor.windowWidth != 0.0)
    {
        setSize(audioProcessor.windowWidth, audioProcessor.windowHeight);
    }
        
    else
    {
        setSize (width, height);
    }
}
