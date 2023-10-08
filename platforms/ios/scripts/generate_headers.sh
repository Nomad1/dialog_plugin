#!/bin/bash
pwd=$PWD

if [[ "$1" == "3.x" ]];
then
    cd ../../godot-3.x && \
        $pwd/scripts/timeout scons platform=iphone target=release_debug
else
    cd ../../godot && \
        $pwd/scripts/timeout scons platform=ios target=release_debug  
fi