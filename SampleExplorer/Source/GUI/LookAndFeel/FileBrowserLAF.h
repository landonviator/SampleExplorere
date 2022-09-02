/*
  ==============================================================================

    FileBrowserLAF.h
    Created: 2 Sep 2022 12:56:27pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace juce
{
class CustomFileBrowserLAF : public LookAndFeel_V4
{
public:
    CustomFileBrowserLAF();
    
    void drawFileBrowserRow (Graphics& g, int width, int height,
                                             const File&, const String& filename, Image* icon,
                                             const String& fileSizeDescription,
                                             const String& fileTimeDescription,
                                             bool isDirectory, bool isItemSelected,
                             int /*itemIndex*/, DirectoryContentsDisplayComponent& dcc) override;
};
}
