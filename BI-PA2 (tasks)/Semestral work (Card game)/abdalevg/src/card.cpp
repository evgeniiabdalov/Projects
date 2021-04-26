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

        Card::Card(){
          this->nameCard = "card";
          //this->currentEffectIndex = 0;
          effects = vector<Effect>();
        }

        Card::Card( string nameCard ){
          this->nameCard = nameCard;
          //this->currentEffectIndex = 0;
          effects = vector<Effect>();
        }   

        Card::Card( const Card & other ){
            this->nameCard = other.getNameCard();
            this->effects = vector<Effect>();
            this->effects = other.getEffect();
        }
  
        void     Card::setName( string nameCard ) { this->nameCard = nameCard; }

        void     Card::setEffect( const Effect & effect){          
                    this->effects.push_back( effect ); 
        }

        vector<Effect> Card::getEffect() const{
          return this->effects;
        }

        string Card::getNameCard() const {
            return this->nameCard;
        }

        int Card::getEffectsNumber() const {
          return this->effects.size();
        }
 
        void Card::clear(){
          this->effects.clear();
        }
 


        std::ostream & operator << ( std::ostream & os, const Card & x ){

                os << x.getNameCard() << ":" << endl;
                
                for( int i = 0; i < x.getEffectsNumber(); i++ ){

                      os << "\t\t" << x.effects[i];

                }

                return os;

        }


 

       

        


