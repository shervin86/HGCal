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
#include "diode.h"

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
			   kGreen+2,   
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



void PlotQvsV(measMap_t& referencesMap, float startSignal, float endSignal){
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


void PlotQvsV(measurementMap_t& referencesMap, float startSignal, float endSignal){
  //--------------- plots of QvsV for references
  for(measurementMap_t::iterator m_itr=referencesMap.begin();
      m_itr!=referencesMap.end();
      m_itr++){

    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((m_itr->first+"_QvsV").c_str());
    baselineGraphs.SetTitle("QvsV");
    
    TCTmeasurements *v_itr = &m_itr->second;
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
  
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("U [V]");
    baselineGraphs.GetYaxis()->SetTitle("Q [C]");

    baselineGraphs.Write();
  }
}

void PlotCCEvsV(configFileParser& parser, measMap_t& referencesMap, float startSignal, float endSignal){
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
	TGraph *ggref= v_itr->GetReference()->GetCCEvsV(startSignal, endSignal);
	//TGraph *gg = v_itr->GetQvsV(1.00e-7,1.10e-7); //
	//gg->SetLineColor(fPaletteColor[i]);
	gg->SetTitle(parser.GetLegend(m_itr->first).c_str());
	gg->SetLineStyle(1);
	gg->SetMarkerStyle(20);
	gg->Draw("A");
	gg->GetXaxis()->SetTitle("U [V]");
	gg->GetYaxis()->SetTitle("CCE");
	baselineGraphs.Add(gg, "p");

	ggref->SetTitle(parser.GetLegend(v_itr->GetReferenceType()).c_str());
	ggref->SetLineStyle(1);
	ggref->SetMarkerStyle(23);
	ggref->SetMarkerColor(2);
	ggref->Draw("A");
	ggref->GetXaxis()->SetTitle("U [V]");
	ggref->GetYaxis()->SetTitle("CCE");
	baselineGraphs.Add(ggref, "p");
	//}
    }
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("U [V]");
    baselineGraphs.GetYaxis()->SetTitle("CCE");

    baselineGraphs.Write();
  }
}


void PlotCCEvsV(configFileParser& parser, measurementMap_t& referencesMap, float startSignal, float endSignal){
  //--------------- plots of CCEvsV for references
  for(measurementMap_t::iterator m_itr=referencesMap.begin();
      m_itr!=referencesMap.end();
      m_itr++){

    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((m_itr->first+"_CCEvsV").c_str());
    baselineGraphs.SetTitle("CCEvsV");
    TCTmeasurements *v_itr = &m_itr->second;
    TGraph *gg = v_itr->GetCCEvsV(startSignal,endSignal); //
    TGraph *ggref= v_itr->GetReference()->GetCCEvsV(startSignal, endSignal);
	
    //TGraph *gg = v_itr->GetQvsV(1.00e-7,1.10e-7); //
    //gg->SetLineColor(fPaletteColor[i]);
    gg->SetTitle(parser.GetLegend(m_itr->first).c_str());
	gg->SetLineStyle(1);
	gg->SetMarkerStyle(20);
	gg->Draw("A");
	gg->GetXaxis()->SetTitle("U [V]");
	gg->GetYaxis()->SetTitle("CCE");
	baselineGraphs.Add(gg, "p");

	ggref->SetTitle(parser.GetLegend(v_itr->GetReferenceType()).c_str());
	ggref->SetLineStyle(1);
	ggref->SetMarkerStyle(23);
	ggref->SetMarkerColor(2);
	ggref->Draw("A");
	ggref->GetXaxis()->SetTitle("U [V]");
	ggref->GetYaxis()->SetTitle("CCE");
	baselineGraphs.Add(ggref, "p");

    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("U [V]");
    baselineGraphs.GetYaxis()->SetTitle("CCE");

    baselineGraphs.Write();
  }
}

void DumpCCE(configFileParser& parser, measurementMap_t& referencesMap, float bias, float startSignal, float endSignal){
  //--------------- plots of CCEvsV for references
  for(measurementMap_t::iterator m_itr=referencesMap.begin();
      m_itr!=referencesMap.end();
      m_itr++){
    TCTmeasurements *v_itr = &m_itr->second;
    float CCE=v_itr->GetCCE(startSignal, endSignal, bias);
    parser.SetCCE(m_itr->first, CCE);
    std::cout << "[DUMP CEE]" << m_itr->first << "\t" << CCE << std::endl;
    

  }
}

void SetIV(configFileParser& parser, IVMap_t ivmap){
  for(IVMap_t::iterator itr = ivmap.begin();
      itr!= ivmap.end();
      itr++){
    std::string type=itr->first;
    IVmeasurement& iv = itr->second;
    iv.FindVdep(false);

    float Vdep = iv.GetVdep();
    float Irev = iv.GetIrev();
    float IrevError = iv.GetIrevError();
    parser.SetVdepIV(type, Vdep, 0.);
    parser.SetIrev(type, Irev, IrevError);
    std::cout << Irev << "\t" << IrevError << std::endl;
  }
}


void SetCV(configFileParser& parser, CVMap_t ivmap){
  for(CVMap_t::iterator itr = ivmap.begin();
      itr!= ivmap.end();
      itr++){
    std::string type=itr->first;
    CVmeasurement& cv = itr->second;
    cv.FindVdep(false);

    float Vdep = cv.GetVdep();
    float Irev = cv.GetCend();
    float IrevError = cv.GetCendError();
    parser.SetVdepCV(type, Vdep, 0.);
    parser.SetCend(type, Irev, IrevError);
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

      
      if(basName==m_itr->first){ // use the V=0 as baseline
	TCTmeasurements *ref = new TCTmeasurements();
	ref->AddSpectrum((*v_itr)[0]->second, true);
	v_itr->SetBaseline(*ref);
      }else{

      if(referenceMap.count(basName)==0) continue;
      assert(referenceMap.count(basName)!=0);
      const TCTmeasurements& ref = referenceMap[basName];
      v_itr->SetBaseline(ref);
      }
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

  //  Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 100);
  //  for (int i=0;i<100;i++) fPaletteColor[i] = FI+i;

  //using namespace boost;
  namespace po = boost::program_options;


  //------------------------------ setting option categories
  po::options_description desc("Main options");

  
  po::options_description outputOption("Output options");
  std::string imgFormat, outDir;
  float timeMin, timeMax;

  po::options_description inputOption("Input options");
  std::string baseDir, configFilename, tctFilename, ivFilename, cvFilename, ivGRFilename, cvGRFilename;
  std::string tctType;
  std::vector< std::string > tctBaselineFiles, tctReferenceFiles;
  float spectrumDumpBias=0;
   //po::options_description cmd_line_options;
  //cmd_line_options.add(desc).add(fitOption).add(smearOption);

  //------------------------------ adding options' description
  desc.add_options()
    ("help,h","Help message")
    ("CV", "do CV analysis")
    ("IV", "do IV analysis")
    ("TCT", "do TCT analysis")
    ("checkBaselines", "do a check on the baselines for TCT")
    ("onlyBaselines", "")
    ("noBaselineSubtraction","")
    ;
  inputOption.add_options()
    ("baseDir,d", po::value< std::string >(&baseDir)->default_value("data/shervin"), "directory with data files")
    ("configFile,f", po::value< std::string >(&configFilename), "File with list of measurements")
    //    ("type", po::value< std::vector< std::string > >(&types), "")
    ("tctType,t", po::value<std::string >(&tctType), "Index in the config file")
    ("tctFile", po::value< std::string >(&tctFilename), "single tct file") // for single validation
    ("tctBaseline", po::value< std::vector<std::string> >(&tctBaselineFiles), "tct measurement uses ad baseline, can be called multiple times") 
    ("tctReference", po::value< std::vector<std::string> >(&tctReferenceFiles), "tct measurement uses ad baseline, can be called multiple times") 
    ("ivFile", po::value< std::string >(&ivFilename), "single iv file") // for single validation
    ("cvFile", po::value< std::string >(&cvFilename), "single cv file") // for single validation
    ("spectrumDump", po::value< float>(&spectrumDumpBias), "bias voltage")
    ;
  outputOption.add_options()
    ("outDir", po::value<std::string>(&outDir)->default_value("result"),"")
    ("imgFormat", po::value<std::string>(&imgFormat)->default_value("eps"),"")
    ("timeMin", po::value<float>(&timeMin)->default_value(0.085e-6),"")
    ("timeMax", po::value<float>(&timeMax)->default_value(0.096e-6),"")
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

  //  float startSignal=timeMin, endSignal=timeMax;
  // if a TCT file is given as input, this means fast validation of the measurement and exit
  TCTmeasurementsCollection_t baselines;
  TCTmeasurements baseline;
  TCTmeasurements reference;
  TCTmeasurements tct;


  //------------------------------ parsing the config file
  // all the information is stored in parser
  configFileParser parser(configFilename);


  if(vm.count("tctBaseline")){
    for(std::vector<std::string>::const_iterator itr=tctBaselineFiles.begin();
	itr!=tctBaselineFiles.end();
	itr++){
      std::cout << "[STATUS] Reading TCT baseline file: ";
      std::cout << *itr << std::endl;
      baselines.push_back(TCTmeasurements(*itr,-999));
      
    }
  }

    // check the compatiblity between baseline measurements
  if(baselines.size()>0)
    //make the average of the baselines
    baseline.Average(baselines);

  if(vm.count("tctReference")){
    if(vm.count("tctReference")>1){
      std::cerr << "[ERROR] Multiple tctReference measurement defined, but not yet implemented" << std::endl;
      return 1;
    }
    reference=TCTmeasurements(tctReferenceFiles[0],-999);
  }

  //  reference-=baseline.GetAverageMeasurement();
  

  if(vm.count("tctFile")){
    tct=TCTmeasurements(tctFilename, -999);
    std::cout << tct.GetSpectrum(0).GetN() << std::endl;
    std::ofstream tf("tmp/test.dat");
    tct[626]->second.dump(tf);
    //tct-=baseline.GetAverageMeasurement();
    tct.SetReference(reference, "");
    
    tct.SetPaletteColor(fPaletteColor, 50);
    TMultiGraph *g = tct.GetAllSpectra("tctValidation", "");
    g->SaveAs("tmp/tctValidation/tctValidation.root");
    TCanvas c("tctValidation","");
    g->Draw("A");
    g->GetXaxis()->SetRangeUser(timeMin, timeMax);
    c.SaveAs(("tmp/tctValidation/tctValidation."+imgFormat).c_str());

    TGraph *QvsV = tct.GetQvsV(timeMin, timeMax);
    TGraph *QvsVref = reference.GetQvsV(timeMin, timeMax);
    QvsVref->SaveAs("QvsVref.root");
    TMultiGraph multi;
    QvsV->SetMarkerStyle(22);
    multi.Add(QvsV,"p");
    multi.Add(QvsVref, "p");
    multi.Draw("A");
    multi.SaveAs("tmp/tctValidation/QvsV.root");

    ofstream ff_out("result/tmp/result.dat");
    configFileContent p;
    diode d("tmp", p,  tct, timeMin, timeMax);
    //if(ivItr!=ivMap.end()) d.SetIV(ivItr->second); 
    //if(cvItr!=cvMap.end()) d.SetCV(cvItr->second); 
    //diode d(p, ivMap[oldVal], cvMap[oldVal], irradiatedMap[oldVal], timeMin, timeMax);
    d.dump(ff_out);

    ff_out.close();
    return 0;
    TGraph *CCEvsV = tct.GetCCEvsV(timeMin, timeMax);
    CCEvsV->SaveAs("tmp/tctValidation/CCEvsV.root");
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

  // all the measurements in the config file
  measMap_t baselinesMap, referencesMap, irradiatedsMap;
  
  // averaged measurements of the same type
  measurementMap_t baselineMap, referenceMap, irradiatedMap;  

  IVMap_t ivMap, ivGRMap;
  CVMap_t cvMap, cvGRMap;

  // import all the measurements indicated in the config file
  IVimport ivimporter;  
  CVimport cvimporter;

  std::string basType, refType;
  if(vm.count("tctType")){
    if(parser.check(tctType)==false){
      std::cerr << "[ERROR] ID not found for tct measurement" << std::endl;
      exit(1);
    }
    auto itr = parser.find(tctType);
    basType=itr->second.TCTbaseline;
    refType=itr->second.TCTreference;
  }


  for(unsigned int i=0; i < parser.size(); i++){
    std::string type = parser.GetType(i);
    if(vm.count("tctType") && type!=basType && type!=refType && type!=tctType) continue;

    ivFilename = parser.GetIVfilename(i);
    ivGRFilename = parser.GetIVGRfilename(i);

    std::cout << "[INFO] IV file: " << ivFilename << std::endl;
    cvFilename = parser.GetCVfilename(i);
    cvGRFilename = parser.GetCVGRfilename(i);
    tctFilename = parser.GetTCTfilename(i);
    TCTmeasurements *tct=NULL;
    if(tctFilename!=""){ 
      tct=new TCTmeasurements(baseDir+"/"+tctFilename, parser.GetTemperature(i)); // add baseline measurement
      tct->SetPaletteColor(fPaletteColor,50);
    }

    // skip -20 degree
    //    if(type.rfind("20")==type.size()-2) continue;
    //    std::cout << type << "\t" << type.rfind("20") << std::endl;
    if(type.find("bas")!=std::string::npos){
      baselinesMap[type].push_back(*tct);
    } else if(type.find("ref")!=std::string::npos){
      if(ivFilename!="")  ivMap[type]= ivimporter.ImportFromFile(baseDir+"/"+ivFilename);
      if(cvFilename!="")  cvMap[type]= cvimporter.ImportFromFile(baseDir+"/"+cvFilename);
      if(ivGRFilename!="")  ivGRMap[type]= ivimporter.ImportFromFile(baseDir+"/"+ivGRFilename);
      if(cvGRFilename!="")  cvGRMap[type]= cvimporter.ImportFromFile(baseDir+"/"+cvGRFilename);

      if(tctFilename!="") referencesMap[type].push_back(*tct);
    } else if(type.find("irr")!=std::string::npos){
      if(ivFilename!="") ivMap[type]= ivimporter.ImportFromFile(baseDir+"/"+ivFilename);
      if(cvFilename!="") cvMap[type]= cvimporter.ImportFromFile(baseDir+"/"+cvFilename);
      if(ivGRFilename!="") ivGRMap[type]= ivimporter.ImportFromFile(baseDir+"/"+ivGRFilename);
      if(cvGRFilename!="") cvGRMap[type]= cvimporter.ImportFromFile(baseDir+"/"+cvGRFilename);

      if(tctFilename!="") irradiatedsMap[type].push_back(*tct);
    } else if(type.find("rbl")!=std::string::npos){
      TCTmeasurements *referenceBaseline = new TCTmeasurements(*tct);
      referenceBaseline->reset();
      referenceBaseline->AddSpectrum((*tct)[0]->second, true);
      baselinesMap[type].push_back(*referenceBaseline);
    } else{
      std::cout << type << std::endl;
      assert(false); /// \todo replace with message and exception
    }
    
    //std::cout << parser.GetTCTfilename(i, baseDir) << std::endl;;
  }





  std::cout << "[INFO] Spectrum integration range: " << timeMin << " - " << timeMax << std::endl;
 
  TFile outFile("outFile.root","RECREATE");
  outFile.cd();
  TCanvas *c1 = new TCanvas("c1","");


  if(vm.count("CV")){
    std::cout << "------------------------------\n";
    std::cout << "[STATUS] CV measurements" << std::endl;
    // make individual plots
    for(CVMap_t::iterator iter = cvMap.begin();
	iter!=cvMap.end();
	iter++){

      continue;
      std::string type=iter->first;
      CVmeasurement& cv= iter->second;
      cv.FindVdep(false);

      cv.SetPaletteColor(pPaletteColor, 6); 
      std::vector<std::string> freqs = cv.GetFrequencies();
      unsigned int iFreq=0; 
      for(std::vector<std::string>::const_iterator freq_itr = freqs.begin();
	  freq_itr!=freqs.end();
	  freq_itr++, iFreq++){
	
	TGraph *g = cv.GetCvsV(iFreq);
	float Vdep = cv.GetVdep();
	float Cend = cv.GetCend();
	//    std::cout << Vdep << "\t" << Irev << std::endl;

	parser.SetVdepCV(type, Vdep, 0.);
	parser.SetCend(type, Cend, 0.);
	g->SetMarkerStyle(20+(freq_itr-freqs.begin()));

	//g->SetName(iter->first.c_str());
      char filename[100];
      sprintf(filename, "%s #mu m, %s, %.0f C, %s", parser.GetThickness(iter->first).c_str(), parser.GetFluence(iter->first).c_str(), cv.GetTemperature(), freq_itr->c_str());
      g->SetTitle(filename);
      
    // g->GetXaxis()->SetTitle("U [V]");
    // g->GetYaxis()->SetTitle("I [A]");
    // g->SetName((type+"_iv").c_str());
    // g->SetTitle(parser.GetLegend(type).c_str());
    // g->Write();
       

      //sprintf(filename,"tmp/CV/%s-%.0f-%s-CvsV.root",cv.GetDiodeName().c_str(),cv.GetTemperature(), TString(freq_itr->c_str()).ReplaceAll(" ","").Data());
 #ifdef DEBUG     
      std::cout << cv.GetDiodeName()
		<< "\t" << parser.GetThickness(iter->first)
	      << "\t" << std::setprecision(2) << cv.GetTemperature() 
      //      << std::setprecision(3)
      //	      << "\t" << cv.GetVdep()
      //	      << "\t" << cv.GetIrev()
	      << std::endl;
#endif
    g->Draw("A");
    g->GetXaxis()->SetTitle("U [V]");
    g->GetYaxis()->SetTitle("1/C^{2} [1/F^{2}]");
    //g->SaveAs(filename);
      }

       char filename[100];
       sprintf(filename, "%s #mu m, %s, %.0f C", parser.GetThickness(iter->first).c_str(), parser.GetFluence(iter->first).c_str(), cv.GetTemperature());
     TMultiGraph *gg = cv.GetCvsV(iter->first, filename);      
 //      sprintf(filename,"tmp/CV/%s-%.0f-CvsV.root",cv.GetDiodeName().c_str(),cv.GetTemperature());
 // TCanvas c("c","");    
   gg->Draw("A");
      gg->GetXaxis()->SetTitle("U [V]");
      gg->GetYaxis()->SetTitle("1/C^{2} [F^{-2}]");
 //      g->SaveAs(filename);
      //gg->GetYaxis()->SetRangeUser(1e-12,1e-6);
 //      c.SetLogy();
 //      c.BuildLegend(0.9,0.4,1,0.6);
      gg->Write();
 //      c.SaveAs(TString(filename).ReplaceAll(".root",("."+imgFormat).c_str()));
    }
  SetCV(parser, cvMap);
  //  SetCVGR(parser, cvGRMap);
  // now you have all the measurements as vectors
  }

  if(vm.count("IV")){
    std::cout << "------------------------------\n";
    std::cout << "[STATUS] IV measurements" << std::endl;

  // make individual plots
    for(IVMap_t::iterator itr = ivMap.begin();
      itr!=ivMap.end();
      itr++){
    std::string type=itr->first;
    IVmeasurement& iv = itr->second;
    iv.FindVdep(false);

    float Vdep = iv.GetVdep();
    float Irev = iv.GetIrev();
    float IrevError = iv.GetIrevError();
    parser.SetVdepIV(type, Vdep, 0.);
    parser.SetIrev(type, Irev, IrevError);
    TGraph *g = iv.GetIvsV();
    g->SetMarkerStyle(20);
    g->Draw("A");

    g->GetXaxis()->SetTitle("U [V]");
    g->GetYaxis()->SetTitle("I [A]");
    g->SetName((type+"_iv").c_str());
    g->SetTitle(parser.GetLegend(type).c_str());
    g->Write();
    } 
    SetIV(parser, ivMap);
    SetIV(parser, ivGRMap);
  //SetIVGR(parser, ivGRMap);

  }

  if(vm.count("TCT")){
  std::cout << "------------------------------------------------------------\n"; 
  std::cout << "[STATUS] TCT measurements\n" << std::endl;

  // make the average over measurements of the same type for the baselines
  float RMS=0.;
  for(measMap_t::iterator m_itr=baselinesMap.begin();  // loop over different baselines
      m_itr!=baselinesMap.end();
      m_itr++){

    // set the palette
    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over measurements of the same type
	v_itr!=m_itr->second.end();
	v_itr++){
      v_itr->SetPaletteColor(fPaletteColor, 50);
    }

    baselineMap[m_itr->first]=TCTmeasurements(); // declare the new baseline (will be the average)
    baselineMap[m_itr->first].Average(m_itr->second,false); // assign the average over all the measurements of the same type
  }



  std::cout << "[STATUS] " << "Checking baselines compatibility" << std::endl;
  // for every baseline type make the average of all the measurements
  
  if(checkCompatiblityGnuplot(baselinesMap, outDir+"/baselines", RMS)) 
    std::cout << "[INFO] Baselines compatiblity... [OK]" << std::endl;

  // if(checkCompatiblity(baselinesMap, "checkBaselines", RMS)) 
  //   std::cout << "[INFO] Baselines compatiblity... [OK]" << std::endl;
  
  std::cout << "------------------------------\n";
  std::cout << "[STATUS] "<< "plot individual measurements" << std::endl;
  
  
  if(vm.count("onlyBaselines")) return 0;


  std::cout << "[INFO] " << "Setting baselines' pointers" << std::endl;
  SetBaselines(referencesMap, baselineMap, parser);
  SetBaselines(irradiatedsMap, baselineMap, parser);

  
  std::cout << "[STATUS] Plotting all the references" << std::endl;
  //--------------- plot of all the references
  for(measMap_t::const_iterator type_itr=referencesMap.begin(); // loop over all references' types
      type_itr!=referencesMap.end();
      type_itr++){

    std::ofstream f(outDir+"/"+type_itr->first+"/spectra_baseline.dat");

    // root
    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((type_itr->first+"_spectra_baseline").c_str());
    baselineGraphs.SetTitle(parser.GetLegend((type_itr->first)).c_str());
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

    // gnuplot
      v_itr->DumpAllSpectra(f);

    }
    // TH2F h("paletteHist","",2000,-1000,1000,100,0,100);    
    // for(float val=-1000; val<=1000; val+=10){
    //   h.Fill(val,0.,val);
    // }
    // h.Write();
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("time [s]");
    baselineGraphs.GetYaxis()->SetTitle("I [A]");
    baselineGraphs.GetXaxis()->SetRangeUser(timeMin,timeMin+3e-8);
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
    baselineGraphs.SetTitle(parser.GetLegend(type_itr->first).c_str());

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
	//if(!vm.count("noBaselineSubtraction")) (*v_itr) -= baselineMap[basName].GetAverageMeasurement();
	if(!vm.count("noBaselineSubtraction")) (*v_itr) -= (*v_itr).GetBaseline().GetAverageMeasurement();
	for(auto spec_itr = v_itr->begin(); spec_itr!=v_itr->end(); spec_itr++){ // loop over all the bias voltages
	  TCTspectrum& spec = spec_itr->second;
	  if(spec.empty()) continue; ///\todo check why
	  float mean = spec.GetMean(0., timeMin);
	  //	  std::cout << type_itr->first << "\t" << spec_itr->first << "\t" << mean; // << std::endl;
	  spec-=mean;
	  //	  std::cout <<  "\t" << spec.GetMean(0., timeMin) << std::endl;

	  //spec-=-5e-3;
	} 
      }

      for(unsigned int i=0; i < v_itr->size(); i++){ // loop over all bias voltage	
	TGraph *gg = v_itr->GetWaveForm(i,"",""); //
	gg->SetLineColor(fPaletteColor[i]);
	if(i==v_itr->size()-1){
	  gg->SetLineColor(kBlack);
	  gg->SetLineWidth(5);
	}
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
    baselineGraphs.GetYaxis()->SetTitle("I [A]");
    baselineGraphs.GetXaxis()->SetRangeUser(timeMin,timeMin+3e-8);
    baselineGraphs.Write();
  
  }
  fixedBiasGraphs.Write();
  
  //  checkCompatiblity(referencesMap, "checkReferences", RMS);

  

  if(!checkMeasurementBaseline(RMS, timeMin, timeMax, referencesMap, "checkReferencesBaseline")){
    std::cerr << "[WARNING] check association of references and baselines" << std::endl;
    //return 1;
  }

  


  // for every reference type make the average of all the measurements
  for(measMap_t::const_iterator m_itr=referencesMap.begin();  // loop over different measurements
      m_itr!=referencesMap.end();
      m_itr++){
    referenceMap[m_itr->first]=TCTmeasurements(); // declare the new reference (will be the average)
    referenceMap[m_itr->first].Average(m_itr->second, true); // assign the average over all the measurements of the same type
    //referenceMap[m_itr->first]=m_itr->second[0]; // take the first one for the moment 
  }


  //--------------- remove baseline from measurements and plot them
  for(measMap_t::iterator m_itr=irradiatedsMap.begin();
      m_itr!=irradiatedsMap.end();
      m_itr++){
    
    TMultiGraph baselineGraphs;
    baselineGraphs.SetName((m_itr->first+"_spectra").c_str());
    baselineGraphs.SetTitle(parser.GetLegend((m_itr->first)).c_str());
    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over references of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
      std::string basName = (parser.find(m_itr->first))->second.GetBaseline();

      std::ofstream f(outDir+"/"+m_itr->first+"/biasSpectra.dat");
      std::ofstream f_bas(outDir+"/"+m_itr->first+"/biasSpectra_baseline.dat");
      // subtract the baseline 
      //      std::cout << (parser.find(m_itr->first))->second.type << "\t" << basName << "\t" << std::endl;
      if(baselineMap.count(basName)!=0){
	(*v_itr).DumpAllSpectra(f_bas);
	(*v_itr) -= baselineMap[basName].GetAverageMeasurement();
	(*v_itr).DumpAllSpectra(f);
      }
	for(auto spec_itr = v_itr->begin(); spec_itr!=v_itr->end(); spec_itr++){ // loop over all the bias voltages
	  TCTspectrum& spec = spec_itr->second;
	  if(spec.empty()) continue; ///\todo check why
	  float mean = spec.GetMean(0., timeMin);
	  //	  std::cout << type_itr->first << "\t" << spec_itr->first << "\t" << mean; // << std::endl;
	  spec-=mean;
	  //	  std::cout <<  "\t" << spec.GetMean(0., timeMin) << std::endl;

	  //spec-=-5e-3;
	} 

      unsigned int i=0;
      for(auto itr=v_itr->begin(); itr!=  v_itr->end(); itr++){ // loop over all bias voltage
	TGraph *gg = v_itr->GetWaveForm(itr,"",""); //
	if(gg->GetN()==0) std::cout << i << "\t" << itr->first<< std::endl;
	gg->SetLineColor(fPaletteColor[i]);
	gg->SetLineStyle(1);
	if(i==v_itr->size()-1){
	  gg->SetLineColor(kBlack);
	  gg->SetLineWidth(5);
	}

	baselineGraphs.Add(gg, "l");
	i++;
      }
    }
    baselineGraphs.Draw("A");
    baselineGraphs.GetXaxis()->SetTitle("time [s]");
    baselineGraphs.GetYaxis()->SetTitle("I [A]");

    baselineGraphs.Write();
  
  }

  if(!checkMeasurementBaseline(RMS, timeMin, timeMax, irradiatedsMap, "checkIrradiatesBaseline")){
    std::cerr << "[WARNING] check association of irradiated diodes and baselines" << std::endl;
    //return 1;
  }

  // for every irradiate type make the average of all the measurements
  for(measMap_t::const_iterator m_itr=irradiatedsMap.begin();  // loop over different measurements
      m_itr!=irradiatedsMap.end();
      m_itr++){
    std::ofstream f(outDir+"/"+m_itr->first+"/spectra.dat");
    irradiatedMap[m_itr->first]=TCTmeasurements(); // declare the new irradiate (will be the average)
    irradiatedMap[m_itr->first].Average(m_itr->second,true); // assign the average over all the measurements of the same type
    
    // gnuplot
    irradiatedMap[m_itr->first].DumpAllSpectra(f);

    //irradiateMap[m_itr->first]=m_itr->second[0]; // take the first one for the moment 
  }

 // if(!checkMeasurementBaseline(RMS, timeMin, irradiatedsMap, "checkIrradiatesBaseline")){
 //   std::cerr << "[WARNING] check association of irradiated diodes and baselines" << std::endl;
 //   return 1;
 //  }

  //  std::cout << timeMin << "\t" << timeMax << "\t" <<
  //  irradiatedsMap.begin()->second[0].GetSpectrum(0).GetTimeScanUnit() << "\t" << irradiatedsMap.begin()->second[0].GetSpectrum(0).GetNsamples(timeMin, timeMax) << std::endl;
  PlotQvsV(referenceMap, timeMin,timeMax);
  //  return 0;
  PlotQvsV(irradiatedMap, timeMin,timeMax);
  //return 0;
  //SetReferences(irradiatedsMap, referenceMap, parser);
  SetReferences(irradiatedMap, referenceMap, parser);
  PlotCCEvsV(parser,irradiatedMap, timeMin,timeMax);


  outFile.Close();

  //bias Qref Qirr CCE IV CV455 CV1MHz 
  //  gnuplot(parser, referenceMap, irradiatedMap, cvMap, ivMap);

  std::cout << "------------------------------CCE at 700 V" << std::endl;
  DumpCCE(parser, irradiatedMap, 600, timeMin, timeMax);
  
  std::cout << "------------------------------CCE at 1000 V" << std::endl;
  //DumpCCE(parser, irradiatedMap, 1000, timeMin,timeMax);

  
  // std::cout << "------------------------------CCE at 400 V" << std::endl;
  // DumpCCE(parser, irradiatedMap, 400, timeMin,timeMax);

  }

  parser.Dump();

    
  std::ofstream f_out("test.dat");
  std::string typeMatch="ref";
  configFileParser::lines_t::key_type oldVal;
  for(auto itr = parser.begin(); itr!=parser.end(); itr++){
    if(oldVal==itr->first) continue;
    oldVal=itr->first;
    if(oldVal.substr(0,3)!=typeMatch) continue;	
    std::string dir="result/"+oldVal+"/";
    system(("[ -d "+dir+" ] || mkdir -p "+dir).c_str());
    std::ofstream ff_out(dir+"result.dat");
    //std::cout << itr->first << std::endl;
//       for
    configFileContent & p = itr->second; //
    //    p << std::cout;
    //    std::cout << irradiatedMap[oldVal].GetReferenceType() << std::endl;
    auto ivItr = ivMap.find(oldVal);
    auto cvItr = cvMap.find(oldVal);
    auto ivGRItr = ivGRMap.find(oldVal);
    auto cvGRItr = cvGRMap.find(oldVal);

    diode d(oldVal, p,  referenceMap[oldVal], timeMin, timeMax);
    if(ivItr!=ivMap.end()) d.SetIV(ivItr->second); 
    if(cvItr!=cvMap.end()) d.SetCV(cvItr->second); 
    if(ivGRItr!=ivGRMap.end()) d.SetIVGR(ivGRItr->second); 
    if(cvGRItr!=cvGRMap.end()) d.SetCVGR(cvGRItr->second); 
    
    //diode d(p, ivMap[oldVal], cvMap[oldVal], irradiatedMap[oldVal], timeMin, timeMax);
    d.dump(ff_out);
    ff_out.close();
  }

  typeMatch="irr";
  for(auto itr = parser.begin(); itr!=parser.end(); itr++){
    if(oldVal==itr->first) continue;
    oldVal=itr->first;
    if(oldVal.substr(0,3)!=typeMatch) continue;	
    std::string dir="result/"+oldVal+"/";
    system(("[ -d "+dir+" ] || mkdir -p "+dir).c_str());
    std::ofstream ff_out(dir+"result.dat");
    //std::cout << itr->first << std::endl;
//       for
    configFileContent & p = itr->second; //
    //    p << std::cout;
    //    std::cout << irradiatedMap[oldVal].GetReferenceType() << std::endl;
    auto ivItr = ivMap.find(oldVal);
    auto cvItr = cvMap.find(oldVal);
    auto ivGRItr = ivGRMap.find(oldVal);
    auto cvGRItr = cvGRMap.find(oldVal);

    
    diode d(oldVal, p,  irradiatedMap[oldVal], timeMin, timeMax);
    if(ivItr!=ivMap.end()) d.SetIV(ivItr->second); 
    if(cvItr!=cvMap.end()) d.SetCV(cvItr->second); 
    if(ivGRItr!=ivGRMap.end()) d.SetIVGR(ivGRItr->second); 
    if(cvGRItr!=cvGRMap.end()) d.SetCVGR(cvGRItr->second); 

    //diode d(p, ivMap[oldVal], cvMap[oldVal], irradiatedMap[oldVal], timeMin, timeMax);
    d.dump(ff_out);
    ff_out.close();
  }

  f_out.close();

  if(vm.count("spectrumDump")){
    char spectrumDumpBiasStr[30];
    sprintf(spectrumDumpBiasStr, "%.0f", spectrumDumpBias);
    for(auto itr = referenceMap.begin(); itr!=referenceMap.end(); itr++){
      std::ofstream ff_out("result/"+itr->first+"/spectrum_"+spectrumDumpBiasStr+".dat");
      itr->second[spectrumDumpBias]->second.dump(ff_out);
    }
    for(auto itr = irradiatedMap.begin(); itr!=irradiatedMap.end(); itr++){
      std::ofstream ff_out("result/"+itr->first+"/spectrum_"+spectrumDumpBiasStr+".dat");
      itr->second[spectrumDumpBias]->second.dump(ff_out);
    }

  }

    return 0;
  

}

