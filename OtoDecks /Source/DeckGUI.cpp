/*
  ==============================================================================

    DeckGUI.cpp
    Created: 19 Feb 2024 11:20:12pm
    Author:  Kirby Loh

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(int _id,
                 DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManager,
                 juce::AudioThumbnailCache& thumbCache
                 ) 
: id(_id),
player(_player),
waveformDisplay(id, formatManager, thumbCache)
{
    // add all components and make visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopStartButton);
    addAndMakeVisible(loopEndButton);
    addAndMakeVisible(loopRemoveButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(wetLevelSlider);
    addAndMakeVisible(wetLevelLabel);
    addAndMakeVisible(dryLevelSlider);
    addAndMakeVisible(dryLevelLabel);
    addAndMakeVisible(waveformDisplay);

    // add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopStartButton.addListener(this);
    loopEndButton.addListener(this);
    loopRemoveButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    wetLevelSlider.addListener(this);
    dryLevelSlider.addListener(this);
    
    // Set the background color for play button
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
    // Set the background color for stop button
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    // Set the background color for load button
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
    // Set the background color for start loop button
    loopStartButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkmagenta);
    // Set the background color for end loop button
    loopEndButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkviolet);
    // Set the background color for end loop button
    loopRemoveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkorchid);

    //configure volume slider and label
    double volDefaultValue = 0.5;
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setValue(volDefaultValue);
    volSlider.setSkewFactorFromMidPoint(volDefaultValue);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

    //configure speed slider and label
    double speedDefaultValue = 1.0;
    speedSlider.setRange(0.25, 4.0); //reaches breakpoint if sliderValue == 0
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
                              false,
                              50,
                              speedSlider.getTextBoxHeight()
                             );
    speedSlider.setValue(speedDefaultValue);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setSkewFactorFromMidPoint(speedDefaultValue);
    speedSlider.setMouseDragSensitivity(80);

    
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(juce::Justification::centred);

    //configure position slider and label
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
                              false,
                              50,
                              posSlider.getTextBoxHeight()
                             );
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);
    
    //configure wet level slider and label
    double wetLevelDefaultValue = 0.1;
    wetLevelSlider.setRange(0.0, 1.0);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
                              false,
                              50,
                              wetLevelSlider.getTextBoxHeight()
                             );
    wetLevelSlider.setNumDecimalPlacesToDisplay(2);
    wetLevelSlider.setValue(wetLevelDefaultValue);
    wetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
    wetLevelLabel.attachToComponent(&wetLevelSlider, false);
    //configure dry level slider and label
    double dryLevelDefaultValue = 0.8;
    dryLevelSlider.setRange(0.0, 1.0);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
                              false,
                              50,
                              dryLevelSlider.getTextBoxHeight()
                             );
    dryLevelSlider.setNumDecimalPlacesToDisplay(2);
    dryLevelSlider.setValue(dryLevelDefaultValue);
    dryLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
    dryLevelLabel.attachToComponent(&dryLevelSlider, false);
    
    //set colours for sliders
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::springgreen); //dial
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::slategrey); //body
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::slategrey); //body
    
    loopEnabled = false;
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setFont(25.0f);
    g.setColour(juce::Colours::orange);
    g.drawText("DeckGUI: " + std::to_string(id), 0, 3 * getHeight() / 8, getWidth(), 5 * getHeight() / 8, juce::Justification::topLeft , true);
}

void DeckGUI::resized()
{
     /*This method is where you should set the bounds of any child
     components that your component contains..*/
    //                   x start, y start, width, height
    waveformDisplay.setBounds(0, 0, getWidth(), 3 * getHeight() / 8);
    // buttons
    playButton.setBounds(2 * getWidth() / 4, 3 * getHeight() / 8, getWidth() / 4, getHeight() / 8);
    stopButton.setBounds(2 * getWidth() / 4, 4 * getHeight() / 8, getWidth() / 4, getHeight() / 8);
    loadButton.setBounds(2 * getWidth() / 4, 5 * getHeight() / 8, getWidth() / 4, getHeight() / 8);
    loopStartButton.setBounds(3 * getWidth() / 4, 3 * getHeight() / 8, getWidth() / 4, getHeight() / 8);
    loopEndButton.setBounds(3 * getWidth() / 4, 4 * getHeight() / 8, getWidth() / 4, getHeight() / 8);
    loopRemoveButton.setBounds(3 * getWidth() / 4, 5 * getHeight() / 8, getWidth() / 4, getHeight() / 8);
    // sliders
    volSlider.setBounds(getWidth() / 11 , 4 * getHeight() / 8, getWidth() / 16, getHeight() / 3);
    speedSlider.setBounds(3.5 * getWidth() / 10, 4 * getHeight() / 8, getWidth() / 6, getHeight() / 3);
    wetLevelSlider.setBounds(getWidth() / 6 , 4 * getHeight() / 8, getWidth() / 10, getHeight() / 3);
    dryLevelSlider.setBounds(getWidth() / 4 + 10, 4 * getHeight() / 8, getWidth() / 10, getHeight() / 3);
    posSlider.setBounds(getWidth() / 10, 7 * getHeight() / 8,  8 * getWidth() / 10, getHeight() / 8);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked ");
        player->play();
    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked ");
        player->stop();
    }
    if (button == &loadButton)
    {
        DBG("Load button was clicked ");
        juce::FileChooser chooser{"Select a file"};
        if (chooser.browseForFileToOpen())
        {
            loadFile(juce::URL{ chooser.getResult() });
        }
    }
    if(button == &loopStartButton)
    {
        DBG("Start Loop Button was clicked ");
        if (!loopEnabled) 
        {
            loopStartPosition = player->getPositionRelative(); // Store the loop start
        } 
        else
        {
            juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                "To create a new Loop:",
                "Please click on the Remove Loop Button.",
                "OKAY",
                nullptr
            );
        }
    }
    if(button == &loopEndButton)
    {
        DBG("End Loop Button was clicked ");
        loopEndPosition = player->getPositionRelative();
        if(loopEndPosition > loopStartPosition)
        {
            loopEnabled = true; // Enable looping when loop end is set
        }
    }
    if(button == &loopRemoveButton)
    {
        DBG("Remove Loop Button was clicked ");
        loopEnabled = false;
    }
    
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        DBG("Volume slider moved " << slider->getValue());
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        DBG("Speed slider moved " << slider->getValue());
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        DBG("Position slider moved " << slider->getValue());
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &wetLevelSlider)
    {
        DBG("Wet Level slider moved " << slider->getValue());
        player->setReverbWetLevel(slider->getValue());
    }
    if (slider == &dryLevelSlider)
    {
        DBG("Dry Level slider moved " << slider->getValue());
        player->setReverbDryLevel(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag called. "
        + std::to_string(files.size()) + " file(s) being dragged.");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{

    DBG("DeckGUI::filesDropped at " + std::to_string(x)
        + "x and " + std::to_string(y) + "y" );
    if (files.size() == 1)
    {
        loadFile(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback()
{
    if (loopEnabled && player->getPositionRelative() >= loopEndPosition)
        {
            player->setPositionRelative(loopStartPosition);
            player->play();
        }
    //check if the relative position is greater than 0
    //otherwise loading file causes error
    if (player->getPositionRelative() > 0)
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
    //check if the relative position is greater than or equal to 1
    //then loop the audio back to the beginning
    if(player->getPositionRelative() >= 1)
    {
        player->setPositionRelative(0);
        player->play();
    }
}
