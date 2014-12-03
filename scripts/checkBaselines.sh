#!/bin/bash

# for file in result/baselines/*.dat
# do
#     outFile=`dirname $file`/`basename $file .dat`.pdf
#     gnuplot -e "call 'macro/baselineSpectra.gnu' '$file' '$outFile'"
# done


for file in result/baselines/bas*.dat
do
    outFile=`dirname $file`/`basename $file .dat`-rbl.pdf
    fileRBL=`dirname $file`/`basename $file | sed 's|bas|rbl|'`
    if [ -e "$fileRBL" ];then
	gnuplot -e "call 'macro/baselineSpectra.gnu' '$file' '$outFile' '$fileRBL'"
    fi
done
