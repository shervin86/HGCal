#ifndef CVimport_h
#define CVimport_h
/// program to import CV measurement files

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<string>
#include<CVmeasurement.h>

/** \class CVimport CVimport.h include/CVimport.h

\todo 
- remove assert
*/

class CVimport{
 public:
  /// basic constructor
  CVimport(){};

  inline void ReadLine(std::ifstream& f, CVmeasurement &meas, unsigned int nFreq){
    float bias, temp, current, guardCurrent;
    std::vector<float> cVec,gVec;
    std::string line;
    f >> bias >> temp;
    for(unsigned int iFreq=0; iFreq < nFreq; iFreq++){
      f >> current;
      if(f.fail()) return;
      cVec.push_back(current);
    }
    
    for(unsigned int iFreq=0; iFreq < nFreq; iFreq++){
      f >> guardCurrent;
      gVec.push_back(guardCurrent);
    }
    
    for(unsigned int iFreq=0; iFreq < nFreq; iFreq++){
      //std::cout << bias << "\t" << temp << "\t" << current << std::endl; 
       meas.AddMeasurement(bias, cVec[iFreq], gVec[iFreq], iFreq);
    }
  };

  inline std::vector<std::string> GetFrequencies(std::ifstream& f){
    std::vector<std::string> freq;
    //    char freqVal[10];
    
    std::string s;
    while(f.peek()!=13){
      std::getline(f, s, ',');
#ifdef DEBUG 
     std::cout << "V = " << s << std::endl;
#endif
      freq.push_back(s);
    }
    return freq;
  }

  CVmeasurement ImportFromFile(std::string filename);

};
#endif
