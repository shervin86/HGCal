files=""
do for [n=1:ARGC]{
   files=sprintf("%s %s",files, value("ARG".n))
}
#print files
#exit 
columnLegend=12

set key outside center top hor

set ylabel "Q [C]" 
set xlabel "U [V]" offset first 565, char 1.5

absQ='(abs(column("Q")))'
absQref='(abs(column("Qref")))'


#plot [-10:1100][0:3e-11] ARG1 u "bias":@absQ w p  t columnhead(columnLegend), \
#ARG2 u "bias":@absQ w p  ps 2 t columnhead(columnLegend), \
#ARG3 u "bias":@absQ w p  ps 2 t columnhead(columnLegend)

plot [-10:1100][0:3e-11] for [file in files ] file u "bias":@absQ  t columnhead(columnLegend), \
 for [file in files] file u "bias":((@absQ == @absQref) ? 1/0 :@absQref) w p ls 3 t "reference", 1 notitle

    
#plot [-10:1010][0:3e-11] for [IDX=0:IDXMAX] file  u "bias":(abs(column("Q")))  i IDX w p ls IDX*2+1 t columnhead(columnLegend), \
#for [IDX=0:IDXMAX] file  u "bias":(abs(column("Qref"))) i IDX w p ls IDX*2+2 t "reference".t(IDX), 1 notitle
