#!/bin/bash


# plot I vs V with different temperatures in different linestyles
awk -f awk/iv.awk `cat data/ivCalibration.dat` > iv-N-calibration.dat
awk -f awk/iv.awk `cat data/ivCalibration2.dat` > iv-P-calibration.dat
awk -f awk/iv.awk `cat data/ivCalibration3.dat` > iv-N-120-calibration.dat

#tail iv-N-calibration.dat
#exit 0

file=iv-N-calibration.dat
file=iv-P-calibration.dat
file=iv-N-120-calibration.dat
cat > macro/ivCalibration1.gnu <<EOF
set key inside left top

set auto fix
set offsets 50, 50, 0.1, 0.1
n=12
#n=4
p for [i=2:n+1] '${file}' u 1:i i 0 t columnhead
#exit 0
set auto fix
set offsets graph 0.1,0.2,0.1,0.1

f(x)=m*x+q
fit f(x) '${file}' u 2:1 i 1 noerrors via m,q
b=1
a=1
c=0
g(x)=b*exp(a*x)+c
fit [-50:] g(x) '${file}' u 2:1 i 1 noerrors via a,b
set auto fix
set offsets graph 0.05, 0.05, 0.1, 0.1
set xlabel 'Temperature [^{o}C]'
set ylabel 'I(T) / I(-20^{o}C)'
p [:-7][] 'iv-N-calibration.dat' u 2:1 i 1 t '320 um p-in-n', 'iv-P-calibration.dat' u 2:1 i 1  t '320 um n-in-p', 'iv-N-120-calibration.dat' u 2:1 i 1 t '120 um p-in-n'
@OUTPUT 'ivCalibration.pdf'
EOF

gnuplot -persist macro/ivCalibration1.gnu
