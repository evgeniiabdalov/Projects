#ifndef GAMER_H
#define GAMER_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "attack.h"
#include "card.h"
#include "effect.h"


using namespace std;



/*! gamer could be in certain states:
 \  active - action from him
 \  inactive - lost or not playing
 \  waiting  - action from another player
*/
enum gamerState{ 
                 active,
                 inactive,
                 waiting
               };

/*! 
    \brief Gamer class.

    Gamer represents gamer 
*/
class Gamer{
  
  public:

        /*! 
           \brief gamer constructor without name
        */
        Gamer();
        
        /*! 
           \brief gamer constructor whith name
        */
        Gamer( string name );

        /*! 
           \brief gamer copy constructor
        */ 
        Gamer( const Gamer & other );

        /*! 
           \brief getter gamer name
        */
        string       getName() const;
        
        /*! 
           \brief getter gamer health
        */ 
        int          getHealth() const;

        /*! 
           \brief getter gamer protection
        */
        int          getProtection() const;

        /*! 
           \brief getter gamer power
        */
        int          getPower() const;

        /*! 
           \brief getter gamer energy
        */
        int          getEnergy() const;

        /*! 
           \brief setter gamer name
        */
        void         setName( string name );

        /*! 
           \brief setter gamer health
        */  
        void         setHealth( int health );
 
        /*! 
           \brief setter gamer protection
        */ 
        void         setProtection( int protection ); 

        /*! 
           \brief setter gamer power
        */ 
        void         setPower( int power );

        /*! 
           \brief setter gamer energy
        */ 
        void         setEnergy( int energy );

        /*! 
           \brief getter gamer cards map
        */  
        map<int, Card> getCardsMap() const;

        /*! 
           \brief getter gamer cards size
        */ 
        int          getCardsSize() const;   

        /*! 
           \brief getter gamer state
        */  
        gamerState   getState() const; 

        /*! 
           \brief adds card to map, in case card is already added returns false
        */ 
        bool         addCard( int indexCard, Card card );

        /*! 
           \brief deletes card from map
        */ 
        void         deleteCard( int indexCard );

        /*! 
           \brief applies card effects to gamer parameters
        */ 
        bool         apply( int cardIndex );

        /*! 
           \brief gamer is put to active state
        */ 
        void         activate();

        /*! 
           \brief gamer is put to inactive state
        */ 
        void         deactivate();

        /*! 
           \brief gamer is put to waiting state
        */ 
        void         wait();

        /*! 
           \brief applies card effects to attack parameters
        */ 
        bool         applyAttack( int cardIndex );

        /*! 
           \brief sends attack to second gamer
        */ 
        Attack       sendAttack();
 
        /*! 
           \brief receives attack from second gamer
        */ 
        void         receiveAttack(Attack attack);

        /*! 
           \brief shows gamer cards
        */   
        string       showCards();
  
        /*! 
           \brief shows gamer parameters
        */ 
        string       showParameters();

        /*! 
           \brief deletes all units
        */ 
        void         clear();

  private:
 
        Attack attack;

        
        int power;
        int health;
        int protection;

        int energy;

        string name;

        enum gamerState state;

        map<int, Card> cardsMap;

       

};
#endif
