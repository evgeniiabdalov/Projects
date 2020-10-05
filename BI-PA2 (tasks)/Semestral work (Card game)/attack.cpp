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

#include "attack.h"  
 
             Attack::Attack(){
                  this->health = 0;
                  this->protection = 0;
                  this->power = 0;
             }

             void Attack::setHealth( int value ){
                  health = value;
             }
             
             void Attack::setProtection( int value ){
                  protection = value;
             }
             
             void Attack::setPower( int value ){
                  power = value; 
             }

             int Attack::getHealth() const { return this->health; }

             int Attack::getProtection() const { return this->protection; }

             int Attack::getPower() const { return this->power; }
             

    