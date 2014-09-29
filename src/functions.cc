// void gnuplot(configFileParser& parser, measurementMap_t& referenceMap, measurementMap_t& irradiatedMap, CVMap_t& cvMap, IVMap_t& ivMap){
//   std::string type="irr";
//   for(auto 
// 	lines_t::key_type oldVal;
//       for(configFileParser::lines_t::iterator itr = lines.begin();
// 	itr!=lines.end();
// 	itr++){
//       if(oldVal==itr->first) continue;
//       oldVal=itr->first;
//       if(itr->first.substr(0,3)=!=type) continue;	

//       for
//       configFileContent & p = itr->second; //
//       p << std::cout;
//       std::cout << std::endl;
//       }
//   }

// }

typedef std::map<std::string, TCTmeasurements*> splitMeasurements_t;

splitMeasurements_t splitMeasurements2(measMap_t m){
  //make a new map, separating the measurements of the same type
  splitMeasurements_t newMap;
  
  for(measMap_t::iterator type_itr=m.begin();  // loop over different types
      type_itr!=m.end();
      type_itr++){
    std::string typeName=type_itr->first;
    for(TCTmeasurementsCollection_t::iterator v_itr=type_itr->second.begin(); // loop over measurements of the same type
	v_itr!=type_itr->second.end();
	v_itr++){
      unsigned long long int index = v_itr-type_itr->second.begin();
      std::string newName = typeName+"x";  // assign a new name to the measurement
      std::cout << newName << "\t" << index << std::endl;
      newName+=std::to_string(index);
	 
      newMap[newName]=&(*v_itr);
    }
  }
  return newMap;
}

measurementMap_t splitMeasurements(measMap_t m){
  //make a new map, separating the measurements of the same type
  measurementMap_t newMap;
  
  for(measMap_t::const_iterator type_itr=m.begin();  // loop over different types
      type_itr!=m.end();
      type_itr++){
    std::string typeName=type_itr->first;
    for(TCTmeasurementsCollection_t::const_iterator v_itr=type_itr->second.begin(); // loop over measurements of the same type
	v_itr!=type_itr->second.end();
	v_itr++){
      unsigned long long int index = v_itr-type_itr->second.begin();
      std::string newName = typeName+"x";  // assign a new name to the measurement
      //std::cout << newName << "\t" << index << std::endl;
      newName+=std::to_string(index);
	 
      newMap[newName]=*v_itr;
    }
  }
  return newMap;
}

// return true if ok
bool checkCompatiblity(measMap_t &baselinesMap, std::string checkName, float& RMS){
  RMS=0;
  bool returnValue=false;

  //make a new map, separating the measurements of the same type
  measurementMap_t newBaselines = splitMeasurements(baselinesMap);
  
  std::cout << "[INFO] Checking baselines compatibility...\n" 
	    << "  - Take one baseline as reference\n" 
	    << "  - For each baseline measurement (several bias voltages applied), take the average\n"
	    << "  - Subtract the reference baseline\n"
	    << "  - All baseline measurements (average over several bias voltages) should have mean=0 or compatbile with 0 within std. dev.\n" 
	    << "  - Std. dev. = electronic noise" 
	    << std::endl;
  // take one measurement as reference
  TCTmeasurements baselinesDiffRef = newBaselines.begin()->second; // init the baselineDiff: take the first as reference
  std::cout << "[INFO] BaselineDiffRef bias voltage: " << baselinesDiffRef.GetSpectrum(baselinesDiffRef.size()-1).GetBias() << std::endl;
  std::cout << std::endl;
  std::cout << "measurement\tmean\tstd.dev." << std::endl;
  TMultiGraph gg;
  TMultiGraph gg2;
  gg.SetName((checkName+"_m30").c_str());
  gg2.SetName((checkName+"_m20").c_str());
  int index=0, index2=0;
  for(measurementMap_t::const_iterator itr = newBaselines.begin();
      itr!=newBaselines.end();
      itr++){

    // for each baseline plot all the spectra
    TMultiGraph *allSpectra = itr->second.GetAllSpectra(itr->first+"_allSpectra",itr->first+"_allSpectra");
    allSpectra->Write();

    TCTspectrum spec = itr->second.GetAverage();
    spec-=baselinesDiffRef.GetSpectrum(baselinesDiffRef.size()-1); // only 0 by construction
    

    float mean = spec.GetMean(0.,1e10); //full range
    float rms  = spec.GetRMS(0.,1e10);
    unsigned int n=spec.GetNsamples(0.,1e10);
    RMS+=rms;

    std::cout << std::setprecision(2) << std::setw(12) << itr->first << "\t" << mean << " +/- " << rms/sqrt(n)
	      << "\t" << rms << std::endl;
    
    // check that baselines are compatible
    assert(abs(mean)<rms); 
    returnValue = abs(mean)<rms;
    TGraph *g = spec.GetWaveForm(itr->first,itr->first);
    //std::cout << spec.GetTemperature() << std::endl;
    g->SetLineWidth(2);
    g->SetMarkerSize(0);
    if(spec.GetTemperature()<-25){
      g->SetLineColor(pPaletteColor[index+1]);
      gg.Add(g,"l");
    index++;
    } else {
      gg2.Add(g,"l");
      g->SetLineColor(pPaletteColor[index2+1]);
    index2++;
    }
  }
  RMS/=(index+index2);
  //gg.SaveAs("checkBaselines.root");
  gg.Draw("A");
  gg.GetXaxis()->SetTitle("time [s]");
  gg.GetYaxis()->SetTitle("I [A]");
  gg.Write();

  gg2.Draw("A");
  gg2.GetXaxis()->SetTitle("time [s]");
  gg2.GetYaxis()->SetTitle("I [A]");
  gg2.Write();

  return returnValue;
};





bool checkMeasurementBaseline(float RMS, float signalStart, float signalEnd, measMap_t baselinesMap, std::string checkName="checkReferencesBaseline"){
  std::cout << "------------------------------\n"
	    << "[STATUS] Checking Measurement Baseline\n"
	    << "         Checking if the baseline associated to a give measurement is correct:\n"
	    << "      - subtract the baseline from the spectra\n"
	    << "      - check if the pre-signal part of the spectra has mean=0 within the estimated uncertainty from the noiseRMS/sqrt(N)\n"
	    << "      - estimated noise is given by the std.dev. of the baselines"
	    << std::endl;
  bool returnValue=true;

  //make a new map, separating the measurements of the same type
  measurementMap_t newBaselines = splitMeasurements(baselinesMap);
  //splitMeasurements_t newBaselines = splitMeasurements2(baselinesMap);
 
  TMultiGraph gg;
  gg.SetName(checkName.c_str());
  int index=0;
   std::cout << "Name    " << "\t" << "mean" << "\t" << "meanError" << "\t" <<"rms" << "\t" << "RMS" << std::endl;
  for(measurementMap_t::iterator itr = newBaselines.begin();
      itr!=newBaselines.end();
      itr++){
   
    // std::cout << itr->first << "\t" << ref<< "\t" << ref->size() << refstd::endl;
    //if(ref==NULL) continue;
    //unsigned int i=0; //i < itr->second.size(); i++){ // loop over all bias voltages
    for(auto biter = itr->second.begin(); biter!=itr->second.end(); biter++){
      TCTspectrum& spec = biter->second;
      if(spec.empty()) continue; ///\todo check why
      
      float mean = spec.GetMean(0.,signalStart);
      float rms  = spec.GetRMS(0.,signalStart);
      unsigned int n    = spec.GetNsamples(0., signalStart);
      //      assert(n>0);
      float meanError= rms/sqrt(n);
      // if(fabs(mean)>2*meanError){
      // 	spec-=mean;
      // 	mean=spec.GetMean(0.,signalStart);
      // }
      if(rms>RMS*1.5 || (fabs(mean)>2*meanError)){
	if(fabs(spec.GetBias())>100) returnValue=false; 
       
	if(rms>RMS*1.5 || returnValue==false){
	  spec.SetNoisy();
	std::cout << std::setprecision(2) <<std::setw(9)<< itr->first << "\t" << mean << "\t+/- " << meanError
		  << "\t" << rms << "\t" << RMS << "\t" << (signalEnd-signalStart)/spec.GetTimeScanUnit()*mean << "\t";
	std::cout << "<--- " << std::setprecision(4) << spec.GetBias() << std::endl;
	}
	RMS+=rms;
	//TGraph *g = spec.GetWaveForm(itr->first);
	//g->SaveAs(("tmp/"+itr->first+".root").c_str());
      }else{
	//std::cout << itr->first << "\t" << mean << "\t" << rms << "\t" << RMS << std::endl;
      }

      // remove the baseline bulk current
      //spec-=mean;
      //      std::cout << mean << "\t" << spec.GetMean(0., signalStart) << std::endl;
    //returnValue = abs(mean)<rms;
      TGraph *g = spec.GetWaveForm(itr->first,checkName+"_"+itr->first);
      // std::cout << g->GetN() << std::endl;
      if(index<100) g->SetLineColor(fPaletteColor[index+1]);
      else g->SetLineColor(kGray);
      gg.Add(g,"l");
      index++;
    }
  }
  //RMS/=index;
  //gg.SaveAs("checkBaselines.root");
  gg.Write();
  
  return returnValue;
}

bool checkMeasurementBaseline2(float RMS, float signalStart, measMap_t baselinesMap, std::string checkName="checkReferencesBaseline"){
  std::cout << "------------------------------\n"
	    << "[STATUS] Checking Measurement Baseline\n"
	    << "         Checking if the baseline associated to a give measurement is correct:\n"
	    << "      - subtract the baseline from the spectra\n"
	    << "      - check if the pre-signal part of the spectra has mean=0 within the estimated uncertainty from the noiseRMS/sqrt(N)\n"
	    << "      - estimated noise is given by the std.dev. of the baselines"
	    << std::endl;
  bool returnValue=true;

  //make a new map, separating the measurements of the same type
  //measurementMap_t newBaselines = splitMeasurements(baselinesMap);
  splitMeasurements_t newBaselines = splitMeasurements2(baselinesMap);
  
  TMultiGraph gg;
  gg.SetName(checkName.c_str());
  int index=0;
  std::cout << "Name    " << "\t" << "mean" << "\t" << "meanError" << "\t" <<"rms" << "\t" << "RMS" << std::endl;
  for(splitMeasurements_t::iterator itr = newBaselines.begin();
      itr!=newBaselines.end();
      itr++){
    TCTmeasurements *ref = itr->second;
    //std::cout << itr->first << "\t" << ref<< "\t" << ref->size() << std::endl;
    if(ref==NULL) continue;
    for(unsigned int i=0; i < ref->size(); i++){ // loop over all bias voltages
      
      TCTspectrum& spec = ref->GetSpectrum(i);
      //    std::cout << "i=" << i << "\t" << spec.GetN() << std::endl;
      float mean = spec.GetMean(0.,signalStart);
      float rms  = spec.GetRMS(0.,signalStart);
      unsigned int n    = spec.GetNsamples(0., signalStart);
      if(rms>RMS*1.5 || (mean>RMS)){
	if(fabs(spec.GetBias())>100) returnValue=false; 

	std::cout << std::setprecision(2) <<std::setw(9)<< itr->first << "\t" << mean << "\t+/- " << rms/sqrt(n)
		  << "\t" << rms << "\t" << RMS << "\t";
	std::cout << "<--- " << std::setprecision(4) << spec.GetBias() << std::endl;
	if(rms>RMS*1.5) spec.SetNoisy();
  //  RMS+=rms;
	//TGraph *g = spec.GetWaveForm(itr->first);
	//g->SaveAs(("tmp/"+itr->first+".root").c_str());
      }else{
	//std::cout << itr->first << "\t" << mean << "\t" << rms << "\t" << RMS << std::endl;
      }
    
      // remove the baseline bulk current
      //spec-=mean;
      std::cout << mean << "\t" << spec.GetMean(0., signalStart) << std::endl;
    //returnValue = abs(mean)<rms;
    TGraph *g = spec.GetWaveForm(itr->first,checkName+"_"+itr->first);
    // if(index<20)
    //   g->SetLineColor(kBlack) ; //fPaletteColor[index+1]);
    // else g->SetLineColor(kGray);
    gg.Add(g,"l");
    index++;
    }
  }
  //RMS/=index;
  //gg.SaveAs("checkBaselines.root");
  gg.Write();
  
  return returnValue;
}

void SetReferences(measMap_t& irradiatedsMap, measurementMap_t& referenceMap, configFileParser& parser){

  for(measMap_t::iterator m_itr=irradiatedsMap.begin();
      m_itr!=irradiatedsMap.end();
      m_itr++){

    for(TCTmeasurementsCollection_t::iterator v_itr=m_itr->second.begin(); // loop over references of the same type
    	v_itr!=m_itr->second.end();
    	v_itr++){
      std::string basName = parser.find(m_itr->first)->second.GetReference();
      if(referenceMap.count(basName)==0){
	std::cerr << "[ERROR] No reference found for diode: " << m_itr->first << std::endl;
	exit(1);
      }
      const TCTmeasurements& ref = referenceMap[basName];
      v_itr->SetReference(ref, basName);
    }
  }
}

void SetReferences(measurementMap_t& irradiatedsMap, measurementMap_t& referenceMap, configFileParser& parser){
  std::cout << "[STATUS] Setting references" << std::endl;
  for(measurementMap_t::iterator m_itr=irradiatedsMap.begin();
      m_itr!=irradiatedsMap.end();
      m_itr++){
    TCTmeasurements *v_itr = &(m_itr->second);
      std::string basName = parser.find(m_itr->first)->second.GetReference();
      if(referenceMap.count(basName)==0){
	std::cerr << "[ERROR] No reference found for diode: " << m_itr->first << std::endl;
	exit(1);
      }
      const TCTmeasurements& ref = referenceMap[basName];
      v_itr->SetReference(ref, basName);
  }
}
