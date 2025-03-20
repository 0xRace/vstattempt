#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_core/juce_core.h>
#include "BinaryData.h"

//==============================================================================
SimpleGainEditor::SimpleGainEditor(SimpleGainProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Load the background image
    int size = 0;
    const char* imageData = BinaryData::getNamedResource("Untitled_jpg", size);
    if (imageData != nullptr && size > 0)
    {
        backgroundImage = juce::ImageCache::getFromMemory(imageData, size);
    }

    // Set up title label
    titleLabel.setText("Simple Gain + FM", juce::dontSendNotification);
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
    
    // Set up modulation frequency slider
    modFreqSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    modFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    modFreqSlider.setPopupDisplayEnabled(true, false, this);
    modFreqSlider.setColour(juce::Slider::rotarySliderFillColourId, accentColour);
    modFreqSlider.setColour(juce::Slider::thumbColourId, accentColour);
    modFreqSlider.addListener(this);
    addAndMakeVisible(modFreqSlider);
    
    // Set up modulation depth slider
    modDepthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    modDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    modDepthSlider.setPopupDisplayEnabled(true, false, this);
    modDepthSlider.setColour(juce::Slider::rotarySliderFillColourId, accentColour);
    modDepthSlider.setColour(juce::Slider::thumbColourId, accentColour);
    modDepthSlider.addListener(this);
    addAndMakeVisible(modDepthSlider);
    
    // Set up labels
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setFont(juce::Font(16.0f));
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(gainLabel);
    
    modFreqLabel.setText("Mod Freq", juce::dontSendNotification);
    modFreqLabel.setFont(juce::Font(16.0f));
    modFreqLabel.setJustificationType(juce::Justification::centred);
    modFreqLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(modFreqLabel);
    
    modDepthLabel.setText("Mod Depth", juce::dontSendNotification);
    modDepthLabel.setFont(juce::Font(16.0f));
    modDepthLabel.setJustificationType(juce::Justification::centred);
    modDepthLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(modDepthLabel);
    
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
            
        modFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            processor.getParameters(), SimpleGainProcessor::modFreqID, modFreqSlider));
            
        modDepthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
            processor.getParameters(), SimpleGainProcessor::modDepthID, modDepthSlider));
    }
    catch (...)
    {
        // Handle attachment creation failure
        gainAttachment.reset();
        modFreqAttachment.reset();
        modDepthAttachment.reset();
    }
    
    // Start a timer to update the display
    startTimerHz(30);
    
    // Set editor size
    setSize(400, 500);
}

SimpleGainEditor::~SimpleGainEditor()
{
    stopTimer();
    gainAttachment.reset();
    modFreqAttachment.reset();
    modDepthAttachment.reset();
}

//==============================================================================
void SimpleGainEditor::paint(juce::Graphics& g)
{
    // Draw the background image if available
    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
    }
    else
    {
        // Fallback to solid color if image is not available
        g.fillAll(juce::Colour(30, 30, 30));
    }
    
    // Draw rounded rectangles around the controls
    auto drawControlBackground = [&](const juce::Rectangle<float>& bounds)
    {
        g.setColour(accentColour.withAlpha(0.1f));
        g.fillRoundedRectangle(bounds, 10.0f);
        g.setColour(accentColour.withAlpha(0.2f));
        g.drawRoundedRectangle(bounds, 10.0f, 1.0f);
    };
    
    float controlWidth = getWidth() * 0.25f;
    float controlHeight = controlWidth;
    float yPos = getHeight() * 0.25f;
    
    // Draw backgrounds for each control
    drawControlBackground(juce::Rectangle<float>(getWidth() * 0.1f, yPos, controlWidth, controlHeight));
    drawControlBackground(juce::Rectangle<float>(getWidth() * 0.4f, yPos, controlWidth, controlHeight));
    drawControlBackground(juce::Rectangle<float>(getWidth() * 0.7f, yPos, controlWidth, controlHeight));
}

void SimpleGainEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    // Position the title at the top
    titleLabel.setBounds(area.removeFromTop(40));
    
    // Add some spacing
    area.removeFromTop(20);
    
    // Calculate control sizes
    float controlWidth = getWidth() * 0.25f;
    float controlHeight = controlWidth;
    float yPos = getHeight() * 0.25f;
    
    // Position the controls
    gainSlider.setBounds(getWidth() * 0.1f, yPos, controlWidth, controlHeight);
    modFreqSlider.setBounds(getWidth() * 0.4f, yPos, controlWidth, controlHeight);
    modDepthSlider.setBounds(getWidth() * 0.7f, yPos, controlWidth, controlHeight);
    
    // Position the labels
    yPos += controlHeight + 10;
    gainLabel.setBounds(getWidth() * 0.1f, yPos, controlWidth, 30);
    modFreqLabel.setBounds(getWidth() * 0.4f, yPos, controlWidth, 30);
    modDepthLabel.setBounds(getWidth() * 0.7f, yPos, controlWidth, 30);
    
    // Position the value label
    valueLabel.setBounds(area.removeFromBottom(30));
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
    const float modFreq = modFreqSlider.getValue();
    const float modDepth = modDepthSlider.getValue() * 100.0f;
    
    juce::String displayText = juce::String(gainValue, 1) + " dB, " + 
                              juce::String(modFreq, 1) + " Hz, " +
                              juce::String(modDepth, 1) + "%";
    valueLabel.setText(displayText, juce::dontSendNotification);
} 