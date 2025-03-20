#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
/**
 * SimpleGainProcessor handles the audio processing for our gain plugin.
 * It applies a simple gain scaling to the audio signal.
 */
class SimpleGainProcessor : public juce::AudioProcessor,
                           public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    SimpleGainProcessor();
    ~SimpleGainProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }

    // Parameter IDs
    static const juce::String gainID;
    static const juce::String modFreqID;
    static const juce::String modDepthID;

private:
    // Value Tree State for managing parameters
    juce::AudioProcessorValueTreeState parameters;
    
    // Current parameter values
    std::atomic<float> currentGain { 1.0f };
    std::atomic<float> currentModFreq { 1.0f };
    std::atomic<float> currentModDepth { 0.0f };
    
    // DSP objects
    juce::dsp::Oscillator<float> modulator;
    double currentSampleRate { 44100.0 };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGainProcessor)
}; 