/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

namespace
{
auto toggle (const char* id, const char* name, bool enabled)
{
    return std::make_unique<juce::AudioParameterBool> (juce::ParameterID { id, 3 }, name, enabled);
}

auto amount (const char* id, const char* name, float initialValue)
{
    return std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { id, 3 }, name, juce::NormalisableRange<float> { 0.0f, 1.0f }, initialValue);
}
}

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
                "Tone", juce::NormalisableRange<float> { 0.0f, 1.0f }, 0.6f),
            std::make_unique<juce::AudioParameterInt> (juce::ParameterID { "pedalOrder", 5 }, "Pedal board order", 0, 4095, 2257),
            toggle ("driveOn", "Overdrive On", true),
            toggle ("boostOn", "Clean Boost On", false),
            amount ("boostAmount", "Boost", 0.4f),
            toggle ("ampOn", "Amp On", false),
            toggle ("ampPower", "Amp Power", true),
            toggle ("ampStandby", "Amp Standby", false),
            amount ("ampLevel", "Normal Volume", 0.5f),
            toggle ("cabOn", "Cabinet On", false),
            amount ("cabTone", "Cabinet Brightness", 0.5f),
            amount ("normalTreble", "Normal Treble", 0.50f),
            amount ("normalMiddle", "Normal Middle", 0.50f),
            amount ("normalBass", "Normal Bass", 0.50f),
            amount ("vibratoVolume", "Vibrato Volume", 0.50f),
            amount ("vibratoTreble", "Vibrato Treble", 0.50f),
            amount ("vibratoMiddle", "Vibrato Middle", 0.50f),
            amount ("vibratoBass", "Vibrato Bass", 0.50f),
            amount ("reverb", "Reverb", 0.25f),
            amount ("speed", "Speed", 0.40f),
            amount ("intensity", "Intensity", 0.00f),
            toggle ("normalBright", "Normal Bright", false),
            toggle ("vibratoBright", "Vibrato Bright", false),
            toggle ("vibratoChannel", "Vibrato Channel", false),
            toggle ("inputTwo", "Input 2", false),
            toggle ("reverbOn", "Reverb On", true),
            toggle ("tremoloOn", "Tremolo On", true),
            toggle ("gateOn", "Noise Gate On", false),
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID { "gateThreshold", 4 }, "Gate Threshold",
                juce::NormalisableRange<float> { -80.0f, -20.0f, 0.1f }, -55.0f,
                juce::AudioParameterFloatAttributes().withLabel ("dB")),
            toggle ("delayOn", "Delay On", false),
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID { "delayTime", 4 }, "Delay Time",
                juce::NormalisableRange<float> { 25.0f, 1000.0f, 1.0f, 0.5f }, 320.0f,
                juce::AudioParameterFloatAttributes().withLabel ("ms")),
            amount ("delayFeedback", "Delay Feedback", 0.30f),
            amount ("delayMix", "Delay Mix", 0.25f)
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
    return 20.0;
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
SlowDriveAudioProcessor::RigStage::RigStage (
    juce::AudioProcessorValueTreeState& state, const char* enabledId, const char* amountId)
    : enabledParameter (state.getRawParameterValue (enabledId)),
      amountParameter (state.getRawParameterValue (amountId))
{
}

void SlowDriveAudioProcessor::RigStage::prepare (double sampleRate)
{
    mix.reset (sampleRate, 0.02);
    amount.reset (sampleRate, 0.02);
    mix.setCurrentAndTargetValue (enabledParameter->load());
    amount.setCurrentAndTargetValue (amountParameter->load());
}

void SlowDriveAudioProcessor::RigStage::updateTargets()
{
    mix.setTargetValue (enabledParameter->load());
    amount.setTargetValue (amountParameter->load());
}

void SlowDriveAudioProcessor::updateExtraControls (double sampleRate)
{
    for (auto* control : { &ampPower, &ampStandby, &normalTreble, &normalMiddle, &normalBass, &vibratoVolume, &vibratoTreble, &vibratoMiddle, &vibratoBass, &reverb, &speed, &intensity, &normalBright, &vibratoBright, &vibratoChannel, &inputTwo, &reverbOn, &tremoloOn,
                           &delayFeedback, &delayBlend })
    {
        const auto target = control->parameter->load();
        if (sampleRate > 0.0)
        {
            control->value.reset (sampleRate, 0.02);
            control->value.setCurrentAndTargetValue (target);
        }
        else
        {
            control->value.setTargetValue (target);
        }
    }
}

void SlowDriveAudioProcessor::prepareToPlay (double sampleRate, int)
{
    constexpr double smoothingSeconds = 0.02;
    level.reset (sampleRate, smoothingSeconds);
    drive.reset (sampleRate, smoothingSeconds);
    tone.reset (sampleRate, smoothingSeconds);
    level.setCurrentAndTargetValue (levelParameter->load());
    drive.setCurrentAndTargetValue (driveParameter->load());
    tone.setCurrentAndTargetValue (toneParameter->load());

    driveMix.reset (sampleRate, smoothingSeconds);
    driveMix.setCurrentAndTargetValue (driveEnabled->load());
    boost.prepare (sampleRate);
    amp.prepare (sampleRate);
    gate.prepare (sampleRate);
    delay.prepare (sampleRate);
    delayBuffer.setSize (2, static_cast<int> (std::ceil (sampleRate)) + 2);
    delayBuffer.clear();
    delayWritePosition = 0;
    gateEnvelope = 0.0f;
    gateGain = 1.0f;
    gateDetectorRelease = static_cast<float> (std::exp (-1.0 / (sampleRate * 0.025)));
    gateAttack = static_cast<float> (-std::expm1 (-1.0 / (sampleRate * 0.002)));
    gateRelease = static_cast<float> (-std::expm1 (-1.0 / (sampleRate * 0.075)));
    updateExtraControls (sampleRate);
    inverseSampleRate = static_cast<float> (1.0 / sampleRate);
    tremoloPhase = 0.0f;
    ampBassFilter.fill (0.0f);
    ampTrebleFilter.fill (0.0f);
    springInputFilter.fill (0.0f);
    spring.setSampleRate (sampleRate);
    spring.reset();
    juce::Reverb::Parameters springSettings;
    springSettings.roomSize = 0.62f;
    springSettings.damping = 0.45f;
    springSettings.wetLevel = 0.65f;
    springSettings.dryLevel = 0.0f;
    springSettings.width = 0.30f;
    spring.setParameters (springSettings);
    cabinet.prepare (sampleRate);
    cabBass.fill (0.0f);
    cabLowpass1.fill (0.0f);
    cabLowpass2.fill (0.0f);

    inputLowpass.fill (0.0f);
    outputLowpass.fill (0.0f);
    const auto filterStep = [sampleRate] (double cutoffHz)
    {
        return static_cast<float> (-std::expm1 (-juce::MathConstants<double>::twoPi
                                                * cutoffHz / sampleRate));
    };
    ampBassStep = filterStep (170.0);
    ampTrebleStep = filterStep (1250.0);
    springInputStep = filterStep (450.0);
    inputFilterStep = filterStep (130.0);
    outputFilterStep = filterStep (2500.0);
    cabBassStep = filterStep (90.0);
    cabDarkStep = filterStep (2200.0);
    cabBrightStep = filterStep (5400.0);
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
    driveMix.setTargetValue (driveEnabled->load());
    boost.updateTargets();
    amp.updateTargets();
    gate.updateTargets();
    delay.updateTargets();
    updateExtraControls();
    cabinet.updateTargets();

    const auto inputChannels = getTotalNumInputChannels();
    const auto sampleCount = buffer.getNumSamples();
    for (int channel = inputChannels; channel < getTotalNumOutputChannels(); ++channel)
        buffer.clear (channel, 0, sampleCount);

    auto* const* channels = buffer.getArrayOfWritePointers();
    auto* const* delayLines = delayBuffer.getArrayOfWritePointers();
    const auto delaySize = delayBuffer.getNumSamples();
    const int order = static_cast<int> (parameters.getRawParameterValue ("pedalOrder")->load());
    for (int sample = 0; sample < sampleCount; ++sample)
    {
        const float threshold = juce::Decibels::decibelsToGain (gate.amount.getNextValue());
        const float gateMix = gate.mix.getNextValue();
        const float delayEnabled = delay.mix.getNextValue();
        const float feedback = 0.8f * delayFeedback.value.getNextValue();
        const float wetMix = delayEnabled * delayBlend.value.getNextValue();
        const float samplesBack = 0.001f * delay.amount.getNextValue() / inverseSampleRate;
        const auto wholeSamples = static_cast<int> (samplesBack);
        const float fraction = samplesBack - static_cast<float> (wholeSamples);
        const int firstTap = (delayWritePosition - wholeSamples + delaySize) % delaySize;
        const int secondTap = (firstTap + delaySize - 1) % delaySize;

        const float boostGain = 1.0f + 3.0f * boost.amount.getNextValue() * boost.mix.getNextValue();
        const float overdriveMix = driveMix.getNextValue();
        const float ampMix = amp.mix.getNextValue();
        const float powerGain = ampPower.value.getNextValue() * (1.0f - ampStandby.value.getNextValue());
        const float channelMix = vibratoChannel.value.getNextValue();
        const float volume = juce::jmap (channelMix, amp.amount.getNextValue(),
                                         vibratoVolume.value.getNextValue());
        const float bass = juce::jmap (channelMix, normalBass.value.getNextValue(),
                                       vibratoBass.value.getNextValue());
        const float middle = juce::jmap (channelMix, normalMiddle.value.getNextValue(),
                                         vibratoMiddle.value.getNextValue());
        const float treble = juce::jmap (channelMix, normalTreble.value.getNextValue(),
                                         vibratoTreble.value.getNextValue());
        const float bright = juce::jmap (channelMix, normalBright.value.getNextValue(),
                                         vibratoBright.value.getNextValue());
        const float inputGain = 1.0f - 0.5f * inputTwo.value.getNextValue();
        const float reverbMix = channelMix * reverb.value.getNextValue() * reverbOn.value.getNextValue();
        const float tremoloDepth = channelMix * intensity.value.getNextValue() * tremoloOn.value.getNextValue();
        const float pulse = 0.5f + 0.5f * std::sin (tremoloPhase * juce::MathConstants<float>::twoPi);
        const float tremoloGain = 1.0f - 0.95f * tremoloDepth * pulse * pulse;
        tremoloPhase += (1.5f + 7.5f * speed.value.getNextValue()) * inverseSampleRate;
        if (tremoloPhase >= 1.0f)
            tremoloPhase -= 1.0f;

        const float bassGain = 0.12f + 1.65f * bass;
        const float middleGain = 0.15f + 1.25f * middle;
        const float trebleGain = 0.15f + 1.65f * treble + bright * (1.0f - volume) * 0.85f;
        std::array<float, 2> pedalSignal {}, ampSignal {}, springSignal {};
        const float cabMix = cabinet.mix.getNextValue();
        const float cabStep = juce::jmap (cabinet.amount.getNextValue(), cabDarkStep, cabBrightStep);
        const float outputLevel = level.getNextValue();
        const float driveAmount = drive.getNextValue();
        const float toneAmount = tone.getNextValue();
        const float preampGain = 1.0f + 8.0f * driveAmount;

        std::array<float, 2> chain {};
        for (int c = 0; c < inputChannels; ++c) chain[c] = channels[c][sample];
        unsigned seen = 0;
        for (int slot = 0; slot < 4; ++slot)
        {
            const int stage = (order >> (slot * 3)) & 7;
            if (stage < 1 || stage > 4 || (seen & (1u << stage))) continue;
            seen |= 1u << stage;
            if (stage == 1)
            {
                gateEnvelope *= gateDetectorRelease;
                for (int c = 0; c < inputChannels; ++c)
                    gateEnvelope = juce::jmax (gateEnvelope, std::abs (chain[c]));
                const float target = gateEnvelope >= threshold ? 1.0f : 0.0f;
                gateGain += (target - gateGain) * (target > gateGain ? gateAttack : gateRelease);
                for (int c = 0; c < inputChannels; ++c)
                    chain[c] *= juce::jmap (gateMix, 1.0f, gateGain);
            }
            for (int c = 0; c < inputChannels; ++c)
            {
                auto& value = chain[c];
                if (stage == 2) value *= boostGain;
                if (stage == 3)
                {
                    inputLowpass[c] += inputFilterStep * (value - inputLowpass[c]);
                    const float clipped = std::tanh (preampGain * (value - inputLowpass[c]));
                    outputLowpass[c] += outputFilterStep * (clipped - outputLowpass[c]);
                    const float wet = outputLowpass[c] + toneAmount * (clipped - outputLowpass[c]);
                    value = juce::jmap (overdriveMix, value, outputLevel * (value + driveAmount * (wet - value)));
                }
                if (stage == 4)
                {
                    auto* line = delayLines[c];
                    const float echo = juce::jmap (fraction, line[firstTap], line[secondTap]);
                    line[delayWritePosition] = delayEnabled * (value + feedback * echo);
                    value = juce::jmap (wetMix, value, echo);
                }
            }
        }
        if (!(seen & (1u << 4)))
            for (int c = 0; c < inputChannels; ++c) delayLines[c][delayWritePosition] = 0;
        for (int channel = 0; channel < inputChannels; ++channel)
        {
            const auto index = static_cast<size_t> (channel);
            const float pedalOut = chain[index];

            pedalSignal[index] = pedalOut;
            const float ampInput = inputGain * pedalOut;
            ampBassFilter[index] += ampBassStep * (ampInput - ampBassFilter[index]);
            ampTrebleFilter[index] += ampTrebleStep * (ampInput - ampTrebleFilter[index]);
            const float voiced = bassGain * ampBassFilter[index]
                               + middleGain * (ampTrebleFilter[index] - ampBassFilter[index])
                               + trebleGain * (ampInput - ampTrebleFilter[index]);

            // Increasing the channel volume pushes the preamp into soft breakup.
            ampSignal[index] = std::tanh (voiced * volume * (1.5f + 2.0f * volume));
            springInputFilter[index] += springInputStep * (ampSignal[index] - springInputFilter[index]);
            springSignal[index] = channelMix * (ampSignal[index] - springInputFilter[index]);
        }

        if (++delayWritePosition == delaySize)
            delayWritePosition = 0;

        if (inputChannels == 2)
            spring.processStereo (&springSignal[0], &springSignal[1], 1);
        else if (inputChannels == 1)
            spring.processMono (&springSignal[0], 1);

        for (int channel = 0; channel < inputChannels; ++channel)
        {
            const auto index = static_cast<size_t> (channel);
            const float powerAmp = (ampSignal[index] + reverbMix * springSignal[index]) * tremoloGain * powerGain;
            const float ampOut = juce::jmap (ampMix, pedalSignal[index], powerAmp);

            // A bass rolloff and two low-pass stages approximate speaker bandwidth.
            cabBass[index] += cabBassStep * (ampOut - cabBass[index]);
            cabLowpass1[index] += cabStep * (ampOut - cabBass[index] - cabLowpass1[index]);
            cabLowpass2[index] += cabStep * (cabLowpass1[index] - cabLowpass2[index]);
            channels[channel][sample] = juce::jmap (cabMix, ampOut, cabLowpass2[index]);
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
