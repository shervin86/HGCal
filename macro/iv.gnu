set key outside center top hor

files=""
do for [n=1:ARGC]{
#   f=""
#   f=sprintf("%s", value("ARG".n))
print f
#   if( f eq 'notitle ' ){
#     unset key
#   } else { 
     files=sprintf("%s %s",files, value("ARG".n))
#   }
}

columnLegend=12

set ylabel "IV [uA]" 
set xlabel "U [V]" offset first 565, char 1.5

plot [-10:1100][0:*] for [file in files ] file  u (abs(column("bias"))):((abs(column("IV"))*1e6)) w p ls 1 t columnhead(columnLegend)

#plot [-10:1010][0:1.15] for [IDX=0:IDXMAX] file  u "bias":"IV"  i IDX w p ls IDX*2+1 t columnhead(columnLegend), \
#for [IDX=0:IDXMAX] file  u "bias":(column("CCE")/(column("Q")/column("Qref"))) i IDX w p ls IDX*2+2 t "reference".t(IDX), 1 notitle
