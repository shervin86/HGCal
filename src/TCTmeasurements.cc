#include <TCTmeasurements.h>
#include <iostream>


//average over all the acquisitions of this measurement, regardless the bias voltage applied (biasCheck==false)
TCTspectrum TCTmeasurements::Average(bool checkBias)const{ 
  assert(checkBias==false); /// \todo implement the case of biasCheck
  TCTspectrum meas=acquisition[0];
  meas.clear(); // reset the values
  unsigned int n=0;
  for(TCTspectrumCollection_t::const_iterator itr = acquisition.begin();
      itr!=acquisition.end();
      itr++){
    //std::cout << meas.GetSamples()[10] << "\t" << itr->GetSamples()[10];
    if(itr->GetBias()!=0)
      meas+=*itr;
    //else 
    //  std::cout << "\t" << meas.GetSamples()[9] << itr->GetBias() << std::endl;
    n++;
  }
  meas/=n; //acquisition.size();
  //std::cout << "\t" << meas.GetSamples()[10] << std::endl;
  
  //std::cout << "Average over " << acquisition.size() << " acquisitions" << std::endl;
  return meas;
}


TCTspectrum TCTmeasurements::Sum(bool checkBias)const{ 
  assert(checkBias==false); /// \todo implement the case of biasCheck
  TCTspectrum meas=acquisition[0];
  meas.clear(); // reset the values
  for(TCTspectrumCollection_t::const_iterator itr = acquisition.begin();
      itr!=acquisition.end();
      itr++){
    meas+=*itr;
  }
  return meas;
}

TCTspectrum TCTmeasurements::Sum2(bool checkBias)const{ 
  assert(checkBias==false); /// \todo implement the case of biasCheck
  TCTspectrum meas=acquisition[0];
  meas.clear(); // reset the values
  for(TCTspectrumCollection_t::const_iterator itr = acquisition.begin();
      itr!=acquisition.end();
      itr++){
    meas+=*itr * *itr;
  }
  return meas;
}

//average over several measuremets 
void TCTmeasurements::Average(std::vector<TCTmeasurements> others, bool checkBias){
  _isAverage=true;

  
  assert(others.size()!=0);
  
  *this=*others.begin();

  // merge of the measurements
  std::vector<unsigned int> nV; nV.assign(size(),1);
  for(std::vector<TCTmeasurements>::const_iterator itr = (++others.begin()); // start from the second
      itr!=others.end();
      itr++){

    // loop over all the spectra
    for(TCTspectrumCollection_t::const_iterator sp_itr = itr->acquisition.begin();
	sp_itr!=itr->acquisition.end();
	sp_itr++){
      float bias = sp_itr->GetBias();
      unsigned int index=GetSpectrumIndexWithAbsBias(bias);  // return size() if not found
      if(index==size()){
	std::cout << index << std::endl;
	acquisition.push_back(*sp_itr); // add a new one if not found
	nV.push_back(1);
      } else{
	acquisition[index]+=*sp_itr; // sum the new one if found
	nV[index]++;
      }
    }
     
  }
 
  if(checkBias){
    for(unsigned int i=0; i < size(); i++){
      acquisition[i]/=nV[i];
    }
  } else{
    _isAverage=true;
    unsigned int n=nV[0];
    TCTspectrum temp=acquisition[0], tempRMS=acquisition[0]*acquisition[0];
    
    //    reset();
    //acquisition.push_back(temp);
    //acquisitionRMS.push_back(temp);
    for(unsigned int i=1; i < size(); i++){
      temp+=acquisition[i];
      tempRMS+=acquisition[i]*acquisition[i];
      n+=nV[i];
    }
    std::cout << "N=" <<n << std::endl;
    temp/=n; // mean
    tempRMS=(tempRMS/n-temp*temp).Sqrt(); // std. dev. = x2/n - (x/n)^2
    reset();
    acquisition.push_back(temp);
    acquisitionRMS.push_back(tempRMS);
  }
  std::cout << "RMS = " << acquisitionRMS[0].GetSamples()[10] << std::endl;
  acqAbsBiasIndex.clear();
  SetAcqAbsBias();
  return;
}

#ifdef shervin
//average over several measuremets 
void TCTmeasurements::Average(std::vector<TCTmeasurements> others){
  _isAverage=true;
  this->reset(); // remove all the measurements

  assert(others.size()!=0);

  //two measurements as place holder
  TCTspectrum m = others.begin()->acquisition[0];
  acquisition.push_back(m);
  acquisitionRMS.push_back(m);
  this->clear(); // initialize to 0 the sum

  //std::cout << "Average over " << others.size() << " measurements" << std::endl;
  unsigned int n=0;
  for(std::vector<TCTmeasurements>::const_iterator itr = others.begin();
      itr!=others.end();
      itr++){
    assert(checkBias==false);
    //std::cout << itr->Sum(checkBias) << "\t" << itr->Sum2(checkBias) << "\t" << n << std::endl;
    this->acquisition[0]+=itr->Sum(checkBias);
    this->acquisitionRMS[0]+=itr->Sum2(checkBias);
    n+=itr->size();
       
    //// this is the average of the means! average of the average!
    //this->acquisition[0]+=itr->Average(checkBias);
  }
  this->acquisition[0]/=n; // mean
  this->acquisitionRMS[0]=(this->acquisitionRMS[0]/n-this->acquisition[0]*this->acquisition[0]).Sqrt(); // std. dev. = x2/n - (x/n)^2
  return;
}
#endif

TGraphErrors *TCTmeasurements::GetAverageWaveForm(std::string graphName, std::string graphTitle) const{
  assert(acquisition.size()==1);
  TGraph *g = GetWaveForm(0, "","");
  TGraphErrors *ge = new TGraphErrors(g->GetN(), acquisition[0].GetTimes(), acquisition[0].GetSamples(), NULL, acquisitionRMS[0].GetSamples());
  delete g;
  return ge;
}

/// index = acquisition index
TGraph *TCTmeasurements::GetWaveForm(unsigned int index, std::string graphName, std::string graphTitle)const{ 
  assert(index<acquisition.size());
  //assert(acquisition[index].GetBias()!=
  //  std::cout << acquisition[index].GetBias() << "\t" << index << std::endl;
  //for(unsigned int i=0; i < acquisition.size(); i++){
  //  if(acquisition[i].GetBias()==-600) index=i;
  //}
  // std::cout << graphName << "\tBias = " << acquisition[index].GetBias()<< std::endl;	
  return acquisition[index].GetWaveForm(graphName, graphTitle);
};

TMultiGraph *TCTmeasurements::GetAllSpectra(std::string graphName, std::string graphTitle)const{ 
  
  if(graphName=="") graphName="validationTCT";
  TMultiGraph *baselineGraphs = new TMultiGraph();
  baselineGraphs->SetName(graphName.c_str());
  baselineGraphs->SetTitle(graphTitle.c_str());
  
  for(unsigned int i=0; i<size(); i++){ // loop over all bias voltage
      TGraph *gg = GetWaveForm(i,"",""); //
      gg->SetLineColor(fPaletteColor[i]);
      gg->SetLineStyle(1);
      baselineGraphs->Add(gg, "l");
    }
	return baselineGraphs;
      }
    
