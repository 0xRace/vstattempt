#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
const juce::String SimpleGainProcessor::gainID = "gain";
const juce::String SimpleGainProcessor::modFreqID = "modfreq";
const juce::String SimpleGainProcessor::modDepthID = "moddepth";

SimpleGainProcessor::SimpleGainProcessor()
    : AudioProcessor(BusesProperties()
                   .withInput("Input", juce::AudioChannelSet::stereo(), true)
                   .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS",
                {
                    std::make_unique<juce::AudioParameterFloat>(
                        juce::ParameterID(gainID, 1),
                        "Gain",
                        juce::NormalisableRange<float>(-96.0f, 24.0f, 0.1f),
                        0.0f,
                        juce::AudioParameterFloatAttributes()
                            .withLabel("dB")
                            .withCategory(juce::AudioParameterFloat::genericParameter)),
                    std::make_unique<juce::AudioParameterFloat>(
                        juce::ParameterID(modFreqID, 1),
                        "Mod Freq",
                        juce::NormalisableRange<float>(0.1f, 10.0f, 0.1f),
                        1.0f,
                        juce::AudioParameterFloatAttributes()
                            .withLabel("Hz")
                            .withCategory(juce::AudioParameterFloat::genericParameter)),
                    std::make_unique<juce::AudioParameterFloat>(
                        juce::ParameterID(modDepthID, 1),
                        "Mod Depth",
                        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
                        0.0f,
                        juce::AudioParameterFloatAttributes()
                            .withLabel("%")
                            .withCategory(juce::AudioParameterFloat::genericParameter))
                })
{
    // Add parameter listeners
    parameters.addParameterListener(gainID, this);
    parameters.addParameterListener(modFreqID, this);
    parameters.addParameterListener(modDepthID, this);
    
    // Initialize parameter values
    if (auto* param = parameters.getParameter(gainID))
        currentGain = juce::Decibels::decibelsToGain(param->getValue());
    if (auto* param = parameters.getParameter(modFreqID))
        currentModFreq = param->getValue();
    if (auto* param = parameters.getParameter(modDepthID))
        currentModDepth = param->getValue();
        
    // Initialize oscillator
    modulator.initialise([](float x) { return std::sin(x); });
}

SimpleGainProcessor::~SimpleGainProcessor()
{
    parameters.removeParameterListener(gainID, this);
    parameters.removeParameterListener(modFreqID, this);
    parameters.removeParameterListener(modDepthID, this);
}

//==============================================================================
void SimpleGainProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == gainID)
    {
        currentGain = juce::Decibels::decibelsToGain(newValue);
    }
    else if (parameterID == modFreqID)
    {
        currentModFreq = newValue;
        if (currentSampleRate > 0)
            modulator.setFrequency(currentModFreq);
    }
    else if (parameterID == modDepthID)
    {
        currentModDepth = newValue;
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
    currentSampleRate = sampleRate;
    modulator.prepare({ sampleRate, (juce::uint32) samplesPerBlock, 1 });
    modulator.setFrequency(currentModFreq);
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
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Get modulation value (-1 to 1)
            float modValue = modulator.processSample(0.0f);
            
            // Scale modulation by depth (0 to 1)
            modValue *= currentModDepth;
            
            // Apply modulation and gain
            float inputSample = channelData[sample];
            float modulatedSample = inputSample * (1.0f + modValue);
            channelData[sample] = modulatedSample * currentGain;
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
    try
    {
        return std::make_unique<SimpleGainEditor>(*this).release();
    }
    catch (...)
    {
        return nullptr;
    }
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