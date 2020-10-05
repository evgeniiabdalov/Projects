//
//  game.h
//  Card game
//
//  Created by Evgenii on 01.06.19.
//  Copyright © 2019 DROP TABLE productions. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "card.h"
#include "gamer.h"
#include "console.h"
#include "person.h"
#include "ai.h"

using namespace std;


/*! 
    \brief Game class.

    Game represents a game that is being played 
*/
class Game{

     public:

                 /*! 
                     \brief game constructor
                 */ 
                 Game(Gamer gamer1, Gamer gamer2, vector<Card> cards);

                 Game(Person gamer1, Person gamer2, vector<Card> cards);

                 Game( Person gamer1, AI gamer2, vector<Card> cards );

                 /*! 
                      \brief setter for game name
                 */
                 void setGameName( string gameName );
                
                 /*! 
                      \brief getter for game name
                 */
                 string getGameName() const;   
             

                 /*! 
                     \brief runs the whole process of game, game loop
                 */
                 bool run();

                 /*! 
                     \brief deletes all units
                 */
                 void clear();
            
            

              
            /*! 
                 \brief initiates start parameters if game, start game process
            */
            void startGame();

            /*! 
                \brief stops game, exit from game loop
            */
            void stopGame();

            /*! 
                \brief getter for gamer first
            */
            Gamer getGamer1() const;

            /*! 
                \brief getter for gamer second
            */
            Gamer getGamer2() const;
            
            /*! 
                \brief getter for gamer that has won
            */
            string getWonGamer() const;

            /*! 
                \brief getter for gamer that has lost
            */
            string getLostGamer() const;



     
     private:

            string gameName; 

            /*! 
                 \brief running process for two persons
            */
            void runPersons();

            /*! 
                 \brief running persons for one person and ai
            */
            void runAI(); 

            /*! 
                \brief shows game actions, that gamer can do
            */
            string showGameActions();

            /*! 
                \brief runs the whole process of game, game loop
            */
            bool inputGameAction( int indexGamer );

            /*! 
                \brief shows gamer cards
            */
            void showGamerCards( int indexGamer );

            /*! 
                \brief allows gamer to change card
            */
            void chooseCard( int indexGamer );

            /*! 
                \brief process attack
            */
            void attack( int indexGamer ); 

            /*! 
                \brief setter for gamer that has won
            */
            void setWonGamer( string gamerName );

            /*! 
                 \brief setter for gamer that has lost
            */
            void setLostGamer( string gamerName );            

            /*! 
                 \brief getter for index card
            */
            int  getIndex( string str );

            /*! 
                 \brief initiate cards of the game
            */
            void initiateCards( vector<Card> cards );

            /*! 
                 \brief initiate gamers of the game
            */
            void initiateGamers( Gamer gamer1, Gamer gamer2 );

            /*! 
                 \brief initiate gamers of the game
            */
            void initiateGamersAI( Gamer gamer1, Gamer gamer2 );

             /*! 
                 \brief initiate gamers of the game
            */
            //void initiateGamers( Person gamer1, Person gamer2 );

            /*! 
                 \brief generate card package that gamer has during the game
            */ 
            void generateCardPackage();

            /*! 
                 \brief allows gamer to change card
            */
            void changeCard( int indexGamer );

            /*! 
                 \brief algorithm to manipulate generating energy
            */
            void energyGenerator();  
                     
            Console console;            

            vector<Card> cards;
 
            bool isPlaying;

            Person gamer1;
            Person gamer2;

            AI     gamerAI;
          

            string wonGamer;

            string lostGamer;

            bool   isAI;            
 
            int currentCardAmount;

            /*! 
                 \brief getter gamer response
            */
            string getGamerResponse( int indexGamer );
   
            
                  


          
};
#endif
