//
//  effect.cpp
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


#include "effect.h"


            Effect::Effect(){
                this->attribute = "";
                this->value = 0;
                this->attackEffect = false;
             }  

             Effect::Effect(string attribute, int value, bool attackEffect)
             {
                 this->attribute = attribute;
                 this->value = value;
                 this->attackEffect = attackEffect;                 
             }

             Effect::Effect( const Effect & otherEffect ){
                     this->attribute = otherEffect.getAttribute();
                     this->value = otherEffect.getValue();
             }

             string Effect::getAttribute() const { return this->attribute; }
             
             int Effect::getValue() const { return this->value; }
             
             bool Effect::isAttackEffect() const { return this->attackEffect; }

             std::ostream & operator << ( std::ostream & os, const Effect & x ){
                              
                    //if( x.isAttackEffect() )
                        //os << "this is attack effect" << endl;          
                    os << x.getAttribute() << ": " << x.getValue() << endl;           

                    return os; 

             }


    