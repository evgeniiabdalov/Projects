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
          
            Person(): Gamer() {};

            Person( string name ): Gamer( name ) {};
            
            Person( const Person & other ): Gamer( other ) {};        
        
         
    private:
    
           /*! 
                 \brief set action from reader
           */ 
           void setAction(); 

           

};
#endif
