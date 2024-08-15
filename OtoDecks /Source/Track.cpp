/*
  ==============================================================================

    Track.cpp
    Created: 19 Feb 2024 11:20:25pm
    Author:  Kirby Loh

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Track.h"

//==============================================================================
Track::Track(juce::File _file) : file(_file),
URL(juce::URL{ _file }),
title(_file.getFileNameWithoutExtension())
{
    DBG("Created track with title: " << title);
}

bool Track::operator==(const juce::String& other) const
{
    return title == other;
}
