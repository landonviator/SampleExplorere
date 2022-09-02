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
        iconImage = juce::ImageCache::getFromMemory(BinaryData::icons8folder64_png, BinaryData::icons8folder64_pngSize);
    }
    
    else
    {
        iconImage = juce::ImageCache::getFromMemory(BinaryData::musicboxoutline_png, BinaryData::musicboxoutline_pngSize);
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
}
