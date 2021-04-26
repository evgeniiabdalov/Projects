//
//  file.h
//  Card game
//
//  Created by Evgenii on 01.06.19.
//  Copyright © 2019 DROP TABLE productions. All rights reserved.
//


#ifndef FILE_H
#define FILE_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "data_game.h"
#include "game.h"
#include "gamer.h" 

using namespace std;


/*! 
    \brief File class.

    File conducts the whole process with files 
*/

class File{

  public:

         /*! 
             \brief file constructor
         */        
         File();
                     
         /*! 
             \brief loads game from file
             \param name of the file from which the game is being loaded
         */
         DataGame loadGame( string fileName );

         /*! 
             \brief saves game to file
             \param game that is being saved
         */  
         bool saveGame( Game game );
         
         /*! 
             \brief loads cards from file
             \param name of the file from which cards is being loaded
         */   
         vector<Card> loadCards( string fileName );

          /*! 
             \brief loads score from file             
         */ 
         map<string, int> loadScore();

          /*! 
             \brief saves score to file
             \param score map to save
         */ 
         void saveScore( map<string, int> score );

 private:

         ifstream file;

         ofstream ofile;

         string input;

          


};
#endif
