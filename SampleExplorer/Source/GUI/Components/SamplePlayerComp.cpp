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
    g.fillAll(juce::Colour::fromRGB(39, 60, 117).withAlpha(0.5f));
    
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
    const auto brighter = 1.0;
    const auto alpha = 0.5f;
    addAndMakeVisible(btn);
    btn.setButtonText(btnText);
    btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);
    btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::transparentBlack);
    btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colour::fromRGB(90, 182, 223));
    btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour::fromRGB(90, 182, 223));
    btn.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGB(172, 67, 237).withAlpha(alpha));
    btn.setLookAndFeel(&customTextButton);
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
                loadFile(file);
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
        if (audioProcessor.readerSource == nullptr) return;
        audioProcessor.readerSource->setLooping(loopBtn.getToggleState());
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
