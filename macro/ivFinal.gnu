files=""
file=ARG1
file2=ARG2

columnLabel=12

#@TERMCH 'pdf'
TERM='pdf'
set logscale x
set offsets graph 0, 0.1, graph 0.15, 0.15
set xlabel "Fluence [neutron/cm^{2}]" 

set ylabel "IV [uA]" 

set arrow 1 from 1e15,0 to 1e15,graph 1 nohead lt 2 lw 2
set arrow 2 from 4e15,0 to 4e15,graph 1 nohead lt 2 lw 2
set arrow 320 from  2e14,graph 0.9 to 1e15,  graph 0.90 heads
set arrow 200 from  1e15,graph 0.9 to 4e15,  graph 0.90 heads
set arrow 120 from  4e15,graph 0.9 to 1.8e16,graph 0.90 heads
#set arrow from 0,0 to 0,1 nohead
set label 320 "320 um" at 3e14,graph 0.97
set label 200 "200 um" at 2e15,graph 0.97
set label 120 "120 um" at 1e16,graph 0.97
set grid x
set grid y
set xtics (4e14,6e14,1.5e15,2.5e15,1e16,6.25e15,1.6e16)

set key inside bottom right vertical Right noreverse enhanced autotitle box #nobox

if(ARGC==1){
p [2e14:1.8e16][0:1.1] file u "fluence":((abs(column("IV"))*1e6))  w p ls 1 title column(columnLabel)
}else{
p [2e14:1.8e16][0:*] file u "fluence":((abs(column("IV"))*1e6))  w p ls 1 title column(columnLabel), file2 u "fluence":((abs(column("IV"))*1e6))  w p ls 3 title column(columnLabel)
}
@OUTPUT 'IV.pdf'

