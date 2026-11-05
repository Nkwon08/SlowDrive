# SlowDrive

SlowDrive is a C++/JUCE guitar overdrive inspired by the low-gain guitar sound on John Mayer's *Where the Light Is*. It runs as a VST3 audio effect and has Drive, Tone, and Level controls.

The driven signal goes through a bass-cut filter, soft clipping, and a tone filter before blending with the dry guitar. Drive controls both the input boost into the clipping stage and the amount of driven signal in the blend. The controls use 20 ms smoothing and their settings are saved by the host.

## Build on macOS

1. Install Xcode and download JUCE with Projucer from [JUCE](https://juce.com/download/).
2. Put the extracted JUCE folder next to `SlowDrive.jucer` as `JUCE/`, so the modules are at `JUCE/modules/`.
3. Open `SlowDrive.jucer` in Projucer. Save and open the project in Xcode.
4. Select **SlowDrive - VST3** and choose **Product → Build**. If needed, copy the resulting bundle from `Builds/MacOSX/build/Debug/` to `~/Library/Audio/Plug-Ins/VST3/`.
5. Enable VST3 plug-ins in Ableton Live and rescan. Place SlowDrive before a clean amp and cab simulator.

Start with Drive at 0.25 and Tone at 0.6. Match the bypassed volume with Level when comparing sounds.
