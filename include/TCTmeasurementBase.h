/// basic info from measurement
#ifndef TCTmeasurementBase_h
#define TCTmeasurementBase_h

#include <string>
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>
/** \class TCTmeasurementBase TCTmeasurementBase.h include/TCTmeasurementBase.h
    This class provides the basic informations contained in the txt
    files. Operations on the measurement and more advanced operations
    should be done in another class inheriting from this.

    \todo 
      - add the type of irradiation
      - add the illumination on front/back
      - add the type of irradiation: unirradiate, neutron, other
      - add the type of illumination: red laser, IR laser, beta, alfa
      - remove assert in favor of exceptions
*/

#define MAX_SAMPLES 1005
class TCTmeasurementBase{
 public:
 TCTmeasurementBase(std::string& diodeName):
  _nSamples(0)
    {};


  inline TCTmeasurementBase& operator = (const TCTmeasurementBase& other){
    _diodeName     = other._diodeName;
    _annealing     = other._annealing;
    _leakage       = other._leakage;
    _temperature   = other._temperature;
    _timeScanUnit  = other._timeScanUnit;
    _bias          = other._bias;
    return *this;
  }

  /* friend */
  TCTmeasurementBase(const TCTmeasurementBase &other);
  /*   this = base; */
  /* }; */
  
  /// multiple annealing cycles are allowed
  inline void SetAnnealing(float annealingTime, float annealingTemp){
    _annealing.push_back(std::make_pair<float, float>(annealingTime, annealingTemp));
  }; 

  /// acquisition temperature
  inline void SetTemperature(float temperature){ _temperature=temperature; };  
  ///leakage currente
  inline void SetLeakage(float leakage){ _leakage=leakage; };

  /// input voltage
  inline void SetBias(float bias){ _bias=bias;};
  
  /// time scan unit
  inline void SetTimeScanUnit(float timeScanUnit){ _timeScanUnit=timeScanUnit; };    

  ///one	    value	    of	    the	    waveform,	    they	    are	    supposed	    to	    be contiguous
  inline void AddMeasurement(float sample){
    std::cout << _nSamples << "\t" << sample << std::endl;
    assert(_nSamples<MAX_SAMPLES); // check that you have not reached the maximum number of samples
    _samples[_nSamples++]=sample; // add the new value and then increment the counter
  };

  inline std::string GetDiodeName(void) const{ return _diodeName;   };
  inline std::vector< std::pair<float, float> > GetAnnealing(void) const{ return _annealing;   };
  inline float GetTemperature(void)  const{ return _temperature; };
  inline float GetLeakage(void)      const{ return _leakage;     };
  inline float GetBias(void)         const{ return _bias;        };
  inline float GetTimeScanUnit(void) const{ return _timeScanUnit;};
  inline unsigned int GetN(void)     const{ return _nSamples;    };
  inline float* GetSamples(void)     { return _samples;     };
  inline const float* GetSamples(void)     const{ return _samples;     };
  
 private:
  std::string _diodeName;       ///< code of the diode

  std::vector< std::pair<float,float> > _annealing; ///< annealing: vector of (time, temperature) pairs
  float _timeScanUnit; ///< time for sampling
  unsigned int   _nSamples;     ///< number of samples
  float _samples[MAX_SAMPLES]; ///< samples of the waveform [V]
  float _temperature; ///< temperature
  float _leakage;     ///< leakage current
  float _bias;        ///< input voltage 
};

#endif
