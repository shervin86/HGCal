/// basic info from measurement
#ifndef TCTspectrum_h
#define TCTspectrum_h


/** \class TCTspectrum TCTspectrum.h include/TCTspectrum.h
    This class provides the high level informations contained of TCT measurement from txt
    files. Contains also manipulation and advanced methods of the measurement
*/

#include "TCTspectrumBase.h"
#include <cmath>
#include <TGraph.h>
#include <map>

class TCTspectrum: public TCTspectrumBase{
 public:
 TCTspectrum(TCTspectrumBase base): TCTspectrumBase(base), _noisy(false){};
  
 TCTspectrum(std::string diodeName): TCTspectrumBase(diodeName), _noisy(false){};
 TCTspectrum(): TCTspectrumBase("null"), _noisy(false){};

  //
  inline bool isnull() const{ return empty();};
  inline bool null() const{   return empty();};

  inline void SetNoisy(){ _noisy=true;};

  //  TCTspectrum& operator* (const TCTspectrum& lhs, const TCTspectrum& rhs){
  TCTspectrum operator- (const TCTspectrum& rhs) const{
    TCTspectrum newSpectrum(*this);
    newSpectrum.clear();
    const TCTspectrum& lhs = *this;
    unsigned int nSamples=lhs.GetN();
    assert(nSamples==rhs.GetN());

    //if(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit()){
    const float *samples_rhs = rhs.GetSamples();
    const float *samples_lhs = lhs.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      newSpectrum.GetSamples()[i]=samples_lhs[i]-samples_rhs[i];;
    }
    return newSpectrum;
  };

  //  TCTspectrum& operator* (const TCTspectrum& lhs, const TCTspectrum& rhs){
  TCTspectrum operator* (const TCTspectrum& rhs) const{
    TCTspectrum newSpectrum(*this);
    newSpectrum.clear();
    const TCTspectrum& lhs = *this;
    unsigned int nSamples=lhs.GetN();
    assert(nSamples==rhs.GetN());

    //if(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit()){
    const float *samples_rhs = rhs.GetSamples();
    const float *samples_lhs = lhs.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      newSpectrum.GetSamples()[i]=samples_lhs[i]*samples_rhs[i];
      //      std::cout << samples_rhs[i] << "\t" << samples_lhs[i] << "\t" << newSpectrum.GetSamples()[i] << std::endl; 
    }
    return newSpectrum;
  };

  TCTspectrum& Sqrt(void){
    unsigned int nSamples=this->GetN();
    
    float *samples_lhs = this->GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      samples_lhs[i]=sqrt(samples_lhs[i]);
    }
    return *this;
  };
    

  TCTspectrum operator/ (double value) const{
    TCTspectrum newSpectrum(*this);
    const TCTspectrum& lhs = *this;
    unsigned int nSamples=lhs.GetN();
    
    const float *samples_lhs = lhs.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      newSpectrum.GetSamples()[i]=samples_lhs[i]/value;
    }
    return newSpectrum;
  };
  
  TCTspectrum& operator-= (double value){
    //TCTspectrum newSpectrum(*this);
    const TCTspectrum& lhs = *this;
    unsigned int nSamples=lhs.GetN();
    
    const float *samples_lhs = lhs.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]=samples_lhs[i]-value;
    }
    return *this;
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
    if(nSamples!=other.GetN()){
      std::cout << "nSamples= "<<nSamples << "\t" << other.GetN() << "\t" << GetDiodeName() << std::endl;
    }
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

  float GetMean(float start, float end) const{
    unsigned int nSamples=GetN(), jSamples=0;
    float mean=0;
    //assert(nSamples == other.GetN() && GetTimeScanUnit() == 1);// other.GetTimeScanUnit());
    //const float *samples = other.GetSamples();
    float t=start;
    float dt=GetTimeScanUnit();
    
    for(unsigned int i=(unsigned int) (start/dt); i < nSamples && t<end; i++, t+=dt){
      mean+=GetSamples()[i];
      jSamples++;
    }
    mean/=jSamples;
    return mean;
  }

  unsigned int GetNsamples(float start, float end) const{
    unsigned int nSamples=GetN(), jSamples=0;
    float t=start;
    float dt=GetTimeScanUnit();
    
    for(unsigned int i=(unsigned int) (start/dt); i < nSamples && t<end; i++, t+=dt){
      jSamples++;
    }
    return jSamples;
  }
    
  float GetRMS(float start, float end) const{
    unsigned int nSamples=GetN();
    float sum=0., sum2=0.;
    //assert(nSamples == other.GetN() && GetTimeScanUnit() == 1);// other.GetTimeScanUnit());
    //const float *samples = other.GetSamples();
    float dt=GetTimeScanUnit();
    float t=0.;
    unsigned int jSamples=0;
    for(unsigned int i=(unsigned int)(start/dt); i < nSamples && t<end; i++, t+=dt){
      float v=GetSamples()[i];
      jSamples++;
      sum+=v;
      sum2+=v*v;
    }
    float mean= sum/jSamples;
    return sqrt(sum2/jSamples -mean*mean);
  }

  /// reset the spectrum to 0 (conserve the number of samples)
  void clear(void){
    *this = 0;
  };
  
  TGraph *GetWaveForm(std::string graphName="Graph", std::string graphTitle="graph") const;
  float *GetTimes(void)const;

  float GetWaveIntegral(float min, float max, float baselineMean=0.) const{
    if(GetN()<=0){
      std::cerr << "[ERROR] No points for this spectrum: " << GetDiodeName() << "\t" << GetBias() << std::endl;
      assert(GetN()>0);
    }
    assert(min<max);
    assert(min<_timeScanUnit*GetN());
    
    float integral=0.;
    unsigned int binMax=(unsigned int)(max/_timeScanUnit);
    //std::cout << binMax << "\t" << GetN() << "\t" << _timeScanUnit << "\t" << max << std::endl;
    if(binMax>GetN()) binMax=GetN();

 //unsigned int binMin=(unsigned int)(min/_timeScanUnit);
    
    for(unsigned int i=(unsigned int)(min/_timeScanUnit); i < binMax; i++){
      //std::cout << "integral = " << integral << "\t" << i << "\t" << _samples[i] << std::endl;
      integral += _samples[i]-baselineMean;
    }
    return integral*_timeScanUnit;
  };
 private:
  bool _noisy;
  
};

//typedef std::vector<TCTspectrum> TCTspectrumCollection_t;
/// spectra ordered by bias voltage
typedef std::map< float, TCTspectrum > TCTspectrumCollection_t;


#endif

