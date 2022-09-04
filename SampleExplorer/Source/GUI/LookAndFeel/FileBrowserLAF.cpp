/*
  ==============================================================================

    FileBrowserLAF.cpp
    Created: 2 Sep 2022 12:56:27pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "FileBrowserLAF.h"

namespace juce
{
CustomFileBrowserLAF::CustomFileBrowserLAF()
{
        
}

void CustomFileBrowserLAF::drawFileBrowserRow (Graphics& g, int width, int height,
                                                   const File&, const String& filename, Image* icon,
                                                   const String& fileSizeDescription,
                                                   const String& fileTimeDescription,
                                                   bool isDirectory, bool isItemSelected,
                                   int /*itemIndex*/, DirectoryContentsDisplayComponent& dcc)
{
    
    juce::Image iconImage;
    
    if (isDirectory)
    {
        iconImage = juce::ImageCache::getFromMemory(BinaryData::openfolder_png, BinaryData::openfolder_pngSize);
    }
    
    else
    {
        iconImage = juce::ImageCache::getFromMemory(BinaryData::audiofile_png, BinaryData::audiofile_pngSize);
    }
    
    auto fileListComp = dynamic_cast<Component*> (&dcc);

    if (isItemSelected)
        g.fillAll (fileListComp != nullptr ? fileListComp->findColour (DirectoryContentsDisplayComponent::highlightColourId)
                                           : findColour (DirectoryContentsDisplayComponent::highlightColourId));

    const int x = 32;
    
    icon = &iconImage;

    if (icon != nullptr && icon->isValid())
    {
            g.drawImageWithin (*icon, 2, 2, x - 4, height - 4,
                               RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                               false);
    }

    if (isItemSelected)
        g.setColour (fileListComp != nullptr ? fileListComp->findColour (DirectoryContentsDisplayComponent::highlightedTextColourId)
                                             : findColour (DirectoryContentsDisplayComponent::highlightedTextColourId));
    else
        g.setColour (fileListComp != nullptr ? fileListComp->findColour (DirectoryContentsDisplayComponent::textColourId)
                                             : findColour (DirectoryContentsDisplayComponent::textColourId));

    g.setFont ((float) height * 0.7f);

    if (width > 450 && ! isDirectory)
    {
        auto sizeX = roundToInt ((float) width * 0.7f);
        auto dateX = roundToInt ((float) width * 0.8f);

        g.drawFittedText (filename,
                          x, 0, sizeX - x, height,
                          Justification::centredLeft, 1);

        g.setFont ((float) height * 0.5f);
        g.setColour (Colours::transparentBlack);

        if (! isDirectory)
        {
            g.drawFittedText (fileSizeDescription,
                              sizeX, 0, dateX - sizeX - 8, height,
                              Justification::centredRight, 1);

            g.drawFittedText (fileTimeDescription,
                              dateX, 0, width - 8 - dateX, height,
                              Justification::centredRight, 1);
        }
    }
    else
    {
        g.drawFittedText (filename,
                          x, 0, width - x, height,
                          Justification::centredLeft, 1);

    }
}

Button* CustomFileBrowserLAF::createFileBrowserGoUpButton()
{
    auto* goUpButton = new DrawableButton ("up", DrawableButton::ImageOnButtonBackground);

    const auto brighter = 1.0;
    const auto alpha = 0.5f;
    goUpButton->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGB(59, 59, 152).withAlpha(0.0f));
    goUpButton->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB(59, 59, 152).withAlpha(0.0f));
    goUpButton->setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(alpha));
    goUpButton->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(alpha));
    goUpButton->setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(0.125f));
    
    Path arrowPath;
    arrowPath.addArrow ({ 50.0f, 100.0f, 50.0f, 0.0f }, 40.0f, 100.0f, 50.0f);

    DrawablePath arrowImage;
    arrowImage.setFill (goUpButton->findColour (TextButton::textColourOffId));
    arrowImage.setPath (arrowPath);

    goUpButton->setImages (&arrowImage);

    return goUpButton;
}

void CustomFileBrowserLAF::layoutFileBrowserComponent (FileBrowserComponent& browserComp,
                                                 DirectoryContentsDisplayComponent* fileListComponent,
                                                 FilePreviewComponent* previewComp,
                                                 ComboBox* currentPathBox,
                                                 TextEditor* filenameBox,
                                                 Button* goUpButton)
{
    const auto brighter = 1.0;
    const auto alpha = 0.9f;
    setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::ListBox::ColourIds::outlineColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(0.125f));
    setColour(juce::DirectoryContentsDisplayComponent::ColourIds::textColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(alpha));
    
    filenameBox->setColour(juce::TextEditor::ColourIds::outlineColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(0.125f));
    filenameBox->setColour(juce::TextEditor::ColourIds::textColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(alpha));
    filenameBox->setColour(juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(0.25f));
    
    currentPathBox->setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(0.125f));
    currentPathBox->setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(0.125f));
    currentPathBox->setColour(juce::ComboBox::ColourIds::textColourId, juce::Colour::fromRGB(40, 42, 53).brighter(brighter).withAlpha(alpha));
    

    const int x = 8;
    auto w = browserComp.getWidth() - x - x;

    if (previewComp != nullptr)
    {
        auto previewWidth = w / 3;
        previewComp->setBounds (x + w - previewWidth, 0, previewWidth, browserComp.getHeight());

        w -= previewWidth + 4;
    }

    int y = 4;

    const int controlsHeight = 22;
    const int upButtonWidth = 50;
    auto bottomSectionHeight = controlsHeight + 8;

    currentPathBox->setBounds (x, y, w - upButtonWidth - 6, controlsHeight);
    goUpButton->setBounds (x + w - upButtonWidth, y, upButtonWidth, controlsHeight);

    y += controlsHeight + 4;

    if (auto listAsComp = dynamic_cast<Component*> (fileListComponent))
    {
        
        listAsComp->setBounds (x, y, w, browserComp.getHeight() - y - bottomSectionHeight);
        y = listAsComp->getBottom() + 4;
    }

    filenameBox->setBounds (x + 50, y, w - 50, controlsHeight);
}
}
