#pragma once

#include "PluginProcessor.h"

//==============================================================================
/**
 * SimpleGainEditor provides the GUI for our gain plugin.
 * It creates a simple interface with one main gain knob.
 */
class SimpleGainEditor : public juce::AudioProcessorEditor,
                        private juce::Slider::Listener,
                        private juce::Timer
{
public:
    //==============================================================================
    SimpleGainEditor(SimpleGainProcessor&);
    ~SimpleGainEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Called when the slider value changes
    void sliderValueChanged(juce::Slider* slider) override;
    
    // Called regularly by the timer
    void timerCallback() override;
    
    // Reference to the processor
    SimpleGainProcessor& processor;
    
    // GUI components
    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::Label titleLabel;
    juce::Label valueLabel;
    
    // Parameter attachment for gain slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    
    // Colors
    juce::Colour backgroundColour = juce::Colour(30, 30, 30);
    juce::Colour accentColour = juce::Colour(42, 128, 185);
    juce::Colour textColour = juce::Colour(225, 225, 225);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGainEditor)
}; 