cmake_minimum_required(VERSION 3.15)

project(SimpleGainPlugin VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the JUCE subdirectory to the build - adjust the path as needed
# This assumes you have JUCE in a subdirectory called "JUCE"
# If you have JUCE installed elsewhere, modify this path
add_subdirectory(JUCE)

# Initialize JUCE project
juce_add_plugin(SimpleGain
    VERSION 1.0.0
    COMPANY_NAME "YourCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Ycmp
    PLUGIN_CODE Sgai
    FORMATS VST3 Standalone
    PRODUCT_NAME "Simple Gain")

# Source files
target_sources(SimpleGain
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp)

# Set include directories
target_include_directories(SimpleGain
    PRIVATE
        Source)

# Add JUCE modules
target_link_libraries(SimpleGain
    PRIVATE
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Set up binary data
# If you need to embed any binary files (like images, fonts, etc.), uncomment and add them here:
# juce_add_binary_data(SimpleGainData SOURCES
#     Resources/logo.png
#     Resources/font.ttf
#     # Add more files as needed
# )

# Link with binary data if any
# target_link_libraries(SimpleGain
#     PRIVATE
#         SimpleGainData) 