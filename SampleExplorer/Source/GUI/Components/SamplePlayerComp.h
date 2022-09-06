/*
  ==============================================================================

    SamplePlayerComp.h
    Created: 2 Sep 2022 2:00:00pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "../LookAndFeel/TextButtonLAF.h"

//==============================================================================
/*
*/
class SamplePlayerComp  : public juce::Component
, public juce::ChangeListener
, private juce::Timer
{
public:
    SamplePlayerComp(SampleExplorerAudioProcessor&);
    ~SamplePlayerComp() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void loadFile(const juce::File& file);
    
    void timerCallback () override
    {
        repaint();
    }

private:
    
    SampleExplorerAudioProcessor& audioProcessor;
    
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };
    
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::TextButton openBtn, playBtn, stopBtn, loopBtn, exportBtn;
    juce::CustomTextButton customTextButton;
    void initButtons(juce::TextButton& btn, const juce::String btnText);
    void cleanMemory();
    void setButtonEvents();
    std::unique_ptr<juce::AlertWindow> alertWindow;
    
    void transportSourceChanged();
    void thumbnailChanged();
    void changeState (TransportState newState);
    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplePlayerComp)
};
