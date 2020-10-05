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

using namespace std;

class Data{

   public:

   	      Data( string line, int counter ){
                this->line = line;
                this->counter = counter;    
   	      }

   	      string getLine() const { return line; };
   	      int    getCounter() const { return counter; };
    
   private:  
  
          string line;
          int    counter;    

};

class CTransport{

   public:

         void AddLine( istringstream & iss );
         set<string> FindDest( string station, int cost );
         void  show();



   private:	

   	     bool BFS( string line, int cost, int counter, set<string> * linesSet );
         void DFS( string line );



         map<string, set<string>> lines;
         map<string, bool> isVisited; 

  
};


void CTransport::AddLine( istringstream & iss ){

     string input1 = "";
     string input2 = "";


     getline( iss, input1);
     lines[input1];

     while( getline( iss, input2 ) ){
 
            lines[input1].insert( input2 );
            lines[input2].insert( input1 );

            input1 = input2;              

     }
     


}

set<string> CTransport::FindDest( string station, int cost ){
 
    set<string> linesSet = set<string>();

    isVisited.clear();

    linesSet.insert( station );
    isVisited[station] = true;
    

    if( cost != 0)
      BFS( station, cost, cost - 1, &linesSet );

   cout << "result" << endl;
    for( auto it = linesSet.begin(); it != linesSet.end(); it++ )
            cout << (*it) << endl;
    cout << "---------------------------" << endl; 

    return linesSet;

}

void CTransport::DFS( string line ){

        isVisited[line] = true;
        
        for( auto it = lines[line].begin(); it != lines[line].end(); it++ ){

             if( !isVisited[ (*it) ] )
             	DFS(line);

        }

}

bool CTransport::BFS( string line, int cost, int counter, set<string> * linesSet ){


   list<Data> queue;

   isVisited[line] = true;

   Data tmpData( line, counter);
   queue.push_back( tmpData );

   while( !queue.empty() ){

        Data currentData = queue.front();         
        queue.pop_front();
 

        string currentLine = currentData.getLine();
        int    currentCounter = currentData.getCounter();

        isVisited[currentLine] = true; 

        for( auto it = lines[currentLine].begin(); it != lines[currentLine].end(); it++ ){

              if( !isVisited[ (*it) ] ){
 
                 if( currentCounter >= 0){

                   Data tmpData( (*it), currentCounter - 1); 
                   queue.push_back( tmpData );

                   linesSet->insert( (*it) );
                }   

              }

        } 


   }

   return true;


}

void CTransport::show(){

     for( auto it = lines.begin(); it != lines.end(); it++ ){
            cout << (*it).first << endl;
            
            for( auto it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++ ){
                    cout << "\t\t"<< (*it2) << endl;
            }


     }
     cout << "--------------------" << endl;

}


int main( void ){
	
    istringstream iss;
    CTransport t0;
    iss.clear();
    iss.str ("Newton\n" "Black Hill\n" "Wood Side\n" "Green Hill\n" "Lakeside\n");
    t0.AddLine(iss);
    iss.clear();
    iss.str("Great Newton\n" "Little Burnside\n" "Green Hill\n" "Wood Side\n");
    t0.AddLine(iss);
    iss.clear();
    iss.str("Little Newton\n" "Little Burnside\n" "Castle Hill\n" "Newton Crossroad\n"
            "Lakeside Central\n");
    t0.AddLine(iss);
    iss.clear();
    iss.str("Tidmouth\n" "Gordon's Hill\n" "Suderry\n" "Knapford\n" "Great Waterton\n"
            "Brendam Docks\n");
    t0.AddLine(iss);
    iss.clear();
    iss.str ("Sodor Ironworks\n" "Sodor Streamwork\n" "Knapford\n" "Maron\n");
    t0.AddLine(iss);
    iss.clear();
    iss.str("Lakeside Central\n" "Little Waterton\n");
    t0.AddLine(iss);
    iss.clear();
    iss.str("Waterton Central\n" "Waterton East\n" "Waterton Woods\n" "Little Waterton\n"
            "Waterton West\n" "Waterton Central\n");
    t0.AddLine(iss);
    t0.show(); 
 
    assert (t0.FindDest ("Maron", 0) == (set<string> {"Maron"}) );
    assert (t0.FindDest ("Maron", 1) == (set<string> {"Knapford", "Maron"}) );
    assert (t0.FindDest ("Maron", 2) == (set<string> {"Great Waterton", "Knapford", "Maron", "Sodor Streamwork", "Suderry"}) );
    assert (t0.FindDest ("Maron", 3) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
                                                      "Sodor Ironworks", "Sodor Streamwork", "Suderry"}) );
    assert (t0.FindDest ("Maron", 4) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
                                                      "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth"}) );
    assert (t0.FindDest ("Maron", 5) == (set<string> {"Brendam Docks", "Gordon's Hill", "Great Waterton", "Knapford", "Maron",
                                                      "Sodor Ironworks", "Sodor Streamwork", "Suderry", "Tidmouth"}) );
    


     assert (t0.FindDest ("Waterton East", 0) == (set<string> {"Waterton East"}) );
    assert (t0.FindDest ("Waterton East", 1) == (set<string> {"Waterton Central", "Waterton East", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 2) == (set<string> {"Little Waterton", "Waterton Central", "Waterton East",
                                                              "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 3) == (set<string> {"Lakeside Central", "Little Waterton", "Waterton Central",
                                                              "Waterton East", "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 4) == (set<string> {"Lakeside Central", "Little Waterton", "Newton Crossroad",
                                                              "Waterton Central", "Waterton East", "Waterton West",
                                                              "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 5) == (set<string> {"Castle Hill", "Lakeside Central", "Little Waterton", "Newton Crossroad",
                                                              "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 6) == (set<string> {"Castle Hill", "Lakeside Central", "Little Burnside", "Little Waterton",
                                                              "Newton Crossroad", "Waterton Central", "Waterton East", "Waterton West",
                                                              "Waterton Woods"}) );
    assert (t0.FindDest ("Waterton East", 9) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill", "Lakeside",
                                                              "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton", "Newton Crossroad",
                                                              "Waterton Central", "Waterton East", "Waterton West", "Waterton Woods", "Wood Side"}) );
    assert (t0.FindDest ("Waterton East", 10) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill", "Lakeside",
                                                               "Lakeside Central", "Little Burnside", "Little Newton", "Little Waterton",
                                                               "Newton", "Newton Crossroad", "Waterton Central", "Waterton East",
                                                               "Waterton West", "Waterton Woods", "Wood Side"}) );
    assert (t0.FindDest ("Waterton East", 11) == (set<string> {"Black Hill", "Castle Hill", "Great Newton", "Green Hill",
                                                               "Lakeside", "Lakeside Central", "Little Burnside",
                                                               "Little Newton", "Little Waterton", "Newton",
                                                               "Newton Crossroad", "Waterton Central", "Waterton East",
                                                               "Waterton West", "Waterton Woods", "Wood Side"}) );
    

	return 0;
}