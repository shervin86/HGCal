#include "configFileParser.h"

void configFileParser::Import(void){

  // input file parsing
  std::ifstream f_in(_filename.c_str());

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

    lines.insert(std::pair<std::string, configFileContent>(configLine.type, configLine));
  }
}
