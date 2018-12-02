#!/bin/sh
set -e

APP_NAME="opene2140"
APP_BUNDLE="$APP_NAME.app"

#"Fix" the app
dylibbundler -od -b -x "build/$APP_BUNDLE/Contents/MacOS/$APP_NAME" -d "build/$APP_BUNDLE/Contents/libs/" -p @executable_path/../libs/

DEST="./opene2140-macos.zip"
rm -f $DEST
zip $DEST -r "build/$APP_BUNDLE"
