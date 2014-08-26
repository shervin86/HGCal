/// basic info from measurement
#ifndef TCTspectrum_h
#define TCTspectrum_h


/** \class TCTspectrum TCTspectrum.h include/TCTspectrum.h
    This class provides the high level informations contained of TCT measurement from txt
    files. Contains also manipulation and advanced methods of the measurement
*/

#include "TCTspectrumBase.h"

#include <TGraph.h>

class TCTspectrum: public TCTspectrumBase{
 public:
 TCTspectrum(TCTspectrumBase base): TCTspectrumBase(base){};
  
 TCTspectrum(std::string diodeName): TCTspectrumBase(diodeName){
  };
  
  /// \todo fixit, assert should work
  TCTspectrum& operator -= (const TCTspectrum& other){
    unsigned int nSamples=GetN();
    //std::cout << other.GetN() << "\t" << nSamples << std::endl;
    //assert(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit());// other.GetTimeScanUnit());
    if(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit()){
      const float *samples = other.GetSamples();
      for(unsigned int i=0; i < nSamples; i++){
	GetSamples()[i]-=samples[i];
      }
      return *this;
    }else return *this;
  };

  TCTspectrum& operator += (const TCTspectrum& other){
    unsigned int nSamples=GetN();
    assert(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit());// other.GetTimeScanUnit());
    const float *samples = other.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]+=samples[i];
    }
    return *this;
  };

  TCTspectrum& operator /= (const float scale){
    unsigned int nSamples=GetN();
    //assert(nSamples == other.GetN() && GetTimeScanUnit() == 1);// other.GetTimeScanUnit());
    //const float *samples = other.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]/=scale;
    }
    return *this;
  };

  TCTspectrum& operator = (const float scale){
    unsigned int nSamples=GetN();
    //assert(nSamples == other.GetN() && GetTimeScanUnit() == 1);// other.GetTimeScanUnit());
    //const float *samples = other.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]=scale;
    }
    return *this;
  };

  void clear(void){
    *this = 0;
  };
  
  TGraph *GetWaveForm(std::string graphName="Graph", std::string graphTitle="graph") const;
  
  float GetWaveIntegral(float min, float max) const{
    assert(min<max);
    float integral=0.;
    unsigned int binMax=(unsigned int)(max/_timeScanUnit);
    //unsigned int binMin=(unsigned int)(min/_timeScanUnit);
    
    for(unsigned int i=(unsigned int)min/_timeScanUnit; i < binMax; i++){
      integral += _samples[i];
      //      std::cout << "integral = " << integral << "\t" << i << std::endl;
    }
    return integral;
  };
  
};

typedef std::vector<TCTspectrum> TCTspectrumCollection_t;


#endif

