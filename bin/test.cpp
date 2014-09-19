/// test program
#include <iostream>
#include <iomanip>
//#include <fstream>
#include <boost/program_options.hpp>
#include "configFileParser.h"

#include "TCTmeasurements.h"
#include "IVmeasurement.h"
#include "IVimport.h"
#include "CVmeasurement.h"
#include "CVimport.h"


#include <TFile.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TColor.h>

/// \file test.cpp bin/test.cpp 
/// for all the diodes indicatd in the config file this program produces validation plots of single measurements
Int_t fPaletteColor[150] = {kBlack, kBlue+4, kBlue-1, kBlue+3, kBlue+2, kBlue-2, kBlue+1, kBlue-3, kBlue, kBlue-4, 
			    kBlue-7, kBlue-9, kBlue-10, kAzure-9, kAzure+6, kAzure+10, kCyan-10, kCyan-8, kCyan-6, kCyan-3, kCyan+1, 
			    kGreen-10, kGreen-9, kGreen-7, kGreen-4, kGreen, 
			    kYellow, kOrange-4,  kOrange-2, kOrange, kOrange+6, kOrange+8,
			    kRed,kRed+1, kRed+2, kRed+3, kRed+4, kRed+5,
			    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
			    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 
			    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
			    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
			    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
                            100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                            110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
                            120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
                            130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
                            140, 141, 142, 143, 144, 145, 146, 147, 148, 149};
Int_t pPaletteColor[10] = {kBlack,   
			   kRed,   
			   kBlue,   
			   kGreen+4,   
			   kGray+1,  
			   kMagenta, kRed+2, kBlue+2, kAzure, kYellow+4};


Double_t r[]    = {0., 0.0, 1.0, 1.0, 1.0};
Double_t g[]    = {0., 0.0, 0.0, 1.0, 1.0};
Double_t b[]    = {0., 1.0, 0.0, 0.0, 1.0};
Double_t stop[] = {0., .25, .50, .75, 1.0};


//TCTmeasurements: content of each acquisition (file)
typedef std::vector<TCTmeasurements> TCTmeasurementsCollection_t;

// for every line in the config file allocate a new TCTmeasurements: need to read all the files
typedef std::map<std::string, TCTmeasurementsCollection_t> measMap_t; // all the measurements indicated in the config file

typedef std::map<std::string, TCTmeasurements> measurementMap_t;      // for each type defined in the first column of the config file
                                                                      // keep only one measurement (average of all the indicated ones


typedef std::map<std::string, IVmeasurement> IVMap_t;
typedef std::map<std::string, CVmeasurement> CVMap_t;

#include "src/functions.cc"



void PlotQvsV(measMap_t& referencesMap, float startSignal, float endSignal=1.00e-7){
  //--------------- plots of QvsV for references
  for(measMap_t::iterator m_itr=referencesMap.begin();
      m_itr!=referencesMap.end();
      m_itr++){

    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((m_itr->first+"_QvsV").c_str());
    baselineGraphs.SetTitle("QvsV");
    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over references of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
      // std::cout << m_itr->first << std::endl;
      //for(unsigned int i=0; i < v_itr->size(); i++){ // loop over all bias voltage
	TGraph *gg = v_itr->GetQvsV(startSignal,endSignal); //
	
	//TGraph *gg = v_itr->GetQvsV(1.00e-7,1.10e-7); //
	//gg->SetLineColor(fPaletteColor[i]);
	gg->SetLineStyle(1);
	gg->SetMarkerStyle(20);
	gg->Draw("A");
	gg->GetXaxis()->SetTitle("U [V]");
	gg->GetYaxis()->SetTitle("Q [A s]");
	baselineGraphs.Add(gg, "p");
	//}
    }
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("U [V]");
    baselineGraphs.GetYaxis()->SetTitle("Q [C]");

    baselineGraphs.Write();
  }
}

void PlotCCEvsV(measMap_t& referencesMap, float startSignal, float endSignal=1.00e-7){
  //--------------- plots of CCEvsV for references
  for(measMap_t::iterator m_itr=referencesMap.begin();
      m_itr!=referencesMap.end();
      m_itr++){

    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((m_itr->first+"_CCEvsV").c_str());
    baselineGraphs.SetTitle("CCEvsV");
    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over references of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
            
      //for(unsigned int i=0; i < v_itr->size(); i++){ // loop over all bias voltage
	TGraph *gg = v_itr->GetCCEvsV(startSignal,endSignal); //
	
	//TGraph *gg = v_itr->GetQvsV(1.00e-7,1.10e-7); //
	//gg->SetLineColor(fPaletteColor[i]);
	gg->SetLineStyle(1);
	gg->SetMarkerStyle(20);
	gg->Draw("A");
	gg->GetXaxis()->SetTitle("U [V]");
	gg->GetYaxis()->SetTitle("CCE");
	baselineGraphs.Add(gg, "p");
	//}
    }
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("U [V]");
    baselineGraphs.GetYaxis()->SetTitle("CCE");

    baselineGraphs.Write();
  }
}


void DumpCCE(configFileParser& parser, measMap_t& referencesMap, float bias, float startSignal, float endSignal=1.00e-7){
  //--------------- plots of CCEvsV for references
  for(measMap_t::iterator m_itr=referencesMap.begin();
      m_itr!=referencesMap.end();
      m_itr++){

    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over references of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
      float CCE=v_itr->GetCCE(startSignal, endSignal, bias);
      parser.SetCCE(m_itr->first, CCE);
      std::cout << "[DUMP CEE]" << m_itr->first << "\t" << CCE << std::endl;
    }

  }
}

void SetReferences(measMap_t& irradiatedsMap, measurementMap_t& referenceMap, configFileParser& parser){

  for(measMap_t::iterator m_itr=irradiatedsMap.begin();
      m_itr!=irradiatedsMap.end();
      m_itr++){

    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over references of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
      std::string basName = parser.find(m_itr->first)->second.GetReference();
      assert(referenceMap.count(basName)!=0);
      const TCTmeasurements& ref = referenceMap[basName];
      v_itr->SetReference(ref);
    }
  }
}


void SetBaselines(measMap_t& irradiatedsMap, measurementMap_t& referenceMap, configFileParser& parser){

  // loop over measurements
  for(measMap_t::iterator m_itr=irradiatedsMap.begin();
      m_itr!=irradiatedsMap.end();
      m_itr++){

    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over measurements of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
      // get the baseline
      std::string basName = parser.find(m_itr->first)->second.GetBaseline();
      assert(referenceMap.count(basName)!=0);
      const TCTmeasurements& ref = referenceMap[basName];
      v_itr->SetBaseline(ref);
    }
  }
}

void BaselineNoiseContribution(TCTmeasurements& baseline, float startSignal, float endSignal, float timeScanUnit, float RMS){
    std::cout << "[INFO] Baseline RMS = " << RMS << " V" << std::endl;
    std::cout << "[INFO] Integration interval [start, end]: [" << startSignal << ":" << endSignal << "]" << std::endl;
    std::cout << "[INFO] Baseline noise contribution to integral: sqrt((endSignal-startSignal)/timeStep) *RMS/timeStep: " << sqrt((endSignal-startSignal)/timeScanUnit)*RMS*timeScanUnit << std::endl;
}

#include <TF1.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>
 
int main(int argc, char **argv){

  Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 100);
  //  for (int i=0;i<100;i++) fPaletteColor[i] = FI+i;

  //using namespace boost;
  namespace po = boost::program_options;


  //------------------------------ setting option categories
  po::options_description desc("Main options");
  float startSignal=0.85e-7, endSignal=1.00e-7;
  
  po::options_description outputOption("Output options");
  std::string imgFormat, outDir;
  float timeMin, timeMax;

  po::options_description inputOption("Input options");
  std::string baseDir, configFilename, tctFilename, ivFilename, cvFilename;
  std::vector< std::string > tctBaselines, tctReferences;
   //po::options_description cmd_line_options;
  //cmd_line_options.add(desc).add(fitOption).add(smearOption);

  //------------------------------ adding options' description
  desc.add_options()
    ("help,h","Help message")
    ("CV", "do CV analysis")
    ("IV", "do IV analysis")
    ("checkBaselines", "do a check on the baselines for TCT")
    ("onlyBaselines", "")
    ;
  inputOption.add_options()
    ("baseDir,d", po::value< std::string >(&baseDir)->default_value("data/shervin"), "directory with data files")
    ("configFile,f", po::value< std::string >(&configFilename), "File with list of measurements")
    ("tctFile", po::value< std::string >(&tctFilename), "single tct file") // for single validation
    //    ("tctBaseline", po::value< std::vector<std::string> >(&tctBaselines), "tct measurement uses ad baseline, can be called multiple times") 
    ("ivFile", po::value< std::string >(&ivFilename), "single iv file") // for single validation
    ("cvFile", po::value< std::string >(&cvFilename), "single cv file") // for single validation
    ;
  outputOption.add_options()
    ("outDir", po::value<std::string>(&outDir),"")
    ("imgFormat", po::value<std::string>(&imgFormat)->default_value("eps"),"")
    ("timemMin", po::value<float>(&timeMin)->default_value(0.085e-6),"")
    ("timemMax", po::value<float>(&timeMax)->default_value(0.116e-6),"")
    //("outFile", po::value<std::string>(&outFile),"");
    ;
  
     
  desc.add(inputOption);
  desc.add(outputOption);

  po::variables_map vm;
  //
  // po::store(po::parse_command_line(argc, argv, smearOption), vm);
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  //------------------------------ checking options
  if (vm.count("help")) {
    std::cerr << desc << "\n";
    return 1;
  }

  if(!vm.count("configFile") && !vm.count("tctFile") && !vm.count("ivFile") && !vm.count("cvFile")){
    std::cerr << "[ERROR] No input file indicated (mandatory)" << std::endl;
    std::cerr << inputOption << std::endl;
    exit(1);
  }

  
  // if a TCT file is given as input, this means fast validation of the measurement and exit
  if(vm.count("tctFile")){
    TCTmeasurements tct(tctFilename, -999);
    tct.SetPaletteColor(fPaletteColor, 50);
    TMultiGraph *g = tct.GetAllSpectra("tctValidation", "");
    g->SaveAs("tmp/tctValidation/tctValidation.root");
    TCanvas c("tctValidation","");
    g->Draw("A");
    g->GetXaxis()->SetRangeUser(timeMin, timeMax);
    c.SaveAs(("tmp/tctValidation/tctValidation."+imgFormat).c_str());
    return 0; 
  }

  if(vm.count("ivFile")){
    IVimport ivimporter;
    IVmeasurement iv = ivimporter.ImportFromFile(ivFilename);
    TGraph *g = iv.GetIvsV();
    float Vdep = iv.GetVdep();
    float Irev = iv.GetIrev();
    std::cout << Vdep << "\t" << Irev << std::endl;
    g->SetName("IvsV");
    g->SaveAs("IvsV.root");
    return 0;
  }

  if(vm.count("cvFile")){
    CVimport cvimporter;
    CVmeasurement cv = cvimporter.ImportFromFile(cvFilename);
    TGraph *gg = cv.GetCvsV(3);
    gg->SetName("CvsV");
    gg->SaveAs("CvsV.root");
  
   TMultiGraph *g = cv.GetCvsV("cvValidation", "");
    g->SaveAs("tmp/cvValidation/cvValidation.root");
    TCanvas c("cvValidation","");
    g->Draw("A");
    //g->GetXaxis()->SetRangeUser(timeMin, timeMax);
    c.SaveAs(("tmp/cvValidation/cvValidation."+imgFormat).c_str());
    
    return 0;
  }

  //------------------------------ parsing the config file
  // all the information is stored in parser
  configFileParser parser(configFilename);



  measMap_t baselinesMap, referencesMap, irradiatedsMap;

  measurementMap_t baselineMap, referenceMap;  
  IVMap_t ivMap;
  CVMap_t cvMap;

  IVimport ivimporter;  
  CVimport cvimporter;
  for(unsigned int i=0; i < parser.size(); i++){
    std::string type = parser.GetType(i);
    ivFilename = parser.GetIVfilename(i);
    cvFilename = parser.GetCVfilename(i);
    tctFilename = parser.GetTCTfilename(i);
    if(type.find("bas")!=std::string::npos){
      if(tctFilename!="") baselinesMap[type].push_back(TCTmeasurements(baseDir+"/"+tctFilename, parser.GetTemperature(i))); // add baseline measurement
    } else if(type.find("ref")!=std::string::npos){
      if(ivFilename!="")  ivMap[type]= ivimporter.ImportFromFile(baseDir+"/"+ivFilename);
      if(cvFilename!="")  cvMap[type]= cvimporter.ImportFromFile(baseDir+"/"+cvFilename);
      if(tctFilename!="") referencesMap[type].push_back(TCTmeasurements(baseDir+"/"+tctFilename,parser.GetTemperature(i))); // add reference measurement
    } else if(type.find("irr")!=std::string::npos){
      if(ivFilename!="") ivMap[type]= ivimporter.ImportFromFile(baseDir+"/"+ivFilename);
      if(cvFilename!="") cvMap[type]= cvimporter.ImportFromFile(baseDir+"/"+cvFilename);
      if(tctFilename!="") irradiatedsMap[type].push_back(TCTmeasurements(baseDir+"/"+tctFilename,parser.GetTemperature(i))); // add irradiated measurement
    } else{
      std::cout << type << std::endl;
      assert(false); /// \todo replace with message and exception
    }
    std::cout << parser.GetTCTfilename(i, baseDir) << std::endl;;
  }


  if(vm.count("IV")){
  // make individual plots
  for(IVMap_t::iterator iter = ivMap.begin();
      iter!=ivMap.end();
      iter++){
    IVmeasurement& iv= iter->second;
    TGraph *g = iv.GetIvsV(true, false);
    float Vdep = iv.GetVdep();
    float Irev = iv.GetIrev();
    //    std::cout << Vdep << "\t" << Irev << std::endl;
    g->SetName(iter->first.c_str());
    char filename[100];
    sprintf(filename, "%s #mum, %s, %.0f C", parser.GetThickness(iter->first).c_str(), parser.GetFluence(iter->first).c_str(), iv.GetTemperature());
    g->SetTitle(filename);

    sprintf(filename,"tmp/IV/%s-%.0f-IvsV.root",iv.GetDiodeName().c_str(),iv.GetTemperature());

    std::cout << iv.GetDiodeName()
	      << "\t" << parser.GetThickness(iter->first)
	      << "\t" << std::setprecision(2) << iv.GetTemperature() 
	      << std::setprecision(3)
	      << "\t" << iv.GetVdep()
	      << "\t" << iv.GetIrev()
	      << std::endl;
    TCanvas c("c","");

    g->Draw("A");
    g->GetXaxis()->SetTitle("U [V]");
    g->GetYaxis()->SetTitle("I [A]");
    g->SaveAs(filename);
    c.SaveAs(TString(filename).ReplaceAll(".root",("."+imgFormat).c_str()));
    
  } 
  }


  if(vm.count("CV")){
  // make individual plots
  for(CVMap_t::iterator iter = cvMap.begin();
      iter!=cvMap.end();
      iter++){
    CVmeasurement& cv= iter->second;
    cv.SetPaletteColor(pPaletteColor, 6); 
   std::vector<std::string> freqs = cv.GetFrequencies();
    unsigned int iFreq=0; 
    for(std::vector<std::string>::const_iterator freq_itr = freqs.begin();
	freq_itr!=freqs.end();
	freq_itr++, iFreq++){
      
      TGraph *g = cv.GetCvsV(iFreq);
      //float Vdep = cv.GetVdep();
      //float Irev = cv.GetIrev();
      //    std::cout << Vdep << "\t" << Irev << std::endl;
      g->SetName(iter->first.c_str());
      char filename[100];
      sprintf(filename, "%s #mu m, %s, %.0f C, %s", parser.GetThickness(iter->first).c_str(), parser.GetFluence(iter->first).c_str(), cv.GetTemperature(), freq_itr->c_str());
      g->SetTitle(filename);
      
      sprintf(filename,"tmp/CV/%s-%.0f-%s-CvsV.root",cv.GetDiodeName().c_str(),cv.GetTemperature(), TString(freq_itr->c_str()).ReplaceAll(" ","").Data());
      
      std::cout << cv.GetDiodeName()
		<< "\t" << parser.GetThickness(iter->first)
	      << "\t" << std::setprecision(2) << cv.GetTemperature() 
      //      << std::setprecision(3)
      //	      << "\t" << cv.GetVdep()
      //	      << "\t" << cv.GetIrev()
	      << std::endl;
    g->Draw("A");
    g->GetXaxis()->SetTitle("U [V]");
    g->GetYaxis()->SetTitle("C [F]");
    g->SaveAs(filename);
    }

      char filename[100];
      sprintf(filename, "%s #mu m, %s, %.0f C", parser.GetThickness(iter->first).c_str(), parser.GetFluence(iter->first).c_str(), cv.GetTemperature());
    TMultiGraph *g = cv.GetCvsV(iter->first, filename);      
      sprintf(filename,"tmp/CV/%s-%.0f-CvsV.root",cv.GetDiodeName().c_str(),cv.GetTemperature());
 TCanvas c("c","");    
  g->Draw("A");
      g->GetXaxis()->SetTitle("U [V]");
      g->GetYaxis()->SetTitle("C [F]");
      g->SaveAs(filename);
      g->GetYaxis()->SetRangeUser(1e-12,1e-6);
      c.SetLogy();
      c.BuildLegend(0.9,0.4,1,0.6);
      
      c.SaveAs(TString(filename).ReplaceAll(".root",("."+imgFormat).c_str()));
  } 

  // now you have all the measurements as vectors
  }

 
  TFile outFile("outFile.root","RECREATE");
  outFile.cd();

  float RMS=0.;
  for(measMap_t::iterator m_itr=baselinesMap.begin();  // loop over different baselines
      m_itr!=baselinesMap.end();
      m_itr++){
      for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over measurements of the same type
	v_itr!=m_itr->second.end();
	v_itr++){
	
	v_itr->SetPaletteColor(fPaletteColor, 50);
      }
    baselineMap[m_itr->first]=TCTmeasurements(); // declare the new baseline (will be the average)
    baselineMap[m_itr->first].Average(m_itr->second); // assign the average over all the measurements of the same type
  }

  // procedure to check the compatiblity of the baselines
  if(vm.count("checkBaselines")){
    std::cout << "------------------------------\n"; 
    std::cout << "[STATUS] " << "Checking baselines compatibility" << std::endl;
    // for every baseline type make the average of all the measurements
    float timeScanUnit=baselineMap.begin()->second.GetSpectrum(0).GetTimeScanUnit();
    if(checkCompatiblity(baselinesMap, "checkBaselines", RMS)) 
      std::cout << "[INFO] Baselines compatiblity... [OK]" << std::endl;

    std::cout << "------------------------------\n";
    std::cout << "[STATUS] "<< "plot individual measurements" << std::endl;

  //--------------- plot baseline averages
  // plot individual measurements and the average to check correctness of the average
  // plot also the difference between the averages
 
  for(measMap_t::const_iterator type_itr=baselinesMap.begin();  // loop over different baseline types
      type_itr!=baselinesMap.end();
      type_itr++){

    std::string typeName=type_itr->first;
    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((typeName+"_baselinesAverage").c_str()); // set the name of the type
    baselineGraphs.SetTitle(typeName.c_str()); // set the name of the type
    
    for(TCTmeasurementsCollection_t::const_iterator v_itr=type_itr->second.begin(); // loop over baselines of the same type
    	v_itr!=type_itr->second.end();
    	v_itr++){

      // the average baseline is obtained averaging over all bias voltages assuming no difference among them
      for(unsigned int i=0; i < v_itr->size(); i++){ // loop over all bias voltages
	TGraph *gg = v_itr->GetWaveForm(i,"",""); //
	baselineGraphs.Add(gg, "p");
      }
    }

    TGraphErrors *gg = baselineMap[typeName].GetAverageWaveForm(typeName,"average");
    gg->SetLineColor(kBlue);
    gg->SetFillColor(kAzure-4);
    gg->SetLineWidth(1);
    baselineGraphs.Add(gg,"l3");
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("time [s]");
    baselineGraphs.GetYaxis()->SetTitle("Signal [V]");
    baselineGraphs.Write();
    // TCTmeasurements baselineDiff = (baselineMap[typeName]); 
    // // baselineDiff-=baselinesDiffRef.GetAverageMeasurement();
    // TGraphErrors *gDiff= baselineDiff.GetAverageWaveForm(typeName+"_diff", "average diff");
    // gDiff->SaveAs("gDiff.root");
  }  
  }

 
  if(vm.count("onlyBaselines")) return 0;


  std::cout << "[INFO] " << "Setting baselines' pointers" << std::endl;
  SetBaselines(referencesMap, baselineMap, parser);
  SetBaselines(irradiatedsMap, baselineMap, parser);

  
  std::cout << "[STATUS] Plotting all the references" << std::endl;
  //--------------- plot of all the references
  for(measMap_t::const_iterator type_itr=referencesMap.begin(); // loop over all references' types
      type_itr!=referencesMap.end();
      type_itr++){
    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((type_itr->first+"_spectra_baseline").c_str());
    baselineGraphs.SetTitle((type_itr->first+"_spectra_baseline").c_str());
    for(TCTmeasurementsCollection_t::const_iterator v_itr=type_itr->second.begin(); // loop over references of the same type
    	v_itr!=type_itr->second.end();
    	v_itr++){
      // plot all the bias voltage on the same canvas
      for(unsigned int i=0; i < v_itr->size(); i++){ // loop over all bias voltage
	TGraph *gg = v_itr->GetWaveForm(i,"",""); //
	gg->SetLineColor(fPaletteColor[i]);
	gg->SetLineStyle(2);
	baselineGraphs.Add(gg, "l");
      }
    }
    // TH2F h("paletteHist","",2000,-1000,1000,100,0,100);    
    // for(float val=-1000; val<=1000; val+=10){
    //   h.Fill(val,0.,val);
    // }
    // h.Write();
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("time [s]");
    baselineGraphs.GetYaxis()->SetTitle("Signal [V]");
    baselineGraphs.GetXaxis()->SetRangeUser(startSignal,startSignal+3e-8);
    baselineGraphs.Write();
  }    

  //--------------- remove baseline from reference and plot them
  TMultiGraph fixedBiasGraphs;

  fixedBiasGraphs.SetName(("ref_600")); // put only 600V graphs
  fixedBiasGraphs.SetTitle(fixedBiasGraphs.GetName());
  unsigned int fixedIndex=1;
  for(measMap_t::iterator type_itr=referencesMap.begin();
      type_itr!=referencesMap.end();
      type_itr++){
    
    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((type_itr->first+"_spectra").c_str());
    baselineGraphs.SetTitle(type_itr->first.c_str());

    for(TCTmeasurementsCollection_t::iterator v_itr=type_itr->second.begin(); // loop over references of the same type
    	v_itr!=type_itr->second.end();
    	v_itr++){
      // name of the baseline type
      std::string basName = (parser.find(type_itr->first))->second.GetBaseline();
      if(!parser.check(basName)){
	std::cerr << "[WARNING] No baseline defined for " << type_itr->first << std::endl;
      } else {
	// subtract the baseline 
	//std::cout << basName << std::endl;
	(*v_itr) -= baselineMap[basName].GetAverageMeasurement();
      }

      for(unsigned int i=0; i < v_itr->size(); i++){ // loop over all bias voltage	
	TGraph *gg = v_itr->GetWaveForm(i,"",""); //
	gg->SetLineColor(fPaletteColor[i]);
	gg->SetLineStyle(1);
	baselineGraphs.Add(gg, "l");
	if(abs(v_itr->GetSpectrum(i).GetBias())==600){
	  TGraph *gg1 = v_itr->GetWaveForm(i,"",""); //
       
	  gg1->SetLineColor(fixedIndex);
	  gg1->SetLineStyle(1);
	  fixedBiasGraphs.Add(gg1, "l");
	  fixedIndex++;
	}

      }
    }
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("time [s]");
    baselineGraphs.GetYaxis()->SetTitle("Signal [V]");
    baselineGraphs.GetXaxis()->SetRangeUser(startSignal,startSignal+3e-8);
    baselineGraphs.Write();
  
  }
  fixedBiasGraphs.Write();
  
  //  checkCompatiblity(referencesMap, "checkReferences", RMS);

  if(!checkMeasurementBaseline(RMS, startSignal, referencesMap, "checkReferencesBaseline")){
    std::cerr << "[WARNING] check association of references and baselines" << std::endl;
    //return 1;
  }

  


  // for every reference type make the average of all the measurements
  for(measMap_t::const_iterator m_itr=referencesMap.begin();  // loop over different measurements
      m_itr!=referencesMap.end();
      m_itr++){
    //referenceMap[m_itr->first]=TCTmeasurements(); // declare the new reference (will be the average)
    //referenceMap[m_itr->first].Average(m_itr->second); // assign the average over all the measurements of the same type
    referenceMap[m_itr->first]=m_itr->second[0]; // take the first one for the moment 
  }


  //--------------- remove baseline from measurements and plot them
  for(measMap_t::iterator m_itr=irradiatedsMap.begin();
      m_itr!=irradiatedsMap.end();
      m_itr++){
    
    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((m_itr->first+"_spectra").c_str());
    baselineGraphs.SetTitle(m_itr->first.c_str());
   
    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over references of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
      std::string basName = (parser.find(m_itr->first))->second.GetBaseline();

      // subtract the baseline 
      //      std::cout << (parser.find(m_itr->first))->second.type << "\t" << basName << "\t" << std::endl;
      (*v_itr) -= baselineMap[basName].GetAverageMeasurement();

      for(unsigned int i=0; i < v_itr->size(); i++){ // loop over all bias voltage
	TGraph *gg = v_itr->GetWaveForm(i,"",""); //
	gg->SetLineColor(fPaletteColor[i]);
	gg->SetLineStyle(1);
	baselineGraphs.Add(gg, "l");
      }
    }
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("time [s]");
    baselineGraphs.GetYaxis()->SetTitle("Signal [V]");

    baselineGraphs.Write();
  
  }


  if(!checkMeasurementBaseline(RMS, startSignal, irradiatedsMap, "checkIrradiatesBaseline")){
    //   std::cerr << "[WARNING] check association of irradiated diodes and baselines" << std::endl;
    //return 1;
  }
 
 // if(!checkMeasurementBaseline(RMS, startSignal, irradiatedsMap, "checkIrradiatesBaseline")){
 //   std::cerr << "[WARNING] check association of irradiated diodes and baselines" << std::endl;
 //   return 1;
 //  }

  std::cout << startSignal << "\t" << endSignal << "\t" <<
    irradiatedsMap.begin()->second[0].GetSpectrum(0).GetTimeScanUnit() << "\t" << irradiatedsMap.begin()->second[0].GetSpectrum(0).GetNsamples(startSignal, endSignal) << std::endl;
  PlotQvsV(referencesMap, startSignal);
  //  return 0;
  PlotQvsV(irradiatedsMap, startSignal);
  //return 0;
  SetReferences(irradiatedsMap, referenceMap, parser);
  PlotCCEvsV(irradiatedsMap, startSignal);
  std::cout << "------------------------------CCE at 700 V" << std::endl;
  DumpCCE(parser, irradiatedsMap, 700, startSignal);
  std::cout << "------------------------------CCE at 1000 V" << std::endl;
  DumpCCE(parser, irradiatedsMap, 1000, startSignal);

  parser.Dump();


  outFile.Close();
    return 0;
  
  
  

  return 0;
#ifdef shervin
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

#endif
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

