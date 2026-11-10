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
class SlowDriveAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    SlowDriveAudioProcessorEditor (SlowDriveAudioProcessor&);
    ~SlowDriveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SlowDriveAudioProcessor& processor;
    juce::TextButton shelfButton { "Shelf" };
    juce::GroupComponent shelfPanel;
    juce::Label boardHint, shelfHint;
    std::array<juce::TextButton, 4> addButtons, leftButtons, rightButtons, removeButtons;
    bool shelfOpen = false;
    int lastOrder = -1;
    std::vector<int> boardOrder() const;
    void saveOrder (const std::vector<int>&);
    void timerCallback() override;
    struct Control
    {
        Control (juce::AudioProcessorValueTreeState&, const char* id, const char* name);
        juce::Slider slider;
        juce::Label label;
        juce::AudioProcessorValueTreeState::SliderAttachment attachment;
    };

    struct StageSwitch
    {
        StageSwitch (juce::AudioProcessorValueTreeState&, const char* id, const char* name);
        juce::ToggleButton button;
        juce::AudioProcessorValueTreeState::ButtonAttachment attachment;
    };

    struct PedalEnclosure : juce::GroupComponent
    {
        PedalEnclosure (juce::uint32 finishColour, const char* modelName);
        void paint (juce::Graphics&) override;
        const juce::Colour finish;
        const juce::String model;
    };

    struct PedalLookAndFeel : juce::LookAndFeel_V4
    {
        void drawToggleButton (juce::Graphics&, juce::ToggleButton&, bool, bool) override;
        void drawRotarySlider (juce::Graphics&, int, int, int, int, float, float, float,
                               juce::Slider&) override;
    };

    juce::LookAndFeel_V4 inkStyle;
    PedalLookAndFeel pedalStyle;
    struct StudioPage : juce::Component
    {
        explicit StudioPage (bool ampRoom) : isAmpRoom (ampRoom) {}
        void paint (juce::Graphics&) override;
        const bool isAmpRoom;
    };

    StudioPage pedalsPage { false }, ampPage { true };
    juce::TabbedComponent tabs { juce::TabbedButtonBar::TabsAtTop };
    PedalEnclosure gateCard { 0xffcbcbcb, "NG-01" };
    PedalEnclosure boostCard { 0xffcbcbcb, "CB-02" };
    PedalEnclosure driveCard { 0xffcbcbcb, "OD-03" };
    PedalEnclosure delayCard { 0xffcbcbcb, "DL-04" };
    void updateAmpChannelView();
    StageSwitch gateSwitch, delaySwitch;
    StageSwitch boostSwitch, driveSwitch, ampSwitch, cabSwitch, powerSwitch, standbySwitch;
    StageSwitch normalBright, vibratoBright, channelSelect, inputSelect, reverbSwitch, tremoloSwitch;
    Control gateThreshold, delayTime, feedback, delayMix;
    Control boost, drive, tone, level, ampLevel, cabTone;
    Control normalTreble, normalMiddle, normalBass;
    Control vibratoVolume, vibratoTreble, vibratoMiddle, vibratoBass, reverb, speed, intensity;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlowDriveAudioProcessorEditor)
};
