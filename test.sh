#!/bin/sh

clear

filePath=output/test.out

make $filePath || exit 1

. <({ berr=$({ bout=$($filePath); } 2>&1; declare -p bout >&2); declare -p berr; } 2>&1)


echo "test resualt: "
echo "$bout"

echo "test errors: "
echo "$berr"