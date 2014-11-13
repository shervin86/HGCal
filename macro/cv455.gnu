files=""
do for [n=1:ARGC]{
   files=sprintf("%s %s",files, value("ARG".n))
}
#print files

columnLegend=13

set key outside center top hor

set ylabel "CV at 455 Hz: 1/F^{2} [10^{12} C^{-2}]" 
#set xlabel "U [V]" offset first 565, char 1.5
set xlabel "U [V]" offset first 0, char 1.5

plot [-10:1010][0:*] for [file in files ] file  u (abs(column("bias"))):((abs(column("CV455"))*1e12)) w p ls 1 t columnhead(columnLegend)

#plot [-10:1010][0:1.15] for [IDX=0:IDXMAX] file  u "bias":"IV"  i IDX w p ls IDX*2+1 t columnhead(columnLegend), \
#for [IDX=0:IDXMAX] file  u "bias":(column("CCE")/(column("Q")/column("Qref"))) i IDX w p ls IDX*2+2 t "reference".t(IDX), 1 notitle
