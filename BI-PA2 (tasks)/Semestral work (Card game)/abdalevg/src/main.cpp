//
//  main.cpp
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

#include "game_manager.h" 
#include "gamer.h"
#include "person.h"


using namespace std;


/*! 
    \brief main function.

    main starts programm 
*/
int main ( void )
{

  
  Effect effect1("power", 1, false);
  Effect effect2("protection", 1, false);
  Effect effect3("health", 1, false);
  Effect effect4("power", 1, false);
  Effect effect5("protection", 1, false);
  Effect effect6("health", 1, false);

  Card card1("card1");
  Card card2("card2");
  Card card3("card3");
  Card card4("card4");

  card1.setEffect(effect1);
  card1.setEffect(effect2);
  card1.setEffect(effect3);

  card2.setEffect(effect4);
  card2.setEffect(effect5);
  card2.setEffect(effect6);

  card3.setEffect(effect1);
  card3.setEffect(effect2);
  card3.setEffect(effect3);

  card4.setEffect(effect4);
  card4.setEffect(effect5);
  card4.setEffect(effect6);
  
  vector<Card> cards;

  cards.push_back(card1);
  cards.push_back(card2);
  cards.push_back(card3);
  cards.push_back(card4);

  //--------------------------------------------------------------------------

  //Gamer gamer1("Evgen");
  //Gamer gamer2("notEvgen");

  //Game game( gamer1, gamer2, cards );
  //game.setGameName( "game2" ); 
  
  
  
  //GameManager gameManager( cards );  
  GameManager gameManager;
  gameManager.start();


  //File file;
  //file.loadScore();
  //file.loadCards("cards.txt");
  //file.loadGame("game1.txt");

  //file.saveGame( game );

  return 0;
}
