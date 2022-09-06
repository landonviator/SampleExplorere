/*
  ==============================================================================

    HeaderComp.cpp
    Created: 4 Sep 2022 12:48:36am
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HeaderComp.h"

//==============================================================================
HeaderComp::HeaderComp(SampleExplorerAudioProcessor& p) : audioProcessor(p)
{
    settingsDialog = std::make_unique<juce::AlertWindow>("", "", juce::MessageBoxIconType::WarningIcon);
    initButtons(settingsBtn, "?");
    setSettingsBtnProps();
}

HeaderComp::~HeaderComp()
{
}

void HeaderComp::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(30, 39, 46).withAlpha(0.5f));
}

void HeaderComp::resized()
{
    const auto btnWidth = getWidth() * 0.04;
    const auto btnX = getWidth() - btnWidth * 1.5;
    const auto btnHeight = getHeight() * 0.5;
    const auto btnY = getHeight() * 0.25;
    settingsBtn.setBounds(btnX, btnY, btnWidth, btnHeight);
}

void HeaderComp::setSettingsBtnProps()
{
    settingsBtn.onClick = [this]()
    {
        chooser = std::make_unique<juce::FileChooser>("Choose Folder",
                                                                  juce::File(audioProcessor.sampleRoot.getFullPathName()),
                                                                  "*");
        
        const auto fileChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        
        chooser->launchAsync (fileChooserFlags, [this] (const auto& fc)
        {
            if (chooser->getResult().exists())
            {
                audioProcessor.sampleSaveLocation = chooser->getResult();
                
                DBG(audioProcessor.sampleSaveLocation.getFullPathName());
                
                audioProcessor.variableTree.setProperty("sampleSaveLocation", audioProcessor.sampleSaveLocation.getFullPathName(), nullptr);
            }
            
            else
            {
                settingsDialog->showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Bruh", "You didn't set where to save your files. Weird things might happen...");
            }
        });
    };
}

void HeaderComp::initButtons(juce::TextButton &btn, const juce::String btnText)
{
    const auto brighter = 1.0;
    const auto alpha = 0.5f;
    addAndMakeVisible(btn);
    btn.setButtonText(btnText);
    btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGB(59, 59, 152).withAlpha(0.0f));
    btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB(59, 59, 152).withAlpha(0.0f));
    btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(alpha));
    btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(alpha));
    btn.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(0.125f));
}
