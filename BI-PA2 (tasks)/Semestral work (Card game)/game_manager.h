#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "card.h"
#include "game.h"
#include "gamer.h"
#include "file.h"

using namespace std;

/*! 
  
    \brief GameManager class.

    GameManager manages games, the whole user experience is running through this class 
*/
class GameManager{

     public:

             /*! 
                 \brief GameManager constructor                 
             */
             GameManager();


             /*! 
                 \brief GameManager constructor with cards
                 \param manually inserted cards 
              
             */
             GameManager( vector<Card> cards );

             /*! 
                 \brief starts the main menu, which allows to create new game or load/save game                               
             */              
             void start();

             /*! 
                 \brief shows main menu                               
             */
             string showMenu();

             /*! 
                 \brief assess user input                          
             */
             bool mainMenu( string option );


       

     private:             
        
        File file;
 
        /*! 
              \brief creates new game                               
        */        
        void newGame();

        /*! 
              \brief saves game                               
        */
        void saveGame();

        /*! 
              \brief loads game                               
        */ 
        bool loadGame();

        /*! 
              \brief shows score                               
        */
        void showScore();
        
        /*! 
              \brief shows credits                               
        */
        void showCredits();

        /*! 
              \brief exits the whole process                               
        */
        void exit();

        /*! 
              \brief starts gaming process                               
        */
        bool play();

        /*! 
              \brief deletes all units                               
        */ 
        void clear();

        //--------------------------------------------------------
 
         
        map<string, int> gamersScore;

        vector<Game> games;

        vector<Card> cards;      

        int currentGameIndex;

        Console console;

         
        
 };
#endif
