#include <TCTspectrum.h>
#include <iostream>
#include <TString.h>
#include <TAxis.h>
#include <iomanip>

void TCTspectrum::dump(std::ostream& f, std::string legend, const TCTspectrum *rmsSpectrum) const{
  float	dt = GetTimeScanUnit();
  float	t  = 0;
  f<< "time\tsample\t";
  if(rmsSpectrum!=NULL) f << "stdDev\t";
  f << legend << "\n";

  if(rmsSpectrum!=NULL){
      
    for(unsigned int i=0; i< _nSamples; i++){
      f << std::scientific << std::setprecision(4) << t << "\t" << _samples[i] << "\t" << rmsSpectrum->GetSamples()[i] << "\n";
      t	+= dt;
    }
  }else {
    for(unsigned int i=0; i< _nSamples; i++){
      f << t << "\t" << _samples[i] << "\n";
      t	+= dt;
    }
  }
  f<< std::endl;
}

TCTspectrum TCTspectrum::operator- (const TCTspectrum& rhs) const{
    TCTspectrum newSpectrum(*this); // copy to have the informations
    newSpectrum.clear();            // make null the spectrum
    const TCTspectrum& lhs = *this;
    unsigned int nSamples=lhs.GetN();
    assert(nSamples==rhs.GetN());

    //if(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit()){
    const float *samples_rhs = rhs.GetSamples();
    const float *samples_lhs = lhs.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      newSpectrum.GetSamples()[i]=samples_lhs[i]-samples_rhs[i];;
    }
    return newSpectrum;
};

TCTspectrum TCTspectrum::operator* (const TCTspectrum& rhs) const{
  TCTspectrum newSpectrum(*this);
  newSpectrum.clear();
  const TCTspectrum& lhs = *this;
  unsigned int nSamples=lhs.GetN();
  assert(nSamples==rhs.GetN());

  //if(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit()){
  const float *samples_rhs = rhs.GetSamples();
  const float *samples_lhs = lhs.GetSamples();
  for(unsigned int i=0; i < nSamples; i++){
    newSpectrum.GetSamples()[i]=samples_lhs[i]*samples_rhs[i];
  }
  return newSpectrum;
};

  /// \todo fixit, assert should work
TCTspectrum& TCTspectrum::operator -= (const TCTspectrum& other){
    unsigned int nSamples=GetN();
    //std::cout << other.GetN() << "\t" << nSamples << std::endl;
    //assert(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit());// other.GetTimeScanUnit());
    if(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit()){
      const float *samples = other.GetSamples();
      for(unsigned int i=0; i < nSamples; i++){
	GetSamples()[i]-=samples[i];
      }
      return *this;
    }else return *this;
  };

TCTspectrum& TCTspectrum::operator += (const TCTspectrum& other){
    unsigned int nSamples=GetN();
    if(nSamples!=other.GetN()){
        std::cout << "nSamples= "<<nSamples << "\t" << other.GetN() << "\t" << GetDiodeName() << std::endl;
    }
    assert(nSamples == other.GetN() && GetTimeScanUnit() == other.GetTimeScanUnit());// other.GetTimeScanUnit());
    const float *samples = other.GetSamples();
    for(unsigned int i=0; i < nSamples; i++){
      GetSamples()[i]+=samples[i];
      
    }
    return *this;
  };

float *TCTspectrum::GetTimes(void) const{
  float *x = new float[MAX_SAMPLES_SPECTRUM];

  unsigned int nSamples = GetN();
  //std::cout << nSamples << std::endl;
  float timeScanUnit = GetTimeScanUnit();

  x[0] = 0;// initial value
  
  for(unsigned i=1; i < nSamples; i++){
    x[i] = x[i-1]+timeScanUnit;
  }
  
  return x;
}

TGraph *TCTspectrum::GetWaveForm(std::string graphName, std::string graphTitle) const{
  if(graphName==""){
    graphName=GetDiodeName()+GetTime()+"_"; 
    char st[10];
    if(GetBias()>0)
      sprintf(st, "p%.0f", fabs(GetBias()));
    else sprintf(st, "m%.0f", fabs(GetBias()));
    graphName+=st;
  }
  
  if(graphTitle=="") graphTitle=graphName;

  Float_t x[MAX_SAMPLES_SPECTRUM], y[MAX_SAMPLES_SPECTRUM];
  const float *_y = GetSamples();

  unsigned int nSamples = GetN();
  //std::cout << nSamples << std::endl;
  float timeScanUnit = GetTimeScanUnit();

  x[0] = 0;// initial value
  y[0] = _y[0];
  
  for(unsigned i=1; i < nSamples; i++){
    x[i] = x[i-1]+timeScanUnit;
    y[i] = _y[i];
    //std::cout << x[i] << "\t" << y[i] << std::endl;
  }

  
  
  TGraph *graph = new TGraph(nSamples, x, y);
  graph->SetName(graphName.c_str());
  graph->SetTitle(graphTitle.c_str());
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.);
  graph->SetFillColor(0);
  graph->Draw("A");
  graph->GetXaxis()->SetTitle("time [s]");
  graph->GetYaxis()->SetTitle("I [A]");

  return graph;
}


float TCTspectrum::GetWaveIntegral(float min, float max, float baselineMean) const{
    if(GetN()<=0){
      std::cerr << "[ERROR] No points for this spectrum: " << GetDiodeName() << "\t" << GetBias() << std::endl;
      assert(GetN()>0);
    }
    assert( (max > 0 && min<max) || max<0);
    assert(min<_timeScanUnit*GetN());

    float integral=0.;
    if(max>0){
      unsigned int binMax = (unsigned int)(max/_timeScanUnit);
	//    std::cout << binMax << "\t" << GetN() << "\t" << _timeScanUnit << "\t" << max << "\t" << max-min << std::endl;
	if(binMax>GetN()) binMax=GetN();

	for(unsigned int i=(unsigned int)(min/_timeScanUnit); i < binMax; i++){
	  integral += _samples[i]-baselineMean;
	}
    }else{
      unsigned int binMax=GetN(), binMin=(unsigned int)(min/_timeScanUnit);
      auto oldSample=_samples[binMin];

     for(unsigned int i=binMin;  i < binMax; i++){
      if(oldSample*_samples[i]>0 || (fabs(oldSample)>fabs(_samples[i])) ){ // if not opposite sign means it has passed the 0
	integral += _samples[i]-baselineMean;
	//	std::cout << "integral = " << integral << "\t" << i << "\t" << _samples[i] << "\t" << oldSample << std::endl;  
      }
      if(i-binMin>10 && oldSample*_samples[i]<0){
	//	std::cout << "--> integral = " << integral << "\t" << i << "\t" << _samples[i] << "\t" << oldSample << std::endl;  
	break;
      }

      //
      oldSample=_samples[i];
  
     }
    }
      

    return integral*_timeScanUnit;
  };

