#include <TCTspectrum.h>
#include <iostream>
#include <TString.h>
#include <TAxis.h>
float *TCTspectrum::GetTimes(void) const{
  float *x = new float[MAX_SAMPLES];

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

  Float_t x[MAX_SAMPLES], y[MAX_SAMPLES];
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
  graph->GetYaxis()->SetTitle("Signal [V]");

  return graph;
}

