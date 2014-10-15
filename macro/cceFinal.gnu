files=""
file=ARG1
file2=ARG2

columnLabel=12

#@TERMCH 'pdf'
TERM='pdf'
set logscale x
set offsets graph 0, 0.1, 0., 0
set xlabel "Fluence [neutron/cm^{2}]" 
set ylabel "CCE" 
set arrow 1 from 1e15,0 to 1e15,1 nohead lt 2
set arrow 2 from 4e15,0 to 4e15,1 nohead lt 2
set arrow 320 from  2e14,0.42 to 1e15,0.42 heads
set arrow 200 from  1e15,0.47 to 4e15,0.47 heads
set arrow 120 from  4e15,0.52 to 1.8e16,0.52 heads
#set arrow from 0,0 to 0,1 nohead
set label 320 "320 um" at 3e14,0.4
set label 200 "200 um" at 2e15,0.45
set label 120 "120 um" at 1e16,0.5
set grid x
set grid y
set xtics (4e14,6e14,1.5e15,2.5e15,1e16,6.25e15,1.6e16)

set key inside bottom right vertical Right noreverse enhanced autotitle box #nobox

#p [2e14:1.8e16][0:1.1]'l.dat' i 0 u ($3):($11)  w p ps 2 lc 31 pt 7 title columnhead(1), 'l.dat' i 1 u 3:11 w p ps 2 pt 7 lc 1 title columnhead(1)
if(ARGC==1){
p [2e14:1.8e16][0:1.1] file u "fluence":"CCE"  w p ls 1 title column(columnLabel)
}else{
p [2e14:1.8e16][0:1.1] file u "fluence":"CCE"  w p ls 1 title column(columnLabel), file2 u "fluence":"CCE"  w p ls 3 title column(columnLabel)
}
@OUTPUT 'CCE.pdf'

