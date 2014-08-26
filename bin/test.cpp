/// test program
#include <iostream>
//#include <fstream>
#include <boost/program_options.hpp>
#include "configFileParser.h"

#include "TCTmeasurements.h"

#include <TGraph.h>
#include <TMultiGraph.h>
/// \file test.cpp bin/test.cpp 
/// for all the diodes indicatd in the config file this program produces validation plots of single measurements

int main(int argc, char **argv){

  //using namespace boost;
  namespace po = boost::program_options;


  //------------------------------ setting option categories
  po::options_description desc("Main options");

  po::options_description outputOption("Output options");
  std::string imgFormat, outDir;

  po::options_description inputOption("Input options");
  std::string baseDir, configFilename, tctFilename, ivFilename, cvFilename;

   //po::options_description cmd_line_options;
  //cmd_line_options.add(desc).add(fitOption).add(smearOption);

  //------------------------------ adding options' description
  desc.add_options()
    ("help,h","Help message")
    ;
  inputOption.add_options()
    ("baseDir,d", po::value< std::string >(&baseDir)->default_value("data"), "directory with data files")
    ("configFile,f", po::value< std::string >(&configFilename), "File with list of measurements")
    ("tctFile", po::value< std::string >(&tctFilename), "single tct file") // for single validation
    ("ivFile", po::value< std::string >(&ivFilename), "single iv file") // for single validation
    ("cvFile", po::value< std::string >(&cvFilename), "single cv file") // for single validation
    ;
  outputOption.add_options()
    ("imgFormat", po::value<std::string>(&imgFormat)->default_value("eps"),"")
    ("outDir", po::value<std::string>(&outDir),"")
    ;
  
     
  desc.add(inputOption);
  desc.add(outputOption);

  po::variables_map vm;
  //
  // po::store(po::parse_command_line(argc, argv, smearOption), vm);
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cerr << desc << "\n";
    return 1;
  }


  //------------------------------ checking options
  if(!vm.count("configFile") && !vm.count("tctFile") && !vm.count("ivFile") && !vm.count("cvFile")){
    std::cerr << "[ERROR] No input file indicated (mandatory)" << std::endl;
    std::cerr << inputOption << std::endl;
    exit(1);
  }

  //------------------------------ parsing the config file
  configFileParser parser(configFilename);
  
  TCTmeasurements l(baseDir+"/"+parser.GetTCTfilename(0));
  int Vindex=0;
  TGraph *g = l.GetWaveForm(Vindex, "FZ320N_04_DiodeL_5_2014_08_18_15_37_-700 reference ", "");
  g->SaveAs("test.root");
  return 0;


  Vindex=0;
  //------------------------------ list of acquisition
  std::vector<TCTmeasurements> FZ320N_ref;
  //FZ320N_ref.push_back(TCTmeasurements ("data/FZ320N_04_DiodeL_5/2014_08_18_15_12.mct")); // test do not use it
  FZ320N_ref.push_back(TCTmeasurements("data/FZ320N_04_DiodeL_5/2014_08_18_15_37.mct")); //
  //FZ320N_ref.push_back(TCTmeasurements("data/FZ320N_04_DiodeL_5/2014_08_19_13_53.mct")); // 
  FZ320N_ref.push_back(TCTmeasurements("data/FZ320N_04_DiodeL_5/2014_08_19_13_57.mct")); // 
  //FZ320N_ref.push_back(TCTmeasurements("data/FZ320N_04_DiodeL_5/2014_08_19_16_39.mct")); // 


  //--------------- baseline
  std::vector<TCTmeasurements> baselines;
  //baselines.push_back(TCTmeasurements("data/FZ320N_04_DiodeL_5/2014_08_19_16_42.mct")); // with reference diode
  baselines.push_back(TCTmeasurements("data/FZ320N_010_DiodeL_11/2014_08_18_17_29.mct"));
  //--------------- neutron irradiated 4e14
  std::vector<TCTmeasurements> FZ320N_010_DiodeL_11;
  FZ320N_010_DiodeL_11.push_back(TCTmeasurements("data/FZ320N_010_DiodeL_11/2014_08_18_17_08.mct"));
  
  std::vector<TCTmeasurements> FZ320N_03_DiodeL_8;
  
  std::vector<TCTmeasurements> FZ320P_01_DiodeL_11;
  std::vector<TCTmeasurements> FZ320P_03_DiodeL_9;

  //--------------- neutron irradiated 6e14
  std::vector<TCTmeasurements> FZ320N_03_DiodeL_9;
  std::vector<TCTmeasurements> FZ320N_04_DiodeL_9;
  
  std::vector<TCTmeasurements> FZ320P_04_DiodeL_8;
  std::vector<TCTmeasurements> FZ320P_06_DiodeL_9;
  

  //------------------------------ calculate baseline
  TCTmeasurements baseline=baselines[0]; //initialize
  baseline.clear();
  baseline.Average(baselines);

  //------------------------------ plot the waveforms
  TMultiGraph FZ320N_multigraph;
  FZ320N_multigraph.SetName("multi");
  g = FZ320N_ref[0].GetWaveForm(Vindex, "FZ320N_04_DiodeL_5_2014_08_18_15_37_-700 reference ", "");
  
  
  FZ320N_multigraph.Add(g,"l");

  g= FZ320N_ref[1].GetWaveForm(Vindex, "FZ320N_04_DiodeL_5_2014_08_19_13_57_-700 reference", "");
  g->SetLineColor(kRed);
  FZ320N_multigraph.Add(g,"l");


  g = baselines[0].GetWaveForm(0, "FZ320N_010_DiodeL_11_2014_08_18_17_29_-150 irr. baseline 0", "");
  g->SetLineColor(kBlue);
  g->SetLineStyle(2);
  FZ320N_multigraph.Add(g,"l");
 
  g = baselines[0].GetWaveForm(1, "FZ320N_010_DiodeL_11_2014_08_18_17_29_-150 irr. baseline 1", "");
  g->SetLineColor(kBlue); 
  g->SetLineStyle(1);

  FZ320N_multigraph.Add(g,"l");

  g = baseline.GetWaveForm(0, "FZ320N_010_DiodeL_11_2014_08_18_17_29_-150 irr. baseline average", "");
  g->SetLineColor(kBlue);
  g->SetLineStyle(0);
  FZ320N_multigraph.Add(g,"l");


  g = FZ320N_010_DiodeL_11[0].GetWaveForm(Vindex, "FZ320N_010_DiodeL_11_2014_08_18_17_08_-700 irradiated 4e14", "");
  g->SetLineColor(kGreen);
  FZ320N_multigraph.Add(g,"l");
  //std::cout << "N= " << FZ320N_010_DiodeL_11[0].GetN() << std::endl;
  FZ320N_010_DiodeL_11[0]-=baseline.GetAverageMeasurement();
  g = FZ320N_010_DiodeL_11[0].GetWaveForm(0, "FZ320N_010_DiodeL_11_2014_08_18_17_08_-700 irradiated 4e14 baseline subtr.", "");
  g->SetLineColor(kBlack);
  g->SetLineStyle(2);
  FZ320N_multigraph.Add(g,"l");
  
  FZ320N_multigraph.SaveAs("spectra.root");
 

  TGraph FZ320N_ref_2_0_QvsV = FZ320N_ref[0].GetQvsV(86e-9,116e-9);
  FZ320N_ref_2_0_QvsV.SaveAs("QvsV.root");

  TCTmeasurements CCE(FZ320N_ref[0]);
  CCE=FZ320N_ref[0];
  TGraph FZ320N_CCE_2_0_QvsV = CCE.GetCCEvsV(FZ320N_ref[0],86e-9,96e-9);
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

