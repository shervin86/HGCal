/// class containing CVmeasurement
#ifndef CVmeasurement_h
#define CVmeasurement_h 

#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include <TGraph.h>
#include <TMultiGraph.h>

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
      x[i]= GetBiases(iFreq)[i];
      //y[i]= 1/(GetCurrents(iFreq)[i]*GetCurrents(iFreq)[i]);
      y[i]= GetCurrents(iFreq)[i];
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
};


  
#endif
