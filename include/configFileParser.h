#ifndef configFileParser_h
#define configFileParser_h

/// Parser of the config file

#include <fstream>
#include <iostream>
#include <vector>

class configFileContent{
 public:
  std::string diodeName, irradiation, temp, TCTdate, TCTreference, TCTreferenceDate, TCTbaseline, TCTbaselineDate, IVdate, CVdate;
  //struct tm TCT_timeinfo, TCTreference_timeinfo, TCTbaseline_timeinfo;
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
 public:
    /* std::cout << asctime(&timeinfo) << std::endl; */
  configFileContent& operator<<(std::ifstream& f){
    f >> std::skipws >>  diodeName 
      >> std::skipws >>  irradiation 
      >> std::skipws >>  temp  
      >> std::skipws >>  TCTdate 
      >> std::skipws >>  TCTreference 
      >> std::skipws >>  TCTreferenceDate 
      >> std::skipws >>  TCTbaseline 
      >> std::skipws >>  TCTbaselineDate 
      >> std::skipws >>  IVdate 
      >> std::skipws >>  CVdate;
    TCTtime = stringToTime(TCTdate);
    TCTreferenceTime = stringToTime(TCTreferenceDate);
    TCTbaselineTime  = stringToTime(TCTbaselineDate);
  
    char t[50]; 
    strftime(t, 50,"%Y_%m_%d_%H_%M", localtime(&TCTtime));
    TCTdate=t;
    strftime(t, 50,"%Y_%m_%d_%H_%M", localtime(&TCTreferenceTime));
    TCTreferenceDate=t;
    strftime(t, 50,"%Y_%m_%d_%H_%M", localtime(&TCTbaselineTime));
    TCTbaselineDate=t;
    
    return *this;
  };

};

/** \class configFileParser configFileParser.h include/configFileParser.h
 */
class configFileParser{

 public:
  configFileParser(std::string filename){
    _filename=filename;
    Import();
  };

  void Import(void);

  std::string GetTCTfilename(int index){
    //tctFilename=baseDir+"/"+diodeName+"/"+TCTdate;
    return lines[index].diodeName+"/"+lines[index].TCTdate+".mct";
  }

 private:
  std::string _filename;
  std::vector<configFileContent> lines;
};

#endif




