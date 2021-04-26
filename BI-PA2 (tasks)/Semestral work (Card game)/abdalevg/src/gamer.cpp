//
//  gamer.cpp
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

#include "card.h"
#include "gamer.h"






  

        Gamer::Gamer(){
             
             this->name = "no name";
             
             this->health = 10;   
             this->protection = 2;
             this->power = 1;

             this->energy = 1;

             this->cardsMap = map<int, Card>(); 
              
             this->deactivate();

             this->response = "";

        }
        
        Gamer::Gamer( string name ){
                
              this->name = name;
              
              this->health = 10;   
              this->protection = 2;
              this->power = 1;

              this->energy = 1;

              this->cardsMap = map<int, Card>();

              this->deactivate(); 

              this->response = "";  
 
        }

        Gamer::Gamer( const Gamer & other ){

            this->name = other.getName();
            
            this->health = other.getHealth();
            this->protection = other.getProtection();
            this->power = other.getPower();
            
            this->energy = other.getEnergy();
            
            this->cardsMap = other.getCardsMap();

            this->deactivate();

            this->response = "";

            

        }

        string       Gamer::getName() const { return this->name; }
        
        int          Gamer::getHealth() const { return this->health; }

        int          Gamer::getProtection() const { return this->protection; }

        int          Gamer::getPower() const { return this->power; }

        int          Gamer::getEnergy() const { return this->energy; }

        void         Gamer::setName( string name ) { this->name = name; }

        void         Gamer::setHealth( int health ) { this->health = health; }
 
        void         Gamer::setProtection( int protection ) { this->protection = protection; }

        void         Gamer::setPower( int power ) { this->power = power; }

        void         Gamer::setEnergy( int energy ) { this->energy = energy; }

        void         Gamer::setCardsMap( map<int, Card> cardsMap ){
            
                                  for( auto it = cardsMap.begin(); it != cardsMap.end(); it++ )
                                     this->addCard( (*it).first, (*it).second );

                                 }

        map<int, Card> Gamer::getCardsMap() const { return this->cardsMap; }

        int          Gamer::getCardsSize() const { return this->cardsMap.size(); }   

        gamerState   Gamer::getState() const { return this->state; }

        bool         Gamer::addCard( int indexCard, Card card ){

                if( this->cardsMap.find( indexCard ) == this->cardsMap.end() ){
                  Card newCard = card;
                  this->cardsMap.insert( make_pair( indexCard, newCard) );
                  cout << "card " << card.getNameCard() << " has been added" << endl; 
                  return true;
                }
                else
                  return false;         

                
        }

        void         Gamer::deleteCard( int indexCard ){
           if( this->cardsMap.find( indexCard ) == this->cardsMap.end() )
                this->cardsMap.erase( indexCard );
        }

        bool  Gamer::apply( int cardIndex ){
                        
                        auto it = this->cardsMap.find( cardIndex );

                        if( it == this->cardsMap.end() )
                           return false;

                        vector<Effect> currentEffects = (*it).second.getEffect();

                        for( unsigned int i = 0; i < currentEffects.size(); i++ ){                            

                             string attribute = currentEffects[i].getAttribute();
                             int    value     = currentEffects[i].getValue();

                             
                             if( attribute == "health" )
                                 this->setHealth( this->health + value );
                             else if( attribute == "protection" )
                                 this->setProtection( this->protection + value );
                             else if( attribute == "power" )
                                 this->setPower( this->power + value );    


                             cout << "parameters: " << endl;
                             cout << "health " << this->getHealth() << endl;
                             cout << "protection " << this->getProtection() << endl;
                             cout << "power " << this->getPower() << endl;
                             cout << "-----------------------------" << endl;   
          
                              

                        }

                        return true;

        }

        void Gamer::activate(){
          this->state = active;
        }

        void Gamer::deactivate(){
          this->state = inactive;
        }

        void Gamer::wait(){
          this->state = waiting;
        }

        bool     Gamer::applyAttack( int cardIndex ){

                  auto it = this->cardsMap.find( cardIndex );

                  if( it == this->cardsMap.end() )
                        return false;

                  vector<Effect> currentEffects = (*it).second.getEffect();
    
                  
                  for( unsigned int i = 0; i < currentEffects.size(); i++ ){                            

                        string attribute = currentEffects[i].getAttribute();
                        int    value     = currentEffects[i].getValue();

                        if(  attribute == "health" )
                             this->attack.setHealth( value );
                        else if( attribute == "protection" )
                             this->attack.setProtection( value );
                        else if( attribute == "power" )
                             this->attack.setPower( value );

                  }

                 

                  return true;

   

        }



        Attack Gamer::sendAttack() const{
              return this->attack;
        }        
 
        void Gamer::receiveAttack( Attack attack ){
                
              int healthAttack = attack.getHealth();
              int protectionAttack = attack.getProtection();
              int powerAttack = attack.getPower();

              int kick = healthAttack + powerAttack - this->protection;

              this->health -= kick;
              this->protection -= protectionAttack;
              this->power--;

              if( this->health < 0 )
                 this->health = 0;

              if( this->protection < 0 )
                 this->protection = 0;

              if( this->power < 0 )
                 this->power = 0;    

              cout << this->getName() << " has been attacked" << endl;
              cout << "health " << this->health << endl;
              cout << "protection " << this->protection << endl;
              cout << "power " << this->power << endl;   

        }

        string Gamer::showCards(){
              
             ostringstream os; 

             os << this->getName() << ":" << endl;
             
             for( auto it = this->cardsMap.begin(); it != cardsMap.end(); it++ ){
                 os << "card number: " << (*it).first << "\n" << (*it).second;
                 os << "--------------------------" << endl;
             }        
             


             return os.str();              
              
        }

      string Gamer::showParameters(){

             ostringstream os;
 
             os << "-----------" << endl;
             os << "HEALTH-----" << this->getHealth() << endl;
             os << "PROTECTION-" << this->getProtection() << endl;
             os << "POWER------" << this->getProtection() << endl; 
             os << "ENERGY-----" << this->getEnergy() << endl;
             os << "-----------" << endl;

             return os.str();              

      }

      void Gamer::clear(){
           this->cardsMap.clear();
      }

      string Gamer::getResponse(){ return this->response; }

      void   Gamer::setResponse( ){}


 
