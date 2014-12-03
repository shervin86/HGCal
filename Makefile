INCLUDEDIR=include
SRCDIR=src
BUILDDIR=bin
LIBDIR=lib

ROOT_LIB=`root-config --libs --glibs`
ROOT_FLAGS=`root-config --cflags --ldflags`
ROOT_INCLUDE=`root-config --incdir`
# Boost
#BOOST = /afs/cern.ch/cms/slc5_amd64_gcc434/external/boost/1.47.0
#BOOST=/afs/cern.ch/cms/slc5_amd64_gcc472/external/boost/1.50.0
	SCRAMTOOL=$(shell scram || echo 1)
ifeq ($(SCRAMTOOL),1)
	ROOFIT_LIB="-lRooFit"
	ROOSTAT_LIB="-lRooStats"
	ROOFIT_INCLUDE="./"
else
	ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
	ROOFIT_LIB := -l$(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIB= | sed 's|LIB=||')
	ROOFIT_LIB += -l$(shell cd $(CMSSW_BASE); scram tool info roofit | grep LIB= | sed 's|LIB=||')
	ROOFIT_LIBDIR = -L$(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')
	ROOFIT_LIB+=$(ROOFIT_LIBDIR)
endif

OPT=-Wall -std=c++0x

INCLUDE=-I$(INCLUDEDIR) -I$(ROOT_INCLUDE)  -I$(ROOFIT_INCLUDE) -I$(BOOST)/include
LIB=-lboost_program_options ${ROOT_LIB} -lboost_regex

default: test link

all: ${LIBDIR}/TCTmeasurementBase.o ${LIBDIR}/TCTmeasurements.o ${LIBDIR}/TCTimport.o test link

test: ${BUILDDIR}/test.exe 

${BUILDDIR}/test.exe: ${BUILDDIR}/test.cpp ${SRCDIR}/functions.cc ${LIBDIR}/TCTimport.o ${LIBDIR}/TCTmeasurements.o ${LIBDIR}/configFileParser.o ${LIBDIR}/IVimport.o ${LIBDIR}/IVmeasurement.o ${LIBDIR}/CVimport.o ${LIBDIR}/CVmeasurement.o ${LIBDIR}/diode.o
	@echo "-> Making test program"
	@g++ ${OPT} -Wunused-value -Wunused-variable ${INCLUDE} ${LIB} -o ${BUILDDIR}/test.exe ${BUILDDIR}/test.cpp ${LIBDIR}/*.o

${LIBDIR}/TCTspectrumBase.o: include/TCTspectrumBase.h
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTspectrumBase.o include/TCTspectrumBase.h

spectrum: ${LIBDIR}/TCTspectrum.o
${LIBDIR}/TCTspectrum.o: ${INCLUDEDIR}/TCTspectrum.h ${SRCDIR}/TCTspectrum.cc ${INCLUDEDIR}/TCTspectrumBase.h
	@echo "-> Making TCTspectrum"
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTspectrum.o ${SRCDIR}/TCTspectrum.cc

${LIBDIR}/TCTimport.o: include/TCTimport.h ${SRCDIR}/TCTimport.cc ${INCLUDEDIR}/TCTmeasurements.h
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTimport.o ${SRCDIR}/TCTimport.cc

${LIBDIR}/TCTmeasurements.o: ${INCLUDEDIR}/TCTmeasurements.h ${SRCDIR}/TCTmeasurements.cc ${LIBDIR}/TCTspectrum.o 
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTmeasurements.o ${SRCDIR}/TCTmeasurements.cc

${LIBDIR}/configFileParser.o: ${INCLUDEDIR}/configFileParser.h ${SRCDIR}/configFileParser.cc
	@echo "-> Making configFileParser" 
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/configFileParser.o ${SRCDIR}/configFileParser.cc

${LIBDIR}/IVmeasurement.o: ${INCLUDEDIR}/IVmeasurement.h ${SRCDIR}/IVmeasurement.cc
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/IVmeasurement.o ${SRCDIR}/IVmeasurement.cc

${LIBDIR}/IVimport.o: ${INCLUDEDIR}/IVimport.h ${SRCDIR}/IVimport.cc ${INCLUDEDIR}/IVmeasurement.h
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/IVimport.o ${SRCDIR}/IVimport.cc


${LIBDIR}/CVmeasurement.o: ${INCLUDEDIR}/CVmeasurement.h ${SRCDIR}/CVmeasurement.cc
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/CVmeasurement.o ${SRCDIR}/CVmeasurement.cc

${LIBDIR}/CVimport.o: ${INCLUDEDIR}/CVimport.h ${SRCDIR}/CVimport.cc ${INCLUDEDIR}/CVmeasurement.h
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/CVimport.o ${SRCDIR}/CVimport.cc

diode: ${LIBDIR}/diode.o
${LIBDIR}/diode.o: ${INCLUDEDIR}/diode.h ${SRCDIR}/diode.cc ${INCLUDEDIR}/CVmeasurement.h ${INCLUDEDIR}/IVmeasurement.h ${INCLUDEDIR}/TCTmeasurements.h
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/diode.o ${SRCDIR}/diode.cc


link: 
	@./linkdef.sh
	@rootcint -f lib/TCTlib.cxx -c ${INCLUDEDIR}/*.h

clean:
	rm ${LIBDIR}/*.o 