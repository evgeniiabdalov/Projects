//
//  ai.h
//  Card game
//
//  Created by Evgenii on 01.06.19.
//  Copyright © 2019 DROP TABLE productions. All rights reserved.
//


#ifndef AI_H
#define AI_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "card.h"
#include "gamer.h"
#include "console.h"

using namespace std;

/*! 
    \brief AI class.

    AI represents gamer ai 
*/
class AI : public Gamer{

   public:

           /*! 
              \brief Empty constuctor 
           */
           AI() : Gamer(){ responses = vector<bool>(); };

           /*! 
              \brief constructor with name 
           */
           AI( string name ) : Gamer( name ){ responses = vector<bool>(); };
            
           /*! 
              \brief copy constructor with another AI 
           */
           AI( const AI & other ): Gamer( other ){ responses = vector<bool>(); }; 

           /*! 
              \brief copy constructor with another Gamer 
           */ 
           AI( const Gamer & other ): Gamer( other ){ responses = vector<bool>(); };


           /*! 
              \brief setter response 
           */
           void  setResponse();

           /*! 
              \brief initiate state 
           */
           void  initiateAI();            


   private:
   
          
          bool protectionMode;
          bool attackMode;
          
          vector<bool> responses;


};

#endif
