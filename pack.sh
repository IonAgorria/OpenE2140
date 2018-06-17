#!/bin/sh
set -e

#Get destination
DEST="$1"
if [ -z $DEST ]; then
    DEST="./opene2140.zip"
fi

#Zip
rm -f $DEST
zip $DEST -r "build/opene2140"
