#ifndef CARD_H
#define CARD_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "effect.h"

using namespace std;


/*! 
    \brief Card class.

    Card represents card 
*/
class Card{

  public:

        /*! 
            \brief card constructor without name          
        */
        Card();

        /*! 
            \brief card constructor with name 
        */ 
        Card( string nameCard );

        /*! 
           \brief card copy constructor            
        */       
        Card( const Card & other );
  
        /*! 
            \brief setter card name 
        */
        void     setName( string nameCard );

        /*! 
            \brief setter effect for card  
        */ 
        void     setEffect( const Effect & effect);

        /*! 
            \brief getter effect for card 
        */ 
        vector<Effect> getEffect() const;

        /*! 
            \brief getter name for card 
        */   
        string getNameCard() const; 
 
        /*! 
            \brief getter size effects 
        */ 
        int getEffectsNumber() const;

        /*! 
            \brief deletes all units 
        */  
        void clear();

 


        friend std::ostream & operator << ( std::ostream & os, const Card & x );


  private:

        string         nameCard;

        vector<Effect> effects;  
  
        Effect         currentEffect;

        int            currentEffectIndex;

        

};
#endif

