/*
  ==============================================================================

    PlaylistComponent.h
    Created: 19 Feb 2024 11:20:12pm
    Author:  Kirby Loh

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Track.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2,
                      DJAudioPlayer* _playerForParsingMetaData
                     );
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected
                           ) override;
    void paintCell(juce::Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected
                  ) override;
    
    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component* existingComponentToUpdate) override;
    void buttonClicked(juce::Button* button) override;
private:
    std::vector<Track> tracks;
    
    juce::TextButton importButton{ "ADD TRACKS TO LIBRARY" };
    juce::TableListBox library;
    juce::TextEditor searchArea;
    juce::TextButton loadToDeckGUI1Button{ "LOAD TO DECKGUI 1" };
    juce::TextButton loadToDeckGUI2Button{ "LOAD TO DECKGUI 2" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;
    
    juce::String getLength(juce::URL audioURL);
    juce::String secondsToMinutes(double seconds);

    void importToLibrary();
    void saveLibrary();
    void loadLibrary();
    void deleteFromTracks(int id);
    void searchLibrary(juce::String searchText);
    int whereInTracks(juce::String searchText);
    bool isInTracks(juce::String fileNameWithoutExtension);
    void loadInPlayer(DeckGUI* deckGUI);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
