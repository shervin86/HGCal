ptypes=`cut -d ' ' -f 1 data/dataP.dat | cut -f 1 |grep -v '#' | grep -v 'bas' | sort | uniq`
ntypes=`cut -d ' ' -f 1 data/data.list | cut -f 1 |grep -v '#' | grep -v 'bas' | sort | uniq`
#echo $ntypes
#echo $ptypes

BIAS=1000
TEMP=-30
cat > tmp/awk.awk <<EOF
BEGIN{
}

(\$4 == BIAS && \$3==TEMP){
print \$0
}
END{
}
EOF

echo "" > tmp/head
echo "" >> tmp/head
head -1 result/irrP1020/result.dat |sed 's|".*"|"LEGEND"|' >> tmp/head

cat tmp/head | sed 's|LEGEND|-30C|'> tmp/p30.dat
for type in $ptypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/p30.dat
done

cat tmp/head | sed 's|LEGEND|-20C|'> tmp/p20.dat
TEMP=-20
for type in $ptypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/p20.dat
done



cat tmp/head | sed 's|LEGEND|-30C|'> tmp/n30.dat
for type in $ntypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/n30.dat
done

cat tmp/head | sed 's|LEGEND|-20C|'> tmp/n20.dat
TEMP=-20
for type in $ntypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/n20.dat
done




gnuplot -e "set title 'Charge collection efficiency \@${BIAS} V" -e 'call "macro/cceFinal.gnu" "tmp/p20.dat" "tmp/p30.dat"'
gnuplot -e "set title ''" -e 'call "macro/ivFinal.gnu" "tmp/p20.dat" "tmp/p30.dat"'

mv CCE.pdf CCE-p.pdf
mv IV.pdf IV-p.pdf

gnuplot -e "set title 'Charge collection efficiency \@${BIAS} V" -e 'call "macro/cceFinal.gnu" "tmp/n20.dat" "tmp/n30.dat"'
gnuplot -e "set title ''" -e 'call "macro/ivFinal.gnu" "tmp/n20.dat" "tmp/n30.dat"'

mv CCE.pdf CCE-n.pdf
mv IV.pdf IV-n.pdf




cat tmp/head | sed 's|LEGEND|p-type, -30C|'> tmp/p-30.dat
for type in $ptypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/p-30.dat
done

cat tmp/head | sed 's|LEGEND|n-type, -30c|'> tmp/n-30.dat
for type in $ntypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/n-30.dat
done

gnuplot -e "set title 'Charge collection efficiency \@${BIAS} V" -e 'call "macro/cceFinal.gnu" "tmp/p-30.dat" "tmp/n-30.dat"'
mv CCE.pdf CCE-pn.pdf
