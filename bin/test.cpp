/// test program
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#include "TCTmeasurements.h"
//#include "TCTimport.h"

#include <TGraph.h>
#include <TMultiGraph.h>
int main(int argc, char **argv){

  
  TCTmeasurements FZ320N_ref_1("data/FZ320N_04_DiodeL_5/2014_08_18_15_12.mct"); // test do not use it
  TCTmeasurements FZ320N_ref_2("data/FZ320N_04_DiodeL_5/2014_08_18_15_37.mct"); // 

  TGraph FZ320N_ref_2_0_wave = FZ320N_ref_2.GetWaveForm(0, "ciao");
  FZ320N_ref_2_0_wave.SaveAs("f.root");
 
  TGraph FZ320N_ref_2_0_QvsV = FZ320N_ref_2.GetQvsV(86e-9,96e-9);
  FZ320N_ref_2_0_QvsV.SaveAs("f1.root");

  TCTmeasurements CCE(FZ320N_ref_2);
  CCE=FZ320N_ref_2;
  TGraph FZ320N_CCE_2_0_QvsV = CCE.GetCCEvsV(FZ320N_ref_2,86e-9,96e-9);
  FZ320N_CCE_2_0_QvsV.SaveAs("f2.root");


  return 0;
}

#ifdef shervin

  //TCTimport import;

  //reference diode, N-type, FZ320N_04_DiodeL_5
  TCTmeasurementCollection_t FZ320N_ref_1 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_15_12.mct"); // test do not use it
  std::cout << FZ320N_ref_1.GetDiodeName() << "\t" << FZ320N_ref_1.GetTime() << std::endl;
  TCTmeasurementCollection_t FZ320N_ref_2 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_15_37.mct");


  //  TCTmeasurement FZ320N_ref_3 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_17_08.mct");
  //TCTmeasurement FZ320N_ref_4 = import.ImportFromFile("data/FZ320N_04_DiodeL_5/2014_08_18_17_29.mct");



  TGraph FZ320N_ref_2_graph_0=FZ320N_ref_2[0].GetWaveForm();
  TGraph FZ320N_ref_2_graph_1=FZ320N_ref_2[10].GetWaveForm();
  //TGraph FZ320N_ref_3_graph=FZ320N_ref_3.GetWaveForm();
  //TGraph FZ320N_ref_4_graph=FZ320N_ref_4.GetWaveForm();

  TMultiGraph FZ320N_ref_multi;
  FZ320N_ref_multi.SetName("multi");

  FZ320N_ref_multi.Add(&FZ320N_ref_2_graph_0);
  std::cout << "integral = " << FZ320N_ref_2[0].GetWaveIntegral(86e-9,96e-9) << std::endl;
  FZ320N_ref_multi.Add(&FZ320N_ref_2_graph_1);
  
  FZ320N_ref_multi.SaveAs("f.root");

  ///////////////////////////////////////////////////
  float Q[MAX_SAMPLES]={0.};
  float V[MAX_SAMPLES]={0.};
  
  for(TCTmeasurementCollection_t::const_iterator m_itr = FZ320N_ref_2.begin();
      m_itr != FZ320N_ref_2.end();
      m_itr++){
    Q[m_itr-FZ320N_ref_2.begin()] = m_itr->GetWaveIntegral(86e-9,96e-9);
    V[m_itr-FZ320N_ref_2.begin()] = abs(m_itr->GetBias());
  }
  TGraph FZ320N_ref_2_QvsV(FZ320N_ref_2.size()-1,V,Q);
  
  FZ320N_ref_2_QvsV.SaveAs("QvsV.root");

  return 0;
}


#endif

