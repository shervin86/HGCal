#!/bin/bash
# this script make the plots for all the measurements
EXT="png"
OUTDIR=result




for file in ${OUTDIR}/*/result.dat
do
    echo $file
    #gnuplot -e "call \"g.gnu\" \"${file}\" \"${fout}\" \"${EXT}\""
   fout=`dirname $file`/`basename $file .dat`-cce.${EXT}
   gnuplot  -e " call \"macro/cce.gnu\" \"${file}\"" -e " @TERMCH ${EXT}" -e "@OUTPUT \"${fout}\" "

   fout=`dirname $file`/`basename $file .dat`-iv.${EXT}
   gnuplot -e " call \"macro/iv.gnu\" \"${file}\"" -e " @TERMCH ${EXT}" -e "@OUTPUT \"${fout}\" "

    fout=`dirname $file`/`basename $file .dat`-cv.${EXT}
    #gnuplot -e " call \"macro/cv455.gnu\" \"${file}\"" -e " @TERMCH ${EXT}" -e "@OUTPUT \"${fout}\" "
    gnuplot -e " call \"macro/cv.gnu\" \"${file}\"" -e " @TERMCH ${EXT}" -e "@OUTPUT \"${fout}\" "

   fout=`dirname $file`/`basename $file .dat`-cce_cv_iv.${EXT}
   gnuplot -e " call \"macro/cce_cv_iv.gnu\" \"${fout}\" \"${file}\"" #-e " @TERMCH png" -e "@OUTPUT \"${fout}\" "

#    fout=`dirname $file`/`basename $file .dat`-cce_div_cv.${EXT}
#    gnuplot -e " call \"macro/cce_div_cv.gnu\" \"${fout}\" \"${file}\"" #-e " @TERMCH png" -e "@OUTPUT \"${fout}\" "
done

 exit 0

##############################
# only -20 and -30 degree on the same plot for each diode
for file in ${OUTDIR}/*30/*.dat
do
    ID=`echo $file | sed "s|${OUTDIR}/||;s|/.*||"`
    ID20=`echo ${ID} | sed 's|\(.*\)30|\1 20|;s| ||g'`
    file20=${OUTDIR}/${ID20}/result.dat
    echo ${file20}
    cat ${file} ${file20} > tmp/tmp.dat
    fout=tmp/tmp.${EXT}
    gnuplot -persistent -e "call \"g.gnu\" \"tmp/tmp.dat\" \"${fout}\" \"${EXT}\""
    exit 0
done



##############################
# only -20 and -30 degree on the same plot for each diode
for file in $@
do
   
    cat ${file} >> tmp/tmp.dat
    fout=tmp/tmp.${EXT}
    gnuplot -persistent -e "call \"g.gnu\" \"tmp/tmp.dat\" \"${fout}\" \"${EXT}\""
    exit 0
done

