/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

SlowDriveAudioProcessorEditor::Control::Control (
    juce::AudioProcessorValueTreeState& parameters, const char* id, const char* name)
    : attachment (parameters, id, slider)
{
    slider.setName (name);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 68, 22);
    slider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffefba71));
    slider.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff4b5154));
    slider.setColour (juce::Slider::thumbColourId, juce::Colour (0xffffdfb0));
    label.setText (name, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
}

SlowDriveAudioProcessorEditor::SlowDriveAudioProcessorEditor (SlowDriveAudioProcessor& processor)
    : AudioProcessorEditor (&processor),
      drive (processor.parameters, "drive", "Drive"),
      tone (processor.parameters, "tone", "Tone"),
      level (processor.parameters, "gain", "Level")
{
    for (auto* control : { &drive, &tone, &level })
    {
        addAndMakeVisible (control->slider);
        addAndMakeVisible (control->label);
    }

    setSize (390, 216);
}

SlowDriveAudioProcessorEditor::~SlowDriveAudioProcessorEditor() = default;

void SlowDriveAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff202729));
    g.setColour (juce::Colour (0xffefba71));
    g.setFont (juce::FontOptions (27.0f));
    g.drawText ("SlowDrive", 18, 12, getWidth() - 36, 34, juce::Justification::centredLeft);
    g.setColour (juce::Colour (0xffc3c8c4));
    g.setFont (juce::FontOptions (13.0f));
    g.drawText ("Guitar overdrive", 19, 44, getWidth() - 38, 20, juce::Justification::centredLeft);
}

void SlowDriveAudioProcessorEditor::resized()
{
    auto row = getLocalBounds().reduced (18, 14);
    row.removeFromTop (57);
    const auto width = row.getWidth() / 3;
    for (auto* control : { &drive, &tone, &level })
    {
        auto column = row.removeFromLeft (width).reduced (6, 0);
        control->label.setBounds (column.removeFromTop (22));
        control->slider.setBounds (column);
    }
}
