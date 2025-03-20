cmake_minimum_required(VERSION 3.15)

project(SimpleGainPlugin VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the JUCE subdirectory to the build
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
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Ycmp
    PLUGIN_CODE Sgai
    FORMATS VST3 Standalone
    PRODUCT_NAME "Simple Gain"
    VST3_CATEGORIES "Fx"
    MICROPHONE_PERMISSION_ENABLED FALSE
    MICROPHONE_PERMISSION_TEXT "This plugin does not require microphone access"
    CAMERA_PERMISSION_ENABLED FALSE
    CAMERA_PERMISSION_TEXT "This plugin does not require camera access"
    BLUETOOTH_PERMISSION_ENABLED FALSE
    BLUETOOTH_PERMISSION_TEXT "This plugin does not require bluetooth access"
    BLUETOOTH_ADMIN_PERMISSION_ENABLED FALSE
    BLUETOOTH_ADMIN_PERMISSION_TEXT "This plugin does not require bluetooth admin access"
    NETWORK_MICROPHONE_PERMISSION_ENABLED FALSE
    NETWORK_MICROPHONE_PERMISSION_TEXT "This plugin does not require network microphone access"
    NETWORK_CAMERA_PERMISSION_ENABLED FALSE
    NETWORK_CAMERA_PERMISSION_TEXT "This plugin does not require network camera access"
    NETWORK_BLUETOOTH_PERMISSION_ENABLED FALSE
    NETWORK_BLUETOOTH_PERMISSION_TEXT "This plugin does not require network bluetooth access"
    NETWORK_BLUETOOTH_ADMIN_PERMISSION_ENABLED FALSE
    NETWORK_BLUETOOTH_ADMIN_PERMISSION_TEXT "This plugin does not require network bluetooth admin access")

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
        juce::juce_audio_processors
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_opengl
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Set Windows-specific options
if(WIN32)
    target_compile_definitions(SimpleGain PRIVATE
        JUCE_WINDOWS=1
        JUCE_WIN32=1
        JUCE_WINDOWS_DPI_AWARENESS=1)
endif()

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

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