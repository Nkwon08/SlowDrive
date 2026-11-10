/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

namespace
{

}

void SlowDriveAudioProcessorEditor::StudioPage::paint (juce::Graphics& g)
{
    const auto room = getLocalBounds().toFloat();
    g.fillAll (juce::Colour (0xff4a4a4a));
    if (! isAmpRoom)
    {
        g.setColour (juce::Colour (0x126d8196));
        for (float y = 0; y < room.getHeight(); y += 4)
            g.drawHorizontalLine (juce::roundToInt (y), 0, room.getWidth());
        return;
    }

    // The working controls live in this separate rack, never on the illustration.
    auto rack = juce::Rectangle<float> (12, 10, room.getWidth() - 24, 198);
    g.setColour (juce::Colour (0xff4a4a4a));
    g.fillRoundedRectangle (rack, 8);
    g.setColour (juce::Colour (0xff6d8196));
    g.drawRoundedRectangle (rack, 8, 1);
    g.drawLine (28, 53, room.getWidth() - 28, 53, 1);
    g.drawLine (324, 66, 324, 159, 1);
    g.drawLine (28, 167, room.getWidth() - 28, 167, 1);
    g.setColour (juce::Colour (0xffffffe3));
    g.setFont (juce::FontOptions (11.0f, juce::Font::bold));
    g.drawText ("NORMAL", 28, 58, 110, 18, juce::Justification::centredLeft);
    g.drawText ("VIBRATO", 342, 58, 110, 18, juce::Justification::centredLeft);

    // Original vector combo artwork: black tolex, silver grille, handle and casters.
    // All faceplate knobs below are paint only; they do not intercept input.
    juce::Graphics::ScopedSaveState artwork (g);
    const float scale = juce::jmin ((room.getWidth() - 80) / 640.0f,
                                   (room.getHeight() - 221) / 370.0f);
    g.addTransform (juce::AffineTransform::scale (scale).translated (
        room.getCentreX() - 320 * scale, 218));
    auto body = juce::Rectangle<float> (22, 33, 596, 300);
    g.setColour (juce::Colour (0x99000000));
    g.fillEllipse (5, 339, 630, 25);
    for (float x : { 72.0f, 548.0f })
    {
        g.setColour (juce::Colour (0xff92979b));
        g.fillRoundedRectangle (x - 6, 325, 12, 21, 3);
        g.setColour (juce::Colour (0xff090a0c));
        g.fillEllipse (x - 12, 337, 24, 29);
        g.setColour (juce::Colour (0xff81868b));
        g.fillEllipse (x - 6, 341, 12, 21);
        g.setColour (juce::Colour (0xff292c2e));
        g.fillEllipse (x - 2, 344, 7, 15);
    }
    g.setColour (juce::Colour (0xff9da2a6));
    g.fillRoundedRectangle (242, 18, 156, 13, 4);
    g.setColour (juce::Colour (0xff090a0b));
    g.fillRoundedRectangle (249, 8, 142, 20, 7);
    g.setColour (juce::Colour (0xff85898c));
    g.drawRoundedRectangle (254, 12, 132, 11, 4, 2);
    g.setColour (juce::Colour (0xff090a0b));
    g.fillRoundedRectangle (body, 15);
    g.setColour (juce::Colour (0xff61666b));
    g.drawRoundedRectangle (body.reduced (2), 13, 2);
    g.setColour (juce::Colour (0xff292c2f));
    g.drawRoundedRectangle (body.reduced (8), 8, 5);
    juce::Random texture (65212);
    for (int i = 0; i < 4200; ++i)
    {
        float x = 28 + texture.nextFloat() * 584, y = 39 + texture.nextFloat() * 288;
        g.setColour (juce::Colour (0x305f6468));
        g.fillEllipse (x, y, 1.4f, 0.8f);
    }
    const auto grille = juce::Rectangle<float> (46, 113, 548, 196);
    g.setColour (juce::Colour (0xff666a6c));
    g.fillRect (grille);
    for (float x : { 190.0f, 449.0f })
    {
        g.setColour (juce::Colour (0xff55595c));
        g.fillEllipse (x - 88, 123, 176, 176);
        g.setColour (juce::Colour (0xff4c5052));
        g.fillEllipse (x - 37, 174, 74, 74);
    }
    for (float y = 114; y < 309; y += 4)
    {
        g.setColour (juce::Colour (0xffa0a4a5));
        g.drawHorizontalLine (juce::roundToInt (y), 46, 594);
        g.setColour (juce::Colour (0xff373b3e));
        g.drawHorizontalLine (juce::roundToInt (y + 1), 46, 594);
    }
    for (float x = 47; x < 594; x += 4)
    {
        g.setColour (juce::Colour (0x999da2a4));
        g.drawVerticalLine (juce::roundToInt (x), 113, 309);
        g.setColour (juce::Colour (0x990d1012));
        g.drawVerticalLine (juce::roundToInt (x + 1), 113, 309);
    }
    g.setColour (juce::Colour (0xffc0c3c5));
    g.drawRect (grille.expanded (3), 2);
    auto face = juce::Rectangle<float> (46, 55, 548, 51);
    g.setColour (juce::Colour (0xffa2a6a9));
    g.fillRect (face.expanded (2));
    g.setColour (juce::Colour (0xff101214));
    g.fillRect (face);
    g.setColour (juce::Colour (0xffd4d6d8));
    g.setFont (juce::FontOptions (7.0f, juce::Font::bold));
    g.drawText ("NORMAL", 59, 58, 160, 10, juce::Justification::centred);
    g.drawText ("VIBRATO", 229, 58, 280, 10, juce::Justification::centred);
    for (float x : { 63.0f, 234.0f })
        for (float y : { 78.0f, 93.0f })
        {
            g.setColour (juce::Colour (0xffb4b7ba)); g.fillEllipse (x - 4, y - 4, 8, 8);
            g.setColour (juce::Colours::black); g.fillEllipse (x - 2, y - 2, 4, 4);
        }
    const char* labels[] = { "VOL", "TREB", "MID", "BASS", "VOL", "TREB", "MID", "BASS", "REV", "SPEED", "INT" };
    for (int i = 0; i < 11; ++i)
    {
        float x = i < 4 ? 95.0f + i * 34 : 265.0f + (i - 4) * 34;
        g.setColour (juce::Colour (0xffbfc2c5)); g.fillEllipse (x - 9, 76, 18, 18);
        g.setColour (juce::Colour (0xff202326)); g.fillEllipse (x - 7, 78, 14, 14);
        g.setColour (juce::Colour (0xffe0e2e3)); g.drawLine (x, 79, x, 83, 1.5f);
        g.setFont (juce::FontOptions (5.5f));
        g.drawText (labels[i], juce::roundToInt (x) - 15, 96, 30, 8, juce::Justification::centred);
    }
    g.setColour (juce::Colour (0xffe0e2e3));
    g.setFont (juce::FontOptions (9.0f, juce::Font::bold));
    g.drawText ("TWIN", 517, 66, 66, 13, juce::Justification::centred);
    g.setFont (juce::FontOptions (6.0f));
    g.drawText ("REVERB AMP", 517, 80, 66, 9, juce::Justification::centred);
    g.setColour (juce::Colour (0xffb7bbbe)); g.fillEllipse (546, 94, 6, 6);
    for (float x : { 25.0f, 594.0f })
        for (float y : { 36.0f, 307.0f })
        {
            g.setGradientFill (juce::ColourGradient (juce::Colour (0xffd0d3d5), x, y,
                                                   juce::Colour (0xff555a60), x + 22, y + 23, false));
            g.fillRoundedRectangle (x, y, 21, 23, 3);
            g.setColour (juce::Colour (0xff25282b)); g.fillEllipse (x + 8, y + 9, 4, 4);
        }
    g.setColour (juce::Colour (0xff151719));
    g.fillRoundedRectangle (468, 128, 111, 25, 3);
    g.setColour (juce::Colour (0xffd9dcde));
    g.setFont (juce::FontOptions (16.0f, juce::Font::italic));
    g.drawText ("KwonTone", 471, 129, 105, 23, juce::Justification::centred);

}

SlowDriveAudioProcessorEditor::PedalEnclosure::PedalEnclosure (
    juce::uint32 finishColour, const char* modelName)
    : finish (finishColour), model (modelName)
{
    setInterceptsMouseClicks (false, false);
}

void SlowDriveAudioProcessorEditor::PedalEnclosure::paint (juce::Graphics& g)
{
    const auto body = getLocalBounds().toFloat().reduced (5.0f, 6.0f);
    const auto face = body.withTrimmedBottom (13.0f);
    g.setColour (juce::Colour (0x550a0504));
    g.fillRoundedRectangle (body.expanded (3.0f).translated (3.0f, 8.0f), 11.0f);
    g.setColour (juce::Colour (0xff101116));
    g.fillRoundedRectangle (body, 9.0f);
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff6d8196), face.getTopLeft(),
                                           juce::Colour (0xff4a4a4a), face.getBottomRight(), false));
    g.fillRoundedRectangle (face, 8.0f);
    g.setColour (juce::Colour (0xffcbcbcb));
    g.drawRoundedRectangle (face.reduced (1.0f), 7.0f, 1.0f);

    {
        juce::Graphics::ScopedSaveState texture (g);
        g.reduceClipRegion (face.reduced (4.0f).getSmallestIntegerContainer());
        juce::Random grain (4491);
        for (float y = face.getY() + 4.0f; y < face.getBottom(); y += 1.6f)
        {
            const float drift = grain.nextFloat() * 5.0f - 2.5f;
            juce::Path groove;
            groove.startNewSubPath (face.getX(), y);
            groove.quadraticTo (face.getCentreX(), y + drift, face.getRight(), y);
            g.setColour (grain.nextBool() ? juce::Colour (0x25cbcbcb) : juce::Colour (0x354a4a4a));
            g.strokePath (groove, juce::PathStrokeType (0.7f));
        }
    }

    auto tape = face.toNearestInt().reduced (12, 0).withY (juce::roundToInt (face.getBottom()) - 106).withHeight (22);
    g.setColour (juce::Colour (0x65000000));
    g.fillRect (tape.translated (0, 2));
    g.setColour (finish.interpolatedWith (juce::Colour (0xffffffe3), 0.58f));
    g.fillRect (tape);
    g.setColour (juce::Colour (0xff171619));
    g.setFont (juce::FontOptions (11.0f, juce::Font::bold));
    g.drawFittedText (getText(), tape.reduced (4, 0), juce::Justification::centred, 1);
    g.setColour (juce::Colour (0xffcbcbcb));
    g.setFont (juce::FontOptions (8.5f));
    g.drawText (model, face.toNearestInt().removeFromBottom (15), juce::Justification::centred);
}

void SlowDriveAudioProcessorEditor::PedalLookAndFeel::drawToggleButton (
    juce::Graphics& g, juce::ToggleButton& button, bool hovered, bool pressed)
{
    const auto bounds = button.getLocalBounds().toFloat();
    const auto centre = bounds.getCentreX();
    const auto led = juce::Rectangle<float> (10.0f, 10.0f).withCentre ({ centre - 31.0f, 38.0f });
    const bool on = button.getToggleState();
    if (on)
    {
        g.setColour (juce::Colour (0xffffffe3).withAlpha (0.35f));
        g.fillEllipse (led.expanded (5.0f));
    }
    g.setColour (juce::Colour (0xff151818));
    g.fillEllipse (led.expanded (2.0f));
    g.setColour (on ? juce::Colour (0xffffffe3) : juce::Colour (0xff4a4a4a));
    g.fillEllipse (led);

    auto metal = juce::Rectangle<float> (42.0f, 42.0f).withCentre ({ centre, 38.0f });
    g.setColour (juce::Colour (0xff1c2020));
    g.fillEllipse (metal.expanded (3.0f));
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xffe9ebeb), metal.getTopLeft(),
                                           juce::Colour (0xff6c767b), metal.getBottomRight(), false));
    g.fillEllipse (metal);
    metal = metal.reduced (pressed ? 9.0f : 6.0f);
    g.setColour (hovered ? juce::Colour (0xffe8eded) : juce::Colour (0xffb8c0c2));
    g.fillEllipse (metal);
    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.drawEllipse (metal, 1.0f);

    if (button.hasKeyboardFocus (false))
    {
        g.setColour (juce::Colour (0xffffffe3));
        g.drawRoundedRectangle (bounds.reduced (1.0f), 5.0f, 1.0f);
    }
}

void SlowDriveAudioProcessorEditor::PedalLookAndFeel::drawRotarySlider (
    juce::Graphics& g, int x, int y, int width, int height, float position,
    float startAngle, float endAngle, juce::Slider& slider)
{
    const auto bounds = juce::Rectangle<float> (static_cast<float> (x), static_cast<float> (y),
                                               static_cast<float> (width), static_cast<float> (height));
    const auto centre = bounds.getCentre();
    const float diameter = juce::jmax (2.0f, juce::jmin (44.0f,
                                      juce::jmin (bounds.getWidth(), bounds.getHeight()) - 8.0f));
    auto dial = juce::Rectangle<float> (diameter, diameter).withCentre (centre);
    g.setColour (juce::Colours::black.withAlpha (0.7f));
    g.fillEllipse (dial.expanded (2.0f).translated (0.0f, 1.0f));
    const auto knob = juce::Colour (static_cast<juce::uint32> (
        static_cast<juce::int64> (slider.getProperties().getWithDefault ("pedalKnob", juce::int64 (0xff6d8196)))));
    g.setGradientFill (juce::ColourGradient (knob.brighter (0.35f), dial.getTopLeft(),
                                           knob.darker (0.65f), dial.getBottomRight(), false));
    g.fillEllipse (dial);
    g.setColour (slider.hasKeyboardFocus (false) ? juce::Colour (0xffffffe3) : juce::Colour (0xff818786));
    g.drawEllipse (dial.reduced (1.0f), 1.0f);
    const float angle = juce::jmap (position, startAngle, endAngle);
    const auto pointerStart = centre.getPointOnCircumference (diameter * 0.21f, angle);
    const auto pointerEnd = centre.getPointOnCircumference (diameter * 0.42f, angle);
    g.setColour (juce::Colour (0xffffffe3));
    g.drawLine ({ pointerStart, pointerEnd }, 2.0f);
}

SlowDriveAudioProcessorEditor::Control::Control (
    juce::AudioProcessorValueTreeState& parameters, const char* id, const char* name)
    : attachment (parameters, id, slider)
{
    slider.setName (name);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 68, 22);
    slider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffffffe3));
    slider.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff4b5154));
    slider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff6d8196));
    label.setText (name, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
}

SlowDriveAudioProcessorEditor::StageSwitch::StageSwitch (
    juce::AudioProcessorValueTreeState& parameters, const char* id, const char* name)
    : button (name), attachment (parameters, id, button)
{
    button.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xffffffe3));
    button.setColour (juce::ToggleButton::textColourId, juce::Colour (0xffffffe3));
}

SlowDriveAudioProcessorEditor::SlowDriveAudioProcessorEditor (SlowDriveAudioProcessor& processor)
    : AudioProcessorEditor (&processor), processor (processor),
      gateSwitch (processor.parameters, "gateOn", "Noise Gate On"),
      delaySwitch (processor.parameters, "delayOn", "Delay On"),
      boostSwitch (processor.parameters, "boostOn", "Clean Boost On"),
      driveSwitch (processor.parameters, "driveOn", "Overdrive On"),
      ampSwitch (processor.parameters, "ampOn", "Amp On"),
      cabSwitch (processor.parameters, "cabOn", "Cabinet On"),
      powerSwitch (processor.parameters, "ampPower", "Power"),
      standbySwitch (processor.parameters, "ampStandby", "Standby"),
      normalBright (processor.parameters, "normalBright", "Bright"),
      vibratoBright (processor.parameters, "vibratoBright", "Bright"),
      channelSelect (processor.parameters, "vibratoChannel", "Vibrato Channel"),
      inputSelect (processor.parameters, "inputTwo", "Input 2 (Low)"),
      reverbSwitch (processor.parameters, "reverbOn", "Reverb On"),
      tremoloSwitch (processor.parameters, "tremoloOn", "Vibrato On"),
      gateThreshold (processor.parameters, "gateThreshold", "Threshold"),
      delayTime (processor.parameters, "delayTime", "Time"),
      feedback (processor.parameters, "delayFeedback", "Feedback"),
      delayMix (processor.parameters, "delayMix", "Mix"),
      boost (processor.parameters, "boostAmount", "Boost"),
      drive (processor.parameters, "drive", "Drive"),
      tone (processor.parameters, "tone", "Tone"),
      level (processor.parameters, "gain", "Level"),
      ampLevel (processor.parameters, "ampLevel", "Volume"),
      cabTone (processor.parameters, "cabTone", "Brightness"),
      normalTreble (processor.parameters, "normalTreble", "Treble"),
      normalMiddle (processor.parameters, "normalMiddle", "Middle"),
      normalBass (processor.parameters, "normalBass", "Bass"),
      vibratoVolume (processor.parameters, "vibratoVolume", "Volume"),
      vibratoTreble (processor.parameters, "vibratoTreble", "Treble"),
      vibratoMiddle (processor.parameters, "vibratoMiddle", "Middle"),
      vibratoBass (processor.parameters, "vibratoBass", "Bass"),
      reverb (processor.parameters, "reverb", "Reverb"),
      speed (processor.parameters, "speed", "Speed"),
      intensity (processor.parameters, "intensity", "Intensity")
{
    const auto charcoal = juce::Colour (0xff4a4a4a);
    const auto grey = juce::Colour (0xffcbcbcb);
    const auto cream = juce::Colour (0xffffffE3);
    const auto blue = juce::Colour (0xff6d8196);
    for (juce::LookAndFeel_V4* style : { &inkStyle, static_cast<juce::LookAndFeel_V4*> (&pedalStyle) })
    {
        style->setColour (juce::TextButton::buttonColourId, grey);
        style->setColour (juce::TextButton::buttonOnColourId, cream);
        style->setColour (juce::TextButton::textColourOffId, charcoal);
        style->setColour (juce::TextButton::textColourOnId, charcoal);
        style->setColour (juce::Label::textColourId, cream);
        style->setColour (juce::GroupComponent::textColourId, cream);
        style->setColour (juce::GroupComponent::outlineColourId, grey);
        style->setColour (juce::Slider::thumbColourId, cream);
        style->setColour (juce::Slider::trackColourId, blue);
        style->setColour (juce::Slider::backgroundColourId, grey);
        style->setColour (juce::TabbedButtonBar::tabTextColourId, cream);
        style->setColour (juce::TabbedButtonBar::frontTextColourId, cream);
        style->setColour (juce::TabbedButtonBar::tabOutlineColourId, grey);
    }
    setLookAndFeel (&inkStyle);
    const auto setupCard = [] (juce::Component& page, juce::GroupComponent& card,
                               StageSwitch& stage, const char* title,
                               std::initializer_list<Control*> controls)
    {
        card.setText (title);
        card.setColour (juce::GroupComponent::outlineColourId, juce::Colour (0xff687271));
        card.setColour (juce::GroupComponent::textColourId, juce::Colour (0xffffffe3));
        page.addAndMakeVisible (card);
        page.addAndMakeVisible (stage.button);

        for (auto* control : controls)
        {
            control->label.setColour (juce::Label::textColourId, juce::Colour (0xffffffe3));
            page.addAndMakeVisible (control->slider);
            page.addAndMakeVisible (control->label);
        }
    };

    setupCard (pedalsPage, gateCard, gateSwitch, "NOISE GATE", { &gateThreshold });
    setupCard (pedalsPage, boostCard, boostSwitch, "CLEAN BOOST", { &boost });
    setupCard (pedalsPage, driveCard, driveSwitch, "OVERDRIVE", { &drive, &tone, &level });
    setupCard (pedalsPage, delayCard, delaySwitch, "DELAY", { &delayTime, &feedback, &delayMix });
    for (auto* stage : { &gateSwitch, &boostSwitch, &driveSwitch, &delaySwitch })
        stage->button.setLookAndFeel (&pedalStyle);

    for (auto* control : { &gateThreshold, &boost, &drive, &tone, &level, &delayTime, &feedback, &delayMix })
    {
        control->slider.setLookAndFeel (&pedalStyle);
        control->slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 48, 20);
        control->slider.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff4a4a4a));
        control->slider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        control->slider.setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffffffe3));
        control->label.setFont (juce::FontOptions (11.0f));
    }

    gateThreshold.slider.getProperties().set ("pedalKnob", juce::int64 (0xff6d8196));
    boost.slider.getProperties().set ("pedalKnob", juce::int64 (0xff6d8196));
    for (auto* control : { &drive, &tone, &level })
        control->slider.getProperties().set ("pedalKnob", juce::int64 (0xff6d8196));
    for (auto* control : { &delayTime, &feedback, &delayMix })
        control->slider.getProperties().set ("pedalKnob", juce::int64 (0xff6d8196));

    for (auto* stage : { &ampSwitch, &channelSelect, &inputSelect, &normalBright,
                          &vibratoBright, &reverbSwitch, &tremoloSwitch, &cabSwitch, &powerSwitch, &standbySwitch })
        ampPage.addAndMakeVisible (stage->button);

    for (auto* control : { &ampLevel, &normalTreble, &normalMiddle, &normalBass,
                           &vibratoVolume, &vibratoTreble, &vibratoMiddle, &vibratoBass,
                           &reverb, &speed, &intensity, &cabTone })
    {
        control->label.setColour (juce::Label::textColourId, juce::Colour (0xffffffe3));
        control->label.setFont (juce::FontOptions (11.0f));
        control->slider.setLookAndFeel (&pedalStyle);
        control->slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 43, 17);
        control->slider.setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffffffe3));
        control->slider.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff4a4a4a));
        control->slider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        ampPage.addAndMakeVisible (control->label);
        ampPage.addAndMakeVisible (control->slider);
    }

    for (auto* control : { &ampLevel, &normalTreble, &normalMiddle, &normalBass,
                           &vibratoVolume, &vibratoTreble, &vibratoMiddle, &vibratoBass,
                           &reverb, &speed, &intensity })
    {
        control->slider.textFromValueFunction = [] (double value) { return juce::String (10.0 * value, 1); };
        control->slider.valueFromTextFunction = [] (const juce::String& text) { return text.getDoubleValue() / 10.0; };
        control->slider.updateText();
    }
    channelSelect.button.onStateChange = [this] { updateAmpChannelView(); };
    updateAmpChannelView();

    const juce::Colour panelColour (0xff6d8196);
    tabs.addTab ("Amp / Cabinet", panelColour, &ampPage, false);
    tabs.addTab ("Pedalboard", panelColour, &pedalsPage, false);
    tabs.setCurrentTabIndex (0);
    tabs.setTabBarDepth (34);
    tabs.setOutline (0);
    addAndMakeVisible (tabs);
    pedalsPage.addAndMakeVisible (shelfButton);
    shelfButton.onClick = [this] { shelfOpen = ! shelfOpen; resized(); };
    shelfPanel.setText ("PEDAL SHELF");
    pedalsPage.addChildComponent (shelfPanel);
    pedalsPage.addAndMakeVisible (boardHint);
    pedalsPage.addChildComponent (shelfHint);
    shelfHint.setText ("Add a pedal to the board. Return pedals here with To shelf.", juce::dontSendNotification);
    const char* names[] = { "Noise Gate", "Clean Boost", "Overdrive", "Delay" };
    for (int i = 0; i < 4; ++i)
    {
        pedalsPage.addChildComponent (addButtons[i]);
        addButtons[i].setButtonText (juce::String ("+ ") + names[i]);
        addButtons[i].onClick = [this, i] {
            auto order = boardOrder();
            if (std::find (order.begin(), order.end(), i) == order.end()) order.push_back (i);
            saveOrder (order);
        };
        for (auto* button : { &leftButtons[i], &rightButtons[i], &removeButtons[i] })
            pedalsPage.addAndMakeVisible (*button);
        leftButtons[i].setButtonText ("<"); rightButtons[i].setButtonText (">");
        removeButtons[i].setButtonText ("To shelf");
        removeButtons[i].onClick = [this, i] {
            auto order = boardOrder(); order.erase (std::remove (order.begin(), order.end(), i), order.end()); saveOrder (order);
        };
        const auto move = [this, i] (int delta) {
            auto order = boardOrder(); auto it = std::find (order.begin(), order.end(), i);
            if (it == order.end()) return;
            int pos = static_cast<int> (it - order.begin()), next = pos + delta;
            if (next >= 0 && next < static_cast<int> (order.size())) { std::swap (order[pos], order[next]); saveOrder (order); }
        };
        leftButtons[i].onClick = [move] { move (-1); };
        rightButtons[i].onClick = [move] { move (1); };
    }
    startTimerHz (10);
    setSize (960, 645);
}

SlowDriveAudioProcessorEditor::~SlowDriveAudioProcessorEditor()
{
    channelSelect.button.onStateChange = nullptr;
    setLookAndFeel (nullptr);
}

void SlowDriveAudioProcessorEditor::updateAmpChannelView()
{
    // Both channels remain editable, including the inactive channel's stored settings.
    ampPage.repaint();

}

void SlowDriveAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff4a4a4a));

}

void SlowDriveAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (16);
    tabs.setBounds (area);

    const auto layoutPedal = [] (juce::Rectangle<int> bounds, PedalEnclosure& enclosure,
                                 StageSwitch& stage, std::initializer_list<Control*> controls)
    {
        enclosure.setBounds (bounds);
        auto face = bounds.reduced (13, 12);
        auto top = face.removeFromTop (80);
        auto middle = face.removeFromTop (78);
        const auto place = [] (Control& control, juce::Rectangle<int> area)
        {
            control.label.setBounds (area.removeFromBottom (16));
            control.slider.setBounds (area);
        };

        auto control = controls.begin();
        if (controls.size() == 1)
        {
            place (**control, top.withSizeKeepingCentre (76, 80));
        }
        else
        {
            const int halfWidth = top.getWidth() / 2;
            place (**control++, top.removeFromLeft (halfWidth));
            place (**control++, top);
            place (**control, middle.withSizeKeepingCentre (76, 78));
        }

        const auto switchY = bounds.getBottom() - 91;
        stage.button.setBounds (bounds.getCentreX() - 43, switchY, 86, 69);
    };

    const auto order = boardOrder();
    shelfButton.setBounds (pedalsPage.getWidth() - 112, 12, 96, 28);
    shelfButton.setButtonText (shelfOpen ? "Close shelf" : "Shelf");
    boardHint.setBounds (18, 12, 710, 28);
    boardHint.setText (order.empty() ? "Your board is empty. Open Shelf to add pedals." : "SIGNAL  >   Left to right   |   Use < and > to reorder pedals", juce::dontSendNotification);
    shelfPanel.setBounds (16, pedalsPage.getHeight() - 132, pedalsPage.getWidth() - 32, 116);
    shelfPanel.setVisible (shelfOpen);
    shelfHint.setBounds (30, shelfPanel.getY() + 22, 800, 22); shelfHint.setVisible (shelfOpen);
    PedalEnclosure* cards[] = { &gateCard, &boostCard, &driveCard, &delayCard };
    StageSwitch* switches[] = { &gateSwitch, &boostSwitch, &driveSwitch, &delaySwitch };
    std::vector<Control*> controls[] = { { &gateThreshold }, { &boost }, { &drive, &level, &tone }, { &delayTime, &feedback, &delayMix } };
    for (int i = 0; i < 4; ++i)
    {
        const auto it = std::find (order.begin(), order.end(), i);
        const bool present = it != order.end();
        cards[i]->setVisible (present); switches[i]->button.setVisible (present);
        for (auto* c : controls[i]) { c->label.setVisible (present); c->slider.setVisible (present); }
        addButtons[i].setVisible (shelfOpen); addButtons[i].setEnabled (! present);
        addButtons[i].setBounds (30 + i * 218, shelfPanel.getY() + 57, 204, 32);
        for (auto* button : { &leftButtons[i], &rightButtons[i], &removeButtons[i] }) button->setVisible (present);
        if (! present) continue;
        const int pos = static_cast<int> (it - order.begin());
        const int start = (pedalsPage.getWidth() - static_cast<int> (order.size()) * 214) / 2;
        juce::Rectangle<int> bounds (start + pos * 214, 55, 202, 326);
        if (i == 0) layoutPedal (bounds, gateCard, gateSwitch, { &gateThreshold });
        if (i == 1) layoutPedal (bounds, boostCard, boostSwitch, { &boost });
        if (i == 2) layoutPedal (bounds, driveCard, driveSwitch, { &drive, &level, &tone });
        if (i == 3) layoutPedal (bounds, delayCard, delaySwitch, { &delayTime, &feedback, &delayMix });
        leftButtons[i].setBounds (bounds.getX() + 6, 385, 34, 26);
        removeButtons[i].setBounds (bounds.getX() + 46, 385, 110, 26);
        rightButtons[i].setBounds (bounds.getX() + 162, 385, 34, 26);
        leftButtons[i].setEnabled (pos > 0); rightButtons[i].setEnabled (pos + 1 < static_cast<int> (order.size()));
    }

    auto routing = juce::Rectangle<int> (26, 18, ampPage.getWidth() - 52, 28);
    ampSwitch.button.setBounds (routing.removeFromLeft (86));
    powerSwitch.button.setBounds (routing.removeFromLeft (76));
    standbySwitch.button.setBounds (routing.removeFromLeft (88));
    channelSelect.button.setBounds (routing.removeFromLeft (143));
    inputSelect.button.setBounds (routing.removeFromLeft (130));
    cabSwitch.button.setBounds (routing.removeFromLeft (120));
    normalBright.button.setBounds (226, 56, 80, 22);
    vibratoBright.button.setBounds (ampPage.getWidth() - 110, 56, 80, 22);
    const auto layoutKnobs = [] (juce::Rectangle<int> row, std::initializer_list<Control*> controls)
    {
        const int width = row.getWidth() / static_cast<int> (controls.size());
        for (auto* control : controls)
        {
            auto column = row.removeFromLeft (width);
            control->label.setBounds (column.removeFromTop (18));
            control->slider.setBounds (column);
        }
    };
    layoutKnobs ({ 26, 80, 282, 80 }, { &ampLevel, &normalTreble, &normalMiddle, &normalBass });
    layoutKnobs ({ 338, 80, ampPage.getWidth() - 366, 80 },
                 { &vibratoVolume, &vibratoTreble, &vibratoMiddle, &vibratoBass, &reverb, &speed, &intensity });
    reverbSwitch.button.setBounds (28, 175, 112, 24);
    tremoloSwitch.button.setBounds (148, 175, 115, 24);
    cabTone.label.setBounds (330, 175, 130, 24);
    cabTone.label.setText ("Cabinet brightness", juce::dontSendNotification);
    cabTone.slider.setSliderStyle (juce::Slider::LinearHorizontal);
    cabTone.slider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 48, 22);
    cabTone.slider.setBounds (465, 175, 180, 24);
    cabTone.slider.textFromValueFunction = [] (double v) { return juce::String (juce::roundToInt (v * 100)) + "%"; };
    cabTone.slider.valueFromTextFunction = [] (const juce::String& t) { return t.getDoubleValue() / 100; };
    cabTone.slider.updateText();
}

std::vector<int> SlowDriveAudioProcessorEditor::boardOrder() const
{
    int packed = static_cast<int> (processor.parameters.getRawParameterValue ("pedalOrder")->load());
    std::vector<int> order;
    for (int slot = 0; slot < 4; ++slot) {
        int id = ((packed >> (slot * 3)) & 7) - 1;
        if (id >= 0 && id < 4 && std::find (order.begin(), order.end(), id) == order.end()) order.push_back (id);
    }
    return order;
}
void SlowDriveAudioProcessorEditor::saveOrder (const std::vector<int>& order)
{
    int packed = 0;
    for (size_t i = 0; i < order.size(); ++i) packed |= (order[i] + 1) << (i * 3);
    auto* p = processor.parameters.getParameter ("pedalOrder");
    p->beginChangeGesture(); p->setValueNotifyingHost (p->convertTo0to1 (static_cast<float> (packed))); p->endChangeGesture();
    resized();
}
void SlowDriveAudioProcessorEditor::timerCallback()
{
    int packed = static_cast<int> (processor.parameters.getRawParameterValue ("pedalOrder")->load());
    if (packed != lastOrder) { lastOrder = packed; resized(); }
}
