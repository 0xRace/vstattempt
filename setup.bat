@echo off
REM Setup script for SimpleGain VST plugin on Windows
REM Downloads JUCE and sets up the project

echo Setting up SimpleGain VST project...

REM Check if git is installed
where git >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Git is not installed. Please install git and try again.
    exit /b 1
)

REM Clone JUCE if it doesn't exist
if not exist JUCE (
    echo Downloading JUCE...
    git clone --depth=1 https://github.com/juce-framework/JUCE.git
    echo JUCE downloaded successfully.
) else (
    echo JUCE directory already exists. Skipping download.
)

echo Setup complete. You can now build the plugin using CMake.
echo.
echo Build instructions:
echo 1. Create a build directory: mkdir build
echo 2. Navigate to the build directory: cd build
echo 3. Run CMake: cmake ..
echo 4. Build the project: cmake --build . --config Release
echo.
echo The VST3 plugin will be available in the build directory. 