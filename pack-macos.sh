#!/bin/sh
set -e

DEST="./opene2140-macos.zip"
rm -f $DEST
zip $DEST -r "build/opene2140.app"
