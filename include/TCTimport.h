#ifndef TCTimport_h
#define TCTimport_h
/// program to import TCT mct files

#include "TCTmeasurement.h"

#include <fstream>
#include <iostream>
/** \class TCTimport TCTimport.h include/TCTimport.h
    
    \todo 
     - remove assert 
 */

class TCTimport{
 public:
  /// basic constructor 
  TCTimport(){};
  
  inline TCTmeasurement ImportFromFile(std::string filename){


    std::cout << "[STATUS] Importing from file " <<  filename << std::endl;
    std::string line;;
    std::ifstream f(filename.c_str());
    assert(f.good());
    

    std::getline(f,line); // first line of header
    //std::cout << line << std::endl;
    

    std::getline(f,line); // Diode header
    //    std::cout << line << std::endl;
    std::getline(f,line); // Diode name
    TCTmeasurement meas(line);           // create the measurement
    std::cout << "Diode: " << line << std::endl;

    std::getline(f,line); // Annealing header
    //std::cout << line << std::endl;
    std::getline(f,line); // Annealing values
    //std::cout << line << std::endl;
    float annealingTime=-1, annealingTemp=-1;
    sscanf(line.c_str(), "%f@%f",  &annealingTime, &annealingTemp);
    //    std::cout << annealingTime << "\t" << annealingTemp << std::endl;
    meas.SetAnnealing(  annealingTime, annealingTemp );

    for(int i=0; i<62; i++){ // skip lines
      std::getline(f,line); // 
    }
    //std::cout << line << std::endl;

    for(int i=0; i<4; i++){ // skip fields
      f >> line; // 
    }

    
    float value;

    f >> value;  // bias
    meas.SetBias(value);

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
      //std::cout << i++ << "\t" << value << std::endl; 
      meas.AddMeasurement(value);
    }
    
    //std::cout << value << std::endl;
    //std::cout << line << std::endl;
    

    assert(!f.fail());
    return meas;
  };
  
 private:
  


};



#endif

