#!/bin/sh
set -e

DEST="./opene2140-linux.zip"
rm -f $DEST
zip $DEST -r "build/opene2140"
