#!/bin/bash

PLOT=$1
EXT=$2

for file in result/irr*/result.dat
do 
    echo $file; 
    tag=`head -1 $file | cut -f 25`
    type=`echo -n $tag | sed 's|irr\([PN]\).*|\1|'`
    head -1 $file  | sed "s|-20 C|$type-type|" > tmp/$tag.legend

    sed '1 d' $file > tmp/$tag.dat
    #cp tmp/$tag.legend tmp/$tag.dat
    #awk '(NR==1){}; (NF>1&& NR!=1){print $5, $8, $10/($1*$2)*1e6/1e3}' $file >> tmp/$tag.dat
done

case $PLOT in 
    CCE)
	USING="\"bias\":\"CCE\""
	YLABEL="CCE"
	OUTFILE="all-CCE_vs_V"
	;;
    IV)
	# area mm2*um*1000 = mm3
	USING="\"bias\":((abs(column(\"IVGR\"))*1e6)/( column(\"area\")*column(\"thick\")/1000))"
	YLABEL="Bulk current/volume  [uA/mm^{3}]"
	OUTFILE="all-I_vs_V"
	;;
    CV1M)
	#USING="\"bias\":((1./(column(\"CV1M\")*column(\"CV1M\")))*1e24)"
	#YLABEL="1/F^{2} [10^{24} F^{-2}]"

	USING="\"bias\":(column(\"CV1MGR\")*1e12)"
	YLABEL="Capacitance at 1 MHz [pF]"
	OUTFILE="all-CV1M_vs_V"
	;;
    CV455)
	USING="\"bias\":(1e24/(column(\"CV455GR\")*column(\"CV455GR\")))"
	YLABEL="1/(Capacitance at 455 MHz)^2 [1/pF^2]"
	OUTFILE="all-CV455_vs_V"
	;;
    *)
	exit 0;
	;;
esac

#filelistP="`ls -1 tmp/irrP*.dat | sed 's|^| |'`"
#filelistN="`ls -1 tmp/irrN*.dat | sed 's|^| |'`"

#p for [file in filelistP] file u 1:2 w p pt 7 ps 2 t columnheader(1)

cat > tmp/g.gnu <<EOF
USING='$USING'
columnLegend=22
set key outside hor
set auto fix
#set offsets  graph 0., 0., 0.05, 0.025
set offsets graph 0., 0., 0.05, 0.01
set ylabel offset 1
set key font "Times,9" samplen 1
p  "result/irrN0001/result.dat" u @USING w p pt 7 ps 1 lc 24 t columnheader(columnLegend), \\
  "result/irrN0002/result.dat" u @USING w p pt 7 ps 1 lc 24 t columnheader(columnLegend), \\
  "result/irrN0003/result.dat" u @USING w p pt 7 ps 1 lc 2 t columnheader(columnLegend), \\
  "result/irrN0004/result.dat" u @USING w p pt 7 ps 1 lc 2 t columnheader(columnLegend), \\
  "result/irrN0011/result.dat" u @USING w p pt 7 ps 1 lc 3 t columnheader(columnLegend), \\
  "result/irrN0012/result.dat" u @USING w p pt 7 ps 1 lc 3 t columnheader(columnLegend), \\
  "result/irrN0013/result.dat" u @USING w p pt 7 ps 1 lc 4 t columnheader(columnLegend), \\
  "result/irrN0014/result.dat" u @USING w p pt 7 ps 1 lc 4 t columnheader(columnLegend), \\
  "result/irrN0021/result.dat" u @USING w p pt 7 ps 1 lc 5 t columnheader(columnLegend), \\
  "result/irrN0022/result.dat" u @USING w p pt 7 ps 1 lc 5 t columnheader(columnLegend), \\
  "result/irrN0024/result.dat" u @USING w p pt 7 ps 1 lc 33 t columnheader(columnLegend), \\
  "result/irrN0023/result.dat" u @USING w p pt 7 ps 1 lc 7 t columnheader(columnLegend), \\
  "result/irrP0001/result.dat" u @USING w p pt 9 ps 1 lc 24 t columnheader(columnLegend), \\
  "result/irrP0002/result.dat" u @USING w p pt 9 ps 1 lc 24 t columnheader(columnLegend), \\
  "result/irrP0003/result.dat" u @USING w p pt 9 ps 1 lc 2 t columnheader(columnLegend), \\
  "result/irrP0004/result.dat" u @USING w p pt 9 ps 1 lc 2 t columnheader(columnLegend), \\
  "result/irrP0011/result.dat" u @USING w p pt 9 ps 1 lc 3 t columnheader(columnLegend), \\
  "result/irrP0012/result.dat" u @USING w p pt 9 ps 1 lc 3 t columnheader(columnLegend), \\
  "result/irrP0013/result.dat" u @USING w p pt 9 ps 1 lc 4 t columnheader(columnLegend), \\
  "result/irrP0014/result.dat" u @USING w p pt 9 ps 1 lc 4 t columnheader(columnLegend), \\
  "result/irrP0022/result.dat" u @USING w p pt 9 ps 1 lc 5 t columnheader(columnLegend), \\
  "result/irrP0023/result.dat" u @USING w p pt 9 ps 1 lc 5 t columnheader(columnLegend), \\
  "result/irrP0021/result.dat" u @USING w p pt 9 ps 1 lc 33 t columnheader(columnLegend), \\
  "result/irrP0024/result.dat" u @USING w p pt 9 ps 1 lc 7 t columnheader(columnLegend)
set xlabel "U [V]"
set ylabel "$YLABEL"
show offsets
show yrange
@OUTPUT '${OUTFILE}.${EXT}'
EOF

gnuplot -persist tmp/g.gnu

exit 0
cat > tmp/gIV.gnu <<EOF
set key outside hor
p [][1e-7:1e-5] "tmp/irrN0001.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 24 t columnheader(1), \
  "tmp/irrN0002.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 24 t columnheader(1), \
  "tmp/irrN0003.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 2 t columnheader(1), \
  "tmp/irrN0004.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 2 t columnheader(1), \
  "tmp/irrN0011.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 3 t columnheader(1), \
  "tmp/irrN0012.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 3 t columnheader(1), \
  "tmp/irrN0013.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 4 t columnheader(1), \
  "tmp/irrN0014.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 4 t columnheader(1), \
  "tmp/irrN0021.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 5 t columnheader(1), \
  "tmp/irrN0022.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 5 t columnheader(1), \
  "tmp/irrN0024.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 33 t columnheader(1), \
  "tmp/irrN0023.dat" u 1:(abs(column(3))) w p pt 7 ps 1 lc 7 t columnheader(1), \
  "tmp/irrP0001.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 24 t columnheader(1), \
  "tmp/irrP0002.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 24 t columnheader(1), \
  "tmp/irrP0003.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 2 t columnheader(1), \
  "tmp/irrP0004.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 2 t columnheader(1), \
  "tmp/irrP0011.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 3 t columnheader(1), \
  "tmp/irrP0012.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 3 t columnheader(1), \
  "tmp/irrP0013.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 4 t columnheader(1), \
  "tmp/irrP0014.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 4 t columnheader(1), \
  "tmp/irrP0022.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 5 t columnheader(1), \
  "tmp/irrP0023.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 5 t columnheader(1), \
  "tmp/irrP0021.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 33 t columnheader(1), \
  "tmp/irrP0024.dat" u 1:(abs(column(3))) w p pt 9 ps 1 lc 7 t columnheader(1)
set xlabel "U [V]"
set ylabel "Bulk current/volume  [uA/mm^{3}]"  offset 1

EOF

cat > tmp/gCCE_IV.gnu <<EOF
set key outside hor
p [][1e-7:1e-5] "tmp/irrN0001.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 24 t columnheader(1), \
  "tmp/irrN0002.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 24 t columnheader(1), \
  "tmp/irrN0003.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 2 t columnheader(1), \
  "tmp/irrN0004.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 2 t columnheader(1), \
  "tmp/irrN0011.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 3 t columnheader(1), \
  "tmp/irrN0012.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 3 t columnheader(1), \
  "tmp/irrN0013.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 4 t columnheader(1), \
  "tmp/irrN0014.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 4 t columnheader(1), \
  "tmp/irrN0021.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 5 t columnheader(1), \
  "tmp/irrN0022.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 5 t columnheader(1), \
  "tmp/irrN0024.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 33 t columnheader(1), \
  "tmp/irrN0023.dat" u 1:(column(2)/abs(column(3))) w p pt 7 ps 1 lc 7 t columnheader(1), \
  "tmp/irrP0001.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 24 t columnheader(1), \
  "tmp/irrP0002.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 24 t columnheader(1), \
  "tmp/irrP0003.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 2 t columnheader(1), \
  "tmp/irrP0004.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 2 t columnheader(1), \
  "tmp/irrP0011.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 3 t columnheader(1), \
  "tmp/irrP0012.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 3 t columnheader(1), \
  "tmp/irrP0013.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 4 t columnheader(1), \
  "tmp/irrP0014.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 4 t columnheader(1), \
  "tmp/irrP0022.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 5 t columnheader(1), \
  "tmp/irrP0023.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 5 t columnheader(1), \
  "tmp/irrP0021.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 33 t columnheader(1), \
  "tmp/irrP0024.dat" u 1:(column(2)/abs(column(3))) w p pt 9 ps 1 lc 7 t columnheader(1)
set xlabel "U [V]"
set ylabel "Bulk current/volume  [uA/mm^{3}]"  offset 1

EOF

