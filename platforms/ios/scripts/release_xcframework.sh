#!/bin/bash
set -e

GODOT_PLUGINS="dialog"

# Compile Plugin
for lib in $GODOT_PLUGINS; do
    ./scripts/generate_xcframework.sh $lib release $1
    ./scripts/generate_xcframework.sh $lib release_debug $1
    mv ./bin/${lib}.release_debug.xcframework ./bin/${lib}.debug.xcframework
done

# Move to release folder
IOS_FOLDER="../../bin/ios/plugins"

rm -rf $IOS_FOLDER
mkdir $IOS_FOLDER

# Move Plugin
for lib in $GODOT_PLUGINS; do
    mkdir $IOS_FOLDER/${lib}
    mv ./bin/${lib}.{release,debug}.xcframework $IOS_FOLDER/${lib}
    cp ./src/${lib}.gdip $IOS_FOLDER/${lib}
done
