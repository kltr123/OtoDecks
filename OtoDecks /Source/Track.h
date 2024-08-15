/*
  ==============================================================================

    Track.h
    Created: 19 Feb 2024 11:20:12pm
    Author:  Kirby Loh

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Track
{
    public:
        Track(juce::File _file);
        juce::URL URL;
        juce::File file;
        juce::String duration;
        juce::String title;
        
        /**objects are compared by title*/
        bool operator==(const juce::String& other) const;
};
