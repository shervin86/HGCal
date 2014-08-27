#ifndef configFileParser_h
#define configFileParser_h

/// Parser of the config file

#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
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

  
 public:
  inline bool operator<(const configFileContent& rhs)const{
    return type < rhs.type;
  }
  inline std::string GetReference(void)const{ return TCTreference;};
  inline std::string GetBaseline(void)const{ return TCTbaseline;};

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
    f >> std::skipws 
      >> type 
      >>  diodeName 
      >>  irradiation 
      >>  temp  
      >>  TCTdate 
      >>  TCTreference 
      >>  TCTbaseline 
      >>  IVdate 
      >>  CVdate;
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
  typedef std::multimap<std::string, configFileContent> lines_t;
 public:
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

  /* inline std::pair< lines_t::const_iterator, lines_t::const_iterator > find(std::string type){ */
  /*   return lines.equal_range(type); */
  /* } */

  inline lines_t::const_iterator find(std::string type) const{
    return lines.equal_range(type).first;
  }

  inline std::string GetTCTfilename(int index, std::string baseDir=""){
    auto iter = lines.begin();
    std::advance(iter,index);
    
    if(baseDir.empty())
      return iter->second.diodeName+"/"+iter->second.TCTdate+".mct";
    else 
      return baseDir+"/"+iter->second.diodeName+"/"+iter->second.TCTdate+".mct";
  }

  /* inline std::string GetTCTreferenceFilename(int index, std::string baseDir=""){ */
  /*   auto iter = lines.begin(); */
  /*   std::advance(iter,index); */

  /*   if(baseDir.empty()) */
  /*     return iter->second.diodeName+"/"+iter->second.TCTdate+".mct"; */
  /*   else  */
  /*     return baseDir+"/"+iter->second.TCTreference+"/"+iter->second.TCTreferenceDate+".mct"; */
  /* } */


 private:
  std::string _filename;
  lines_t lines;

  void Import(void);

};

#endif




