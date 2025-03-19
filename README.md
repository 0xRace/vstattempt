# Simple Gain VST Plugin

A basic VST3 plugin that provides a simple gain/volume control for audio signals. Built with JUCE framework.

## Features
- Clean, minimal interface
- Single gain control knob
- Decibel and percentage value display
- VST3 compatible

## Requirements
- Windows, macOS, or Linux system
- C++ development environment (Visual Studio, Xcode, or GCC)
- JUCE framework (6.1.x or newer recommended)
- CMake (3.15 or newer)

## Getting Started

### Setup
1. Clone this repository
2. Run the setup script to download JUCE:
   - On Windows: `setup.bat`
   - On macOS/Linux: `chmod +x setup.sh && ./setup.sh`

### Building the Plugin
1. Create a build directory: `mkdir build`
2. Navigate to the build directory: `cd build`
3. Run CMake: `cmake ..`
4. Build the project:
   - On Windows: `cmake --build . --config Release`
   - On macOS/Linux: `cmake --build .`

The compiled VST3 plugin will be available in the build directory.

## How This Plugin Works

This SimpleGain plugin demonstrates the core components of VST development:

### 1. Plugin Architecture (SimpleGainProcessor)
- **Audio Processing**: The processor handles audio data, applying a gain factor to each sample
- **Parameter Management**: Uses AudioProcessorValueTreeState to handle plugin parameters
- **State Saving/Loading**: Implements save/restore of plugin state

### 2. User Interface (SimpleGainEditor)
- **Controls**: Implements a rotary knob for adjusting gain
- **Parameter Linking**: Uses parameter attachments to link UI elements to the processor
- **Value Display**: Shows both dB and percentage values in real-time

### 3. Core VST Implementation Details
- **Audio Buffer Processing**: 
  ```cpp
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
  ```

- **Parameter Setup**:
  ```cpp
  parameters(*this, nullptr, "PARAMETERS",
    {
        std::make_unique<juce::AudioParameterFloat>(
            gainID, "Gain", juce::NormalisableRange<float>(-60.0f, 12.0f, 0.1f), 0.0f,
            "dB", juce::AudioProcessorParameter::genericParameter,
            [](float value) { return juce::String(value, 1) + " dB"; },
            [](const juce::String& text) { return text.getFloatValue(); })
    })
  ```

## Extending This Plugin

You can use this simple gain plugin as a starting point for your own audio effects:

1. **Add New Parameters**: Define additional parameters in the SimpleGainProcessor constructor
2. **Add UI Elements**: Create and position additional controls in SimpleGainEditor
3. **Implement New DSP**: Add your own audio processing code in the processBlock method

## Project Structure
- `CMakeLists.txt`: CMake build configuration
- `Source/`: Contains all plugin source code
  - `PluginProcessor.*`: Audio processing logic
  - `PluginEditor.*`: User interface components 