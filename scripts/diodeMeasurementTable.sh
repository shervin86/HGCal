#!/bin/bash
#file=data/dataP.dat
file=$1

diodeList=`cat $file | sed '/^#/ d;/^bas/ d' | awk '{print $2}' | sort | uniq`

cat $file | sed '/^#/ d;/^bas/ d' | awk -f awk/diodeMeasurementTable.awk