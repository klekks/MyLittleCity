#!/bin/bash

mkdir -p build/distr

# Default values
CONFIG="Release"
ARCH="x86_64"

# Parse arguments
if [ "$1" != "" ]; then
    CONFIG="$1"
fi
if [ "$2" != "" ]; then
    ARCH="$2"
fi

# Skip logic based on third argument
if [ "$3" != "SkipAllConan" ] && [ "$3" != "NoClean" ] && [ "$3" != "SkipConanButConfig" ]; then
    echo "Removing build"
    rm -rf build

    if [ $? -ne 0 ]; then
        echo "ERROR: Remove build failed with exit-code: $?"
        exit $?
    fi

    echo "build folders removed"

    echo "Removing old conan packages"
    conan remove -c "*"

    if [ $? -ne 0 ]; then
        echo "ERROR: Remove conan failed with exit-code: $?"
        exit $?
    fi

    conan profile detect --force
    mkdir -p ~/.conan2/profiles
    cp conan/linux_conan_profile ~/.conan2/profiles/default

    if [ $? -ne 0 ]; then
        echo "ERROR: install conan profile failed with exit-code: $?"
        exit $?
    fi
fi

if [ "$3" != "SkipAllConan" ]; then
    echo "Conan profile installed"

    if [ "$3" != "SkipConanButConfig" ]; then
        conan install . --build=missing -s build_type=$CONFIG -s arch=$ARCH
    fi
else
    echo "Conan build steps skipped"
fi

echo "Generate make files"
mkdir -p build
cd build

cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$CONFIG -DCMAKE_TOOLCHAIN_FILE="$CONFIG/generators/conan_toolchain.cmake"

if [ $? -ne 0 ]; then
    echo "ERROR: Cannot generate make files with exit-code: $?"
    exit $?
fi

cmake --build . --config $CONFIG -j 8

if [ $? -ne 0 ]; then
    echo "ERROR: Cannot build with exit-code: $?"
    exit $?
fi

echo "Build Successful!"