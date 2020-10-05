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


#include "game_manager.h"
#include "data_game.h"
#include "file.h"
    
             GameManager::GameManager(){ 
                
                //cards = file.loadCards();
                this->cards = vector<Card>();
                this->games = vector<Game>();
                this->gamersScore = map<string, int>();

                this->currentGameIndex = 0;

                this->cards = file.loadCards("cards.txt");
                this->gamersScore = file.loadScore();                

             
             } 

             GameManager::GameManager( vector<Card> cards ){
             
                this->cards = vector<Card>();
                this->games = vector<Game>();
                this->gamersScore = map<string, int>();

                this->currentGameIndex = 0;
             
                this->cards = cards;                

              }

             void GameManager::start(){
                  
                  console.outputConsole( this->showMenu() );                  
                  
                  while ( true ) {
                     
                      bool response =  this->mainMenu( console.inputConsole() );
                   
                      if( response )
                        break;
                      else{
                        console.outputConsole( this->showMenu() );
                      }

                  }


             }


             string GameManager::showMenu(){
           
                ostringstream os;     

                os << "           Main menu           " << endl;
                os << "-------------------------------" << endl; 
                os << "1:         New game            " << endl;
                os << "2:         Save game           " << endl;
                os << "3:         Load game           " << endl;
                os << "4:         Score               " << endl;
                os << "5:         Credits             " << endl;
                os << "6:         Exit                " << endl;

                return os.str();

           }

             bool GameManager::mainMenu( string option ){
                
                   
                   if      ( option == "1" ){ this->newGame();     return false; }
                   else if ( option == "2" ){ this->saveGame();    return false; }
                   else if ( option == "3" ){ 
                      
                                             if( !this->loadGame() )
                                                  return false;

                                                  return false;
                                            }
                   else if ( option == "4" ){ this->showScore();   return false; }
                   else if ( option == "5" ){ this->showCredits(); return false; }
                   else if ( option == "6" ){ this->exit();        return true;}
                   else                     { console.outputConsole( "Incorrect input" ); return false; }

                   return true;

             }


       

                  
        
        //File file;
 
        void GameManager::newGame(){
              
             //create game : gamers? cards?

             console.outputConsole("input gamer1 name:\n");

             Gamer gamer1( console.inputConsole() );

             console.outputConsole("input gamer2 name:\n");

             Gamer gamer2( console.inputConsole() );
 
             //--------------------------------------------------

             Game game( gamer1, gamer2, this->cards );              

             //-------------------------------------------------
             games.push_back( game );

             currentGameIndex = games.size() - 1;

             //---------------------------------------------------

             this->play();


        }

        void GameManager::saveGame(){
              
              console.outputConsole("saving game\n");

              if( this->games.size() == 0 )
                console.outputConsole("there is no game yet(((((");
              else
                file.saveGame( this->games[currentGameIndex] );



        }

        bool GameManager::loadGame(){

               console.outputConsole("input game name\n");

               DataGame data = file.loadGame( console.inputConsole() );     
             
               if( data.isWrongFileName() ){
                 console.outputConsole("wrong file name\n");
                 return false;
               }

               Game game( data.getGamer1(), data.getGamer2(), this->cards );

               game.setGameName( data.getGameName() );

               this->games.push_back( game );

               this->currentGameIndex = this->games.size() - 1; 

               //---------------------------------------------------
 
                this->play();

                return true;
 
          
        }

        void GameManager::showScore(){  
            
            if( this->gamersScore.size() == 0 )
              cout << "let's play and win" << endl;
            else{
                
                 for( auto it = this->gamersScore.begin(); it != this->gamersScore.end(); it++ ){
                  cout << "gamer " << (*it).first << ": " << (*it).second << endl;    
                 }

            } 


                         

        }
        
        void GameManager::showCredits(){
           cout << "Created by Evgenii Abdalov" << endl;
           cout << "DROP TABLE Productions" << endl; 
        }

        void GameManager::exit(){

               file.saveScore( this->gamersScore );

               console.outputConsole("Nashledanou\n");


        }

        bool GameManager::play(){

              if( this->games.size() == 0 )
                   return false;
              
             
             
             console.outputConsole("game has been started\n\n"); 
 
             this->games[currentGameIndex].startGame();        

              

             if( this->games[currentGameIndex].getWonGamer() != "" ){
                   
                  auto it = this->gamersScore.find( this->games[currentGameIndex].getWonGamer() );

                  if( it == this->gamersScore.end() )
                      this->gamersScore.insert( make_pair( this->games[currentGameIndex].getWonGamer(), 1 ) );
                  else
                      (*it).second++;

             }

  
             
             if( this->games[currentGameIndex].getLostGamer() != "" ){
                   
                  auto it = this->gamersScore.find( this->games[currentGameIndex].getLostGamer() );

                  if( it == this->gamersScore.end() )
                      this->gamersScore.insert( make_pair( this->games[currentGameIndex].getLostGamer(), 0 ) );
                 

             }
               
             return true;
             

        }

        //--------------------------------------------------------

       
