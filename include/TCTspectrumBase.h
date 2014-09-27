/// basic info from measurement
#ifndef TCTspectrumBase_h
#define TCTspectrumBase_h

#include <string>
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>
/** \class TCTspectrumBase TCTspectrumBase.h include/TCTspectrumBase.h
    This class provides the basic informations contained in the txt
    files. Operations on the measurement and more advanced operations
    are implemented in TCTspectrum class

    \todo 
      - add the type of irradiation
      - add the illumination on front/back
      - add the type of irradiation: unirradiate, neutron, other
      - add the type of illumination: red laser, IR laser, beta, alfa
      - remove assert in favor of exceptions
*/

#define MAX_SAMPLES 1005
class TCTspectrumBase{
 public:
  /// default constructor
 TCTspectrumBase(std::string& diodeName): 
  _diodeName(diodeName),
  _nSamples(0)
    {};

 TCTspectrumBase(const char *diodeName):
  _diodeName(diodeName),
  _nSamples(0)
    {};

  /// copy constructor
  TCTspectrumBase(TCTspectrumBase const& other){
    *this = other;
    _diodeName     = other._diodeName;
    _time          = other._time;
    _annealing     = other._annealing;
    _leakage       = other._leakage;
    _temperature   = other._temperature;
    _timeScanUnit  = other._timeScanUnit;
    _bias          = other._bias;
    _nSamples      = other._nSamples;
    for(unsigned int i=0; i < _nSamples; i++){
      _samples[i] = other._samples[i];
    }
  }
    
  ~TCTspectrumBase(){}

  /// copy content from other measurement
  inline TCTspectrumBase& operator = (const TCTspectrumBase& other){
    _diodeName     = other._diodeName;
    _time          = other._time;
    _annealing     = other._annealing;
    _leakage       = other._leakage;
    _temperature   = other._temperature;
    _timeScanUnit  = other._timeScanUnit;
    _bias          = other._bias;
    _nSamples      = other._nSamples;
    for(unsigned int i=0; i < _nSamples; i++){
      _samples[i] = other._samples[i];
    }
    return *this;
  }

  /* std::ostream& operator<<(std::ostream& f, const TCTspectrumBase& b){ */
  /*   f <<  "\t" << b._diodeName << "\t" << b._bias << "\t" << b._time; */
  /*     //<< std::endl; */
  /*   return (f); */
  /* } */

  /// Acquiition time
  inline void SetTime(std::string time){ _time = time; };
   
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
    //   std::cout << _nSamples << "\t" << sample << std::endl;
    assert(_nSamples<MAX_SAMPLES); // check that you have not reached the maximum number of samples
    _samples[_nSamples++]=sample; // add the new value and then increment the counter
  };

  inline void AddMeasurement(float time, float sample){assert(false);}; ///not implemented
  inline std::string GetDiodeName(void) const{ return _diodeName;   };
  inline std::string GetTime(void)      const{ return _time;        };
  inline std::vector< std::pair<float, float> > GetAnnealing(void) const{ return _annealing;   };
  inline float GetTemperature(void)  const{ return _temperature; };
  inline float GetLeakage(void)      const{ return _leakage;     };
  inline float GetBias(void)         const{ return _bias;        };
  inline float GetTimeScanUnit(void) const{ return _timeScanUnit;};
  inline unsigned int GetN(void)     const{ return _nSamples;    };
  inline float* GetSamples(void)     { return _samples;     };
  inline const float* GetSamples(void)     const{ return _samples;     };
  inline bool empty(void) const { return (_nSamples==0);};

 protected:
  std::string _diodeName;       ///< code of the diode
  std::string _time;            ///< date of acquisition
  std::vector< std::pair<float,float> > _annealing; ///< annealing: vector of (time, temperature) pairs
  float _timeScanUnit; ///< time for sampling
  unsigned int   _nSamples;     ///< number of samples
  float _samples[MAX_SAMPLES]; ///< samples of the waveform [V]
  float _temperature; ///< temperature
  float _leakage;     ///< leakage current
  float _bias;        ///< input voltage 
};

#endif
