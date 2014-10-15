#ifndef configFileParser_h
#define configFileParser_h

/// Parser of the config file

#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
/** \class configFileContent configFileParser.h include/configFileParser.h
    class containing all the informations provided by one line of the config file

 */
class configFileContent{
 public:
  /**
     - refX: reference
     - basX: baseline
     - irrX: irradiated diode
     - X: index
  */

 configFileContent(void): 
  type("nan"),
    diodeName("nan"),
    irradiation("nan"),
    temp("nan"),
    TCTdate("nan"),
    TCTreference("nan"),
    TCTbaseline("nan"),
    IVdate("nan"),
    CVdate("nan"),
    Vdep_CV(0.), Vdep_CVerror(0.),
    Vdep_IV(0.), Vdep_IVerror(0.),
    Vdep_TCT(0.),
    CCE(0.), 
    Irev(0.), IrevError(0.),
    Cend(0.), CendError(0.){};
  
  std::string type, ///< type of measurement
    diodeName, ///< name of the diode 
    irradiation, ///< fluence
    temp, ///< temperature of the measurement
    TCTdate, ///< date of the TCT measurement
    TCTreference, ///< name of the diode used as reference
    //    TCTreferenceDate, ///< date and time of the TCT measurement of the reference
    TCTbaseline, ///< name of the diode used for the baseline
    //  TCTbaselineDate, ///< date and time of the baseline measurement
    IVdate, ///< date of the IV measurement
    CVdate; ///< date of the CV measurement
  float Vdep_CV, ///< depletion voltage measured from CV
    Vdep_CVerror,
    Vdep_IV, ///< depletion voltage measured from IV
    Vdep_IVerror, ///<
    Vdep_TCT, ///< depletion voltage measured from TCT
    CCE, ///< charge collection efficiency measured with TCT
    Irev, ///< 
    IrevError,
    Cend, CendError;
  
 public:
  
  inline std::string GetLegend() const{
    std::string legend;
    legend+=GetThickness();
    legend+=" um, ";
    legend+=irradiation;
    legend+=" cm^{-2}, ";
    legend+=GetTemperatureString();
    legend+=" C";
    return legend;
  }

  inline std::string GetThickness() const{ 
    std::string thickness= diodeName.substr(2,3);
    return thickness;
  }

  inline bool operator<(const configFileContent& rhs)const{
    return type < rhs.type;
  }
  inline std::string GetReference(void)const{ return TCTreference;};
  inline std::string GetBaseline(void)const{ return TCTbaseline;};
  inline float GetTemperature(void)const{ return std::stof(temp);};
  inline std::string GetTemperatureString(void)const{ return temp;};
  inline std::string GetFluence(void)const{ return irradiation;};

  configFileContent& operator<<(std::ostream& f){
    f << type << "\t" << diodeName << "\t" << irradiation << "\t" << GetThickness() << "\t" << GetTemperature()
      << "\t" << Vdep_CV
      << "\t" << Vdep_IV
      << "\t" << Vdep_TCT
      << "\t\t" << Irev
      << "\t" << IrevError
      << "\t" << Cend
      << "\t" << CendError
      << "\t\t" << CCE;
      //<< std::endl;
    return *this;
  }
      
  void intestazione(std::ostream& f){
    f << "ID\t" << "\t" << "diodeName" << "\t" << "\tfluence" << "\t" << "Thick" << "\t" << "Temp"
      << "\t" << "Vdep_CV"
      << "\t" << "Vdep_IV"
      << "\t" << "Vdep_TCT"
      << "\t" << "Irev"
      << "\t" << "IrevError"
      << "\t" << "Cend"
      << "\t" << "CendError"
      << "\t" << "CCE";
      //<< std::endl;
    return ;
  }
  
      /// read information from file f 
  /** input file format:
   * 
   \verbatim 
# role	diode                  irradiation	Temperature	TCTdate			reference	baseline	IVdate CVdate
ref1	FZ320P_02_DiodeL_8     -		-20		21/08/2014-19:14	-		-		-      -
bas1	FZ320P_02_DiodeL_8     -		-20		21/08/2014-19:34	-		-		-      -
bas1	FZ320P_02_DiodeL_8     -		-20		21/08/2014-19:55	-		-		-      -
irr1	FZ320P_01_DiodeL_11    4e14		-20		21/08/2014-16:26	ref1		bas1		-      -
\endverbatim
  */
  configFileContent& operator<<(std::ifstream& f){

    // read the line and put it into a string
    char line[200];
    f.getline(line, 200);

    std::stringstream fline;
    fline << line;

    // now read from the stream, so ignore what is after CVdate (comments)
    fline >> std::skipws 
      >> type 
      >>  diodeName 
      >>  irradiation 
      >>  temp  
      >>  TCTdate 
      >>  TCTreference 
      >>  TCTbaseline 
      >>  IVdate 
      >>  CVdate;
    
    if(TCTdate!="-"){
    TCTtime = stringToTime(TCTdate);
    //    TCTreferenceTime = stringToTime(TCTreferenceDate);
    // TCTbaselineTime  = stringToTime(TCTbaselineDate);
  
    char t[50]; 
    strftime(t, 50,"%Y_%m_%d_%H_%M", localtime(&TCTtime));
    TCTdate=t;
   
    //strftime(t, 50,"%Y_%m_%d_%H_%M", localtime(&TCTreferenceTime));
    //TCTreferenceDate=t;
    //strftime(t, 50,"%Y_%m_%d_%H_%M", localtime(&TCTbaselineTime));
    //TCTbaselineDate=t;
    } 

    Vdep_TCT=0., Vdep_CV=0., Vdep_IV=0., CCE=0.;
    return *this;
  };

  
 private:
  time_t TCTtime, TCTreferenceTime, TCTbaselineTime;
  time_t stringToTime(std::string timeString){
    int DD, MM, YYYY, hh,mm;
    sscanf(timeString.c_str(), "%d/%d/%d-%d:%d", &DD, &MM, &YYYY, &hh, &mm);
    struct tm timeinfo;
    timeinfo.tm_min=mm;
    timeinfo.tm_hour=hh;
    timeinfo.tm_mday=DD;
    timeinfo.tm_mon=MM-1;
    timeinfo.tm_year=YYYY-1900;
    timeinfo.tm_sec=0;
    return mktime(&timeinfo);
  }

};



/** \class configFileParser configFileParser.h include/configFileParser.h
    class to read information from config file
    the reading method is implemented in configFileContent class
 */
class configFileParser{

 public:
  /// \name iterators
  /// @{
  
  typedef std::multimap<std::string, configFileContent> lines_t; ///< each line of the config file, the first column (type) is the index

  /// iterator to the lines of the config file
  typedef lines_t::const_iterator  const_iterator;
  typedef lines_t::iterator        iterator;

  const_iterator begin()const{return lines.begin();}; 
  const_iterator end()const{  return lines.end();  };
  iterator       begin(){     return lines.begin();};
  iterator       end(){       return lines.end();  };

  /// return iterator to the first configFileContent line with given type
  /* const_iterator operator[](std::string type) const{ */
  /*   float fbias=fabs(bias); */
  /*   auto iter = (__current.upper_bound(fbias)); */
  /*   if(iter==begin()) return end(); */
  /*   iter--; */
  /*   if(fabs(iter->first-fbias)<1e-2) return iter; */
  /*   return end(); */
  /* } */
  /* iterator operator[](float bias){ */
  /*   float fbias=fabs(bias); */
  /*   auto iter = (__current.upper_bound(fbias)); */
  /*   if(iter==begin()) return end(); */
  /*   iter--; */
  /*   if(fabs(iter->first-fbias)<1e-2) return iter; */
  /*   return end(); */
  /* } */
  ///@}


  configFileParser(std::string filename){
    _filename=filename;
    Import();
  };

  inline size_t size(void){ return lines.size();};

  inline std::string GetType(unsigned int index){
    auto iter = lines.begin();
    std::advance(iter,index);
    return iter->second.type;
  }

  inline std::string GetFluence(std::string type) const { return GetFluence(find(type));};
  inline std::string GetFluence(lines_t::const_iterator iter) const{ std::string fluence = iter->second.irradiation; if(fluence=="-") fluence="0"; return fluence;};
  inline std::string GetThickness(std::string type) const{ 
    std::string thickness= GetThickness(find(type));
    return thickness;
  }
  inline std::string GetThickness(lines_t::const_iterator iter) const{ 
    std::string thickness= iter->second.diodeName.substr(2,3);
    return thickness;
  }


  inline float GetTemperature(lines_t::const_iterator iter)const{
    return iter->second.GetTemperature();
  }

  inline float GetTemperature(unsigned int index) const{ 
    auto iter = lines.begin();
    std::advance(iter,index);
    return GetTemperature(iter);
  }
  
  inline std::string GetTemperatureString(lines_t::const_iterator iter)const{
    return iter->second.GetTemperatureString();
  }

  inline std::string GetLegend(std::string type) const{
    auto iter = find(type);
    return iter->second.GetLegend();
    std::string legend;
    legend+=GetThickness(iter);
    legend+=" um, ";
    legend+=GetFluence(iter);
    legend+=" cm^{-2}, ";
    legend+=GetTemperatureString(iter);
    legend+=" C";
    return legend;
  }

  inline bool check(std::string type){
    return (bool) lines.count(type);
  }
  /* inline std::pair< lines_t::const_iterator, lines_t::const_iterator > find(std::string type){ */
  /*   return lines.equal_range(type); */
  /* } */

  inline lines_t::const_iterator find(std::string type) const{
    return lines.equal_range(type).first;
  }


  inline lines_t::iterator find(std::string type) {
    return lines.equal_range(type).first;
  }

  inline std::string GetDiodeBasename(std::string diodename){
    return diodename.substr(0,diodename.find("_"));
  }

  inline std::string GetTCTfilename(int index, std::string baseDir=""){
    auto iter = lines.begin();
    std::advance(iter,index);
    if(iter->second.TCTdate=="-") return "";
    
    if(baseDir.empty())
      return GetDiodeBasename(iter->second.diodeName)+"/"+iter->second.diodeName+"/"+iter->second.TCTdate+".mct";
    else 
      return baseDir+"/"+GetDiodeBasename(iter->second.diodeName)+"/"+iter->second.diodeName+"/"+iter->second.TCTdate+".mct";
  }

  inline std::string GetIVfilename(int index, std::string baseDir=""){
    auto iter = lines.begin();
    std::advance(iter,index);
    if(iter->second.IVdate=="-") return "";
    
    if(baseDir.empty())
      return GetDiodeBasename(iter->second.diodeName)+"/"+iter->second.diodeName+"/"+iter->second.diodeName+"_"+iter->second.IVdate+".iv";
    else 
      return baseDir+"/"+GetDiodeBasename(iter->second.diodeName)+"/"+iter->second.diodeName+"/"+iter->second.diodeName+"_"+iter->second.IVdate+".iv";
  }
    
  inline std::string GetCVfilename(int index, std::string baseDir=""){
    auto iter = lines.begin();
    std::advance(iter,index);
    if(iter->second.CVdate=="-") return "";
    
    if(baseDir.empty())
      return GetDiodeBasename(iter->second.diodeName)+"/"+iter->second.diodeName+"/"+iter->second.diodeName+"_"+iter->second.CVdate+".cv";
    else 
      return baseDir+"/"+GetDiodeBasename(iter->second.diodeName)+"/"+iter->second.diodeName+"/"+iter->second.diodeName+"_"+iter->second.CVdate+".cv";
  }
    
  /* inline std::string GetTCTreferenceFilename(int index, std::string baseDir=""){ */
  /*   auto iter = lines.begin(); */
  /*   std::advance(iter,index); */

  /*   if(baseDir.empty()) */
  /*     return iter->second.diodeName+"/"+iter->second.TCTdate+".mct"; */
  /*   else  */
  /*     return baseDir+"/"+iter->second.TCTreference+"/"+iter->second.TCTreferenceDate+".mct"; */
  /* } */

  void SetCCE(std::string type, float cce){
    find(type)->second.CCE=cce;
    return;
  }

  void SetVdepIV(std::string type, float value, float valueError){
    auto itr = find(type);
    itr->second.Vdep_IV=value;
    itr->second.Vdep_IVerror=valueError;
  }

  void SetIrev(std::string type, float value, float valueError){
    auto itr = find(type);
    itr->second.Irev=value;
    itr->second.Irev=valueError;
  }

  void SetVdepCV(std::string type, float value, float valueError){
    auto itr = find(type);
    itr->second.Vdep_CV=value;
    itr->second.Vdep_CVerror=valueError;
  }

  void SetCend(std::string type, float value, float valueError){
    auto itr = find(type);
    itr->second.Cend=value;
    itr->second.CendError=valueError;
  }


  void Dump(std::string type="irr"){
    configFileContent & p = lines.begin()->second; //
    p.intestazione(std::cout);
    std::cout << std::endl;
    
    lines_t::key_type oldVal;
    for(lines_t::iterator itr = lines.begin();
	itr!=lines.end();
	itr++){
      if(oldVal==itr->first) continue;
      oldVal=itr->first;
      if(itr->first.substr(0,3)==type){
	
     //   std::cout << 
      configFileContent & p = itr->second; //
      p << std::cout;
      std::cout << std::endl;
      }}
  }
 private:
  std::string _filename;
  lines_t lines;

  void Import(void);

};

#endif




