//
//  person.cpp
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
#include "person.h"

            void Person::setResponse(){                   

                  console.outputConsole("put your action\n");  
                    
                  string gamerInput = console.inputConsole();

                  Gamer::response = gamerInput;

            }                      

            



