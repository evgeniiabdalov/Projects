//
//  ai.cpp
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

#include "card.h"
#include "gamer.h"
#include "ai.h"

          void  AI::setResponse(){


                this->initiateAI();

          	    string gamerInput = "";

                if( this->responses.size() == 0 ){
     
                    if( protectionMode )
                      gamerInput = "2";
                    else if( attackMode )
                      gamerInput = "3";

                    this->responses.push_back( true );
 
                }
                else if( this->responses[ this->responses.size() - 1 ] == false ){
                                      
                      if( protectionMode )
                        gamerInput = "2";
                      else if( attackMode )
                        gamerInput = "3";

                    this->responses.push_back( true );

                }
                else{
                    
                     gamerInput = "3";
                     
                     this->responses.push_back( false );  

                }


                Gamer::response = gamerInput;
               
                cout << "response " << response << endl;



          }

          void  AI::initiateAI(){
           
                if( Gamer::getHealth() < 5 ){
                    this->protectionMode = false;
                    this->attackMode = true;
                }
                else
                {
                   this->protectionMode = true;
                   this->attackMode = false;                	
                }

          }
          

