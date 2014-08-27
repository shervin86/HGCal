#include <TCTmeasurements.h>
#include <iostream>


//average over all the acquisitions of this measurement, regardless the bias voltage applied (biasCheck==false)
TCTspectrum TCTmeasurements::Average(bool checkBias)const{ 
  assert(checkBias==false); /// \todo implement the case of biasCheck
  TCTspectrum meas=acquisition[0];
  meas.clear(); // reset the values
  for(TCTspectrumCollection_t::const_iterator itr = acquisition.begin();
      itr!=acquisition.end();
      itr++){
    //std::cout << meas.GetSamples()[10] << "\t" << itr->GetSamples()[10];
    meas+=*itr;
    //std::cout << "\t" << meas.GetSamples()[10] << std::endl;
  }
  meas/=acquisition.size();
  //std::cout << "\t" << meas.GetSamples()[10] << std::endl;
  
  std::cout << "Average over " << acquisition.size() << " acquisitions" << std::endl;
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
  this->reset(); // remove all the measurements

  //two measurements as place holder
  TCTspectrum m = others.begin()->acquisition[0];
  acquisition.push_back(m);
  acquisition.push_back(m);
  this->clear(); // initialize to 0 the sum

  std::cout << "Average over " << others.size() << " measurements" << std::endl;
  unsigned int n=0;
  for(std::vector<TCTmeasurements>::const_iterator itr = others.begin();
      itr!=others.end();
      itr++){
    assert(checkBias==false);
    //std::cout << itr->Sum(checkBias) << "\t" << itr->Sum2(checkBias) << "\t" << n << std::endl;
    this->acquisition[0]+=itr->Sum(checkBias);
    this->acquisition[1]+=itr->Sum2(checkBias);
    n+=itr->size();

    //// this is the average of the means! average of the average!
    //this->acquisition[0]+=itr->Average(checkBias);
  }
  this->acquisition[0]/=n; // mean
  this->acquisition[1]=(this->acquisition[1]/n-this->acquisition[0]*this->acquisition[0]).Sqrt(); // std. dev. = x2/n - (x/n)^2
  return;
}

TGraphErrors *TCTmeasurements::GetAverageWaveForm(std::string graphName, std::string graphTitle) const{
  assert(acquisition.size()==2);
  TGraph *g = GetWaveForm(0, "","");
  TGraphErrors *ge = new TGraphErrors(g->GetN(), acquisition[0].GetTimes(), acquisition[0].GetSamples(), NULL, acquisition[1].GetSamples());
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
