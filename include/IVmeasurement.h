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
    

  //copy constructor
  IVmeasurement(const IVmeasurement& other){
    _diodeName=other._diodeName;
    _time=other._time;
    _annealing=other._annealing;
    
    _nSamples=other._nSamples;
    _IVgraph= NULL;

    for(unsigned int i=0; i<_nSamples; i++){
      _bias[i]=other._bias[i];
      _current[i]=other._current[i];
      _guardCurrent[i]=other._guardCurrent[i];
    }
  
    __current.insert(other.__current.begin(), other.__current.end());
    __guardCurrent.insert(other.__guardCurrent.begin(), other.__guardCurrent.end());
    _temperature=other._temperature;
    _Vdep=other._Vdep;
    _Irev=other._Irev;
    _IrevError=other._IrevError;
  }

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
    float fbias=fabs(bias);
    __current[fbias]=current;
    __guardCurrent[fbias]=guardCurrent;
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
  
  inline TGraph *GetIvsV(bool clean=true, bool absI=true, bool absV=true){
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
    unsigned int n=GetN();
    if(clean) RemoveWrongValues(n,x,y);

    _IVgraph = new TGraph(n, x, y);
    _IVgraph->SetMarkerStyle(20);
    _IVgraph->SetMarkerSize(1);
    FindVdep(_IVgraph);
    return _IVgraph;
  }

  void RemoveWrongValues(unsigned int& n, Float_t *x, Float_t *y){
    Float_t y2[n],x2[n];
    unsigned int j=0;
    for(unsigned int i=0; i< n; i++){
      y2[j]=y[i];
      x2[j]=x[i];
      if(i<11){
	j++;
	continue;
      }
      float diff1=fabs(y2[j]-y2[j-1]);
      float diff2=fabs(y2[j-1]-y2[j-2]);
      if((diff1<diff2*10 && diff1>diff2/10. )&& y2[j]>1e-9) j++; // accept diff1<1e-7 in case diff2~0
    }

    for(unsigned int i=0; i< j; i++){
      y[i]=y2[i];
      x[i]=x2[i];
    }
    n=j;
  }
  /*   for(auto iter = __current.begin(); */
  /* 	iter!= __current.end(); iter++){ */
  /*     if(iter->first<2) continue; */
     
  /*     auto iter2= iter, iter3=iter; */
  /*     std::advance(iter2,-1); */
  /*     std::advance(iter3,-2); */
  /*     float diff23 = fabs(iter2->second-iter3->second); */
  /*     float diff12 = fabs(iter->second-iter2->second); */
  /*     if(diff12>diff23*10){ */
  /* 	std::cout << "Discard value: " << iter->first << "\t" << iter3->second << " - " << iter2->second << " - " << iter->second << std::endl; */
  /*     } */
	
  /*   } */
  /* } */
    
  void FindVdep(bool clean){
    FindVdep(GetIvsV(clean, true, true));
  };

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

    ///\name iterators 
  /// currents are ordered by bias voltage applied (absolute value) \n
  ///  \code auto itr=begin(); 
  /// float fbias=itr->first; 
  /// float current = itr->second; \endcode
  /// @{
  typedef std::map<float, float> valueMap_t;
  typedef valueMap_t::const_iterator  const_iterator;
  typedef valueMap_t::iterator        iterator;

  const_iterator begin()const{return __current.begin();}; 
  const_iterator end()const{  return __current.end();  };
  iterator       begin(){     return __current.begin();};
  iterator       end(){       return __current.end();  };

  const_iterator beginGR()const{return __guardCurrent.begin();}; 
  const_iterator endGR()const{  return __guardCurrent.end();  };
  iterator       beginGR(){     return __guardCurrent.begin();};
  iterator       endGR(){       return __guardCurrent.end();  };

  /// return iterator to the spectrum with given bias voltage (absolute value), or to end()
  const_iterator operator[](float bias) const{
    float fbias=fabs(bias);
    auto iter = (__current.upper_bound(fbias));
    if(iter==begin()) return end();
    iter--;
    if(fabs(iter->first-fbias)<1e-2) return iter;
    return end();
  }
  iterator operator[](float bias){
    float fbias=fabs(bias);
    auto iter = (__current.upper_bound(fbias));
    if(iter==begin()) return end();
    iter--;
    if(fabs(iter->first-fbias)<1e-2) return iter;
    return end();
  }

  const_iterator find(float bias, bool guardRing)const{
    float fbias=fabs(bias);
    auto iter = guardRing ? (__guardCurrent.upper_bound(fbias)) : (__current.upper_bound(fbias));
    if(iter==begin()) return end();
    iter--;
    if(fabs(iter->first-fbias)<1e-2) return iter;
    return end();
  }
  iterator find(float bias, bool guardRing){
    float fbias=fabs(bias);
    auto iter = guardRing ? (__guardCurrent.upper_bound(fbias)) : (__current.upper_bound(fbias));
    if(iter==begin()) return end();
    iter--;
    if(fabs(iter->first-fbias)<1e-2) return iter;
    return end();
  }

  ///@}
    
 protected:
  std::string _diodeName;       ///< code of the diode
  std::string _time;            ///< date of acquisition
  std::vector< std::pair<float,float> > _annealing; ///< annealing: vector of (time, temperature) pairs
  unsigned int   _nSamples;     ///< number of samples
  float _bias[MAX_SAMPLES]; ///< original measurement values
  float _current[MAX_SAMPLES]; ///< original measurement values
  float _guardCurrent[MAX_SAMPLES]; ///< original measurement values

  valueMap_t __current;
  valueMap_t __guardCurrent;

  float _temperature; ///< temperature

  TGraph *_IVgraph;        ///< IV plot
  float _Vdep;             ///< depletion voltage
  float _Irev, _IrevError; ///< leakage current and uncertainty

};


  
#endif
