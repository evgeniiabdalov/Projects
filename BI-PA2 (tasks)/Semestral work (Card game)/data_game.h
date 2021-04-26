#ifndef DATA_GAME_H
#define DATA_GAME_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

#include "gamer.h" 


/*! 
    \brief DataGame class.

    DataGame keeps data which needed to create game 
*/
class DataGame{

      public:

          
          /*! 
               \brief constructor in case data is correct                               
          */ 
          DataGame(Gamer gamer1, Gamer gamer2, string gameName );

          /*! 
               \brief constructor in case data is incorrect                               
          */  
          DataGame( bool wrongFileName );

          /*!  
              \brief getter for game name
          */
          string getGameName() const;

          /*!  
              \brief getter for first gamer
          */ 
          Gamer getGamer1() const;

          /*!  
              \brief getter for second name
          */
          Gamer getGamer2() const;

          /*!  
              \brief indicates if file name is incorrect
          */
          bool  isWrongFileName() const;


   private:

         string gameName;

         Gamer gamer1;
         Gamer gamer2;  

         bool wrongFileName;

};
#endif
