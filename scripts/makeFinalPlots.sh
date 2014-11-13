fileP=data/dataP-ann10at60.dat
fileN=data/dataN-ann10at60.dat
ptypes=`cut -d ' ' -f 1 $fileP | cut -f 1 |grep -v '#' | grep -v 'bas' | grep -v 'rbl' | sort | uniq`
ntypes=`cut -d ' ' -f 1 $fileN | cut -f 1 |grep -v '#' | grep -v 'bas' | grep -v 'rbl' | sort | uniq`
#echo $ntypes
#echo $ptypes

BIAS=1000
BIAS=600
#BIAS=800
#BIAS=900
cat > tmp/awk.awk <<EOF
BEGIN{
}

(\$5 == BIAS && \$4==TEMP){
print \$0
}
END{
}
EOF

echo "" > tmp/head
echo "" >> tmp/head
head -1 result/irrP1020/result.dat |sed 's|".*"|"LEGEND"|' | sed "s|$|\t${BIAS}|" >> tmp/head

#---
TEMP=-30
cat tmp/head | sed 's|LEGEND|-30C|'> tmp/p30-${BIAS}.dat
for type in $ptypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/p30-${BIAS}.dat
done
#---
cat tmp/head | sed 's|LEGEND|-20C|'> tmp/p20-${BIAS}.dat
TEMP=-20
for type in $ptypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/p20-${BIAS}.dat
done


TEMP=-30
cat tmp/head | sed 's|LEGEND|-30C|'> tmp/n30-${BIAS}.dat
for type in $ntypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/n30-${BIAS}.dat
done

cat tmp/head | sed 's|LEGEND|-20C|'> tmp/n20-${BIAS}.dat
TEMP=-20
for type in $ntypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/n20-${BIAS}.dat
done




gnuplot -e "set title 'Charge collection efficiency \@${BIAS} V" -e "call 'macro/cceFinal.gnu' 'tmp/p20-${BIAS}.dat' 'tmp/p30-${BIAS}.dat'"
gnuplot -e "set title ''" -e "call 'macro/ivFinal.gnu' 'tmp/p20-${BIAS}.dat' 'tmp/p30-${BIAS}.dat'"

mv CCE.pdf CCE-p.pdf
mv IV.pdf IV-p.pdf
#exit 0
gnuplot -e "set title 'Charge collection efficiency \@${BIAS} V" -e "call 'macro/cceFinal.gnu' 'tmp/n20-${BIAS}.dat' 'tmp/n30-${BIAS}.dat'"
gnuplot -e "set title ''" -e "call 'macro/ivFinal.gnu' 'tmp/n20-${BIAS}.dat' 'tmp/n30-${BIAS}.dat'"

mv CCE.pdf CCE-n.pdf
mv IV.pdf IV-n.pdf



TEMP=-30
cat tmp/head | sed 's|LEGEND|p-type, -30C|'> tmp/p-30-${BIAS}.dat
for type in $ptypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/p-30-${BIAS}.dat
done

cat tmp/head | sed 's|LEGEND|n-type, -30c|'> tmp/n-30-${BIAS}.dat
for type in $ntypes
do
    file=result/$type/result.dat
awk -v TEMP=${TEMP} -v BIAS=${BIAS} -f tmp/awk.awk ${file} >> tmp/n-30-${BIAS}.dat
done

gnuplot -e "set title 'Charge collection efficiency \@${BIAS} V" -e "call 'macro/cceFinal.gnu' 'tmp/p-30-${BIAS}.dat' 'tmp/n-30-${BIAS}.dat'"
mv CCE.pdf CCE-pn.pdf

gnuplot -e "set title 'Bulk current \@${BIAS} V" -e "call 'macro/ivFinal.gnu' 'tmp/p-30-${BIAS}.dat' 'tmp/n-30-${BIAS}.dat'"
mv IV.pdf IV-pn.pdf
