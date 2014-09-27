#ifndef TCTmeasurements_h
#define TCTmeasurements_h


#include "TCTspectrum.h"
#include "TCTimport.h"
#include <TGraphErrors.h>
#include <TMultiGraph.h>

#include <TFitResult.h>

#include <cstdlib>
#include <map>
#include <iomanip>
/// Assembly the information from several measurements to get spectra, QvsV, CCEvsV, IvsV,CvsV graphs

/// maximum number of acquisitions in one scan
#define MAX_ACQ 1000

/** \class TCTmeasurements TCTmeasurements.h include/TCTmeasurements.h
 */
class TCTmeasurements{
 public:
  //  typedef std::multimap<float, unsigned int> biasMap_t;
  
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
    if(iter==begin()) return end();
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
    for(unsigned int i=0; i < MAX_ACQ; i++){
      fPaletteColor[i]=other.fPaletteColor[i];
    }
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
    //return new TGraph(i, V,Q);  /// \todo check why size-1
    TGraph *g= new TGraph(i, V,Q);  /// \todo check why size-1
    Vdep(g);
    return g;
  }

  
  Double_t Vdep(TGraph *g, int nmin=25){
  Int_t n = g->GetN();
  Double_t *x = g->GetX();
  Double_t *y = g->GetY();
  Float_t chi2=0;
  Double_t Irev=0;
  int ndf=1;

  
  int min=0;
  int max=n-1;
  TFitResultPtr p = g->Fit("pol1","QS");
  TF1 *f = g->GetFunction("pol1");
  //std::cout << x[min] << "\t" << x[max] << "\t" << p->GetParams()[1] << std::endl;
  while(abs(p->GetParams()[1])<p->GetErrors()[1]){
    //std::cout << x[min] << "\t" << x[max] << std::endl;
    p = g->Fit(f,"SQ","",x[min], x[max]);
    min++;
    if(max-min<nmin) break;
  }
  //  p->Print();
  return x[min];
}


  TGraph *GetCCEvsV(float min, float max)const{
    return GetCCEvsV(*_reference, min, max);
  }

  inline float GetCCE(float min, float max, float bias) const{
    const TCTmeasurements *_r = _reference;
    if(_reference==NULL) _r=&(*this);

    float fbias=fabs(bias);
  
    auto spItr = (*this)[fbias];
    if(spItr==end()){
      std::cerr << "[ERROR] bias voltage not found:" << fbias << "\t" << acquisition.begin()->second.GetDiodeName() << std::endl;
      for(const_iterator iter=begin(); iter!=end(); iter++){
	std::cout << iter->first << ", ";
      }
      std::cout << std::endl;
      exit(1);
    }
  
    float Q = spItr->second.GetWaveIntegral(min, max);
    float Q0 = 0.;
    auto ref=_r->end();
   //take the average over 25 last points
    unsigned int i=0;
    for(; i<2; i++){
      ref--;
      Q0+=ref->second.GetWaveIntegral(min, max);  
    }
    Q0/=i;
    ref = (*_r)[fbias];
    //if(ref!=_r->end()){
    //std::cout << std::setprecision(3) <<Q0 << "\t" << ref->second.GetWaveIntegral(min, max) << std::endl;;  
    //}
 
   
    return Q/Q0;
  }

  TGraph *GetCCEvsV(const TCTmeasurements& other, float min, float max)const {
    float CCE[MAX_ACQ]={0.};
    float V[MAX_ACQ]={0.};
    
    bool warning=true; // warn only once
    unsigned int i=0;
    for(const_iterator iter=begin(); iter!=end(); iter++){
      //      Q[i] = iter->second.GetWaveIntegral(min, max,iter->second.GetMean(0.,min));
      V[i] = iter->first;
      CCE[i]=GetCCE(min, max, V[i]);
      i++;
      assert(i<MAX_ACQ);
    }
 
    TGraph *g= new TGraph(i, V,CCE);  
    return g;
  }
  /// set the reference for CCE measurement
  void SetReference(const TCTmeasurements& reference, std::string type){assert(&reference!=NULL); _reference=&reference; _referenceType=type;}; 
  const TCTmeasurements* GetReference(void){return _reference;};
  std::string GetReferenceType(void){return _referenceType;};

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
  std::string _referenceType;
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
