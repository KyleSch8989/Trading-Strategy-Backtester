#!/bin/bash

if [ -d "build" ]; then
    echo "The build directory exists."
    echo "Run the 'run.sh' script to start the program."
else
    mkdir build
    cd build
    cmake ..
    make
fi
