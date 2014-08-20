/// basic info from measurement
#ifndef TCTmeasurement_h
#define TCTmeasurement_h


/** \class TCTmeasurement TCTmeasurement.h include/TCTmeasurement.h
    This class provides the high level informations contained of TCT measurement from txt
    files. Contains also manipulation and advanced methods of the measurement
*/

#include "TCTmeasurementBase.h"

#include <TGraph.h>

class TCTmeasurement: public TCTmeasurementBase{
 public:
 TCTmeasurement(TCTmeasurementBase base): TCTmeasurementBase(base){};
  
 TCTmeasurement(std::string diodeName): TCTmeasurementBase(diodeName){
  };
  

  TCTmeasurement& operator -= (const TCTmeasurement& other){
    unsigned int nSamples=GetN();
    assert(nSamples == other.GetN() && GetTimeScanUnit() == 1);// other.GetTimeScanUnit());
    const float *samples = other.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]-=samples[i];
    }
    return *this;
  };

  TGraph GetWaveForm(std::string graphName="Graph") const;

  float GetWaveIntegral(float min, float max) const{
    assert(min<max);
    float integral=0.;
    unsigned int binMax=(unsigned int)(max/_timeScanUnit);
    unsigned int binMin=(unsigned int)(min/_timeScanUnit);

    for(unsigned int i=(unsigned int)min/_timeScanUnit; i < binMax; i++){
      integral += _samples[i];
      //      std::cout << "integral = " << integral << "\t" << i << std::endl;
    }
    return integral;
  };

};

typedef std::vector<TCTmeasurement> TCTmeasurementCollection_t;


#endif

