#!/usr/bin/bash

directory=$1
cd $directory
ls -l "$directory"  | grep -v '^d' | wc -l
flie_num=$(find "$directory" -type f | wc -l)
echo $flie_num

