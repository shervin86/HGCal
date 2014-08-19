#include <TCTmeasurement.h>
#include <iostream>
TGraph TCTmeasurement::GetWaveForm(std::string graphName){
  Float_t x[MAX_SAMPLES], y[MAX_SAMPLES];
  float *_y = GetSamples();

  unsigned int nSamples = GetN();
  std::cout << nSamples << std::endl;
  float timeScanUnit = GetTimeScanUnit();

  x[0] = 0;// initial value
  y[0] = _y[0];
  
  for(unsigned i=1; i < nSamples; i++){
    x[i] = x[i-1]+timeScanUnit;
    y[i] = _y[i];
  }

  
  
  TGraph graph(nSamples, x, y);
  graph.SetName(graphName.c_str());
  return graph;
}

