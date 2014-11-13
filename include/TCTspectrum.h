/// Advanced class to manipulate TCT spectra
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
  /// \name Constructors
  ///@{

  /// default empty constructor
 TCTspectrum(): TCTspectrumBase("null"), _noisy(false){}; 
 TCTspectrum(std::string diodeName): TCTspectrumBase(diodeName), _noisy(false){} ///<default constructor
 TCTspectrum(TCTspectrumBase base): TCTspectrumBase(base), _noisy(false){} ///<copy constructor

  ///@}

  inline void SetNoisy(){ _noisy=true;};

  ///\name Operations on/between spectra
  ///@{

  /// return spectrum difference (all acquisition data are copied from the first spectrum)
  TCTspectrum operator- (const TCTspectrum& rhs) const;

  /// return spectrum with values = product of the two spectra (all acquisition data are copied from the first spectrum)
  TCTspectrum operator* (const TCTspectrum& rhs) const;

  /// used to subtract baseline
  TCTspectrum& operator -= (const TCTspectrum& other);

  /// used to make the average of spectra
  TCTspectrum& operator += (const TCTspectrum& other);

  /// used when calculating standard deviation: each value is v[i]=sqrt(v[i])
  TCTspectrum& Sqrt(void){
    unsigned int nSamples=this->GetN();
    
    float *samples_lhs = this->GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      samples_lhs[i]=sqrt(samples_lhs[i]);
    }
    return *this;
  };

  ///@}


  ///\name Operations with constant values, all samples are modified by the same value
  ///@{
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

  TCTspectrum& operator /= (const float scale){
    unsigned int nSamples=GetN();
    //assert(nSamples == other.GetN() && GetTimeScanUnit() == 1);// other.GetTimeScanUnit());
    //const float *samples = other.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]/=scale;
    }
    return *this;
  };


  /// assign the same value to all the samples, used to make it =0
  TCTspectrum& operator = (const float scale){
    unsigned int nSamples=GetN();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]=scale;
    }
    return *this;
  };

  ///@}

  //------------------------------
  ///\name Spectrum information 
  ///@{
  float *GetTimes(void)const; ///< return array with x axis of spectra (time [s])

  /// returns the average value in the time interval [start:stop]
  float GetMean(const float start, const float end) const{
    unsigned int nSamples=GetN(), jSamples=0;
    float mean=0;
    float t=start, dt=GetTimeScanUnit();
    const float *samples = GetSamples();
    for(unsigned int i=(unsigned int) (start/dt); i < nSamples && t<end; i++, t+=dt){
      mean+=samples[i];
      jSamples++; // count number of points
    }
    mean/=jSamples;
    return mean;
  }

  /// returns the number of points in a give time interval [start:end]
  unsigned int GetNsamples(float start, float end) const{
    unsigned int nSamples=GetN(), jSamples=0;
    float t=start, dt=GetTimeScanUnit();
    
    for(unsigned int i=(unsigned int) (start/dt); i < nSamples && t<end; i++, t+=dt){
      jSamples++;
    }
    return jSamples;
  }
    
  /// returns the standard deviation of the values in the time interval [start:end]
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

  /// reset the spectrum to 0 (conserve the number of samples), all values are 0
  void clear(void){*this = 0;};
  
  /// return a TGraph with the spectrum
  TGraph *GetWaveForm(std::string graphName="Graph", std::string graphTitle="graph") const;


  /// make the intregral between min and max [s] and subtract a constant value baselineMean from each sample
  float GetWaveIntegral(float min, float max, float baselineMean=0.) const;
  ///@}

  ///\name gnuplot
  ///@{
  
  /// dump spectrum on text file to be plotted with gnuplot
  inline void dump(std::ostream& f){
    float dt=GetTimeScanUnit();
    float t=0;
    f<< "time\tsample" << std::endl;
    for(unsigned int i=0; i< _nSamples; i++){
      f << t << "\t" << _samples[i] << "\n";
      t+=dt;
    }
   f<< std::endl;
  }

 private:
  bool _noisy;
  
};

//typedef std::vector<TCTspectrum> TCTspectrumCollection_t;
/// spectra ordered by bias voltage
typedef std::map< float, TCTspectrum > TCTspectrumCollection_t;


#endif

