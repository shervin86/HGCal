#ifndef diode_h
#define diode_h
/** \class diode 
 */

#include "IVmeasurement.h"
#include "CVmeasurement.h"
#include "TCTmeasurements.h"
#include "configFileParser.h"

class diode{

 public:
  diode(); ///< default constructor
 diode(std::string id, configFileContent& prop, IVmeasurement& iv, CVmeasurement& cv, TCTmeasurements& tct, float startSignal, float endSignal): 
  _id(id), _property(prop), _iv(iv), _cv(cv), _tct(tct), 
    _startSignal(startSignal), _endSignal(endSignal){};

 diode(std::string id, configFileContent& prop, TCTmeasurements& tct, float startSignal, float endSignal): 
  _id(id), _property(prop), _iv(), _cv(), _tct(tct), 
    _startSignal(startSignal), _endSignal(endSignal){};

 private:
  std::string _id;
  configFileContent _property;
  IVmeasurement     _iv;
  CVmeasurement     _cv;
  TCTmeasurements   _tct;
  float             _startSignal;
  float             _endSignal;

 public:
  void GetTCT(float bias);
  float GetI(float bias);
  float GetC(float bias, float freq);
  
  //  std::ostream& dump(std::ostream& f, float bias);
  std::ostream& dump(std::ostream& f);
 public:
  // properties
  void SetProperties(configFileContent property);

  // Setting measurements
  void SetTCT(TCTmeasurements tct){_tct=tct;};
  void SetIV(IVmeasurement iv){    _iv=iv;};
  void SetCV(CVmeasurement cv){    _cv=cv;};

  void SetStartSignal(float startSignal){ _startSignal=startSignal;};
  void SetEndSignal(float endSignal){ _endSignal=endSignal;};
};



#endif
