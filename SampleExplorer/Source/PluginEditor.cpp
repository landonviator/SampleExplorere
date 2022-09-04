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
, fileListComp(samplePlayerComp, audioProcessor)
, samplePlayerComp(audioProcessor)
, headerComp(audioProcessor)
{
    sampleDropShadow.radius = 24;
    sampleDropShadow.offset = juce::Point<int> (-1, 4);
    sampleDropShadow.colour = juce::Colours::black.withAlpha(0.5f);
    
    sampleDropShadower = std::make_unique<juce::DropShadower>(sampleDropShadow);
    sampleDropShadower->setOwner(&samplePlayerComp);
    
    headerDropShadow.radius = 16;
    headerDropShadow.offset = juce::Point<int> (-1, 4);
    headerDropShadow.colour = juce::Colours::black.withAlpha(0.5f);
    
    headerDropShadower = std::make_unique<juce::DropShadower>(headerDropShadow);
    headerDropShadower->setOwner(&headerComp);
    
    fileDropShadow.radius = 16;
    fileDropShadow.offset = juce::Point<int> (-1, 4);
    fileDropShadow.colour = juce::Colours::black.withAlpha(0.5f);
    
    fileDropShadower = std::make_unique<juce::DropShadower>(fileDropShadow);
    fileDropShadower->setOwner(&fileListComp);
    
    addAndMakeVisible(fileListComp);
    addAndMakeVisible(samplePlayerComp);
    addAndMakeVisible(headerComp);
    initWindow();
}

SampleExplorerAudioProcessorEditor::~SampleExplorerAudioProcessorEditor()
{
}

//==============================================================================
void SampleExplorerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(40, 42, 53).darker(0.85f), getHeight(), juce::Colour::fromRGB(40, 42, 53).brighter(0.02), getHeight() * 0.4));
    g.fillRect(getLocalBounds());
}

void SampleExplorerAudioProcessorEditor::resized()
{
    const auto fileX = getWidth() * 0.02;
    const auto fileY = getHeight() * 0.18;
    const auto fileWidth = getHeight() * 0.5;
    const auto fileHeight = getHeight() * 0.8;
    fileListComp.setBounds(fileX, fileY, fileWidth, fileHeight);
    
    const auto leftPad = fileListComp.getX() + fileListComp.getWidth() * 1.1;
    const auto sampleWidth = getWidth() * 0.68;
    const auto sampleY = getHeight() * 0.63;
    const auto sampleHeight = getHeight()  * 0.35;
    samplePlayerComp.setBounds(leftPad, sampleY, sampleWidth, sampleHeight);
    
    headerComp.setBounds(0, 0, getWidth(), getHeight() * 0.11);
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
