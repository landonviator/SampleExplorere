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
SamplePlayerComp::SamplePlayerComp(SampleExplorerAudioProcessor& p) :
audioProcessor(p),
state (Stopped),
thumbnailCache (5),
thumbnail (512, formatManager, thumbnailCache)
{
    thumbnail.addChangeListener (this);
    
    formatManager.registerBasicFormats();
    audioProcessor.transportSource.addChangeListener (this);
    
    initButtons(openBtn, "Open");
    initButtons(playBtn, "Play");
    initButtons(loopBtn, "Loop");
    initButtons(stopBtn, "Stop");
    setButtonEvents();
    
    addMouseListener(this, true);
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

    g.setColour (juce::Colour::fromRGB(252, 66, 123).withAlpha(0.8f));

    thumbnail.drawChannels (g,
                            thumbnailBounds.withSizeKeepingCentre(thumbnailBounds.getWidth() * 0.9, thumbnailBounds.getHeight() * 0.9),
                            0.0,
                            thumbnail.getTotalLength(),
                            1.0f);
}

void SamplePlayerComp::resized()
{
    const auto buttonX = getWidth() * 0.0125;
    const auto buttonY = getHeight() * 0.62;
    const auto buttonWidth = getWidth() * 0.075;
    const auto buttonHeight = buttonWidth * 0.5;
    const auto spaceBetween = 1.3;
    
    openBtn.setBounds(buttonX,
                      buttonY,
                      buttonWidth,
                      buttonHeight);
    playBtn.setBounds(buttonX,
                      openBtn.getY() + openBtn.getHeight() * spaceBetween,
                      buttonWidth,
                      buttonHeight);
    loopBtn.setBounds(buttonX,
                       playBtn.getY() + playBtn.getHeight() * spaceBetween,
                       buttonWidth,
                       buttonHeight);
    stopBtn.setBounds(buttonX,
                      loopBtn.getY() + loopBtn.getHeight() * spaceBetween,
                      buttonWidth,
                      buttonHeight);
}

void SamplePlayerComp::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    /**Logic for changing the listener callback when the playing state changes*/
     if (source == &audioProcessor.transportSource)
     {
         if (audioProcessor.transportSource.isPlaying())
         {
             changeState (Playing);
         }
         
         else if ((state == Stopping) || (state == Playing))
         {
             changeState (Stopped);
         }
         
         else if (state == Pausing)
         {
             changeState (Paused);
         }
     }
     
     if (source == &thumbnail)
     {
         thumbnailChanged();
     }
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
                playBtn.setButtonText ("Play");
                stopBtn.setButtonText ("Reset");
                stopBtn.setEnabled (false);
                audioProcessor.transportSource.setPosition (0.0);
                break;
     
            case Starting:
                audioProcessor.transportSource.start();
                break;
     
            case Playing:
                playBtn.setButtonText ("Pause");
                stopBtn.setButtonText ("Reset");
                stopBtn.setEnabled (true);
                break;
     
            case Pausing:
                audioProcessor.transportSource.stop();
                break;
     
            case Paused:
                playBtn.setButtonText ("Play");
                stopBtn.setButtonText ("Reset");
                break;
     
            case Stopping:
                audioProcessor.transportSource.stop();
                break;
        }
    }
}

void SamplePlayerComp::loadFile(const juce::File &file)
{
    auto* reader = formatManager.createReaderFor (file);
    
    if (file.existsAsFile() && reader != nullptr)
    {
        /**Create reader source*/
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));
        
        /**Set the player's source in the audio processor to the reader source*/
        audioProcessor.transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        
        /**Enable play button*/
        playBtn.setEnabled (true);
        
        /**Tell the thumbnail to use the reader source for its data*/
        thumbnail.setSource (new juce::FileInputSource (file));
        
        /**Reset the reader source for its next use*/
        audioProcessor.readerSource.reset (newSource.release());
    }
    
    else
    {
        alertWindow = std::make_unique<juce::AlertWindow>("Error!", "There was an error with either the file you tried to load or the format, please verify that the file is valid.", juce::MessageBoxIconType::WarningIcon);
        
        alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Error!", "There was an error with either the file you tried to load or the format, please verify that the file is valid.");
    }
}

void SamplePlayerComp::initButtons(juce::TextButton &btn, const juce::String btnText)
{
    addAndMakeVisible(btn);
    btn.setButtonText(btnText);
}

void SamplePlayerComp::setButtonEvents()
{
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
    
    playBtn.onClick = [this]()
    {
        if (state == Stopped || state == Paused)
        {
            changeState (Starting);
        }
        
        else if (state == Playing)
        {
            changeState (Pausing);
        }
    };
    
    stopBtn.onClick = [this]()
    {
        if (state == Paused)
        {
            changeState (Stopped);
        }
        
        else
        {
            changeState (Stopping);
        }
    };
    
    loopBtn.setClickingTogglesState(true);
    
    loopBtn.onClick = [this]()
    {
        audioProcessor.readerSource->setLooping(loopBtn.getToggleState());
    };
}
