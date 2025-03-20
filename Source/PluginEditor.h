#pragma once

#include "PluginProcessor.h"
#include <juce_graphics/juce_graphics.h>

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
    juce::Slider modFreqSlider;
    juce::Slider modDepthSlider;
    juce::Label gainLabel;
    juce::Label modFreqLabel;
    juce::Label modDepthLabel;
    juce::Label titleLabel;
    juce::Label valueLabel;
    
    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modDepthAttachment;
    
    // Colors
    juce::Colour backgroundColour = juce::Colour(30, 30, 30);
    juce::Colour accentColour = juce::Colour(42, 128, 185);
    juce::Colour textColour = juce::Colour(225, 225, 225);
    
    // Background image
    juce::Image backgroundImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGainEditor)
}; 