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
  f<< "thick\tfluence\ttemp\tbias\tQref\t\tQ\t\tCCE\t\tIV\t\tCV455\t\tCV1M";
  f<< "\t" << _id << "\t\"" << _property.GetLegend()<< "\""<< "\t" << _property.GetThickness() << "\t" << _property.GetFluence() << "\t" << _property.GetTemperatureString() << std::endl;
  for(auto _tctItr=_tct.begin(); _tctItr!=_tct.end(); _tctItr++){
    float bias=_tctItr->first;

    f << _property.GetThickness() << "\t" << _property.GetFluence() << "\t" << _property.GetTemperatureString() << "\t";
    f<< std::setprecision(0) << std::fixed << bias;
    f <<  std::setprecision(2) << std::scientific ;
   
    if(_tct.GetReference()!=NULL){
      auto refItr=(*_tct.GetReference())[bias];
      if(refItr!=_tct.end() && refItr!=_tct.GetReference()->end()) f << "\t" << refItr->second.GetWaveIntegral(_startSignal, _endSignal);
      else f<< "\t-";
    }  else f<< "\t-";
    
    f  << "\t" << _tctItr->second.GetWaveIntegral(_startSignal, _endSignal)
       << "\t" << _tct.GetCCE(_startSignal, _endSignal, bias);

    auto ivItr=_iv[bias];
    if(ivItr!=_iv.end()) f<< "\t" << ivItr->second;
    else f<<"\t-";

    auto cvItr=_cv[bias];
    if(cvItr!=_cv.end()){
      f<< "\t" << cvItr->second[_cv.RefFreqIndex()];
      f<< "\t" << cvItr->second[_cv.MaxFreqIndex()];
    } else f<<"\t-\t-";

    f<<"\n"; 
  }
  f<<"\n"<<std::endl;
  return f;
};
  
