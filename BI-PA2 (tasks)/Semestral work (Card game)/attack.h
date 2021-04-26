#ifndef ATTACK_H
#define ATTACK_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

/*! 
    \brief Attack class.

    Attack represents an attack that is being used by gamer 
*/
class Attack{

    public:   
 
             /*! 
                 \brief Attack constructor
             */ 
             Attack();

             /*! 
                 \brief setter for health attack
             */
             void setHealth( int value );
   
             /*! 
                 \brief setter fro protection attack
             */         
             void setProtection( int value );
             
             /*! 
                 \brief setter for protection attack
             */
             void setPower( int value );

             /*! 
                 \brief getter for protection attack
             */
             int getHealth() const;

             /*! 
                 \brief getter for protection attack
             */ 
             int getProtection() const;

             /*! 
                 \brief getter for power attack
             */
             int getPower() const;
             

    private:

            int power;
            int health;
            int protection;

             

};
#endif
