/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SlowDriveAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SlowDriveAudioProcessorEditor (SlowDriveAudioProcessor&);
    ~SlowDriveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    struct Control
    {
        Control (juce::AudioProcessorValueTreeState&, const char* id, const char* name);
        juce::Slider slider;
        juce::Label label;
        juce::AudioProcessorValueTreeState::SliderAttachment attachment;
    };

    Control drive, tone, level;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlowDriveAudioProcessorEditor)
};
