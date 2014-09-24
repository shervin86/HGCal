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
#define MAX_ACQ 1000

/** \class TCTmeasurements TCTmeasurements.h include/TCTmeasurements.h
 */
class TCTmeasurements{
 public:
  typedef std::multimap<float, unsigned int> biasMap_t;
  
  // default constructor
 TCTmeasurements(std::string filename, float temp): 
  //  _checkBias(false),
  _reference(NULL){
    acquisition = importer.ImportFromFile(filename);
    assert(acquisitionAverage.isnull());

    for(auto itr=begin(); itr!=end(); itr++){
      assert(!itr->second.isnull());
      itr->second.SetTemperature(temp);
    }
  }


  /// constructor for new measurement produced by manipulation of other measurements
 TCTmeasurements(void): 
    _reference(NULL),    fPaletteColor({1}) //_checkBias(false), 
{
    return;
  };


  ~TCTmeasurements(){};

  typedef TCTspectrumCollection_t::const_iterator  const_iterator;
  typedef TCTspectrumCollection_t::iterator        iterator;
  const_iterator begin()const{return acquisition.begin();};
  const_iterator end()const{return acquisition.end();};
  iterator       begin(){return acquisition.begin();};
  iterator       end(){return acquisition.end();};
  
  size_t size(void) const{ return acquisition.size();}; ///< number of spectra acquired with this measurement (if from acquisition)

  /** put the average into acquisition[0] and the std. dev. into acquisitionRMS[0]\n
   * if checkBias==true, make the average only between measurements with the same bias voltage\n
   * if checkBias==false, make the average over all the bias voltages
   */
  void Average(std::vector<TCTmeasurements> others, bool checkBias=false); ///< this measurement will contain the average over several TCTmeasurements

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


  /// return the spectrum with given bias voltage (absolute value), or empty spectrum
  const_iterator operator[](float bias) const{
    float fbias=fabs(bias);
    /*  for(auto it=begin(); it!=end(); it++){ */
    /*   std::cout << it->first << "\t" << it->second.GetDiodeName() << std::endl; */
    /* } */
    auto iter = (acquisition.upper_bound(fbias));
    if(iter==end()) return end();
    iter--;
    if(fabs(iter->first-fbias)<1e-2) return iter;
    return end();
  }

  /// return the spectrum with given bias voltage (absolute value), or empty spectrum
  iterator operator[](float bias){
    float fbias=fabs(bias);
  
    auto iter = (acquisition.upper_bound(fbias));

    if(iter==begin()) return end();
    iter--;
    if(fabs(iter->first-fbias)<1e-2) return iter;
    return end();
  }


  const TCTspectrum& GetSpectrumWithBias(float bias) const{
    auto itr = (*this)[bias];
    if(itr!=end()){
      return itr->second;
    }
    return *(new TCTspectrum(begin()->second.GetDiodeName()));
  }

  TCTspectrum& GetSpectrumWithBias(float bias){
    auto itr = (*this)[bias];
    if(itr!=end()) return itr->second;
    else return *(new TCTspectrum(begin()->second.GetDiodeName()));
  }

  /* /// return the spectrum with given bias voltage (absolute value), or empty spectrum */
  /* const TCTspectrum& operator[](float bias) const{ */
  /*   auto iter = (--acquisition.upper_bound(bias)); */
  /*   if(fabs(iter->first-bias)<1e3) return iter->second; */
  /*   return *(new TCTspectrum(acquisition.begin()->second.GetDiodeName()));  */
  /* } */

  /* /// return the spectrum with given bias voltage (absolute value), or empty spectrum */
  /* TCTspectrum& operator[](float bias) { */
  /*   auto iter = (--acquisition.upper_bound(bias)); */
  /*   if(fabs(iter->first-bias)<1e3) return iter->second; */
  /*   return *(new TCTspectrum(acquisition.begin()->second.GetDiodeName()));  */
  /* } */

  ///copy the content of the other measurement
  TCTmeasurements& operator=(const TCTmeasurements& other){
    reset();
    acquisition=other.acquisition;
    acquisitionRMS=other.acquisition;
    _isAverage=other._isAverage;
    return *this;
  };
  
  ///remove the spectrum from all the measurements (for baseline subtraction)
  TCTmeasurements& operator-=(const TCTspectrum& other){
    for(iterator iter=begin(); iter!=end(); iter++){
      iter->second-=other;
    }
    return *this;
  };


  // sum in order to perform average
  TCTmeasurements& operator+=(const TCTmeasurements& other){
    for(auto itr=other.begin(); itr!=other.end(); itr++){
      auto thisItr = (*this)[itr->first];
      assert(thisItr!=end());
      thisItr->second+=itr->second;
    }
    return *this;
  };

  /// method to get a graph of the Q vs V given min and max range in time for integration
  TGraph *GetQvsV(float min, float max){
    float Q[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
  
    unsigned int i=0;
    for(auto iter=begin(); iter!=end(); iter++){
      Q[i] = iter->second.GetWaveIntegral(min, max, 0.); //iter->second.GetMean(0.,min));
      V[i] = iter->first;
      i++;
      assert(i<MAX_ACQ);
    }
    return new TGraph(i, V,Q);  /// \todo check why size-1
  }

  TGraph *GetCCEvsV(float min, float max){
    return GetCCEvsV(*_reference, min, max);
  }

  inline float GetCCE(float min, float max, float bias) const{
    assert(_reference!=NULL);

    float fbias=fabs(bias);
  
    auto spItr = (*this)[fbias];
    
    const TCTspectrum& sp=GetSpectrumWithBias(bias);
    if(sp.isnull()){
      std::cerr << "[ERROR] bias voltage not found:" << fbias << "\t" << acquisition.begin()->second.GetDiodeName() << std::endl;
      
    for(const_iterator iter=begin(); iter!=end(); iter++){
	std::cout << iter->first << ", ";
      }
      std::cout << std::endl;
      exit(1);
    }
  
    float Q = sp.GetWaveIntegral(min, max);
    const TCTspectrum& ref = _reference->GetSpectrumWithBias(fbias);
    assert(!ref.isnull());
    const float Q0 = ref.GetWaveIntegral(min, max);
    return Q/Q0;
  }

  TGraph *GetCCEvsV(const TCTmeasurements& other, float min, float max){
    float CCE[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
    
    bool warning=true; // warn only once
    unsigned int i=0;
    for(iterator iter=begin(); iter!=end(); iter++){
      //      Q[i] = iter->second.GetWaveIntegral(min, max,iter->second.GetMean(0.,min));
      V[i] = iter->first;
      const TCTspectrum &ref = other.GetSpectrumWithBias(V[i]); 
      if(ref.empty()) continue; /// \todo fix-it
      float fbias=fabs(ref.GetBias());
      if(V[i]!=fbias && warning){ //fbias!=V[gSize] && fbias<V[gSize]){
	warning=false;
	std::cerr << "[WARNING] Not the same bias voltage for CCE measurement for diode: " << iter->second.GetDiodeName() << "\t" << V[i] << " != " << fbias << std::endl;
      }
      float Q = iter->second.GetWaveIntegral(min, max);
      float Q0 = ref.GetWaveIntegral(min, max);
      assert(Q0!=0);
      CCE[i]=Q/Q0;
      i++;
      assert(i<MAX_ACQ);
    }
 
    return new TGraph(i, V,CCE);  /// \todo check why size-1
  }
  /// set the reference for CCE measurement
  void SetReference(const TCTmeasurements& reference){assert(&reference!=NULL); _reference=&reference;}; 
 
  /// set the baseline
  void SetBaseline(const TCTmeasurements& baseline){ _baseline=&baseline;};

  //average over all the acquisitions of this measurement, regardless the bias voltage applied (biasCheck==false)
  TCTspectrum GetAverage(bool checkBias=false)const; ///< average over all acquisitions of this measurement

  /// return one spectrum
  TCTspectrum& GetSpectrum(unsigned int i){

    assert(i<size());
    auto iter = begin();
    std::advance(iter,i);

    return iter->second;
  }

  /// return one spectrum
  const TCTspectrum& GetSpectrum(unsigned int i) const{
    assert(i<size());
    auto iter = begin();
    std::advance(iter,i);

    return iter->second;
  }


 /// Return the average spectrum
  TCTspectrum& GetAverageMeasurement(){
    assert(_isAverage);
    return acquisitionAverage;
  }

 /// Return the average spectrum
  const TCTspectrum& GetAverageMeasurement()const {
    assert(_isAverage);
    return acquisitionAverage;
  }


  TGraph *GetWaveForm(const_iterator itr, std::string graphName, std::string graphTitle)const;

 private:
  //bool _checkBias; // operatations are not meant to be done as function of the bias voltage
  bool _isAverage;
  const TCTmeasurements *_reference, *_baseline;
  //TCTmeasurements &_baseline;
  

  //  TCTspectrum Sum(bool checkBias)const;
  //TCTspectrum Sum2(bool checkBias)const;

  inline void AddFromFile(std::string filename){
    //assert(acquisition.empty());
    TCTspectrumCollection_t tmp =importer.ImportFromFile(filename);
    acquisition.insert( tmp.begin(), tmp.end());
  };


  // divide in order to perform average or rescale
  TCTmeasurements& operator/=(const float scale){
    for(auto itr=begin(); itr!=end(); itr++){
      itr->second/=scale;
    }
    return *this;
  };

  /// set all the spectra at 0
  void clear(void){
    for(auto itr=begin(); itr!=end(); itr++){
      itr->second=0.;
    }
  };

  /// remove all acquisitions, not the averages
 void reset(){
   acquisition.clear();
   acquisitionRMS.clear();
}




  /// bias = bias voltage
  /// \todo can be improved with indexing
  /// look for the correct acquisition
  TGraph *GetWaveForm(float bias, std::string graphName, std::string graphTitle, bool a)const{
    auto itr = (*this)[bias];
    if(itr==end()) return NULL;
    return itr->second.GetWaveForm(graphName);
  };
  


    
 private:




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
 

  private:
  TCTspectrumCollection_t acquisition, acquisitionRMS; 
  TCTspectrum acquisitionAverage, acquisitionAverageRMS;
  TCTimport importer;
  

  int fPaletteColor[MAX_ACQ];
  
};
#endif
