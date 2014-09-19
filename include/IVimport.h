#ifndef IVimport_h
#define IVimport_h
/// program to import IV measurement files

#include<iostream>
#include<fstream>
#include<cstdlib>

#include<IVmeasurement.h>

/** \class IVimport IVimport.h include/IVimport.h

\todo 
- remove assert
*/

class IVimport{
 public:
  /// basic constructor
  IVimport(){};

  inline void ReadLine(std::ifstream& f, IVmeasurement &meas){
    float bias, temp, current, guardCurrent;
    std::string line;
    
    f >> bias >> temp >> current >> guardCurrent;
    //std::cout << bias << "\t" << temp << "\t" << current << std::endl; 
    if(f.good()) meas.AddMeasurement(bias, current, guardCurrent);
  };

  inline IVmeasurement ImportFromFile(std::string filename){
    std::string line;
    
    IVmeasurement meas;
    
    std::ifstream f(filename.c_str());
    std::cout << "[STATUS] Import from file: " << filename << std::endl;
    assert(f.good()); /// 
    

    std::getline(f,line); // first line of header
    std::getline(f,line); // :start

    std::getline(f,line); // :start time
    unsigned int hh,min,sec, dd,mm,yyyy;
    sscanf(line.c_str(), "%d-%d-%d %d:%d:%d", &yyyy, &mm, &dd, &hh, &min, &sec);
    char time[30]; 
    sprintf(time, "%04d_%02d_%02d_%02d_%02d_%02d", yyyy, mm, dd, hh, min, sec);
    std::cout << "Acquisition time: " << time << std::endl;
    meas.SetTime(time);
  
    std::getline(f,line); // :stop
    std::getline(f,line); // :stop time

    std::getline(f,line); // :device
    std::getline(f,line); // :diode name
    if(*line.rbegin()==13){ // 13=carriage return
      line.resize(line.size()-1); //line.pop_back() in c++11
    }
    meas.SetDiodeName(line);
    std::cout << line << "\t" << meas.GetDiodeName() << std::endl;
    for(int i=0; i<10; i++){ // skip lines
      std::getline(f,line); // 
    }
  
    std::getline(f,line); // Annealing header
    //std::cout << line << std::endl;
    std::getline(f,line); // Annealing values
    //std::cout << line << std::endl;
    float annealingTime=-1, annealingTemp=-1;
    sscanf(line.c_str(), "%f@%f",  &annealingTime, &annealingTemp);
    //    std::cout << annealingTime << "\t" << annealingTemp << std::endl;
    meas.SetAnnealing(  annealingTime, annealingTemp );

    std::getline(f,line); // :temperature header
    std::getline(f,line); // :temperature
    float temperature=999;
    sscanf(line.c_str(), "%f",  &temperature);
    meas.SetTemperature(temperature);
    //std::cout << line << "\t" << meas.GetTemperature() << std::endl;

    for(int i=0; i<10; i++){ // skip lines
      std::getline(f,line); // 
    }

    while(line.find("BEGIN")==std::string::npos){
      std::getline(f,line); // 
    }
    // std::cout << "BEGIN: " << line << std::endl;
    while(f.good() && f.peek()!=EOF){
      //TCTspectrum newMeas = meas; // create a new waveform from the base one (the one keeping the general info)
      ReadLine(f, meas);		      
      ///      measurements.push_back(newMeas);
    }
    return meas;
  };

};
#endif
