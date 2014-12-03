dir=tmp
for file in $dir/*-spectra.dat
do
    basenamefile=`basename $file .dat`
    gnuplot -e "call \"macro/baselineSpectra.gnu\" \"$file\" \"$dir/$basenamefile.pdf\""    
done

 