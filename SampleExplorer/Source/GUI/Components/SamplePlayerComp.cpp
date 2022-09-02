/*
  ==============================================================================

    SamplePlayerComp.cpp
    Created: 2 Sep 2022 2:00:00pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SamplePlayerComp.h"

//==============================================================================
SamplePlayerComp::SamplePlayerComp() :
state (Stopped),
thumbnailCache (5),
thumbnail (512, formatManager, thumbnailCache)
{
    thumbnail.addChangeListener (this);
    
    formatManager.registerBasicFormats();
    //audioProcessor.transportSource.addChangeListener (this);
    
    addAndMakeVisible(openBtn);
    openBtn.setButtonText("Open");
    openBtn.onClick = [this]()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File{},
                                                       "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;
 
        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
 
            if (file != juce::File{})
            {
                auto* reader = formatManager.createReaderFor (file);
 
                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                    transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                    thumbnail.setSource (new juce::FileInputSource (file));
                    readerSource.reset (newSource.release());
                }
            }
        });
    };
}

SamplePlayerComp::~SamplePlayerComp()
{
}

void SamplePlayerComp::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.brighter(0.1));
    
    juce::Rectangle<int> thumbnailBounds (getWidth() * 0.1, getHeight() * 0.6, getWidth() - getWidth() * 0.2, getHeight() * 0.3);

    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded (g, thumbnailBounds);
    else
        paintIfFileLoaded (g, thumbnailBounds);
}

void SamplePlayerComp::paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::black.brighter(0.15));
    g.fillRect (thumbnailBounds);
    g.setColour (juce::Colours::white);
    g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void SamplePlayerComp::paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::black.brighter(0.15));
    g.fillRect (thumbnailBounds);

    g.setColour (juce::Colours::whitesmoke.withAlpha(0.1f));

    thumbnail.drawChannels (g,
                            thumbnailBounds.withSizeKeepingCentre(thumbnailBounds.getWidth() * 0.9, thumbnailBounds.getHeight() * 0.9),
                            0.0,
                            thumbnail.getTotalLength(),
                            1.0f);
}

void SamplePlayerComp::resized()
{
    openBtn.setBounds(getLocalBounds().withSizeKeepingCentre(getWidth() * 0.1, getWidth() * 0.05));
}

void SamplePlayerComp::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &transportSource) transportSourceChanged();
    if (source == &thumbnail)       thumbnailChanged();
}

void SamplePlayerComp::transportSourceChanged()
{
    changeState (transportSource.isPlaying() ? Playing : Stopped);
}

void SamplePlayerComp::thumbnailChanged()
{
    repaint();
}

void SamplePlayerComp::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;
     
        switch (state)
        {
            case Stopped:
                
                // Turn of export so we don't write forever
//                if (exportToggle.getToggleState() == true)
//                {
//                    // Turn of export so we don't write forever
//                    exportToggle.triggerClick();
//
//                    // Auto save when the sample is done playing
//                    purchaseButton.triggerClick();
//                }
//
//                playButton.setButtonText ("Play");
//                resetButton.setButtonText ("Reset");
//                resetButton.setEnabled (false);
//                audioProcessor.transportSource.setPosition (0.0);
                break;
     
            case Starting:
                //audioProcessor.transportSource.start();
                break;
     
            case Playing:
                //playButton.setButtonText ("Pause");
                //resetButton.setButtonText ("Reset");
                //resetButton.setEnabled (true);
                break;
     
            case Pausing:
                //audioProcessor.transportSource.stop();
                break;
     
            case Paused:
                //playButton.setButtonText ("Play");
                //resetButton.setButtonText ("Reset");
                break;
     
            case Stopping:
                //audioProcessor.transportSource.stop();
                break;
        }
    }
}
