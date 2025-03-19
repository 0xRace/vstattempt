#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleGainProcessor::SimpleGainProcessor()
    : AudioProcessor(BusesProperties()
                   .withInput("Input", juce::AudioChannelSet::stereo(), true)
                   .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS",
                {
                    std::make_unique<juce::AudioParameterFloat>(
                        gainID, "Gain", juce::NormalisableRange<float>(-60.0f, 12.0f, 0.1f), 0.0f,
                        "dB", juce::AudioProcessorParameter::genericParameter,
                        std::function<juce::String(float, int)>([](float value, int) { return juce::String(value, 1) + " dB"; }),
                        std::function<float(const juce::String&)>([](const juce::String& text) { return text.getFloatValue(); }))
                })
{
    // Add parameter listener
    parameters.addParameterListener(gainID, this);
    
    // Initialize gain value
    float gainParam = *parameters.getRawParameterValue(gainID);
    currentGain = juce::Decibels::decibelsToGain(gainParam);
}

SimpleGainProcessor::~SimpleGainProcessor()
{
    parameters.removeParameterListener(gainID, this);
}

//==============================================================================
void SimpleGainProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == gainID)
    {
        // Convert from dB to linear gain
        currentGain = juce::Decibels::decibelsToGain(newValue);
    }
}

//==============================================================================
const juce::String SimpleGainProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleGainProcessor::acceptsMidi() const
{
    return false;
}

bool SimpleGainProcessor::producesMidi() const
{
    return false;
}

bool SimpleGainProcessor::isMidiEffect() const
{
    return false;
}

double SimpleGainProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleGainProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleGainProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleGainProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String SimpleGainProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void SimpleGainProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void SimpleGainProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialization that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void SimpleGainProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SimpleGainProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // We support mono and stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // We require the same number of input and output channels
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void SimpleGainProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Apply gain to all channels
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        // Apply gain to each sample
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= currentGain;
        }
    }
}

//==============================================================================
bool SimpleGainProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleGainProcessor::createEditor()
{
    return new SimpleGainEditor (*this);
}

//==============================================================================
void SimpleGainProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SimpleGainProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleGainProcessor();
} 