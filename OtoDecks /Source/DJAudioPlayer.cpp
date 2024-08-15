/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 19 Feb 2024 11:20:12pm
    Author:  Kirby Loh

  ==============================================================================
*/

#include "DJAudioPlayer.h"
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager
                            ) : formatManager(_formatManager)
{
    reverbParams.wetLevel = 0.0;
    reverbParams.dryLevel = 1.0;
    reverbAudioSource.setParameters(reverbParams);
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    reverbAudioSource.getNextAudioBlock(bufferToFill); ///////
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbAudioSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    DBG("DJAudioPlayer::loadURL called");
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}
void DJAudioPlayer::play()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative position should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.25 || ratio > 4.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0.25 and 4");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setReverbWetLevel(float wetLevel)
{
    DBG("DJAudioPlayer::setReverbWetLevel called");
    if (wetLevel < 0 || wetLevel > 1.0)
    {
        DBG("DJAudioPlayer::setWetLevel level should be between 0 and 1.0");
    }
    else {
        reverbParams.wetLevel = wetLevel;
        reverbAudioSource.setParameters(reverbParams);
    }
}

void DJAudioPlayer::setReverbDryLevel(float dryLevel)
{
    DBG("DJAudioPlayer::setReverbDryLevel called");
    if (dryLevel < 0 || dryLevel > 1.0)
    {
        DBG("DJAudioPlayer::setDryLevel level should be between 0 and 1.0");
    }
    else {
        reverbParams.dryLevel = dryLevel;
        reverbAudioSource.setParameters(reverbParams);
    }
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}
