#!/bin/sh

clear

filePath=output/main.out

make || exit 1

. <({ berr=$({ bout=$($filePath); } 2>&1; declare -p bout >&2); declare -p berr; } 2>&1)

echo "$bout"
echo "$berr"
echo
echo "program exited with code $?"