#!/bin/bash
compressedFile=/tmp/hgcal.tar.bz2
file=$1

diodeList=`cat $file | grep -v '^#' | awk '{print $2}' | sort  | uniq`

for diode in $diodeList
do
