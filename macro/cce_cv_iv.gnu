outfile=ARG1
files=""
do for [n=2:ARGC]{
   files=sprintf("%s %s",files, value("ARG".n))
}
#print files

columnLegend=13

set term pngcairo fontscale 1 size 10in,10in
set output outfile

set multiplot layout 3,1 

call 'macro/cce.gnu' files
set multiplot next

call 'macro/cv.gnu' files 
set multiplot next

call 'macro/iv.gnu' files 
#'notitle'

unset multiplot

set output
set term GNUTERM
