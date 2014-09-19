#ifndef TCTmeasurements_h
#define TCTmeasurements_h


#include "TCTspectrum.h"
#include "TCTimport.h"
#include <TGraphErrors.h>
#include <TMultiGraph.h>

#include <cstdlib>
#include <map>

/// Assembly the information from several measurements to get spectra, QvsV, CCEvsV, IvsV,CvsV graphs

/// maximum number of acquisitions in one scan
#define MAX_ACQ 100

/** \class TCTmeasurements TCTmeasurements.h include/TCTmeasurements.h
 */
class TCTmeasurements{
  typedef   std::multimap<float, unsigned int> biasMap_t;

  int fPaletteColor[MAX_ACQ];
  biasMap_t acqAbsBiasIndex;
 public:

  /// constructor for new measurement produced by manipulation of other measurements
 TCTmeasurements():
  fPaletteColor({0}),
  _checkBias(false), _reference(NULL){};

  /* TCTmeasurements(std::string filename, TCTspectrum reference, TCTspectrum baseline){ */
  /*   acquisition = importer.ImportFromFile(filename); */
  /* } */

 TCTmeasurements(std::string filename, float temp): 
  _checkBias(false),
    _reference(NULL){
    acquisition = importer.ImportFromFile(filename);
 
    for(unsigned int i=0; i < size(); i++){
      //acqIndex.insert(std::make_pair<float, unsigned int>(acquisition[i].GetBias(), i));
      acqAbsBiasIndex.insert(std::make_pair<float, unsigned int>(fabs(acquisition[i].GetBias()), i));
      acquisition[i].SetTemperature(temp); 
    }
  }

  ~TCTmeasurements(){};

  /// put the average into acquisition[0] and the std. dev. into acquisition[1]
  void Average(std::vector<TCTmeasurements> others, bool checkBias=false); ///< average over several measurements
  TGraph *GetWaveForm(unsigned int index, std::string graphName, std::string graphTitle)const;
  TMultiGraph *GetAllSpectra(std::string graphName, std::string graphTitle)const; ///< plot all the spectra 
  TGraphErrors *GetAverageWaveForm(std::string graphName, std::string graphTitle) const; ///< Graph with average of the spectra, error=std.dev.
  void SetPaletteColor(int *palette, unsigned int size){
    assert(size>0);
    if(size>MAX_ACQ){
      std::cerr << "[WARNING] Maximum number of colors shoud be < MAX_ACQ = " << MAX_ACQ << std::endl;
      std::cerr << "          Ignoring colors after MAX_ACQ" << std::endl;
      size=MAX_ACQ;
    }
    for(unsigned int i=0; i < size; i++){
      fPaletteColor[i]=palette[i];
    }
    for(unsigned int i=size; i < MAX_ACQ; i++){
      fPaletteColor[i]=fPaletteColor[i-1]+1;
    }
  }

  size_t size(void) const{ return acquisition.size();}; ///< number of spectra acquired with this measurement (if from acquisition)

  

  /* // subtraction of waveforms from other acquisition (mainly for baseline subtraction) */
  /* TCTmeasurements& operator-=(const TCTmeasurements& other){ */
  /*   unsigned int size=acquisition.size(); */
  /*   for(unsigned int i=0; i < size; i++){ */
  /*     acquisition[i]-=other.acquisition[i]; */
  /*   } */
  /*   return *this; */
  /* }; */

  /// Return the average spectrum
  TCTspectrum& GetAverageMeasurement(){
    assert(_isAverage);
    return acquisition[0];
  }

  /// return one spectrum
  TCTspectrum& GetSpectrum(unsigned int i){
    assert(i<acquisition.size());
    return acquisition[i];
  }

  /// return one spectrum
  const TCTspectrum& GetSpectrum(unsigned int i) const{
    assert(i<acquisition.size());
    return acquisition[i];
  }


  const TCTspectrum& GetSpectrumWithBias(float bias) const{
    biasMap_t::const_iterator it = acqAbsBiasIndex.begin(); 
    //    std::cout << bias << "\t" << it->first << "\t" << it->second << std::endl;
    if(fabs(it->first -0) < 1e-3){ // if compatible with 0, this means that the order is -1000, -700, 0;
      it = --(acqAbsBiasIndex.upper_bound(bias));
    }  else {
      it = ++(acqAbsBiasIndex.lower_bound(bias));
    }
    
    return acquisition[it->second];
  }


  
  const TCTspectrum &GetSpectrumWithAbsBias(float bias) const{
    biasMap_t::const_iterator it = acqAbsBiasIndex.begin(); 
    //std::cout << bias << "\t" << it->first << "\t" << it->second << std::endl;
    if(fabs(it->first -0) < 1e-3){ // if compatible with 0, this means that the order is -1000, -700, 0;
      it = --(acqAbsBiasIndex.upper_bound(bias));
    }  else {
      it = ++(acqAbsBiasIndex.lower_bound(bias));
    }
    
    return acquisition[it->second];
  }

  unsigned int GetSpectrumIndexWithAbsBias(float bias) const{
    biasMap_t::const_iterator it = acqAbsBiasIndex.begin(); 
    //std::cout << bias << "\t" << it->first << "\t" << it->second << std::endl;
    if(fabs(it->first -0) < 1e-3){ // if compatible with 0, this means that the order is -1000, -700, 0;
      it = --(acqAbsBiasIndex.upper_bound(bias));
    }  else {
      it = ++(acqAbsBiasIndex.lower_bound(bias));
    }
    
    return it->second;
  }    
      
  /// return the spectrum with given bias voltage (absolute value), or empty spectrum
  const TCTspectrum& operator[](float bias) const{
    unsigned int index=GetSpectrumIndexWithAbsBias(bias);
    if(fabs(acquisition[index].GetBias())-fabs(bias) > 1e-2) return *(new TCTspectrum(acquisition[0].GetDiodeName())); 
    return GetSpectrum(index);
  }

  /// return the spectrum with given bias voltage (absolute value), or empty spectrum
  TCTspectrum& operator[](float bias) {
    unsigned int index=GetSpectrumIndexWithAbsBias(bias);
    if(fabs(acquisition[index].GetBias())-fabs(bias) > 1e-2) return *(new TCTspectrum(acquisition[0].GetDiodeName())); 
    return GetSpectrum(index);
  }

  TCTmeasurements& operator-=(const TCTspectrum& other){
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      //   std::cout << "--> " << acquisition[i].GetN() << "\t" << other.GetN()<< std::endl;
      acquisition[i]-=other;
    }
    return *this;
  };


  /// method to get a graph of the Q vs V given min and max range in time for integration
  TGraph *GetQvsV(float min, float max){
    float Q[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
    
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      Q[i] = acquisition[i].GetWaveIntegral(min, max);
      V[i] = abs(acquisition[i].GetBias());
    }
    return new TGraph(size-1, V,Q);  /// \todo check why size-1
  }

  TGraph *GetCCEvsV(float min, float max){
    return GetCCEvsV(*_reference, min, max);
  }

  inline float GetCCE(float min, float max, float bias){
    float fbias=fabs(bias);
    unsigned int acqIndex = 0;
    bool found=false;
    for(acqIndex=0; acqIndex< acquisition.size(); acqIndex++){
      float bias_=fabs(acquisition[acqIndex].GetBias());
      if(fabs(bias_-fbias)<1e-3){
	found=true;
	break;
      }//else std::cout << bias_ << "\t" << fbias << std::endl;
    }
    assert(found);
    float Q = acquisition[acqIndex].GetWaveIntegral(min, max);
    const float Q0 = _reference->acquisition[acqIndex].GetWaveIntegral(min, max);
    return Q/Q0;
  }

  TGraph *GetCCEvsV(const TCTmeasurements& other, float min, float max){
    float CCE[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
    
    unsigned int size=acquisition.size();
    unsigned int gSize=1;
    for(unsigned int i=0; i < size; i++){
      V[gSize] = abs(acquisition[i].GetBias());  
      const TCTspectrum &ref = other[V[gSize]]; //GetSpectrumWithAbsBias(V[i]);
      if(ref.empty()) continue;
      if(fabs(ref.GetBias())!=V[gSize]){
	std::cerr << "[WARNING] Not the same bias voltage for CCE measurement for diode: " << acquisition[i].GetDiodeName() << "\t" << V[i] << " != " << fabs(ref.GetBias()) << std::endl;
      }
      float Q = acquisition[i].GetWaveIntegral(min, max);
      float Q0 = ref.GetWaveIntegral(min, max);
      //float Q0=1;
      CCE[gSize]=Q/Q0;
      gSize++;
    }
    return new TGraph(gSize-1, V,CCE);  /// \todo check why size-1
  }
  /// set the reference for CCE measurement
  void SetReference(const TCTmeasurements& reference){ _reference=&reference;}; 
 
  /// set the baseline
  void SetBaseline(const TCTmeasurements& baseline){ _baseline=&baseline;};

  TCTspectrum Average(bool checkBias=false)const; ///< average over all acquisitions of this measurement
 private:
  bool _checkBias; // operatations are not meant to be done as function of the bias voltage
  bool _isAverage;
  const TCTmeasurements *_reference, *_baseline;
  //TCTmeasurements &_baseline;
  

  TCTspectrum Sum(bool checkBias)const;
  TCTspectrum Sum2(bool checkBias)const;

  inline void AddFromFile(std::string filename){
    //assert(acquisition.empty());
    TCTspectrumCollection_t tmp =importer.ImportFromFile(filename);
    acquisition.insert(acquisition.end(), tmp.begin(), tmp.end());
  };


  // sum in order to perform average
  TCTmeasurements& operator+=(const TCTmeasurements& other){
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      acquisition[i]+=other.acquisition[i];
    }
    return *this;
  };

  // divide in order to perform average or rescale
  TCTmeasurements& operator/=(const float scale){
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      acquisition[i]/=scale;
    }
    return *this;
  };

  void clear(void){
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      acquisition[i]=0;
    }
  };

 void reset(){
   acquisition.clear();
   acqAbsBiasIndex.clear();
}



  /// bias = bias voltage
  /// \todo can be improved with indexing
  /// look for the correct acquisition
  TGraph *GetWaveForm(float bias, std::string graphName, std::string graphTitle, bool a)const{
    unsigned int index=0; // starting from the first element of the vector
    float biasStep = GetBiasScanStep();
    float biasItr = acquisition[0].GetBias();
    while(biasItr<bias){
      biasItr+=biasStep;
      index++;
    }
    if(biasItr-biasStep-bias < biasItr-bias){
      biasItr-=biasStep;
      index--;
    }

    if(abs(biasItr-bias)<0.1)     
      return acquisition[index].GetWaveForm(graphName);
    else return NULL;
  };
  

    
 private:
  float GetBiasScanStep(void) const{
    return acquisition[0].GetBias()-acquisition[1].GetBias();
  }




  /* /// constructor */
  /* TCTmeasurements(std::string filename, std::string reference, std::string baseline){ */
  /*   acquisition = importer.ImportFromFile(filename); */

  /*   /\* for(TCTspectrumCollection_t::const_iterator itr = acquisition.begin(); *\/ */
  /*   /\* 	itr != acquisition.end(); *\/ */
  /*   /\* 	itr++){ *\/ */
  /*   /\*   biases.insert(std::pair<float, unsigned int>( itr->GetBias(), itr-acquisition.begin())); *\/ */
  /*   /\* } *\/ */

  /*   /\* for(biasMap_t::const_iterator itr = biases.begin(); *\/ */
  /*   /\* 	itr!=biases.end();  *\/ */
  /*   /\* 	itr++){ *\/ */
  /*   /\*   std::cout << itr->first <<"\t" << itr->second << std::endl; *\/ */
  /*   /\* } *\/ */
  /* }; */
  
 public:
  TCTspectrumCollection_t acquisition;

  private:
  TCTimport importer;
  
  biasMap_t biases; ///< contains the list of V for the different measurements taking into account the possibility to have multiple times the same bias voltage applied
  
};
#endif
