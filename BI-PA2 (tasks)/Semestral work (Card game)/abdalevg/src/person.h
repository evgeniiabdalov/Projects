//
//  person.h
//  Card game
//
//  Created by Evgenii on 01.06.19.
//  Copyright © 2019 DROP TABLE productions. All rights reserved.
//



#ifndef PERSON_H
#define PERSON_H

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
    \brief Person class.

    Person represents a gamer person 
*/
class Person : public Gamer{

    public:
          
            /*! 
              \brief Empty constuctor 
            */
            Person() : Gamer(){};

            /*! 
              \brief constructor with name 
            */ 
            Person( string name ) : Gamer( name ){};
            
            /*! 
              \brief copy constructor with another AI 
            */
            Person( const Person & other ): Gamer( other ){};

            /*! 
              \brief copy constructor with another Gamer 
            */ 
            Person( const Gamer & other ): Gamer( other ){};
            
            /*! 
              \brief setter response 
            */
            void  setResponse(); 
         
    private:

           Console console;
    
          

           

};
#endif
