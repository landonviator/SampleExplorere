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
    startTimer(40);
    thumbnail.addChangeListener (this);
    
    formatManager.registerBasicFormats();
    audioProcessor.transportSource.addChangeListener (this);
    
    initButtons(openBtn, "Open");
    initButtons(playBtn, "Play");
    initButtons(loopBtn, "Loop");
    initButtons(stopBtn, "Stop");
    initButtons(exportBtn, "Export");
    setButtonEvents();
    
    addMouseListener(this, true);
}

SamplePlayerComp::~SamplePlayerComp()
{
    stopTimer();
    cleanMemory();
}

void SamplePlayerComp::paint (juce::Graphics& g)
{
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(40, 42, 53).darker(0.35f), getHeight(), juce::Colour::fromRGB(40, 42, 53).darker(0.15f), getHeight() * 0.4));
    g.fillRect(getLocalBounds());
    
    juce::Rectangle<int> thumbnailBounds (getWidth() * 0.025,
                                          getHeight() * 0.4,
                                          getWidth() - getWidth() * 0.05,
                                          getHeight() * 0.5);
    
    g.setColour(juce::Colour::fromRGB(47, 54, 64).darker(1.0));
    auto height = 0.3;
    g.drawLine(0, getHeight() * height, getWidth(), getHeight() * height, 1.0);
    
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded (g, thumbnailBounds);
    else
        paintIfFileLoaded (g, thumbnailBounds);
}

void SamplePlayerComp::paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::transparentBlack);
    g.fillRect (thumbnailBounds);
    g.setColour (juce::Colour::fromRGB(40, 42, 53).brighter(1.0f).withAlpha(0.9f));
    g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void SamplePlayerComp::paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::transparentBlack);
    g.fillRect (thumbnailBounds);

    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(172, 67, 237), getHeight(), juce::Colour::fromRGB(90, 182, 223), getHeight() * 0.4));

    /**Draw waveform*/
    auto audioLength = (float) thumbnail.getTotalLength();
    
    thumbnail.drawChannels (g,
                            thumbnailBounds,
                            0.0,
                            audioLength,
                            1.0f);
    
    /**Playhead line color*/
    g.setColour (juce::Colours::whitesmoke.withAlpha(0.1f));
     
    /**Draw playhead*/
    auto audioPosition = (float) audioProcessor.transportSource.getCurrentPosition();
    auto drawPosition = (audioPosition / audioLength) * (float) thumbnailBounds.getWidth()
                                + (float) thumbnailBounds.getX();
    g.drawLine (drawPosition, (float) thumbnailBounds.getY(), drawPosition,
                        (float) thumbnailBounds.getBottom(), 2.0f);
}

void SamplePlayerComp::resized()
{
    const auto buttonX = getWidth() * 0.265;
    const auto buttonY = getHeight() * 0.08;
    const auto buttonWidth = getWidth() * 0.075;
    const auto buttonHeight = buttonWidth * 0.5;
    const auto spaceBetween = 1.3;
    
    openBtn.setBounds(buttonX,
                      buttonY,
                      buttonWidth,
                      buttonHeight);
    playBtn.setBounds(openBtn.getX() + openBtn.getWidth() * spaceBetween,
                      buttonY,
                      buttonWidth,
                      buttonHeight);
    loopBtn.setBounds(playBtn.getX() + playBtn.getWidth() * spaceBetween,
                      buttonY,
                       buttonWidth,
                       buttonHeight);
    stopBtn.setBounds(loopBtn.getX() + loopBtn.getWidth() * spaceBetween,
                      buttonY,
                      buttonWidth,
                      buttonHeight);
    exportBtn.setBounds(stopBtn.getX() + stopBtn.getWidth() * spaceBetween,
                        buttonY,
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
                if (exportBtn.getToggleState() == true)
                {
                    // Turn of export so we don't write forever
                    exportBtn.triggerClick();
                }
                
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
    if (state == Stopped || state == Paused)
    {
        audioProcessor.droppedFile = file;
        audioProcessor.tempFileToWrite = audioProcessor.sampleSaveLocation.getChildFile("temp.wav");
        auto* reader = formatManager.createReaderFor (audioProcessor.droppedFile);
        
        if (file.existsAsFile() && reader != nullptr)
        {
            /**Create reader source*/
            std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));
            
            /**Set the player's source in the audio processor to the reader source*/
            audioProcessor.transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
            
            /**Enable play button*/
            playBtn.setEnabled (true);
            
            /**Tell the thumbnail to use the reader source for its data*/
            thumbnail.setSource (new juce::FileInputSource (audioProcessor.droppedFile));
            
            /**Reset the reader source for its next use*/
            audioProcessor.readerSource.reset (newSource.release());
            
        }
        
        audioProcessor.tempFileToWrite.deleteFile();
        
        audioProcessor.writer.reset (audioProcessor.format.createWriterFor (new juce::FileOutputStream (audioProcessor.tempFileToWrite),
                                                                            audioProcessor.getSampleRate(),
                                                                            2,
                                                                            24,
                                                                            {},
                                                                            0));
    }
    
    else
    {
        alertWindow = std::make_unique<juce::AlertWindow>("Error!", "There was an error with either the file you tried to load or the format, please verify that the file is valid.", juce::MessageBoxIconType::WarningIcon);
        
        alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Error!", "There was an error with either the file you tried to load or the format, please verify that the file is valid.");
    }
    
}

void SamplePlayerComp::initButtons(juce::TextButton &btn, const juce::String btnText)
{
    const auto alpha = 0.25f;
    addAndMakeVisible(btn);
    btn.setButtonText(btnText);
    btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);
    btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB(90, 182, 223).withAlpha(alpha));
    btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colour::fromRGB(90, 182, 223));
    btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour::fromRGB(90, 182, 223));
    btn.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGB(210, 218, 226).withAlpha(alpha));
    btn.setLookAndFeel(&customTextButton);
}

void SamplePlayerComp::setButtonEvents()
{
    // Open
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
                loadFile(file);
            }
        });
    };
    
    // Play
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
    
    // Stop
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
    
    // Loop
    loopBtn.setClickingTogglesState(true);
    
    loopBtn.onClick = [this]()
    {
        if (audioProcessor.readerSource == nullptr) return;
        audioProcessor.readerSource->setLooping(loopBtn.getToggleState());
    };
    
    // Export
    exportBtn.setClickingTogglesState(true);
    exportBtn.onClick = [this]()
    {
            // If the file isn't valid, break out of click event
        if (!audioProcessor.sampleSaveLocation.exists())
        {
            
            alertWindow = std::make_unique<juce::AlertWindow>("Error!", "You didn't set a save location in the settings menu!.", juce::MessageBoxIconType::WarningIcon);
            
            alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Error!", "You didn't set a save location in the settings menu!.");
                
                // Turn off export button so we don't get the above message twice
            exportBtn.setToggleState(false, juce::dontSendNotification);
                
            return;
        }
        
        if (audioProcessor.readerSource == nullptr)
        {
            return;
        }
            
        if (exportBtn.getToggleState() == true)
        {
            if (loopBtn.getToggleState() == true)
            {
                loopBtn.setToggleState(false, juce::dontSendNotification);
            }
                
            // Turn off looping
            audioProcessor.readerSource->setLooping(false);
                
            // Disable loop button
            loopBtn.setEnabled(false);
                
            // Enable exporting
            audioProcessor.exportEnabled.store(true);
                
            // Auto start so that we don't record silence
            playBtn.triggerClick();
        }
            
        else
        {
            audioProcessor.writer.reset();
            
            // Disable exporting so it doesn't write forever
            audioProcessor.exportEnabled.store(false);
                
            // Re-enable the loop button
            loopBtn.setEnabled(true);
                
            // Reset the transport position
            audioProcessor.transportSource.setPosition(0.0);
                
            // Auto reset
            stopBtn.triggerClick();
            
            // Stop the audio during save*/
            if (playBtn.getToggleState())
            {
                playBtn.triggerClick();
            }
            
            // Save the file to the saved location*/
            auto targetFileProcessed = audioProcessor.sampleSaveLocation.getChildFile("processed_" + audioProcessor.droppedFile.getFileName());
            
            // Save a dry and processed version of the file*/
            audioProcessor.tempFileToWrite.copyFileTo(targetFileProcessed);
            
            // Delete temp
            audioProcessor.sampleSaveLocation.getChildFile("temp.wav").deleteFile();
        }
    };
}

void SamplePlayerComp::cleanMemory()
{
    openBtn.setLookAndFeel(nullptr);
    playBtn.setLookAndFeel(nullptr);
    loopBtn.setLookAndFeel(nullptr);
    stopBtn.setLookAndFeel(nullptr);
    exportBtn.setLookAndFeel(nullptr);
}
