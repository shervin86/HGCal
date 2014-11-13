files=""
do for [n=1:ARGC]{
   files=sprintf("%s %s",files, value("ARG".n))
}
#print files
#exit 
columnLegend=13

set key outside center top hor

set ylabel "CCE" 
#set xlabel "U [V]" offset first 565, char 1.5
set xlabel "U [V]" offset first 565, char 1.5

plot [-10:1100][0:1.15] for [file in files ] file u "bias":"CCE" w p ls 1 t columnhead(columnLegend), \
 for [file in files] file u "bias":(column("CCE")/(column("Q")/column("Qref"))) w p ls 3 t "reference", 1 notitle

#plot [-10:1010][0:1.15] for [IDX=0:IDXMAX] file  u "bias":"CCE"  i IDX w p ls IDX*2+1 t columnhead(columnLegend), \
#for [IDX=0:IDXMAX] file  u "bias":(column("CCE")/(column("Q")/column("Qref"))) i IDX w p ls IDX*2+2 t "reference".t(IDX), 1 notitle
