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

#include "data_game.h"
#include "gamer.h" 

          DataGame::DataGame(Gamer gamer1, Gamer gamer2, string gameName ){
            this->gamer1 = gamer1;
            this->gamer2 = gamer2;
            this->gameName = gameName; 
            this->wrongFileName = false;           
          }

          DataGame::DataGame( bool wrongFileName ){
          	this->wrongFileName = wrongFileName;
          }

          string DataGame::getGameName() const {return this->gameName; }

          Gamer DataGame::getGamer1() const { return this->gamer1; }

          Gamer DataGame::getGamer2() const { return this->gamer2; }

          bool  DataGame::isWrongFileName() const { return this->wrongFileName; };


   