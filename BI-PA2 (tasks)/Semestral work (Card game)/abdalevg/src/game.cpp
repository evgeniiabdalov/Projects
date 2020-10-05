//
//  game.cpp
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


#include "game.h"
#include "card.h"
#include "gamer.h"
#include "console.h"




                 Game::Game( Gamer gamer1, Gamer gamer2, vector<Card> cards ){
                    
                     this->gameName = "game";

                     this->initiateCards( cards );
 
                     this->initiateGamers( gamer1, gamer2 );
                    
                     isPlaying = false;

                     this->wonGamer = "";
                     this->lostGamer = "";

                     this->isAI = false;



                 }

                 Game::Game( Person gamer1, Person gamer2, vector<Card> cards ){

                     this->gameName = "game";

                     this->initiateCards( cards );
 
                     this->initiateGamers( gamer1, gamer2 );
                    
                     isPlaying = false;

                     this->wonGamer = "";
                     this->lostGamer = ""; 

                     this->isAI = false;   

                 }


                 Game::Game( Person gamer1, AI gamer2, vector<Card> cards ){

                     this->gameName = "game";

                     this->initiateCards( cards );
 
                     this->initiateGamersAI( gamer1, gamer2 );
                    
                     isPlaying = false;

                     this->wonGamer = "";
                     this->lostGamer = "";

                     this->isAI = true;   

                 }

                void Game::setGameName( string gameName ) { this->gameName = gameName; }
                
                string Game::getGameName() const { return this->gameName; }
             
                bool Game::run(){
                 
                 while( isPlaying ){
                       
                       string gamerInput = "";

                       if( isAI )
                         this->runAI();
                       else
                         this->runPersons();
                      


                 }



                 return true;

            }

            void Game::runPersons(){

                        if( gamer1.getName() == "xvagner" ){
                            gamer2.setHealth( 0 );
                            gamer1.wait();
                            console.outputConsole("FLAWLESS VICTORY!\n");
                        }
                           
 
                        if( gamer1.getState() == active && gamer2.getState() == waiting )
                        {
 
                             console.disableSecondGamer();

                             console.outputConsole( gamer1.getName() + " is playing\n" );
                             console.outputConsole( gamer1.showParameters() );

                             console.outputConsole( this->showGameActions() );                                                         
                              
                             this->inputGameAction( 1 );                             
                             
                             if( gamer1.getEnergy() == 0 ){

                                  gamer1.setEnergy( 1 );

                                  gamer1.wait();
                                  gamer2.activate();
                             }  
                                
                        }
                       else if( gamer2.getState() == active && gamer1.getState() == waiting )
                        {

                             console.enableSecondGamer(); 

                             console.outputConsole( gamer2.getName() + " is playing\n" );
                             console.outputConsole( gamer2.showParameters() );

                             console.outputConsole( this->showGameActions() );
                             
                             this->inputGameAction( 2 );

                             if( gamer2.getEnergy() == 0 ){
                                
                                  gamer2.setEnergy( 1 );  

                                  gamer2.wait();
                                  gamer1.activate(); 
                             
                             }  


                        }     

                 
                        if( gamer1.getHealth() == 0 ){
                            this->isPlaying = false;
                            this->setLostGamer( gamer1.getName() );
                            this->setWonGamer( gamer2.getName() );
                            string output = gamer2.getName() + " has won!\n\n";
                            console.outputConsole( output );
                         }
                         else if( gamer2.getHealth() == 0 ){
                            this->isPlaying = false;
                            this->setLostGamer( gamer2.getName() );
                            this->setWonGamer( gamer1.getName() );
                            string output = gamer1.getName() + " has won!\n\n";
                            console.outputConsole( output );
                        }
 




            }

            void Game::runAI(){

                 
                 if( gamer1.getState() == active && gamerAI.getState() == waiting )
                        {
 
                             console.disableSecondGamer();

                             console.outputConsole( gamer1.getName() + " is playing\n" );
                             console.outputConsole( gamer1.showParameters() );

                             console.outputConsole( this->showGameActions() );                                                         
                              
                             this->inputGameAction( 1 );                             
                             
                             if( gamer1.getEnergy() == 0 ){

                                  gamer1.setEnergy( 1 );

                                  gamer1.wait();
                                  gamerAI.activate();
                             }  
                                
                        }
                       else if( gamerAI.getState() == active && gamer1.getState() == waiting )
                        {

                             console.enableSecondGamer(); 

                             console.outputConsole( gamerAI.getName() + " is playing\n" );
                             console.outputConsole( gamerAI.showParameters() );

                             console.outputConsole( this->showGameActions() );
                             
                             this->inputGameAction( 0 );

                             if( gamerAI.getEnergy() == 0 ){
                                
                                  gamerAI.setEnergy( 1 );  

                                  gamerAI.wait();
                                  gamer1.activate(); 
                             
                             }  


                        }     

                 
                        if( gamer1.getHealth() == 0 ){
                            this->isPlaying = false;
                            this->setLostGamer( gamer1.getName() );
                            this->setWonGamer( gamer2.getName() );
                            string output = gamer2.getName() + " has won!\n\n";
                            console.outputConsole( output );
                         }
                         else if( gamerAI.getHealth() == 0 ){
                            this->isPlaying = false;
                            this->setLostGamer( gamerAI.getName() );
                            this->setWonGamer( gamer1.getName() );
                            string output = gamer1.getName() + " has won!\n\n";
                            console.outputConsole( output );
                        }       





            }



            void Game::startGame(){

              
              
              if( isAI ){
                 gamer1.activate();
                 gamerAI.wait();                 
              }
              else{                  
                  gamer1.activate();
                  gamer2.wait();
              }
              
              this->isPlaying = true;

              this->run(); 

            }

            void Game::stopGame(){
              this->isPlaying = false;
            }

            Gamer Game::getGamer1() const { return this->gamer1; }

            Gamer Game::getGamer2() const { return this->gamer2; }

            
     
            string Game::getGamerResponse( int indexGamer ){
                  
                  string gamerInput; 

                  if( indexGamer == 1){
                      gamer1.setResponse();
                      gamerInput = gamer1.getResponse();
                   } 
                   else if( indexGamer == 2 ){
                      gamer2.setResponse();
                      gamerInput = gamer2.getResponse();
                   }
                   else if( isAI ){

                      gamerAI.setResponse();
                      gamerInput = gamerAI.getResponse();
                   }

                   return gamerInput;

            }

     
            string Game::showGameActions(){
                     
                ostringstream os;
                
                os << "---------------" << endl; 
                os << "1: Show cards  " << endl;
                os << "2: Choose card " << endl;
                os << "3: Attack      " << endl;
                os << "4: New card    " << endl;
                os << "5: End game    " << endl;
                os << "---------------" << endl;                                

                return os.str();

            }

            bool Game::inputGameAction( int indexGamer ){

                   string gamerInput = this->getGamerResponse( indexGamer );                   
                   
                   if      ( gamerInput == "1" ){ this->showGamerCards( indexGamer ); return true; }
                   else if ( gamerInput == "2" ){ this->chooseCard( indexGamer ); return true; }
                   else if ( gamerInput == "3" ){ this->attack( indexGamer ); return true; }
                   else if ( gamerInput == "4" ){ this->changeCard( indexGamer ); return true; }
                   else if ( gamerInput == "5" ){ this->stopGame(); return true; }
                   else                         { console.outputConsole( "Incorrect input\n\n" ); return false; }
                 
                   return true;

            }

            void Game::showGamerCards( int indexGamer ){
            
                 if( indexGamer == 1 )
                   console.outputConsole( gamer1.showCards() );
                 else if( indexGamer == 2 )
                    console.outputConsole( gamer2.showCards() );
                 else if( indexGamer == 0 )
                     console.outputConsole( gamerAI.showCards() );

            }

            void Game::chooseCard( int indexGamer ){

                  Gamer gamer;

                  int cardIndex = 0;

                  if( indexGamer == 1 )
                     gamer = gamer1;
                  else if( indexGamer == 2)
                     gamer = gamer2;
                  else if( indexGamer == 0 )
                     gamer = gamerAI;

                  while( true ){
                               
                      console.outputConsole( gamer.getName() + " choose your card\n");
 
                      cardIndex = this->getIndex( this->getGamerResponse( indexGamer ) );
                      
                      if( gamer.apply( cardIndex ) ){
                      
                         ostringstream os;

                         os << "your card is " << this->cards[cardIndex];
                        
                         console.outputConsole( os.str() );
                         
                         break;
                      }
                      else{
                        console.outputConsole("wrong card number\nchoose more wisely!\n");  

                      }




                  }

                  

                  //cout << "end card" << endl;

                  gamer.setEnergy( gamer.getEnergy() - 1 );

                  if( indexGamer == 1 )
                     gamer1 = gamer;                   
                  else
                     gamer2 = gamer;           
                      



            }

            void Game::attack( int indexGamer ){

 
                 // choosing card

                  Gamer gamer;

                  int cardIndex = 0;

                  if( indexGamer == 1 )
                     gamer = gamer1;
                  else if( indexGamer == 2 )
                     gamer = gamer2;
                  else if( indexGamer == 0 )
                     gamer = gamerAI;

                  while(true){
                               
                      console.outputConsole( gamer.getName() + " choose your attack card\n");
 
                      cardIndex = this->getIndex( this->getGamerResponse( indexGamer ) );
                            
                      if( gamer.applyAttack( cardIndex ) ){
                      
                         ostringstream os;

                         os << "your attack card is " << this->cards[cardIndex];
                        
                         console.outputConsole( os.str() );

                         //-----------------------------------------------
                         
                              // attack
               
                         if( indexGamer == 1 ){
                            gamer2.receiveAttack( gamer.sendAttack() );
                            gamer.setEnergy( 0 );
                         }    
                         else{     
                            gamer1.receiveAttack( gamer.sendAttack() );
                            gamer.setEnergy( 0 );
                          }
                         
                         break;
                      }
                      else{
                        console.outputConsole("wrong card number\nchoose more wisely!\n");  

                       }
                     }  

                      if( indexGamer == 1 )
                          gamer1 = gamer;                   
                      else
                          gamer2 = gamer;      
                    
                     
                     
                 
              
          }
             


            int  Game::getIndex( string str ){
                   
                  int index;

                  istringstream( str ) >> index;

                  return index;

            }

            void Game::initiateCards( vector<Card> cards ){

                this->cards = vector<Card>();
                this->cards = cards;

            }

            void Game::initiateGamers( Gamer gamer1, Gamer gamer2 ){
                  
                  this->gamer1 = gamer1;
                  this->gamer2 = gamer2;

                  this->generateCardPackage();
            
            }

            void Game::initiateGamersAI( Gamer gamer1, Gamer gamer2 ){
                 
                 this->gamer1  = gamer1;
                 this->gamerAI = gamer2;

                 this->generateCardPackage();  

            }

             
            void Game::generateCardPackage(){
                  
                   bool firstGamer = true;  

                  for( unsigned int i = 0; i < this->cards.size(); i++ ){
                        
                        if( firstGamer ){
                           gamer1.addCard( i, cards[i] );
                           firstGamer = false;
                        }
                        else{
                              gamer2.addCard( i, cards[i] );
                              firstGamer = true;
                        }   

                  }
                   
            }

            void Game::changeCard( int indexGamer ){
                   
                   console.outputConsole("enter card number you want to erase\n");
  
                   unsigned int cardIndex = this->getIndex( this->getGamerResponse( indexGamer ) );                   

                   Gamer gamer;

                   if( indexGamer == 1 )
                       gamer = gamer1;
                   else
                       gamer = gamer2;

                  gamer.deleteCard( cardIndex );
                  

                  while( true ){
                           
                          cardIndex++;

                          if( cardIndex >= this->cards.size() )
                             cardIndex = 0;

                          if( gamer.addCard( cardIndex, this->cards[cardIndex] ) )
                            break;
                  
                  }  
             


                          gamer.setEnergy( gamer.getEnergy() - 1 );

                          if( indexGamer == 1 )
                              gamer1 = gamer;
                          else
                              gamer2 = gamer;                      
             


            }

            void Game::energyGenerator(){

                   //generate energy 
                   cout << "energy genertor" << endl;    

            }

            void Game::clear(){
         
                 this->gamer1.clear();
                 this->gamer2.clear();

                 this->cards.clear();  

            }

            void Game::setWonGamer( string gamerName ){
                 this->wonGamer = gamerName;
            }
  
            string Game::getWonGamer() const { return this->wonGamer; }

            void Game::setLostGamer( string gamerName ){
                 this->lostGamer = gamerName;
            }
  
            string Game::getLostGamer() const { return this->lostGamer; }
                     
