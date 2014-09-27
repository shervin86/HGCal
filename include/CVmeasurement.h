/// class containing CVmeasurement
#ifndef CVmeasurement_h
#define CVmeasurement_h 

#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include <TGraph.h>
#include <TMultiGraph.h>
#include <TFitResult.h>
#include <TString.h>

#include <cmath>

/** \class CVmeasurement CVmeasurement.h include/CVmeasurement.h
* This class provides the high level informations
*/
#define MAX_ACQ 100
class CVmeasurement{
#define MAX_SAMPLES 500
#define MAX_FREQ 10
 public:
  Int_t fPaletteColor[100];

  /// base constructor
 CVmeasurement(): 
  fPaletteColor({0}),
  _nSamples(0){
    SetPaletteColor(NULL,0);
  }; 
  ~CVmeasurement(){
    //for(unsigned int i=0; i < _freq.size(); i++){
    //  delete _bias[i];
  };

  void SetPaletteColor(int *palette, unsigned int size){
    //assert(size>0);
    if(size==0){
      fPaletteColor[0]=50;
      size++;
    } else{
      if(size>MAX_ACQ){
	std::cerr << "[WARNING] Maximum number of colors shoud be < MAX_ACQ = " << MAX_ACQ << std::endl;
	std::cerr << "          Ignoring colors after MAX_ACQ" << std::endl;
	size=MAX_ACQ;
      }
      for(unsigned int i=0; i < size; i++){
	fPaletteColor[i]=palette[i];
      }
    }
    for(unsigned int i=size; i < MAX_ACQ; i++){
      fPaletteColor[i]=fPaletteColor[i-1]+1;
    }
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

  /// set the list of frequencies
  inline void SetFreq(std::vector<std::string>& freq){ _freq = freq; 
    for(unsigned int i=0; i < freq.size(); i++){
      float *b = new float[MAX_SAMPLES];
      float *c = new float[MAX_SAMPLES];
      float *g = new float[MAX_SAMPLES];
      _bias.push_back(b);
      _current.push_back(c);
      _guardCurrent.push_back(g);
      _Vdep_vec.push_back(0.);
      _Cend_vec.push_back(0.);
      _CendError_vec.push_back(0.);
      
    }
  };
 
  ///one	    value	    of	    the	    waveform,	    they	    are	    supposed	    to	    be contiguous
  inline void AddMeasurement(float bias, float current, float guardCurrent, unsigned int iFreq){
    //std::cout << bias << "\t" << current << "\t" << guardCurrent << std::endl;
    assert(_nSamples<MAX_SAMPLES); // check that you have not reached the maximum number of samples
    
    _bias[iFreq][_nSamples]=bias; // add the new value and then increment the counter
    _current[iFreq][_nSamples]=current; // add the new value and then increment the counter
    _guardCurrent[iFreq][_nSamples]=guardCurrent; // add the new value and then increment the counter
    //std::cout << "* " << _bias[iFreq][_nSamples] << "\t" << _current[iFreq][_nSamples] << "\t" << std::endl;
    if(iFreq==_freq.size()-1) _nSamples++; // increment only once
  };


  inline std::string GetDiodeName(void) const{ return _diodeName;   };
  inline std::string GetTime(void)      const{ return _time;        };
  inline std::vector< std::pair<float, float> > GetAnnealing(void) const{ return _annealing;   };
  inline float GetTemperature(void)  const{ return _temperature; };
  //inline float GetLeakage(void)      const{ return _leakage;     };

/// number of measurements for frequency iFreq
  inline unsigned int GetN(unsigned int iFreq)     const{ return _nSamples;    }; 

  ///number of frequencies
  inline unsigned int GetNFreq(void) const{ return _freq.size();};
  inline float* GetBiases(unsigned int iFreq)     { return _bias[iFreq];     };
  inline float* GetCurrents(unsigned int iFreq)     { return _current[iFreq];     };
  inline float* GetGuardCurrent(unsigned int iFreq)     { return _guardCurrent[iFreq];     };
   inline const float* GetBiases(unsigned int iFreq)  const   { return _bias[iFreq];     };
  inline const float* GetCurrents(unsigned int iFreq) const    { return _current[iFreq];     };
  inline const float* GetGuardCurrent(unsigned int iFreq)  const   { return _guardCurrent[iFreq];     };
  inline const std::vector<std::string>& GetFrequencies(void) const{ return _freq; };
  inline TGraph *GetCvsV(unsigned int iFreq) const{
    Float_t x[MAX_SAMPLES], y[MAX_SAMPLES];
    for(unsigned int i=0; i < GetN(iFreq); i++){
      x[i]= fabs(GetBiases(iFreq)[i]);
      y[i]= 1/(GetCurrents(iFreq)[i]*GetCurrents(iFreq)[i]);
      //y[i]= GetCurrents(iFreq)[i];
      //std::cout << x[i] << "\t" << y[i] << std::endl;
    }

    TGraph *g = new TGraph(GetN(iFreq), x, y);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1);
    //g->SetLineColor(fPaletteColor[i]);
    g->SetFillStyle(0);
    //g->SetMarkerColor(gg->GetLineColor());
     
    return g;
  }
  
  inline TMultiGraph* GetCvsV(std::string graphName, std::string graphTitle) const{
    TMultiGraph *graph = new TMultiGraph();
    graph->SetName(graphName.c_str());
    for(unsigned int i=0; i < _freq.size(); i++){
      TGraph *gg = GetCvsV(i);
      gg->SetTitle(_freq[i].c_str());
      gg->SetLineColor(fPaletteColor[i]);
      gg->SetFillStyle(0);
      gg->SetMarkerColor(gg->GetLineColor());
      //std::cout << fPaletteColor[i] << "\t" << kBlack << std::endl;
      gg->SetLineStyle(1);
      graph->Add(gg, "l");
    }
    return graph;
  }


    
  void FindVdep(bool clean){
    for(unsigned int i = 0; i < _freq.size(); i++){
      FindVdep(GetCvsV(i),i);
    }
  };

  void FindVdep(TGraph *g, unsigned int iFreq){
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
    _Vdep_vec[iFreq]=x[min];
    double fff=p->GetParams()[0];
    _Cend_vec[iFreq]=1./sqrt(fff);
    _CendError_vec[iFreq]=0.5/sqrt(fff*fff*fff)*p->GetErrors()[0];   
  }

  unsigned int MaxFreqIndex(void){
    float fmax=0;
    unsigned int imax=0;

    for(unsigned int i = 0; i < _freq.size(); i++){
      TString s=_freq[i].c_str();
      s.ReplaceAll("Hz","");
      s.ReplaceAll(" ","");
      s.ReplaceAll("M","e6");
      s.ReplaceAll("k","e3");
      float f = s.Atof();
      if(f>fmax){
	fmax=f;
	imax=i;
      }
    }
    return imax;
  }

  unsigned int RefFreqIndex(void){
    float fmax=0;
    unsigned int imax=0;

    for(unsigned int i = 0; i < _freq.size(); i++){
  
      TString s=_freq[i].c_str();
      s.ReplaceAll("Hz","");
      s.ReplaceAll(" ","");
      s.ReplaceAll("M","e6");
      s.ReplaceAll("k","e3");
      float f = s.Atof();
      if(f==455) return i;
    }
    std::cerr << "[ERROR] Reference frequence 455 Hz not found!" << std::endl;
    return 0;
  }

  float GetVdep(int iFreq=-1){ if(iFreq<0) iFreq=RefFreqIndex(); return _Vdep_vec[iFreq];};
  float GetCend(int iFreq=-1){ if(iFreq<0) iFreq=MaxFreqIndex(); return _Cend_vec[iFreq];};
  float GetCendError(int iFreq=-1){ if(iFreq<0) iFreq=MaxFreqIndex(); return _CendError_vec[iFreq];};

 protected:
  std::string _diodeName;       ///< code of the diode
  std::string _time;            ///< date of acquisition
  std::vector< std::pair<float,float> > _annealing; ///< annealing: vector of (time, temperature) pairs
  unsigned int   _nSamples;     ///< number of samples
  std::vector<float*> _bias; ///< 
  std::vector<float*> _current; ///< 
  std::vector<float*> _guardCurrent; ///< 
  
  float _temperature; ///< temperature
  std::vector<std::string> _freq;

  float _Vdep;             ///< depletion voltage
  float _Cend, _CendError; ///< end capacitance and uncertainty
  std::vector<float> _Vdep_vec, _Cend_vec, _CendError_vec;
};


  
#endif
