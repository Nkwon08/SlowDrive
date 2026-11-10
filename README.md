# SlowDrive

SlowDrive is a JUCE guitar effects plugin inspired by the clean, spacious sound of a vintage American combo amp. It combines a noise gate, clean boost, overdrive, delay, a Twin-inspired amp, and a cabinet stage in one rig.

## Features

- Separate **Pedalboard** and **Amp / Cabinet** pages.
- Switchable noise gate, clean boost, overdrive, and delay.
- Reorderable pedal signal chain with the order saved in the DAW session.
- Twin-inspired Normal and Vibrato channels, with channel routing, input selection, bright switches, reverb, and vibrato controls.
- Amp power, standby, amp bypass, and cabinet bypass controls.
- Host-automatable parameters and session state saving.
- A standalone HTML rig layout designer for arranging amp, cabinet, and pedal mockups before changing the plugin UI.

The amp and cabinet processing are original approximations, not circuit-accurate models of a Fender product. The amp uses simplified saturation and tone shaping, JUCE algorithmic reverb, and cabinet filtering. The standalone layout designer is a visual planning tool; its exported JSON is not currently loaded by the plugin.

## Build on macOS

1. Install JUCE 9 and Xcode.
2. Open `SlowDrive.jucer` in Projucer.
3. Set the JUCE modules path in Projucer, then save and open the generated Xcode project.
4. Build the **SlowDrive - VST3** scheme for Ableton Live, or **SlowDrive - Standalone Plugin** to run it by itself.

Generated Xcode files and build products are intentionally excluded from this repository. The `.jucer` project file is the source of truth for generating them.

## Rig layout mockup

Open [`SlowDrive-Rig-Layout-Designer.html`](SlowDrive-Rig-Layout-Designer.html) in a browser. It has separate Amp / Cabinet and Pedalboard views. Drag items to move them, use the corner handle to resize, and edit labels or colors in the side panel. Export the layout JSON to save or share your arrangement.

## Project structure

- `Source/` — JUCE processor, DSP, and plugin editor.
- `Tests/AmpSmoke.cpp` — standalone smoke-test source for amp behavior, bypass, state restoration, and pedal ordering.
- `SlowDrive-Rig-Layout-Designer.html` — browser-based visual layout mockup.
- `SlowDrive.jucer` — JUCE project configuration.# SlowDrive

SlowDrive is a C++/JUCE guitar overdrive inspired by the low-gain guitar sound on John Mayer's *Where the Light Is*. It runs as a VST3 audio effect and has Drive, Tone, and Level controls.

The driven signal goes through a bass-cut filter, soft clipping, and a tone filter before blending with the dry guitar. Drive controls both the input boost into the clipping stage and the amount of driven signal in the blend. The controls use 20 ms smoothing and their settings are saved by the host.

## Build on macOS

1. Install Xcode and download JUCE with Projucer from [JUCE](https://juce.com/download/).
2. Put the extracted JUCE folder next to `SlowDrive.jucer` as `JUCE/`, so the modules are at `JUCE/modules/`.
3. Open `SlowDrive.jucer` in Projucer. Save and open the project in Xcode.
4. Select **SlowDrive - VST3** and choose **Product → Build**. If needed, copy the resulting bundle from `Builds/MacOSX/build/Debug/` to `~/Library/Audio/Plug-Ins/VST3/`.
5. Enable VST3 plug-ins in Ableton Live and rescan. Place SlowDrive before a clean amp and cab simulator.

Start with Drive at 0.25 and Tone at 0.6. Match the bypassed volume with Level when comparing sounds.
