#!/bin/sh
set -euo pipefail

echo Making $1

cd -- "$( dirname -- "${BASH_SOURCE[0]}" )"

PARENT_FOLDER=`dirname $(pwd)`

BUILD_FOLDER=build/$1/so

echo BUILD_FOLDER=$BUILD_FOLDER

# Remove build directory if it exists
if [ -d $BUILD_FOLDER ]; then
  rm -r $BUILD_FOLDER
fi

# Re-create build directory
mkdir -p $BUILD_FOLDER

# Switch to build directory
cd build/$1/so

# Run CMake with the custom file name
cmake --project-file cmake/$1.cmake $PARENT_FOLDER

# Build the project
make

# Copy the resulting .so to sources directory
cp $1.so $PARENT_FOLDER
