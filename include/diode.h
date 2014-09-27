#ifndef diode_h
#define diode_h

class diode{

 public:
  diode(); ///< default constructor

 private:
  configFileContent _property;
  TCTmeasurement    _tct;
  IVmeasurement     _iv;
  CVmeasurement     _cv;

  void GetTCT(float bias);
  float GetI(float bias);
  float GetC(float bias, float freq);
  
  std::ostream& dump(std::ostream& f, float bias);

 public:
  // properties
  SetProperties(configFileContent property);

  // Setting measurements
  SetTCT(TCTmeasurements tct);
  SetIV(IVmeasurement iv);
  SetCV(CVmeasurement cv);





#endif
