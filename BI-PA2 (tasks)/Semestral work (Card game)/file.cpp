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

#include "file.h"
#include "data_game.h"
#include "game.h"
#include "gamer.h"  

         File::File(){
                input = "";
         }
                     

         DataGame File::loadGame( string fileName ){
 
               file.open( fileName );

               if( !file.is_open() ){
                     DataGame data( true );
                     return data; 
               }

               string gameName;

               Gamer gamer1;
               Gamer gamer2;

               int indexCardGamer1 = 0;
               int indexCardGamer2 = 0;

               bool writeEffectsGamer1 = false;
               bool writeEffectsGamer2 = false;

               string nameCard = "";

               vector<Effect> tmpEffects = vector<Effect>();


               while( std::getline(file, input) ){

                      if( input.find("game_name") != string::npos ){
                            string gameName = input.substr(input.find(":") + 1, input.length() );
                      }
                      else if( input.find("gamer1_name") != string::npos ){
                            string gamerName = input.substr(input.find(":") + 1, input.length() );
                            cout << "gamerName " << gamerName << endl;
                            gamer1.setName( gamerName );
                      }
                      else if( input.find("gamer2_name") != string::npos ){
                            string gamerName = input.substr(input.find(":") + 1, input.length() );
                            cout << "gamerName " << gamerName << endl;
                            gamer2.setName( gamerName );     
                      }
                      else if( input.find("gamer1_health") != string::npos ){
                            
                            int health;

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> health;
                            
                            cout << "health " << health << endl;
                            
                            gamer1.setHealth( health );
                      }
                      else if( input.find("gamer1_protection") != string::npos ){
                            
                            int protection;

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> protection;                             
                      
                            cout << "protection " << protection << endl;
                            
                            gamer1.setProtection( protection );
                      }
                      else if( input.find("gamer1_power") != string::npos ){
                            
                            int power; 

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> power;

                            cout << "power " << power << endl;
                            
                            gamer1.setPower( power ); 
                      }
                      else if( input.find("gamer1_energy") != string::npos ){
                            
                            int energy;

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> energy;

                            cout << "energy " << energy << endl;
                            
                            gamer1.setEnergy( energy );

                      }
                      else if( input.find("gamer2_health") != string::npos ){
                            
                            int health;

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> health;
                            
                            cout << "health " << health << endl;
                            
                            gamer2.setHealth( health );
                      }
                      else if( input.find("gamer2_protection") != string::npos ){
                          
                            int protection;

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> protection;                             
                      
                            cout << "protection " << protection << endl;
                            
                            gamer2.setProtection( protection );

                      }
                      else if( input.find("gamer2_power") != string::npos ){
                            
                            int power; 

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> power;

                            cout << "power " << power << endl;
                            
                            gamer2.setPower( power );

                      }
                      else if( input.find("gamer2_energy") != string::npos ){
                            
                            int energy;

                            istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                            istream >> energy;

                            cout << "energy " << energy << endl;
                            
                            gamer2.setEnergy( energy );

                      }

                   }
                   
                   cout << "gamer1 health " << gamer1.getHealth() << endl;
                   cout << "gamer1 health " << gamer1.getHealth() << endl;
                   cout << "gamer1 health " << gamer1.getHealth() << endl;
                   cout << "--------------------------------------------" << endl; 
                   cout << "gamer1 health " << gamer1.getHealth() << endl;
                   cout << "gamer1 health " << gamer1.getHealth() << endl;
                   cout << "gamer1 health " << gamer1.getHealth() << endl;

                   DataGame data( gamer1, gamer2, gameName );

                   return data;


          }

         bool File::saveGame( Game game ){
                
                 Gamer gamer1 = game.getGamer1();
                 Gamer gamer2 = game.getGamer2();

                 string fileName =  game.getGameName() + ".txt";

                 ofile.open( fileName );

                 ofile << "game_name: " << game.getGameName() << endl;

                 ofile << endl;

                 ofile << "gamer1_name: " << gamer1.getName() << endl;
                 ofile << "gamer1_health: " << gamer1.getHealth() << endl;
                 ofile << "gamer1_protection: " << gamer1.getProtection() << endl;
                 ofile << "gamer1_power: " << gamer1.getPower() << endl;
                 ofile << "gamer1_energy: " << gamer1.getEnergy() << endl;
                 
                 ofile << endl;
                 
                 ofile << "gamer2_name: " << gamer2.getName() << endl;
                 
                 ofile << endl;
                 
                 ofile << "gamer2_health: " << gamer2.getHealth() << endl;
                 ofile << "gamer2_protection: "<< gamer2.getProtection() << endl;
                 ofile << "gamer2_power: " << gamer2.getPower() << endl;
                 ofile << "gamer2_energy: " << gamer2.getEnergy() << endl;
                 
                 ofile << endl;

                 ofile.close();

                 return true;
                 
                

         }
         
         vector<Card> File::loadCards( string fileName ){

                vector<Card> cardsFile;

                int cardsAmount = 0;                

                file.open( fileName ); 

                //Card card;

                string nameCard = "";

                vector<Effect> tmpEffects = vector<Effect>();

                bool writeEffects = false;
               
                while( std::getline(file, input) ){
                
                  //cout << input << endl;

                  if( cardsAmount == 0 ){
                   
                     if( input.find("cards_amount") != string::npos ){
                    
                        istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                        istream >> cardsAmount;             

                      }
                  }
                  else{

                     if( input.find("card_name") != string::npos ){
                         
                         // setting card name
                         

                         istringstream istream( input.substr(input.find(":") + 1, input.length() ) );
                    
                         istream >> nameCard;

                         //cout << "card_name: " << nameCard << endl;

                         

                          
                      }  
                      else if( input.find("effects") != string::npos ){
                              writeEffects = true;
                              continue;
                      }                         
                      else if( writeEffects ){ 

                            // setting effects
                            //cout << "input: " << input << endl;
                            
                            if( input.find("end_effect") != string::npos ){
                            
                                 //cout << "end" << endl;

                                 Card card( nameCard );
                            
                                 writeEffects = false;
                             
                                 for( int i = 0; i < tmpEffects.size(); i++ )
                                     card.setEffect( tmpEffects[i] );                                   

                                 cardsFile.push_back( card );

                                 tmpEffects.clear();
                             } 

                            string effectAttribute = "";
                            int    effectValue = 0; 

                            effectAttribute = input.substr( 0, input.find(",") ); 

                            istringstream istream( input.substr( input.find(",") + 1, input.length() ) );

                            istream >> effectValue;

                            if( effectAttribute == "end_effect" )
                                 continue;

                            //cout << effectAttribute << " " << effectValue << endl;                       

                            //card.setEffect( Effect( effectAttribute, effectValue, false ) );
                            tmpEffects.push_back( Effect( effectAttribute, effectValue, false ) );  
                                             

                        }   
 

                     }






                        


                  }
                       
                 //cout << "size " << cardsFile.size() << endl; 

                 //for( int i = 0; i < cardsFile.size(); i++ )
                      //cout << cardsFile[i] << endl;
                
                 file.close();

                 return cardsFile;

         }

          map<string, int> File::loadScore(){
                
               map<string, int> score; 

               file.open( "score.txt" );

               string name;
               int    value;

               while( std::getline(file, input) ){

                  name = input.substr( 0, input.find(",") ); 

                  istringstream istream( input.substr( input.find(",") + 1, input.length() ) );

                  istream >> value;

                  //cout << name << " " << value << endl;

                  score.insert( make_pair( name, value ) );

               }



               file.close();

               return score;

          }

          void File::saveScore( map<string, int> score ){

                ofile.open( "score.txt" );

                for( auto it = score.begin(); it != score.end(); it++ ){
                    ofile << (*it).first << "," << (*it).second << endl;                    
                }
                
                ofile.close();

          }

 