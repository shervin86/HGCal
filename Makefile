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

OPT=-Wall

INCLUDE=-I$(INCLUDEDIR) -I$(ROOT_INCLUDE)  -I$(ROOFIT_INCLUDE) -I$(BOOST)/include
LIB=-lboost_program_options ${ROOT_LIB}

default: test link

all: ${LIBDIR}/TCTmeasurementBase.o ${LIBDIR}/TCTmeasurements.o ${LIBDIR}/TCTimport.o test link

test: ${BUILDDIR}/test.exe 

${BUILDDIR}/test.exe: ${BUILDDIR}/test.cpp ${INCLUDEDIR}/TCTimport.h ${LIBDIR}/TCTmeasurements.o ${LIBDIR}/configFileParser.o	
	@echo "-> Making test program"
	g++ ${OPT} ${INCLUDE} ${LIB} -o ${BUILDDIR}/test.exe ${BUILDDIR}/test.cpp ${LIBDIR}/*.o

${LIBDIR}/TCTspectrumBase.o: include/TCTspectrumBase.h
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTspectrumBase.o include/TCTspectrumBase.h

${LIBDIR}/TCTspectrum.o: ${INCLUDEDIR}/TCTspectrum.h ${SRCDIR}/TCTspectrum.cc ${INCLUDEDIR}/TCTspectrumBase.h
	echo "-> Making TCTspectrum"
	g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTspectrum.o ${SRCDIR}/TCTspectrum.cc

${LIBDIR}/TCTimport.o: include/TCTimport.h ${INCLUDEDIR}/TCTmeasurement.h
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTimport.o include/TCTimport.h

${LIBDIR}/TCTmeasurements.o: ${INCLUDEDIR}/TCTmeasurements.h ${LIBDIR}/TCTspectrum.o 
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/TCTmeasurements.o ${SRCDIR}/TCTmeasurements.cc

${LIBDIR}/configFileParser.o: ${INCLUDEDIR}/configFileParser.h ${SRCDIR}/configFileParser.cc
	@g++ ${OPT} ${INCLUDE} ${LIB} -c -o ${LIBDIR}/configFileParser.o ${SRCDIR}/configFileParser.cc


link: 
	@./linkdef.sh
	@rootcint -f lib/TCTlib.cxx -c ${INCLUDEDIR}/*.h