/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <array>

//==============================================================================
/**
*/
class SlowDriveAudioProcessor  : public juce::AudioProcessor
{
public:
    juce::AudioProcessorValueTreeState parameters;
    //==============================================================================
    SlowDriveAudioProcessor();
    ~SlowDriveAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    // Parameter storage belongs to the processor and outlives the audio callback.
    std::atomic<float>* const levelParameter = parameters.getRawParameterValue ("gain");
    std::atomic<float>* const driveParameter = parameters.getRawParameterValue ("drive");
    std::atomic<float>* const toneParameter = parameters.getRawParameterValue ("tone");

    juce::SmoothedValue<float> level, drive, tone;
    std::array<float, 2> inputLowpass {}, outputLowpass {};
    struct RigStage
    {
        RigStage (juce::AudioProcessorValueTreeState&, const char* enabledId, const char* amountId);
        void prepare (double sampleRate);
        void updateTargets();

        std::atomic<float>* const enabledParameter;
        std::atomic<float>* const amountParameter;
        juce::SmoothedValue<float> mix, amount;
    };

    RigStage boost { parameters, "boostOn", "boostAmount" };
    RigStage amp { parameters, "ampOn", "ampLevel" };
    RigStage cabinet { parameters, "cabOn", "cabTone" };
    std::atomic<float>* const driveEnabled = parameters.getRawParameterValue ("driveOn");
    juce::SmoothedValue<float> driveMix;

    struct SmoothedControl
    {
        SmoothedControl (juce::AudioProcessorValueTreeState& state, const char* id)
            : parameter (state.getRawParameterValue (id)) {}
        std::atomic<float>* const parameter;
        juce::SmoothedValue<float> value;
    };

    SmoothedControl ampPower { parameters, "ampPower" };
    SmoothedControl ampStandby { parameters, "ampStandby" };
    SmoothedControl normalTreble { parameters, "normalTreble" };
    SmoothedControl normalMiddle { parameters, "normalMiddle" };
    SmoothedControl normalBass { parameters, "normalBass" };
    SmoothedControl vibratoVolume { parameters, "vibratoVolume" };
    SmoothedControl vibratoTreble { parameters, "vibratoTreble" };
    SmoothedControl vibratoMiddle { parameters, "vibratoMiddle" };
    SmoothedControl vibratoBass { parameters, "vibratoBass" };
    SmoothedControl reverb { parameters, "reverb" };
    SmoothedControl speed { parameters, "speed" };
    SmoothedControl intensity { parameters, "intensity" };
    SmoothedControl normalBright { parameters, "normalBright" };
    SmoothedControl vibratoBright { parameters, "vibratoBright" };
    SmoothedControl vibratoChannel { parameters, "vibratoChannel" };
    SmoothedControl inputTwo { parameters, "inputTwo" };
    SmoothedControl reverbOn { parameters, "reverbOn" };
    SmoothedControl tremoloOn { parameters, "tremoloOn" };

    RigStage gate { parameters, "gateOn", "gateThreshold" };
    RigStage delay { parameters, "delayOn", "delayTime" };
    SmoothedControl delayFeedback { parameters, "delayFeedback" };
    SmoothedControl delayBlend { parameters, "delayMix" };
    juce::AudioBuffer<float> delayBuffer;
    int delayWritePosition = 0;
    float gateEnvelope = 0.0f, gateGain = 1.0f;
    float gateDetectorRelease = 0.0f, gateAttack = 0.0f, gateRelease = 0.0f;

    std::array<float, 2> ampBassFilter {}, ampTrebleFilter {}, springInputFilter {};
    juce::Reverb spring;
    float ampBassStep = 0.0f, ampTrebleStep = 0.0f, springInputStep = 0.0f;
    float tremoloPhase = 0.0f, inverseSampleRate = 0.0f;
    void updateExtraControls (double sampleRate = 0.0);

    std::array<float, 2> cabBass {}, cabLowpass1 {}, cabLowpass2 {};
    float cabBassStep = 0.0f;
    float cabDarkStep = 0.0f;
    float cabBrightStep = 0.0f;
    float inputFilterStep = 0.0f;
    float outputFilterStep = 0.0f;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlowDriveAudioProcessor)
};
