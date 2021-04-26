//
//  console.h
//  Card game
//
//  Created by Evgenii on 01.06.19.
//  Copyright © 2019 DROP TABLE productions. All rights reserved.
//


#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>



using namespace std;

/*! 
    \brief Console class.

    Console represents user input/output 
*/
class Console{

     public:

           /*! 
               \brief constructor console                
           */
           Console();

           /*! 
               \brief get input from console                
           */  
           string inputConsole();

           /*! 
               \brief set output to console                
           */
           void outputConsole( string output );

           /*! 
               \brief output for second gamer is different, this function set that difference                
           */ 
           void enableSecondGamer();

           /*! 
               \brief disable difference in output for second gamer                
           */ 
           void disableSecondGamer();

                                               
  
     private:

           string  input;

           bool    secondGamer;

             
 };
#endif
