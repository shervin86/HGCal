set key outside center top hor

files=""
do for [n=1:ARGC]{
f=""
   f=sprintf("%s" , value("ARG".n))
   if( f eq 'notitle' ){
     unset key
   } else { 
     files=sprintf("%s %s",files, value("ARG".n))
   }
}
#print files

columnLegend=13



set ylabel "CV: 1/F^{2} [10^{24} F^{-2}]" 
set xlabel "U [V]" offset first 565, char 1.5

plot [-10:1100][0:*] for [file in files ] file  u (abs(column("bias"))):((1/(column("CV455")*column("CV455")))*1e24) w p ls 1 t '455 Hz, '.columnhead(columnLegend), \
for [file in files ] file  u (abs(column("bias"))):((1./(column("CV1M")*column("CV1M")))*1e24) w p ls 3 t '1 MHz, '.columnhead(columnLegend)

#plot [-10:1010][0:1.15] for [IDX=0:IDXMAX] file  u "bias":"IV"  i IDX w p ls IDX*2+1 t columnhead(columnLegend), \
#for [IDX=0:IDXMAX] file  u "bias":(column("CCE")/(column("Q")/column("Qref"))) i IDX w p ls IDX*2+2 t "reference".t(IDX), 1 notitle
