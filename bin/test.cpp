/// test program
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#include "TCTmeasurement.h"
#include "TCTimport.h"
int main(int argc, char **argv){

  

  TCTimport import;

  //reference diode, N-type, FZ320N_04_DiodeL_5
  TCTmeasurement FZ320N_ref_1 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_15_12.mct");
  TCTmeasurement FZ320N_ref_2 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_15_37.mct");
  TCTmeasurement FZ320N_ref_3 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_17_08.mct");
  TCTmeasurement FZ320N_ref_4 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_17_29.mct");

  TGraph FZ320N_ref_1_graph=meas.GetWaveForm();
  TGraph FZ320N_ref_2_graph=meas.GetWaveForm();
  TGraph FZ320N_ref_3_graph=meas.GetWaveForm();
  TGraph FZ320N_ref_4_graph=meas.GetWaveForm();

  
  graph.SaveAs("f.root");
  return 0;
}


