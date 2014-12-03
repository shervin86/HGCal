#!/bin/bash
#ivCalibration.pdf
#./scripts/calibrationIvsT.sh

EXT="pdf"
#### I vs V for all the diodes
./scripts/allCCE_CV_IV.sh IV ${EXT}

#### CCE vs V for all the diodes
./scripts/allCCE_CV_IV.sh CCE ${EXT}

#### C vs V for alla diodes (end capacitance)
./scripts/allCCE_CV_IV.sh CV1M ${EXT}

exit 0
 ./scripts/checkBaselines.sh

# show that the baseline subtraction is not perfect
# p 'result/irrP0024/spectra.dat' u 1:2 i 100 w l  t 'with baseline', 'result/irrP0024/spectra-nobaseline.dat' u 1:2 i 100 w l lt 1 lc 1 t 'no baseline', 0 lt 0

./scripts/makePlots.sh

./scripts/makeFinalPlots.sh





# I vs fluence ./scripts/allCCE_CV_IV.sh CV1M
