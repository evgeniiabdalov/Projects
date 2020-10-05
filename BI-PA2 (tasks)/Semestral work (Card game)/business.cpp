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


using namespace std;



/*
class Person : public Gamer{

    public:
          
         
    private:
    
           void setAction(); // set action from reader  

           //ActionReader reader;// reads action from gamer     


};

class AI : public Gamer{

   public:

          int generateAction();// generate action based on state 

   private:
   
          int GOAP();// goal orientated action planning, decides action


};*/




class Data_Game{

      public:

          Data_Game(Gamer gamer1, Gamer gamer2, string gameName ){
            this->gamer1 = gamer1;
            this->gamer2 = gamer2;
            this->gameName = gameName;            
          }

          string getGameName() const {return this->gameName; };

          Gamer getGamer1() const { return this->gamer1; };

          Gamer getGamer2() const { return this->gamer2; };


   private:

         string gameName;

         Gamer gamer1;
         Gamer gamer2;  

};


class File{

  public:

         File(){
                input = "";
         }
                     

         Data_Game loadGame( string fileName ){
 
               file.open( fileName );

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

                   Data_Game data( gamer1, gamer2, gameName );

                   return data;


          };

         bool saveGame( Game game ){
                
                 Gamer gamer1 = game.getGamer1();
                 Gamer gamer2 = game.getGamer2();

                 ofile.open( game.getGameName() );

                 ofile << game.getGameName() << endl;

                 ofile << gamer1.getName() << endl;
                 ofile << gamer1.getHealth() << endl;
                 ofile << gamer1.getProtection() << endl;
                 ofile << gamer1.getPower() << endl;
                 ofile << gamer1.getEnergy() << endl;
                 
                 ofile << endl;
                 
                 ofile << gamer2.getName() << endl;
                 ofile << gamer2.getHealth() << endl;
                 ofile << gamer2.getProtection() << endl;
                 ofile << gamer2.getPower() << endl;
                 ofile << gamer2.getEnergy() << endl;
                 
                 ofile << endl;

                 ofile.close();

                 return true;
                 
                

         };
         
         vector<Card> loadCards( string fileName ){

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

                         cout << "card_name: " << nameCard << endl;

                         

                          
                      }  
                      else if( input.find("effects") != string::npos ){
                              writeEffects = true;
                              continue;
                      }                         
                      else if( writeEffects ){ 

                            // setting effects
                            cout << "input: " << input << endl;
                            
                            if( input.find("end_effect") != string::npos ){
                            
                                 cout << "end" << endl;

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

                            cout << effectAttribute << " " << effectValue << endl;                       

                            //card.setEffect( Effect( effectAttribute, effectValue, false ) );
                            tmpEffects.push_back( Effect( effectAttribute, effectValue, false ) );  
                                             

                        }   
 

                     }






                        


                  }
                       
                 cout << "size " << cardsFile.size() << endl; 

                 for( int i = 0; i < cardsFile.size(); i++ )
                      cout << cardsFile[i] << endl;
                
                 return cardsFile;

         };

 private:

         ifstream file;

         ofstream ofile;

         string input;

          


};

           





 


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

  Gamer gamer1("Evgen");
  Gamer gamer2("notEvgen");

  Game game( gamer1, gamer2, cards );  
  
  

  GameManager gameManager( cards );  

  gameManager.start();

  //File file;

  //file.loadCards("cards.txt");
  //file.loadGame("game1.txt");
  return 0;
}
