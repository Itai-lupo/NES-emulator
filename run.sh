#!/bin/sh

clear

filePath=output/main.out

make || exit 1

$($filePath);

echo "program exited with code $?"