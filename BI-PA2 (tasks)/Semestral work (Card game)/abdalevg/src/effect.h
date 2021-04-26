//
//  effect.h
//  Card game
//
//  Created by Evgenii on 01.06.19.
//  Copyright © 2019 DROP TABLE productions. All rights reserved.
//


#ifndef EFFECT_H
#define EFFECT_H


#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;


/*! 
    \brief Effect class.

    Effect represents an effect that is being used by card 
*/
class Effect{

    public:
             
             /*! 
                 \brief Empty effect constructor                  
             */
             Effect();

             /*! 
                 \brief Constructor effect
                 \param attribute represents parameter on which effect has influence
                 \param value shows how big influence will be
                 \param attackEffect shows if effect has influence during attack                  
             */              
             Effect(string attribute, int value, bool attackEffect);             

             /*! 
                 \brief Copy effect constructor                  
             */             
             Effect( const Effect & otherEffect );

             /*! 
                 \brief getter for attribute                  
             */                
             string getAttribute() const;
             
             /*! 
                 \brief getter for value                  
             */             
             int getValue() const;
             
             /*! 
                 \brief shows if effect has influence during attack                 
             */             
             bool isAttackEffect() const;

             friend std::ostream & operator << ( std::ostream & os, const Effect & x );

    private:

             string attribute;
    
             int value;

             bool attackEffect;
};
#endif
