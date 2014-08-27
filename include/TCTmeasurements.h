#ifndef TCTmeasurements_h
#define TCTmeasurements_h


#include "TCTspectrum.h"
#include "TCTimport.h"
#include <TGraphErrors.h>

#include <cstdlib>
#include <map>

/// Assembly the information from several measurements to get spectra, QvsV, CCEvsV, IvsV,CvsV graphs

/// maximum number of acquisitions in one scan
#define MAX_ACQ 100

/** \class TCTmeasurements TCTmeasurements.h include/TCTmeasurements.h
 */
class TCTmeasurements{
  typedef   std::multimap<float, unsigned int> biasMap_t;

 public:

  /// constructor for new measurement produced by manipulation of other measurements
 TCTmeasurements(): _checkBias(false){};

  /* TCTmeasurements(std::string filename, TCTspectrum reference, TCTspectrum baseline){ */
  /*   acquisition = importer.ImportFromFile(filename); */
  /* } */

 TCTmeasurements(std::string filename): _checkBias(false){
    acquisition = importer.ImportFromFile(filename);
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

  ~TCTmeasurements(){};

  /// put the average into acquisition[0] and the std. dev. into acquisition[1]
  void Average(std::vector<TCTmeasurements> others, bool checkBias=false); ///< average over several measurements
  TGraph *GetWaveForm(unsigned int index, std::string graphName, std::string graphTitle)const;
  TGraphErrors *GetAverageWaveForm(std::string graphName, std::string graphTitle) const;

  size_t size(void) const{ return acquisition.size();};


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

  TGraph *GetCCEvsV(TCTmeasurements& other, float min, float max){
    float CCE[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
    
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      float Q = acquisition[i].GetWaveIntegral(min, max);
      float Q0 = other.acquisition[i].GetWaveIntegral(min, max);
      CCE[i]=Q/Q0;
      V[i] = abs(acquisition[i].GetBias());
    }
    return new TGraph(size-1, V,CCE);  /// \todo check why size-1
  }

 private:
  bool _checkBias; // operatations are not meant to be done as function of the bias voltage
  bool _isAverage;
  TCTspectrum Average(bool checkBias=false)const; ///< average over all acquisitions of this measurement
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


  
 public:
  TCTspectrumCollection_t acquisition;

  private:
  TCTimport importer;
  
  biasMap_t biases; ///< contains the list of V for the different measurements taking into account the possibility to have multiple times the same bias voltage applied
  
};
#endif
