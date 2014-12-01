#include "diode.h"

// std::ostream& diode::dump(std::ostream& f, float bias){
//   TCTmeasurements::const_iterator _tctItr =_tct[bias];
//   auto refItr=(*_tct.GetReference())[bias];
//   f << bias;
//   if(refItr!=_tct.GetReference()->end()) f << "\t" << refItr->second.GetWaveIntegral(_startSignal, _endSignal);
  
//   f  << "\t" << _tctItr->second.GetWaveIntegral(_startSignal, _endSignal)
//     << "\t" << _tct.GetCCE(_startSignal, _endSignal, bias);
//     //<< "\t" << _iv;
//   return f;
// };

std::ostream& diode::dump(std::ostream& f){
  f<< "thick\tarea\tfluence\ttemp\tbias\tQref\t\tQ\t\tCCE\t\tIV\t\tIVGR\t\tIGR\t\tCV455\t\tCV1M\t\tCV455GR\t\tCV1MGR";
  f<< "\t" << _id << "\t\"" << _property.GetLegend()<< "\""<< "\t" << _property.GetThickness() << "\t" << _property.GetFluence() << "\t" << _property.GetTemperatureString() 
   << std::endl;

  std::set<float> biases;
  for(auto _tctItr=_tct.begin(); _tctItr!=_tct.end(); _tctItr++){
    float bias=_tctItr->first;
    biases.insert(bias);
  }

  for(auto Itr=_iv.begin(); Itr!=_iv.end(); Itr++){
    float bias=Itr->first;
    biases.insert(bias);
  }


  for(auto Itr=_ivGR.begin(); Itr!=_ivGR.end(); Itr++){
    float bias=Itr->first;
    biases.insert(bias);
  }

  for(auto Itr=_cv.begin(); Itr!=_cv.end(); Itr++){
    float bias=Itr->first;
    biases.insert(bias);
  }
  for(auto Itr=_cvGR.begin(); Itr!=_cvGR.end(); Itr++){
    float bias=Itr->first;
    biases.insert(bias);
  }


  for(auto bias_itr=biases.begin(); bias_itr!=biases.end(); bias_itr++){
    float bias=*bias_itr;

    f << _property.GetThickness() << "\t" << _property.GetSurface() << "\t" << _property.GetFluence() << "\t" << _property.GetTemperatureString() << "\t";
    f<< std::setprecision(0) << std::fixed << bias;
    f <<  std::setprecision(2) << std::scientific ;
   
    if(_tct.GetReference()!=NULL){
      auto refItr=(*_tct.GetReference())[bias];
      if(refItr!=_tct.end() && refItr!=_tct.GetReference()->end()) f << "\t" << refItr->second.GetWaveIntegral(_startSignal, _endSignal);
      else f<< "\t-";
    }  else f<< "\t-";
    
    if(_tct[bias]!=_tct.end()){
    f  << "\t" << _tct[bias]->second.GetWaveIntegral(_startSignal, _endSignal)
       << "\t" << _tct.GetCCE(_startSignal, _endSignal, bias);
    }  else f<< "\t\t-\t\t-";
      
    auto ivItr=_iv[bias];
    if(ivItr!=_iv.end()) f<< "\t" << ivItr->second.first;
    else f<<"\t-";


    auto ivGRItr=_ivGR[bias];
    if(ivGRItr!=_ivGR.end()) f<< "\t" << ivGRItr->second.first << "\t" << ivGRItr->second.second;
    else f<<"\t-\t-";

    auto cvItr=_cv[bias];
    if(cvItr!=_cv.end()){
      f<< "\t" << cvItr->second[_cv.RefFreqIndex()];
      f<< "\t" << cvItr->second[_cv.MaxFreqIndex()];
    } else f<<"\t-\t-";


    auto cvGRItr=_cvGR[bias];
    if(cvGRItr!=_cvGR.end()){
      f<< "\t" << cvGRItr->second[_cvGR.RefFreqIndex()];
      f<< "\t" << cvGRItr->second[_cvGR.MaxFreqIndex()];
    } else f<<"\t-\t-";

    f<<"\n"; 
  }
  f<<"\n"<<std::endl;
  return f;
};
  
