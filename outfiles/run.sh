#!/bin/sh
clear
chmod +x ./NES.out

a=$(find ./DLLS -type d)
b=""
for i in $(echo $a | tr " " "\n")
do
    b+=$i:
done


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/:$b
filePath=./NES.out

clear
echo $1
$filePath $1

echo "program exited with code $?"
