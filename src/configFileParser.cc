#include "configFileParser.h"

void configFileParser::Import(void){

  // input file parsing
  std::ifstream f_in(_filename.c_str());

  std::string diodeName, irradiation, temp, TCTdate, TCTreference, TCTrefereceDate, TCTbaseline, TCTbaselineDate, IVdate, CVdate;
  while(f_in.peek()!=EOF && f_in.good()){
    if(f_in.peek()==10){ // 10 = \n
      f_in.get(); 
      continue;
    }
    
    if(f_in.peek() == 35){ // 35 = #
      f_in.ignore(1000,10); // ignore the rest of the line until \n
      continue;
    }
    
    configFileContent configLine;
    configLine << f_in ;
    lines.push_back(configLine);
    /* f_in >> std::skipws >> configLine.diodeName  */
    /* 	 >> std::skipws >> configLine.irradiation  */
    /* 	 >> std::skipws >> configLine.temp   */
    /* 	 >> std::skipws >> configLine.TCTdate  */
    /* 	 >> std::skipws >> configLine.TCTreference  */
    /* 	 >> std::skipws >> configLine.TCTrefereceDate  */
    /* 	 >> std::skipws >> configLine.TCTbaseline  */
    /* 	 >> std::skipws >> configLine.TCTbaselineDate  */
    /* 	 >> std::skipws >> configLine.IVdate  */
    /* 	 >> std::skipws >> configLine.CVdate; */
    //strftime("%d/%m/%y-%R",)
    
    //std::cout << "###" << diodeName << "XXX" <<   irradiation << "XXX" <<   temp << "XXX" <<   TCTdate << "XXX" <<   TCTreference << "XXX" <<   TCTrefereceDate 
    // << "XXX" <<   TCTbaseline 
    // << "XXX" <<   TCTbaselineDate << "XXX" 
    // <<   IVdate << "XXX" <<   CVdate << "XXX" 
    //<<  std::endl;
    //tctFilename=baseDir+"/"+diodeName+"/"+TCTdate;
    //  std::cout << tctFilename << std::endl;
  }
}
