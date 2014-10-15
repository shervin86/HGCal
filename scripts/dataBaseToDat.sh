#!/bin/bash

cat $1 | sed 's|,|\n|g;' | sed '1,13 d;' | sed '3,21 d; s|\"||g' > tmp/f1.dat

 cat tmp/f1.dat | sed '2 d' | sed 's| |\n|g' > tmp/f2.dat
cat tmp/f1.dat | sed '1 d' | sed 's| |\n|g' > tmp/f3.dat

paste tmp/f3.dat tmp/f2.dat  > tmp/fend.dat

