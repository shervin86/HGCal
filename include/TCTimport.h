#ifndef TCTimport_h
#define TCTimport_h
/// program to import TCT mct files

#include "TCTspectrum.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
/** \class TCTimport TCTimport.h include/TCTimport.h
    
    \todo 
     - remove assert 
 */

class TCTimport{
 public:
  /// basic constructor 
  TCTimport(){};

  inline bool ReadLine(std::ifstream& f, TCTspectrum &meas){
    float value; 
    std::string line;
    for(int i=0; i<4; i++){ // skip fields
      f >> line; // 
    }
    
    if(f.fail()) return false;
    f >> value;  // bias
    std::cout << value << std::endl;
    meas.SetBias(value);
    //    std::cout << value << meas.GetBias() << std::endl;
    f >> value; // Attenuator [10 dB]
    f >> value; // WF-Peakheight [V]
    f >> value; // Leakage [A]

    meas.SetLeakage(value);
    f >> value; //Guard [A], 

    f >> value; //Temp [C], 
    meas.SetTemperature(value);

    f >> value >> value >> value; //0, 0, t(0) [s], 

    f >> value; //dt [s]
    meas.SetTimeScanUnit(value);
    //int i =0;
    while(f.peek()!=10 && f.peek()!=10 && f.peek()!=EOF && f.peek()!=13){
      f >> value;
      if(value<-1 || value>1){
	std::cerr << "[ERROR] Reading file error: value = " << value << std::endl;
	exit(1);
      }
      //std::cout << i++ << "\t" << value << std::endl; 
      meas.AddMeasurement(value);
    }
   
 
    //std::cout << value << std::endl;
    //std::cout << line << std::endl;
    

    //    assert(!f.fail());
    return true;
  }

  inline TCTspectrumCollection_t ImportFromFile(std::string filename){


    std::cout << "[STATUS] Importing from file " <<  filename << std::endl;
    std::string line;;
 
    
    std::ifstream f(filename.c_str());
    if(!f.good()){
      std::cerr << "[ERROR] File " << filename << " not readable" << std::endl;
      exit(1);
    }
    

    std::getline(f,line); // first line of header
    //std::cout << line << std::endl;
    

    std::getline(f,line); // Diode header
    //    std::cout << line << std::endl;
    std::getline(f,line); // Diode name
    if(*line.rbegin()==13){ // 13=carriage return
      line.resize(line.size()-1); //line.pop_back() in c++11
    }
    //std::cout << "Back=" << (int)line[line.size()-1] << std::endl;
    TCTspectrum meas(line);           // create the measurement
    //std::cout << "Diode: " << line << "ciao" << std::endl;

    std::getline(f,line); // Annealing header
    //std::cout << line << std::endl;
    std::getline(f,line); // Annealing values
    //std::cout << line << std::endl;
    float annealingTime=-1, annealingTemp=-1;
    sscanf(line.c_str(), "%f@%f",  &annealingTime, &annealingTemp);
    //    std::cout << annealingTime << "\t" << annealingTemp << std::endl;
    meas.SetAnnealing(  annealingTime, annealingTemp );

    for(int i=0; i<58; i++){ // skip lines
      std::getline(f,line); // 
    }
    
    std::getline(f,line); // Scan started
    if(line.find("Scan")!=0){
      std::getline(f,line); // Scan started
    }
    std::getline(f,line); // Date and time
   if(*line.rbegin()==13){ // 13=carriage return
      line.resize(line.size()-1); //line.pop_back() in c++11
    }
  
   unsigned int hh,min,dd,mm,yyyy;
    sscanf(line.c_str(), "%d:%d on %d/%d/%d (dd/mm/yyyy)", &hh, &min, &dd, &mm, &yyyy);
    char time[30]; 
    sprintf(time, "%04d_%02d_%02d_%02d_%02d", yyyy, mm, dd, hh, min);
    //std::cout << "   Acquisition time: " << time << std::endl;
    meas.SetTime(time);
    std::getline(f,line); // skip
    std::getline(f,line); // skip
    
    //std::cout << line << std::endl;


    // now start to read the different measurements
    TCTspectrumCollection_t measurements;
    while(f.good() && f.peek()!=EOF){
      TCTspectrum newMeas = meas; // create a new waveform from the base one (the one keeping the general info)
      if(ReadLine(f, newMeas))
	measurements.insert(std::make_pair(fabs(newMeas.GetBias()), newMeas));
    }
    //    float lastStepValue=measurements.rbegin()->first;
    //if(lastStepValue!=0 && abs(lastStepValue)<0.001) measurements.pop_back(); /// remove the last point if not significative
    std::cout << "   Bias scan from: " << measurements.begin()->first << " to " << measurements.rbegin()->first << std::endl;
    return measurements;
  };
  
 private:
  


};



#endif

