#!/bin/sh

mkdir ./builds/distr
cmake -B builds
cmake --build builds -j 8
