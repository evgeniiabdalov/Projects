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
              \brief generate action based on state 
          */ 
          int generateAction();// generate action based on state 

   private:
   
          /*! 
              \brief goal orientated action planning, decides action 
          */
          int GOAP();


};

#endif
