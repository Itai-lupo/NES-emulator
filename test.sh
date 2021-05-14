#!/bin/sh

clear

filePath=output/test.out

make $filePath || exit 1
$filePath

echo "program exited with code $?"
