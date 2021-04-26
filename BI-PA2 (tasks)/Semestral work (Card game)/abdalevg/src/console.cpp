//
//  console.cpp
//  Card game
//
//  Created by Evgenii on 01.06.19.
//  Copyright © 2019 DROP TABLE productions. All rights reserved.
//

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <ctime>
#include <cstdlib>


#include "console.h"

           Console::Console(){
                 input = "";
                 secondGamer = false;
           }

           string Console::inputConsole(){
                  cin >> input;
                  //cout << "input " << input << endl;
                  return input;                
           }

           void Console::outputConsole( string output ){
                
                if(secondGamer){
 
                    string       tmpOutput;                  
                    stringstream sstream;

 
                    sstream << output; 
 
                    while( std::getline(sstream, tmpOutput, '\n' ) )
                          cout << "\t\t\t\t" << tmpOutput << endl;
                
                }
                else 
                   cout << output;
           
           }         

           void Console::enableSecondGamer(){ this->secondGamer = true; }

           void Console::disableSecondGamer(){ this->secondGamer = false; }

                                               
  
    