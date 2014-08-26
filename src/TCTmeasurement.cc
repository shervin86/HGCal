#include <TCTmeasurement.h>
#include <iostream>
#include <TString.h>
TGraph *TCTmeasurement::GetWaveForm(std::string graphName, std::string graphTitle) const{
  if(graphName==""){
    graphName=GetDiodeName()+GetTime()+"_"; graphName+=GetBias();
  }
  TString s = graphName.c_str();
  s.ReplaceAll("-","m");
  graphName=s.Data();
  if(graphTitle=="") graphTitle=graphName;

    
  Float_t x[MAX_SAMPLES], y[MAX_SAMPLES];
  const float *_y = GetSamples();

  unsigned int nSamples = GetN();
  std::cout << nSamples << std::endl;
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
  graph->SetMarkerSize(1.2);
  return graph;
}

