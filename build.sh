#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Check if a target directory was provided
if [ -z "$1" ]; then
    echo "❌ Error: Please specify the project folder name."
    echo "Usage: ./build.sh <Folder_Name> [clean]"
    echo "Example: ./build.sh OrderBook"
    exit 1
fi

TARGET_DIR="$1"
CLEAN_MODE="$2"

# Check if the target directory actually exists
if [ ! -d "$TARGET_DIR" ]; then
    echo "❌ Error: Directory '$TARGET_DIR' not found."
    exit 1
fi

echo "🚀 Starting atomic build pipeline for: $TARGET_DIR..."

# Move into the specific project directory
cd "$TARGET_DIR"

# Optional: Wipe out previous build artifacts if 'clean' is passed as a 2nd argument
if [ "$CLEAN_MODE" == "clean" ]; then
    echo "🧹 Cleaning previous build directory..."
    rm -rf build
fi

# Create a dedicated, clean build folder inside that specific project
mkdir -p build
cd build

echo "🛠️  Running CMake configuration..."
cmake ..

echo "🏗️  Compiling binaries..."
cmake --build .

echo "✅ Success! Your executable is ready inside $TARGET_DIR/build/"
