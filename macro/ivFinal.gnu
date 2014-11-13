files=""
file=ARG1
file2=ARG2
file3=ARG3

columnLabel=13

SURFACE=25

#@TERMCH 'pdf'
TERM='pdf'
unset logscale x
#set offsets graph 0, 0.1, graph 0.15, 0.15
set offsets graph 0.08, 0.1, graph 0.25, 0.2
set xlabel "Fluence [neutron/cm^{2}]" offset 10 

set ylabel "Bulk current/volume  [uA/mm^{3}]"  offset 1

set arrow 1 from 1e15,0 to 1e15,graph 1 nohead lt 2 lw 2
set arrow 2 from 4e15,0 to 4e15,graph 1 nohead lt 2 lw 2
set arrow 320 from  graph 0,graph 0.9 to 1e15,  graph 0.90 heads
set arrow 200 from  1e15,graph 0.9 to 4e15,  graph 0.90 heads
set arrow 120 from  4e15,graph 0.9 to 1.8e16,graph 0.90 heads
#set arrow from 0,0 to 0,1 nohead
#set label 320 "320 um" at 0.5e14,graph 0.95
set label 320 "320 um" at graph 0,graph 0.85
set label 200 "200 um" at 1e15,graph 0.95
set label 120 "120 um" at 1e16,graph 0.95
set grid x
set grid y
#set xtics (4e14,6e14,1.5e15,2.5e15,1e16,6.25e15,1.6e16)
set xtics border 
set xtics ("4x10^{14}\t" 4e14 1,"6x10^{14}"6e14,1.5e15 1,"\n2.5x10^{15}"2.5e15,1e16,6.25e15,1.6e16)
#set x2tics ("4x10^{14}" 4e14 0,"6x10^{14}"6e14 1, "\t\t1.5x10^{15}"1.5e15 0,"\n2.5x10^{15}"2.5e15 1,1e16 1,6.25e15 1 ,1.6e16 1)
#set key at 10e15,graph 0.75 vertical Right noreverse enhanced autotitle box #nobox
set key inside bottom right vertical Right noreverse enhanced autotitle box #nobox

if(ARGC==1){
p [3e14:1.8e16][0:*] file u "fluence":((abs(column("IV"))*1e6))/(column("area")*column("thick")/1000)  w p ls 1 title column(columnLabel)
#p [3e14:1.8e16][0:*] file u "fluence":((abs(column("IV"))*1e6))  w p ls 1 title column(columnLabel)
}else{
if(ARGC==2){
p [*:1.8e16][*:*] file u "fluence":((abs(column("IV"))*1e6)/( column("area")*column("thick")/1000))  w p ls 1 title column(columnLabel), file2 u "fluence":( (abs(column("IV"))*1e6)/( column("area")*column("thick")/1000) )  w p ls 2 title column(columnLabel)
#p [*:1.8e16][*:*] file u "fluence":(abs(column("IV"))*1e6)  w p ls 1 title column(columnLabel), file2 u "fluence":(abs(column("IV"))*1e6)  w p ls 2 title column(columnLabel)
}else{
p [2e14:1.8e16][0:*] file u "fluence":((abs(column("IV"))*1e6)/( column("area")*column("thick")/1000))  w p ls 1 title column(columnLabel), file2 u "fluence":( (abs(column("IV"))*1e6)/( column("area")*column("thick")/1000) )  w p ls 2 title column(columnLabel), file3 u "fluence":( (abs(column("IV"))*1e6)/( column("area")*column("thick")/1000) )  w p ls 3 title column(columnLabel)
}
}
@OUTPUT 'IV.pdf'

