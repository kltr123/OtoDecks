/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 19 Feb 2024 11:20:12pm
    Author:  Kirby Loh

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        /**Loads the audio file*/
        void loadURL(juce::URL audioURL);
        /**Plays loaded audio file*/
        void play();
        /**Stops playing audio file*/
        void stop();
        /**Sets relative position of audio file*/
        void setPositionRelative(double pos);
        /**Sets the volume*/
        void setGain(double gain);
        /**Sets the speed*/
        void setSpeed(double ratio);
        /**Sets the amount of reverb for wet level*/
        void setReverbWetLevel(float wetLevel);
        /**Sets the amount of reverb for dry level*/
        void setReverbDryLevel(float dryLevel);
        /**Gets relative position of playhead*/
        double getPositionRelative();
        /**Gets the length of transport source in seconds*/
        double getLengthInSeconds();
        

    private:
        void setPosition(double posInSecs);
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
        juce::ReverbAudioSource reverbAudioSource{ &resampleSource, false };
        juce::Reverb::Parameters reverbParams;
};

