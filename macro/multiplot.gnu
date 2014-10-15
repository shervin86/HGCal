set multiplot

#--------------- first pad
set size 0.95,0.33
set origin 0,0.66

set key outside center top hor

load 'macro/cce.gnu'

#--------------- second pad
set size 0.95, 0.33
set origin 0,0.33

set key outside center top hor

#set title 'CV' offset 0,-3
set ylabel "1/C^{2} [pF^{-2}]"
#unset xlabel
plot [-10:1010][0:0.015] for [IDX=1:1] file  u 4:(1/($9*1e12)*1/($9*1e12)) w p ls IDX*2+1 t '455 Hz'.t(IDX) , \
1 notitle
#for [IDX=0:IDXMAX] file  u 4:(1/($10*1e12)*1/($10*1e12)) w p ls IDX*2+2 t '1 MHz'.t(IDX)

#--------------- third pad
set size 0.95, 0.33
set origin 0,0

set key inside top left ver

#set title 'IV' offset 0,-3
set ylabel "I [uA]"
#unset xlabel
plot [-10:1010][0:*] for [ IDX=0:IDXMAX] file  u 4:(abs($8)*1e6) i IDX w p ls IDX*2+1 t t(IDX) 

#------------------------------
unset multiplot
