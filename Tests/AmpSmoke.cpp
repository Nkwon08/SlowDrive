#include "PluginProcessor.h"
#include <iostream>
#include <stdexcept>
static void set(SlowDriveAudioProcessor& p, const char* id, float value)
{
    auto* parameter = p.parameters.getParameter(id);
    if (!parameter) throw std::runtime_error(id);
    parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
}
static float signal(SlowDriveAudioProcessor& p)
{
    juce::AudioBuffer<float> audio(2, 256); juce::MidiBuffer midi;
    float peak = 0;
    for (int block = 0; block < 30; ++block)
    {
        for(int c=0;c<2;++c) for(int i=0;i<256;++i)
            audio.setSample(c,i,0.1f * std::sin(float(block*256+i)*0.05f));
        p.processBlock(audio,midi);
        if(block==29) peak=audio.getMagnitude(0,0,256);
    }
    return peak;
}
int main()
{
    SlowDriveAudioProcessor p;
    set(p,"driveOn",0); set(p,"ampOn",1); set(p,"cabOn",0);
    p.prepareToPlay(48000,256);
    if(signal(p)<0.01f) return 1;
    set(p,"ampStandby",1); if(signal(p)>0.00001f) return 2;
    set(p,"ampStandby",0); if(signal(p)<0.01f) return 3;
    set(p,"ampPower",0); if(signal(p)>0.00001f) return 4;
    set(p,"ampOn",0); if(signal(p)<0.01f) return 5;
    set(p,"vibratoVolume",0.73f);
    juce::MemoryBlock saved; p.getStateInformation(saved);
    SlowDriveAudioProcessor restored; restored.setStateInformation(saved.getData(),int(saved.getSize()));
    if(std::abs(restored.parameters.getRawParameterValue("vibratoVolume")->load()-0.73f)>0.00001f) return 6;
    if(restored.parameters.getRawParameterValue("ampPower")->load()!=0) return 7;
    SlowDriveAudioProcessor a, b, empty;
    for (auto* fx : { &a, &b, &empty }) {
        set(*fx,"ampOn",0); set(*fx,"cabOn",0); set(*fx,"driveOn",1); set(*fx,"boostOn",1);
        set(*fx,"drive",0.8f); set(*fx,"boostAmount",0.9f);
    }
    set(a,"pedalOrder",2 + (3 << 3));
    set(b,"pedalOrder",3 + (2 << 3));
    set(empty,"pedalOrder",0);
    for (auto* fx : { &a, &b, &empty }) fx->prepareToPlay(48000,256);
    if (std::abs(signal(a)-signal(b)) < 0.001f) return 8;
    if (std::abs(signal(empty)-0.1f)>0.0001f) return 9;
    a.getStateInformation(saved); restored.setStateInformation(saved.getData(),int(saved.getSize()));
    if(restored.parameters.getRawParameterValue("pedalOrder")->load()!=26) return 10;
    std::cout << "PASS: reordered audio differs, empty board bypass, order restored\n";
    std::cout << "PASS: amp signal, standby mute/resume, power mute, bypass, state round-trip\n";
}
