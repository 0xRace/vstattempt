#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleGainEditor::SimpleGainEditor(SimpleGainProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Set up title label
    titleLabel.setText("Simple Gain", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(titleLabel);
    
    // Set up gain slider
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainSlider.setPopupDisplayEnabled(true, false, this);
    gainSlider.setColour(juce::Slider::rotarySliderFillColourId, accentColour);
    gainSlider.setColour(juce::Slider::thumbColourId, accentColour);
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);
    
    // Set up gain label
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setFont(juce::Font(16.0f));
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(gainLabel);
    
    // Set up value display label
    valueLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    valueLabel.setJustificationType(juce::Justification::centred);
    valueLabel.setColour(juce::Label::textColourId, textColour);
    valueLabel.setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    valueLabel.setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(valueLabel);
    
    // Create parameter attachments
    try
    {
        gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            processor.getParameters(), SimpleGainProcessor::gainID, gainSlider));
    }
    catch (...)
    {
        // Handle attachment creation failure
        gainAttachment.reset();
    }
    
    // Start a timer to update the display
    startTimerHz(30);
    
    // Set editor size
    setSize(300, 400);
}

SimpleGainEditor::~SimpleGainEditor()
{
    stopTimer();
    gainAttachment.reset();
}

//==============================================================================
void SimpleGainEditor::paint(juce::Graphics& g)
{
    // Fill the background
    g.fillAll(backgroundColour);
    
    // Draw a rounded rectangle around the gain control
    g.setColour(accentColour.withAlpha(0.1f));
    g.fillRoundedRectangle(getWidth() * 0.25f, getHeight() * 0.25f, 
                          getWidth() * 0.5f, getWidth() * 0.5f, 10.0f);
    
    g.setColour(accentColour.withAlpha(0.2f));
    g.drawRoundedRectangle(getWidth() * 0.25f, getHeight() * 0.25f, 
                          getWidth() * 0.5f, getWidth() * 0.5f, 10.0f, 1.0f);
}

void SimpleGainEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    // Position the title at the top
    titleLabel.setBounds(area.removeFromTop(40));
    
    // Add some spacing
    area.removeFromTop(20);
    
    // Position the gain knob in the center
    auto knobArea = area.removeFromTop(area.getWidth());
    gainSlider.setBounds(knobArea.reduced(20));
    
    // Add some spacing
    area.removeFromTop(10);
    
    // Position the value label below the knob
    valueLabel.setBounds(area.removeFromTop(30));
    
    // Position the gain label below the value
    gainLabel.setBounds(area.removeFromTop(30));
}

void SimpleGainEditor::sliderValueChanged(juce::Slider* slider)
{
    // We don't need to manually update anything here since we're using parameter attachments
    juce::ignoreUnused(slider);
}

void SimpleGainEditor::timerCallback()
{
    // Update the value display
    const float gainValue = gainSlider.getValue();
    const float gainPercent = static_cast<float>(juce::Decibels::decibelsToGain(gainValue) * 100.0);
    
    juce::String displayText = juce::String(gainValue, 1) + " dB (" + 
                              juce::String(gainPercent, 1) + "%)";
    valueLabel.setText(displayText, juce::dontSendNotification);
} 