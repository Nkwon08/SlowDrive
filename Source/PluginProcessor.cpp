/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
SlowDriveAudioProcessor::SlowDriveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
        #if ! JucePlugin_IsMidiEffect
            #if ! JucePlugin_IsSynth
                .withInput ("Input", juce::AudioChannelSet::stereo(), true)
            #endif
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
        #endif
      ),
#else
    :
#endif
    parameters (*this, nullptr, "Parameters",
        juce::AudioProcessorValueTreeState::ParameterLayout {
            std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID { "gain", 1 },
                "Level", juce::NormalisableRange<float> { 0.0f, 1.0f }, 0.5f),
            std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID { "drive", 2 },
                "Drive", juce::NormalisableRange<float> { 0.0f, 1.0f }, 0.0f),
            std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID { "tone", 2 },
                "Tone", juce::NormalisableRange<float> { 0.0f, 1.0f }, 0.6f)
        })
{
}

SlowDriveAudioProcessor::~SlowDriveAudioProcessor()
{
}

//==============================================================================
const juce::String SlowDriveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SlowDriveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SlowDriveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SlowDriveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SlowDriveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SlowDriveAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SlowDriveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SlowDriveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SlowDriveAudioProcessor::getProgramName (int index)
{
    return {};
}

void SlowDriveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SlowDriveAudioProcessor::prepareToPlay (double sampleRate, int)
{
    constexpr double smoothingSeconds = 0.02;
    level.reset (sampleRate, smoothingSeconds);
    drive.reset (sampleRate, smoothingSeconds);
    tone.reset (sampleRate, smoothingSeconds);
    level.setCurrentAndTargetValue (levelParameter->load());
    drive.setCurrentAndTargetValue (driveParameter->load());
    tone.setCurrentAndTargetValue (toneParameter->load());

    inputLowpass.fill (0.0f);
    outputLowpass.fill (0.0f);
    const auto filterStep = [sampleRate] (double cutoffHz)
    {
        return static_cast<float> (-std::expm1 (-juce::MathConstants<double>::twoPi
                                                * cutoffHz / sampleRate));
    };
    inputFilterStep = filterStep (130.0);
    outputFilterStep = filterStep (2500.0);
}

void SlowDriveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SlowDriveAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SlowDriveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    level.setTargetValue (levelParameter->load());
    drive.setTargetValue (driveParameter->load());
    tone.setTargetValue (toneParameter->load());

    const auto inputChannels = getTotalNumInputChannels();
    const auto sampleCount = buffer.getNumSamples();
    for (int channel = inputChannels; channel < getTotalNumOutputChannels(); ++channel)
        buffer.clear (channel, 0, sampleCount);

    auto* const* channels = buffer.getArrayOfWritePointers();
    for (int sample = 0; sample < sampleCount; ++sample)
    {
        const float outputLevel = level.getNextValue();
        const float driveAmount = drive.getNextValue();
        const float toneAmount = tone.getNextValue();
        const float preampGain = 1.0f + 8.0f * driveAmount;

        for (int channel = 0; channel < inputChannels; ++channel)
        {
            const auto index = static_cast<size_t> (channel);
            const float dry = channels[channel][sample];

            // Remove some bass before clipping to keep the driven sound clear.
            inputLowpass[index] += inputFilterStep * (dry - inputLowpass[index]);
            const float clipped = std::tanh (preampGain * (dry - inputLowpass[index]));

            // Tone blends the darker filtered signal with the full clipping signal.
            outputLowpass[index] += outputFilterStep * (clipped - outputLowpass[index]);
            const float wet = outputLowpass[index] + toneAmount * (clipped - outputLowpass[index]);
            channels[channel][sample] = outputLevel * (dry + driveAmount * (wet - dry));
        }
    }
}

//==============================================================================
bool SlowDriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SlowDriveAudioProcessor::createEditor()
{
    return new SlowDriveAudioProcessorEditor (*this);
}

//==============================================================================
void SlowDriveAudioProcessor::getStateInformation (
    juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    auto xml = state.createXml();
    copyXmlToBinary (*xml, destData);
}

void SlowDriveAudioProcessor::setStateInformation (
    const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary (data, sizeInBytes);

    if (xml != nullptr
        && xml->hasTagName (parameters.state.getType()))
    {
        parameters.replaceState (juce::ValueTree::fromXml (*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SlowDriveAudioProcessor();
}
