/// class containing IVmeasurement
#ifndef IVmeasurement_h
#define IVmeasurement_h 

#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include <TGraph.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>

/** \class IVmeasurement IVmeasurement.h include/IVmeasurement.h
* This class provides the high level informations
*/

class IVmeasurement{
#define MAX_SAMPLES 500

 public:
  /// base constructor
 IVmeasurement(): _nSamples(0), _IVgraph(NULL){}; 


  /// DiodeName
  inline void SetDiodeName(std::string name){ _diodeName = name;};

  /// Acquiition time
  inline void SetTime(std::string time){ _time = time; };
   
  /// multiple annealing cycles are allowed
  inline void SetAnnealing(float annealingTime, float annealingTemp){
    _annealing.push_back(std::make_pair<float, float>(annealingTime, annealingTemp));
  }; 

  /// acquisition temperature
  inline void SetTemperature(float temperature){ _temperature=temperature; };  

  ///one	    value	    of	    the	    waveform,	    they	    are	    supposed	    to	    be contiguous
  inline void AddMeasurement(float bias, float current, float guardCurrent){
    //std::cout << bias << "\t" << current << "\t" << guardCurrent << std::endl;
    assert(_nSamples<MAX_SAMPLES); // check that you have not reached the maximum number of samples
    _bias[_nSamples]=bias; // add the new value and then increment the counter
    _current[_nSamples]=current; // add the new value and then increment the counter
    _guardCurrent[_nSamples]=guardCurrent; // add the new value and then increment the counter
    //std::cout << "* " << _bias[_nSamples] << "\t" << _current[_nSamples] << "\t" << std::endl;
    _nSamples++;
  };


  inline std::string GetDiodeName(void) const{ return _diodeName;   };
  inline std::string GetTime(void)      const{ return _time;        };
  inline std::vector< std::pair<float, float> > GetAnnealing(void) const{ return _annealing;   };
  inline float GetTemperature(void)  const{ return _temperature; };

  //inline float GetLeakage(void)      const{ return _leakage;     };
  inline unsigned int GetN(void)     const{ return _nSamples;    };
  inline float* GetBiases(void)     { return _bias;     };
  inline float* GetCurrents(void)     { return _current;     };
  inline float* GetGuardCurrent(void)     { return _guardCurrent;     };
   inline const float* GetBiases(void)  const   { return _bias;     };
  inline const float* GetCurrents(void) const    { return _current;     };
  inline const float* GetGuardCurrent(void)  const   { return _guardCurrent;     };
  
  inline TGraph *GetIvsV(bool absI=false, bool absV=false){
    if(_IVgraph!=NULL) delete _IVgraph; //return _IVgraph;

    Float_t x[MAX_SAMPLES], y[MAX_SAMPLES];
    if(absI){
      for(unsigned int i=0; i < GetN(); i++){
	y[i]= fabs(GetCurrents()[i]);
      }
    }else{
      for(unsigned int i=0; i < GetN(); i++){
	y[i]= GetCurrents()[i];
      }
    }

    if(absV){
      for(unsigned int i=0; i < GetN(); i++){
	x[i]= fabs(GetBiases()[i]);
      }
    } else{
      for(unsigned int i=0; i < GetN(); i++){
	x[i]= GetBiases()[i];
      }
    }
    _IVgraph = new TGraph(GetN(), x, y);
    _IVgraph->SetMarkerStyle(20);
    _IVgraph->SetMarkerSize(1);
    FindVdep(_IVgraph);
    return _IVgraph;
  }

  void FindVdep(TGraph *g){
    Double_t *x = g->GetX();
    unsigned int n = g->GetN();
    int min=0;
    int max=n-1;
      

    TFitResultPtr p = g->Fit("pol1","QS");
    TF1 *f = g->GetFunction("pol1");
    //std::cout << fabs(p->GetParams()[1]) << "\t" << p->GetErrors()[1] << "\t" << (fabs(p->GetParams()[1]) > p->GetErrors()[1]) << true << std::endl;
    while(fabs(p->GetParams()[1])>p->GetErrors()[1] && min<max){
      //std::cout << "ciao" << std::endl;
      if(x[max]>x[min]) p = g->Fit(f,"QS","",x[min], x[max]);
      else p = g->Fit(f,"QS","",x[max], x[min]);
      min++;
      //std::cout << x[min] << "\t" << x[max] << std::endl;
      //p->Print();
    }
    //p->Print();
    _Vdep=x[min];
    _Irev=p->GetParams()[0];
    _IrevError=p->GetErrors()[0];
    
  }

  inline float GetVdep(void) const{ return _Vdep; }
  inline float GetIrev(void) const{ return _Irev; };
  inline float GetIrevError(void) const{ return _IrevError; };

  inline TGraph *GetCvsV(void) const{
    Float_t x[MAX_SAMPLES], y[MAX_SAMPLES];
    for(unsigned int i=0; i < GetN(); i++){
      x[i]= GetBiases()[i];
      y[i]= 1/(GetCurrents()[i]*GetCurrents()[i]);
    }
    TGraph *g = new TGraph(GetN(), x, y);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1);
    return g;
  }

 protected:
  std::string _diodeName;       ///< code of the diode
  std::string _time;            ///< date of acquisition
  std::vector< std::pair<float,float> > _annealing; ///< annealing: vector of (time, temperature) pairs
  unsigned int   _nSamples;     ///< number of samples
  float _bias[MAX_SAMPLES]; ///< 
  float _current[MAX_SAMPLES]; ///< 
  float _guardCurrent[MAX_SAMPLES]; ///< 

  float _temperature; ///< temperature

  TGraph *_IVgraph;        ///< IV plot
  float _Vdep;             ///< depletion voltage
  float _Irev, _IrevError; ///< leakage current and uncertainty
};


  
#endif
