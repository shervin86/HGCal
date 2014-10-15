#### model for plotting TCT, CV, IV results for one diode
# the variable file should be set before calling this macro
file=ARG1
outfile=ARG2
ext=ARG3
columnLegend=12


TITLE=system("head -1 ".file." | sed 's|[^\"]*\"||;s|\".*||'")
TITLES=system("grep bias ".file." | sed 's|[^\"]*\"||;s|\".*||'")
IDXMAX=system("echo \"".TITLES."\" | wc -l")
IDXMAX=IDXMAX-1
TITLESDIFF=system("echo \"".TITLES."\" | awk -F ',' '(NR==1){for(i=1;i<=NF;i++){c[i]=$i};};(NR!=1){for(i=1;i<=NF;i++){if(c[i]!=$i){ l[1]=sprintf(\"%s,%s\",l[1],c[i]);l[NR]=sprintf(\"%s,%s\",l[NR],$i);}}};END{for(i=1; i <=NR;i++){print l[i]}}'") # | sed 's|^, ||'")

t(n)=system("echo \"".TITLESDIFF."\" | awk '(NF==0 && NR==".n."+1){print NR};(NF!=0 && NR==".n."+1){print $0}'")
t(n)=system("echo \"".TITLES."\" | awk '(NF==0 && NR==".n."+1){print NR};(NF!=0 && NR==".n."+1){print \",\",$0}'")
print TITLES
print TITLESDIFF

print t(0)
print t(1)
#exit
#------------------------------ Settings

#set offsets graph 0, 0., 0, 0

#set linetype 1 pt 7 lc 8 ps 1.
#set linetype
#set linetype 2 pt 9 lc 8 ps 1.

set key inside bottom right width 2
set key outside center top hor
#set key outside right 
#set xtics border in 
#------------------------------ Setting terminal
#set term GNUTERM fontscale 0.8

if(ext eq "pdf"){
  set term pdfcairo fontscale 0.8 size 10in,8in
}else {
   if(ext eq "png"){
     set term pngcairo fontscale 0.8 size 10in,8in
   }
}
#set term pdfcairo fontscale 0.8 size 10in,8in	
set output outfile


#--------------- common settings
set xlabel "U [V]" offset first 565, char 1.5
unset title

set linestyle 1 lc black lt 1 lw 1 pt 7  pi 1
set linestyle 2 lc black lt 1 lw 1 pt 13 pi 1
set linestyle 3 lc black lt 1 lw 1 pt 6  pi 1
set linestyle 4 lc black lt 1 lw 1 pt 12 pi 1


set output 
set term GNUTERM


#system("awk 'BEGIN{b=0};(NF==0){print $0};(NF<10){if($5==-30) b=1;else b=0;};(b==1){print $0}' test.dat > t.dat")

#plot for [IDX=1:20] file  u 4:7 i IDX t columnhead(1)
#while(IDX<IDXmax){
#plot [0:1000][0:1] file u 4:7 i IDX lt IDX t columnhead(4)
#IDX=IDX+1
#print IDX
#}

