#!/bin/sh

doxygen doc.conf
git add .
echo "insirt commit desc"
read buffer
echo $buffer
git commit -m "$buffer"
git push