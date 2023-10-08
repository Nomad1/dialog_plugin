#!/bin/bash
set -e

GODOT_PLUGINS="dialog"
GODOT_VERSION="$1"

if [ "$1" != "3.x" ] && [ "$1" != "4.x" ]; then
    echo "Invalid Godot version $1. Please provide '3.x' or '4.x'."
    exit 1
fi

# Compile Plugin
for lib in $GODOT_PLUGINS; do
    rm -rf ./bin/${lib}.release.xcframework
    rm -rf ./bin/${lib}.release_debug.xcframework
    rm -rf ./bin/${lib}.debug.xcframework

    ./scripts/generate_xcframework.sh $lib release $GODOT_VERSION
    ./scripts/generate_xcframework.sh $lib release_debug $GODOT_VERSION
    mv ./bin/${lib}.release_debug.xcframework ./bin/${lib}.debug.xcframework
done

# Move to release folder
IOS_FOLDER="../../bin/$GODOT_VERSION/ios/plugins/"

rm -rf $IOS_FOLDER
mkdir -p $IOS_FOLDER

# Move Plugin
for lib in $GODOT_PLUGINS; do
    mv ./bin/${lib}.{release,debug}.xcframework $IOS_FOLDER
    cp ./src/${lib}.gdip $IOS_FOLDER
done
