#ifndef TCTmeasurements_h
#define TCTmeasurements_h

/// Contains the information of one acquisition

#include "TCTmeasurement.h"
#include "TCTimport.h"

#include <cstdlib>
#include <map>
/// maximum number of acquisitions in one scan
#define MAX_ACQ 100

class TCTmeasurements{
  typedef   std::multimap<float, unsigned int> biasMap_t;
 public:
  TCTmeasurements(){};
  TCTmeasurements(std::string filename){
    acquisition = importer.ImportFromFile(filename);

    for(TCTmeasurementCollection_t::const_iterator itr = acquisition.begin();
	itr != acquisition.end();
	itr++){
      biases.insert(std::pair<float, unsigned int>( itr->GetBias(), itr-acquisition.begin()));
    }

    for(biasMap_t::const_iterator itr = biases.begin();
	itr!=biases.end(); 
	itr++){
      std::cout << itr->first <<"\t" << itr->second << std::endl;
    }
  };

  ~TCTmeasurements(){};


  inline void ReadFromFile(std::string filename){
    assert(acquisition.empty());
    acquisition = importer.ImportFromFile(filename);
  };


  // subtraction of waveforms from other acquisition (mainly for baseline subtraction)
  TCTmeasurements& operator-=(const TCTmeasurements& other){
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      acquisition[i]-=other.acquisition[i];
    }
    return *this;
  };

  TCTmeasurements& operator-=(const TCTmeasurement& other){
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      //   std::cout << "--> " << acquisition[i].GetN() << "\t" << other.GetN()<< std::endl;
      acquisition[i]-=other;
    }
    return *this;
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

  //average over all the acquisitions of this measurement, regardless the bias voltage applied (biasCheck==false)
  TCTmeasurement Average(bool checkBias=false)const{ 
    assert(checkBias==false); /// \todo implement the case of biasCheck
    TCTmeasurement meas=acquisition[0];
    meas.clear(); // reset the values
    for(TCTmeasurementCollection_t::const_iterator itr = acquisition.begin();
	itr!=acquisition.end();
	itr++){
      //std::cout << meas.GetSamples()[10] << "\t" << itr->GetSamples()[10];
      meas+=*itr;
      //std::cout << "\t" << meas.GetSamples()[10] << std::endl;
    }
    meas/=acquisition.size();
    //std::cout << "\t" << meas.GetSamples()[10] << std::endl;
    
    std::cout << "Average over " << acquisition.size() << " acquisitions" << std::endl;
    return meas;
  }

  void reset(){
    acquisition.clear();
  }
  //average over several measuremets 
  void Average(std::vector<TCTmeasurements> others, bool checkBias=false){
    
    this->reset(); // remove all the measurements
    TCTmeasurement m = others.begin()->acquisition[0];
    acquisition.push_back(m);
    this->clear(); // initialize to 0 the sum
    std::cout << "Average over " << others.size() << " measurements" << std::endl;
    for(std::vector<TCTmeasurements>::const_iterator itr = others.begin();
	itr!=others.end();
	itr++){
      assert(checkBias==false);
      //*this+=*itr;
      // this is the average of the means! average of the average!
      this->acquisition[0]+=itr->Average(checkBias);
    }
    this->acquisition[0]/=others.size();
    return;
  }

  TCTmeasurement& GetAverageMeasurement(){
    return acquisition[0];
  }
  /// index = acquisition index
  TGraph *GetWaveForm(unsigned int index, std::string graphName, std::string graphTitle)const{ 
    assert(index<acquisition.size());
    //assert(acquisition[index].GetBias()!=
    std::cout << acquisition[index].GetBias() << "\t" << index << std::endl;
    for(unsigned int i=0; i < acquisition.size(); i++){
      if(acquisition[i].GetBias()==-600) index=i;
    }
    std::cout << graphName << "\tBias = " << acquisition[index].GetBias()<< std::endl;	
    return acquisition[index].GetWaveForm(graphName, graphTitle);
  };

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
  

  /// method to get a graph of the Q vs V given min and max range in time for integration
  TGraph GetQvsV(float min, float max){
    float Q[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
    
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      Q[i] = acquisition[i].GetWaveIntegral(min, max);
      V[i] = abs(acquisition[i].GetBias());
    }
    return TGraph(size-1, V,Q);  /// \todo check why size-1
  }

  TGraph GetCCEvsV(TCTmeasurements& other, float min, float max){
    float CCE[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
    
    unsigned int size=acquisition.size();
    for(unsigned int i=0; i < size; i++){
      float Q = acquisition[i].GetWaveIntegral(min, max);
      float Q0 = other.acquisition[i].GetWaveIntegral(min, max);
      CCE[i]=Q/Q0;
      V[i] = abs(acquisition[i].GetBias());
    }
    return TGraph(size-1, V,CCE);  /// \todo check why size-1
  }
    
 private:
  float GetBiasScanStep(void) const{
    return acquisition[0].GetBias()-acquisition[1].GetBias();
  }


  
 public:
  TCTmeasurementCollection_t acquisition;

  private:
  TCTimport importer;
  
  biasMap_t biases; ///< contains the list of V for the different measurements taking into account the possibility to have multiple times the same bias voltage applied
  
};
#endif
