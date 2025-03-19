#!/bin/bash
# Setup script for SimpleGain VST plugin
# Downloads JUCE and sets up the project

echo "Setting up SimpleGain VST project..."

# Check if git is installed
if ! command -v git &> /dev/null; then
    echo "Git is not installed. Please install git and try again."
    exit 1
fi

# Clone JUCE if it doesn't exist
if [ ! -d "JUCE" ]; then
    echo "Downloading JUCE..."
    git clone --depth=1 https://github.com/juce-framework/JUCE.git
    echo "JUCE downloaded successfully."
else
    echo "JUCE directory already exists. Skipping download."
fi

echo "Setup complete. You can now build the plugin using CMake."
echo ""
echo "Build instructions:"
echo "1. Create a build directory: mkdir -p build"
echo "2. Navigate to the build directory: cd build"
echo "3. Run CMake: cmake .."
echo "4. Build the project: cmake --build ."
echo ""
echo "The VST3 plugin will be available in the build directory." 