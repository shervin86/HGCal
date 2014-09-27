#include <TCTmeasurements.h>
#include <iostream>
#include <TAxis.h>

///average over all the acquisitions of this measurement, regardless the bias voltage applied (biasCheck==false) and ignoring the 0 bias spectrum
TCTspectrum TCTmeasurements::GetAverage(bool checkBias)const{ 
  assert(checkBias==false); /// \todo implement the case of biasCheck
  TCTspectrum meas=begin()->second; // copy the first for diodeName, temp and other values
  meas.clear(); // reset the values
  unsigned int n=0;
  for(const_iterator itr = begin(); itr!=end(); itr++){
    assert(!itr->second.isnull());
    if(itr->first>1e-2){ // exclude the 0 bias from the average
      // if(itr->second.empty()) continue;
      // if(itr->second.GetN()!=meas.GetN()){
      // 	std::cerr << "[ERROR] " << itr->first <<" " << itr->second.GetN() << " " << meas.GetN() << std::endl;
      // }
      //assert(itr->second.GetN()==meas.GetN());
      meas+=itr->second;
      n++;
    }
  }
  meas/=n;   
  //std::cout << "Average over " << acquisition.size() << " acquisitions" << std::endl;
  return meas;
}



// TCTspectrum TCTmeasurements::Sum(bool checkBias)const{ 
//   assert(checkBias==false); /// \todo implement the case of biasCheck
//   TCTspectrum meas=acquisition[0];
//   meas.clear(); // reset the values
//   for(TCTspectrumCollection_t::const_iterator itr = acquisition.begin();
//       itr!=acquisition.end();
//       itr++){
//     meas+=*itr;
//   }
//   return meas;
// }

// TCTspectrum TCTmeasurements::Sum2(bool checkBias)const{ 
//   assert(checkBias==false); /// \todo implement the case of biasCheck
//   TCTspectrum meas=acquisition[0];
//   meas.clear(); // reset the values
//   for(TCTspectrumCollection_t::const_iterator itr = acquisition.begin();
//       itr!=acquisition.end();
//       itr++){
//     meas+=*itr * *itr;
//   }
//   return meas;
// }

//average over several measuremets 
void TCTmeasurements::Average(std::vector<TCTmeasurements> others, bool checkBias){
  _isAverage=true;

  if(others.empty()){
    std::cerr << "[ERROR] Vector of measurements for average is empty!" << std::endl;
    exit(1);
  }

  reset(); // remove all the measurements already present

    //  *this=*others.begin(); /// copy from the first measurement

  // merge of the measurements
  std::map<float,unsigned int> nV; 
  for(std::vector<TCTmeasurements>::const_iterator itr = (others.begin()); // start from the second
      itr!=others.end();
      itr++){

    // loop over all the spectra
    for(TCTmeasurements::const_iterator sp_itr = itr->begin(); sp_itr!=itr->end(); sp_itr++){
      float bias = sp_itr->first;
      auto spec_itr = (*this)[bias]; // return end() if not found
      if(sp_itr->second.GetN()==0){
	std::cerr << "[ERROR] No samples in this spectrum, skipping: " << sp_itr->first <<" " << sp_itr->second.GetDiodeName() << std::endl;
	continue;
      }
      if(spec_itr==end()){
	auto sp2_itr=sp_itr;
	sp2_itr++;
	acquisition.insert(sp_itr, sp2_itr);
	acquisitionRMS[bias]=sp_itr->second * sp_itr->second; // sum2
	nV[bias]=1;
	// std::cout << itr-others.begin() << "\t" <<bias << "\t" 
	// 	  << (*this)[bias]->first << "\t" << begin()->second.GetDiodeName() << std::endl;
      } else{
	auto spec2_itr = begin();
	auto specRMS_itr=acquisitionRMS.begin();
	auto nV_itr=nV.begin();
	
	unsigned int index=0;
	while(spec2_itr==spec_itr){
	  spec2_itr++;
	  index++;
	}
	
	// std::cout << index << "\t bias="  << bias << "\t" 
	// 	  << (*this)[bias]->first << "\t" << spec_itr->first <<"\t" <<begin()->second.GetDiodeName() << std::endl;
	std::advance(specRMS_itr, index);
	std::advance(nV_itr, index);
	spec_itr->second+=sp_itr->second; // sum the new one if found
	//specRMS_itr->second+=sp_itr->second * sp_itr->second; // sum2
	nV_itr->second++;
      }

    }
  }
  if(checkBias){
    // nV and acquisition are ordered by bias voltage, one can increment both iterators
    auto nVitr=nV.begin();
    iterator RMSitr=acquisitionRMS.begin();
    for(iterator itr=begin() ; itr!=end(); itr++,nVitr++,RMSitr++){ 
      assert(!itr->second.isnull());
      itr->second /= nVitr->second;
      RMSitr->second = (RMSitr->second/nVitr->second - itr->second*itr->second).Sqrt(); // RMS= sqrt(x2mean-xmean*xmean)
    }
  } else{
    // make the average over all the measurements regardless the bias voltage
    _isAverage=true;
    unsigned int n=nV.begin()->second;
    //  std::cout << begin()->second.GetDiodeName() << std::endl;
    TCTspectrum temp(begin()->second);

  TCTspectrum tempRMS=(temp*temp);   
    auto nVitr=++nV.begin();
    iterator RMSitr=++acquisitionRMS.begin();
    for(iterator itr=++begin(); itr!=end(); itr++,nVitr++,RMSitr++){  // start from the second element
      assert(!itr->second.isnull());
      temp+=itr->second;
      tempRMS += itr->second*itr->second;
      n+=nVitr->second;
    }
    
    //std::cout << "N=" <<n << std::endl;
    temp/=n; // mean
    tempRMS=(tempRMS/n-temp*temp).Sqrt(); // std. dev. = x2/n - (x/n)^2
    //reset();
    acquisitionAverage=temp;
    acquisitionAverageRMS=tempRMS;
  }
  //std::cout << "RMS = " << acquisitionAverageRMS.GetSamples()[10] << std::endl;

  return;
}


TGraphErrors *TCTmeasurements::GetAverageWaveForm(std::string graphName, std::string graphTitle) const{
  if(acquisitionAverage.isnull()){
    std::cerr << "[ERROR] Average spectrum not calulated yet" << std::endl;
    exit(1);
  }
  TGraphErrors *g = new TGraphErrors(acquisitionAverage.GetN(), acquisitionAverage.GetTimes(), acquisitionAverage.GetSamples(), NULL, acquisitionAverageRMS.GetSamples());
  if(graphTitle=="") graphTitle="Average";
  g->SetTitle(graphTitle.c_str());
  g->Draw("A");
  g->GetXaxis()->SetTitle("time [s]");
  g->GetYaxis()->SetTitle("I [A]");
  return g;
}

TGraph *TCTmeasurements::GetWaveForm(const_iterator itr, std::string graphName, std::string graphTitle)const{ 
  assert(itr!=end());
  assert(!itr->second.isnull());
  return itr->second.GetWaveForm(graphName, graphTitle);
}

/// index = acquisition index
TGraph *TCTmeasurements::GetWaveForm(unsigned int index, std::string graphName, std::string graphTitle)const{ 
  auto itr=begin();
  std::advance(itr, index);
  return GetWaveForm(itr, graphName, graphTitle);
};

TMultiGraph *TCTmeasurements::GetAllSpectra(std::string graphName, std::string graphTitle)const{ 
  
  if(graphName=="") graphName="validationTCT";
  TMultiGraph *baselineGraphs = new TMultiGraph();
  baselineGraphs->SetName(graphName.c_str());
  //  baselineGraphs->SetTitle(graphTitle.c_str());

  unsigned int i=0;
  for(auto itr=begin(); itr!=end(); itr++, i++){ // loop over all bias voltages
    //if(itr->first<1) continue;
    char title[100];
    sprintf(title, "U = %.0f V", itr->first); 
    TGraph *gg = GetWaveForm(itr,"",title); //
    //std::cout << fPaletteColor[i] << std::endl;
    gg->SetLineColor(fPaletteColor[i]);
    gg->SetLineStyle(1);
    if(itr->first<1){
      gg->SetLineWidth(3);
      gg->SetLineColor(kBlack);
    }
    baselineGraphs->Add(gg, "l");
  }
  baselineGraphs->Draw("A");
  baselineGraphs->GetXaxis()->SetTitle("time [s]");
  baselineGraphs->GetYaxis()->SetTitle("I [A]");

  return baselineGraphs;
}
    
