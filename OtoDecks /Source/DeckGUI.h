/*
  ==============================================================================

    DeckGUI.h
    Created: 19 Feb 2024 11:20:12pm
    Author:  Kirby Loh

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(int _id,
            DJAudioPlayer* player,
            juce::AudioFormatManager& formatManager,
            juce::AudioThumbnailCache& thumbCache);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /**Implement Button::Listener*/
    void buttonClicked(juce::Button* button) override;
    /**Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;
    /**Detects if file is being dragged over deck*/
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    /**Detects if file is dropped onto deck*/
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    /**Listen for changes to the waveform*/
    void timerCallback() override;

private:
    int id;
    
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    juce::TextButton loopStartButton{ "START LOOP" };
    juce::TextButton loopEndButton{ "END LOOP" };
    juce::TextButton loopRemoveButton{ "REMOVE LOOP" };
    juce::Slider volSlider;
    juce::Label volLabel;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    juce::Slider posSlider;
    juce::Label posLabel;
    juce::Slider wetLevelSlider;
    juce::Label wetLevelLabel;
    juce::Slider dryLevelSlider;
    juce::Label dryLevelLabel;
    
    bool loopEnabled;
    double loopStartPosition;
    double loopEndPosition;

    void loadFile(juce::URL audioURL);

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    juce::SharedResourcePointer< juce::TooltipWindow > sharedTooltip;

    friend class PlaylistComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
