#!/bin/bash

clean_up () {
    echo Removing builds
    rm -rf ./builds
    if [ $? != 0 ]; then
        echo Cannot remove ./builds
        exit 1
    fi

    echo Removing old conan files
    conan remove -c *

    mkdir -p ~/.conan2/profiles
       
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	    cp ./conan_profile ~/.conan2/profiles/default
    elif [[ "$OSTYPE" == "darwin"* ]]; then
	    cp ./conan_profile ~/.conan2/profiles/default
    else
        echo Unknown os
        exit 1
    fi

    
    if [ $? != 0 ]; then
        echo Cannot copy conan profile
        exit 1
    fi
}

build_conan () {
    echo Build conan
    conan install ./ --output-folder=builds/${BUILDCONANFOLDER}/${ARCH}/${CONFIG} -s build_type=${CONFIG} -s arch=${ARCH} --build=missing
    if [ $? != 0 ]; then
        echo Cannot build base conan
        exit 1
    fi
}


build_city() {
    mkdir -p builds/${ARCH}/${CONFIG}/distr
    echo Generate make files
    cmake -B builds/${ARCH}/${CONFIG} -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=${CONFIG}
    if [ $? != 0 ]; then
        echo Cannot generate make files
        exit 1
    fi
     
    cmake --build builds/${ARCH}/${CONFIG} --config ${CONFIG} -j 8
    if [ $? != 0 ]; then
        echo Cannot BUILD
        exit 1
    fi
}

rc_path_with_install_name_tool() {
    search_dir=${1}
    for entry in "$search_dir"/*
    do
        echo setup rc_path "$entry"
        install_name_tool -add_rpath "@loader_path/./" "$entry"
    done
}


rc_path_with_patchelf() {
    search_dir=${1}
    for entry in "$search_dir"/*
    do
        echo setup rc_path "$entry"
        patchelf --set-rpath '$ORIGIN/.' "$entry"
    done
}

build_universal() {
    rm -rf ./builds/universal/${CONFIG}
    mkdir -p ./builds/universal/${CONFIG}/x86_64
    mkdir -p ./builds/universal/${CONFIG}/armv8
    cp ./builds/x86_64/${CONFIG}/distr/* ./builds/universal/${CONFIG}/x86_64
    cp ./builds/armv8/${CONFIG}/distr/* ./builds/universal/${CONFIG}/armv8
    rc_path_with_install_name_tool ./builds/universal/${CONFIG}/x86_64
    rc_path_with_install_name_tool ./builds/universal/${CONFIG}/armv8
    search_dir=./builds/universal/${CONFIG}/x86_64
    for entry in "$search_dir"/*
    do
        base_name=$(basename ${entry})
        echo build universal ${base_name}
        lipo -create -output ./builds/universal/${CONFIG}/${base_name} ./builds/universal/${CONFIG}/x86_64/${base_name} ./builds/universal/${CONFIG}/armv8/${base_name}
    done
    rm -rf ./builds/universal/${CONFIG}/x86_64
    rm -rf ./builds/universal/${CONFIG}/armv8
}

build_linux_rpath() {
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    rm -rf ./builds/current/${CONFIG}/
    mkdir -p ./builds/current/${CONFIG}/
    cp ./builds/${ARCH}/${CONFIG}/distr/* ./builds/current/${CONFIG}/
    rc_path_with_patchelf ./builds/current/${CONFIG}
fi    
}

CONFIG=Release
ARCH=x86_64
BUILDFOLDER=build_linux
BUILDCONANFOLDER=builds_conan_linux

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	BUILDFOLDER=build_linux
	BUILDCONANFOLDER=builds_conan_linux
elif [[ "$OSTYPE" == "darwin"* ]]; then
	export MACOSX_DEPLOYMENT_TARGET=11.0
	BUILDFOLDER=build_mac
	BUILDCONANFOLDER=builds_conan_mac
else
	echo Unknown os
	exit 1
fi


if [[ ${1} != "" ]]; then
    CONFIG=${1}
fi

if [[ ${2} == "universal" ]]; then
    if [[ ${3} == SkipAllConan ]]; then
        ARCH=x86_64
        build_city
        ARCH=armv8
        build_city
        build_universal
        exit 0
    fi
    if [[ ${3} == NoClean ]]; then
        ARCH=x86_64
        build_conan
        build_city
        ARCH=armv8
        build_conan
        build_city
        build_universal
        exit 0
    fi
    
    ARCH=x86_64
    clean_up
    build_conan
    build_city
    ARCH=armv8
    build_conan
    build_city
    build_universal
    exit 0
fi

if [[ ${2} != "" ]]; then
    ARCH=${2}
fi
if [[ ${3} == SkipAllConan ]]; then
    build_city
    build_linux_rpath
    exit 0
fi
if [[ ${3} == NoClean ]]; then
    build_conan
    build_city
    build_linux_rpath
    exit 0
fi


clean_up
build_conan
build_city
build_linux_rpath

