#ifndef TCTimport_h
#define TCTimport_h
/// program to import TCT mct files

#include "TCTspectrum.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
/** \class TCTimport TCTimport.h include/TCTimport.h
 *
 */

class TCTimport{
 public:
  /// basic constructor 
  TCTimport(){};

  /// import set of spectra from one TCT measurement file
  TCTspectrumCollection_t ImportFromFile(std::string filename);
  
 private:
  /// read one line from the input stream and put the values into the TCTspectrum 
  bool ReadLine(std::ifstream& f, TCTspectrum &meas);
  


};



#endif

