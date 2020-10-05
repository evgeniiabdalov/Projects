#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
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

#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#endif /* C++ 11 */

using namespace std;
#endif /* __PROGTEST__ */

class NoRouteException 
{ 
};

template <typename _T, typename _E>
class CRouteParameter{

      public:

              CRouteParameter(){
                                 map<_T, vector<_E>> wayParameter = map<_T, vector<_E>>();
                               };
 
             bool addRouteParameter( const _T & route, const _E & parameter ){
       
                                 auto itr = wayParameter.find( route );

                                 if( itr != wayParameter.end() )
         	                        (*itr).second.push_back( parameter );
                                 else{
         	                        vector<_E> parametersV = vector<_E>();
         	                        parametersV.push_back( parameter );
          	                        wayParameter.insert(make_pair(route, parametersV));
                                 }

           return true;
 
        };

          map<_T, vector<_E>> getRouteParameter() const{ return wayParameter; }; 

         void show(){
         	for(auto itr = wayParameter.begin(); itr != wayParameter.end(); itr++)
         		cout << (*itr).first << ' ';};
        

      private:	

      	 map<_T, vector<_E>> wayParameter;

      

};
  



template <typename _T, typename _E>
class CRoute
{
  public:

   CRoute(){};

   ~CRoute(){};

   CRoute & Add( const _T & from, const _T & to, const _E & parameter );

   list<_T> Find( const _T & from, const _T & to );

   list<_T> Find( const _T & from, const _T & to, const function<bool(_E)> filtr );

   void show(){
          for(auto itr = ways.begin(); itr != ways.end(); itr++){
          	cout << (*itr).first << " : ";
          	(*itr).second.show();
          	cout << endl;
          }
   	      
   };

   private:

        map<_T, CRouteParameter<_T,_E>> ways;  
        
};

template <typename _T, typename _E>
CRoute<_T, _E> & CRoute<_T,_E>::Add( const _T & from, const _T & to, const _E & parameter ){
 
    
    auto itr1 = ways.find( from ); 


    if(  itr1 != ways.end() )    
        (*itr1).second.addRouteParameter( to, parameter );
    else{
       
        CRouteParameter<_T,_E> rParam;
        rParam.addRouteParameter( to, parameter );
        ways.insert(make_pair( from, rParam ));  

    }

    //------------------------------------------------------

    auto itr2 = ways.find( to ); 


    if(  itr2 != ways.end() )    
        (*itr2).second.addRouteParameter( from, parameter );
    else{
       
        CRouteParameter<_T,_E> rParam;
        rParam.addRouteParameter( from, parameter );
        ways.insert(make_pair( to, rParam ));  

    }   
    

   return *this;

}

template <typename _T, typename _E>
list<_T> CRoute<_T,_E>::Find( const _T & from, const _T & to ){
    
    list<_T> listResult = list<_T>();

    if( from == to ){
    	listResult.push_back(from);
    	return listResult;
    }



    map<_T, _T> isVisited;
    list<_T> queue;
    queue.push_back(from);
    
    
    isVisited.insert(make_pair(from, from));

    bool get_out = false;

    while(!queue.empty()){

 

    	   _T node = queue.front();
           queue.pop_front();
        
           if(get_out)
               break;

           auto current = ways.find(node);

           if( current == ways.end() )
           	throw NoRouteException();

           map<_T, vector<_E>> wayParameter = (*current).second.getRouteParameter();

           for( auto itr = wayParameter.begin(); itr != wayParameter.end(); itr++ ){
                 if( isVisited.find((*itr).first) == isVisited.end() ){  
                   
                   queue.push_back((*itr).first);
                   isVisited.insert(make_pair((*itr).first, node));
                   
                   if( (*itr).first == to )
                   	 get_out = true;

                 }

                 if(get_out)
                    break;  
           }
           

    }

    if( !get_out ){
    	throw NoRouteException();
    }

    auto path = isVisited.find(to); 
    listResult.insert( listResult.begin(), (*path).first );
    _T tmp = (*path).second;
    while( true ){
        
        auto path = isVisited.find( tmp );
        tmp = (*path).second;
        listResult.insert( listResult.begin(), (*path).first );
        

        if((*path).first == from)
        	break;

    }   


    return listResult;

}

template <typename _T, typename _E>
list<_T> CRoute<_T,_E>::Find( const _T & from, const _T & to, const function<bool(_E)> filtr ){

    list<_T> listResult = list<_T>();

    if( from == to ){
    	listResult.push_back(from);
    	return listResult;
    }

    map<_T, _T> isVisited;
    list<_T> queue;
    queue.push_back(from);


    isVisited.insert(make_pair(from, from));

    bool get_out = false;

    while(!queue.empty()){

 

    	   _T node = queue.front();
           queue.pop_front();
        
           if(get_out)
               break;

           auto current = ways.find(node);

           if( current == ways.end() )
           	    throw NoRouteException();

           map<_T, vector<_E>> wayParameter = (*current).second.getRouteParameter();

           for( auto itr = wayParameter.begin(); itr != wayParameter.end(); itr++ ){
                 if( isVisited.find((*itr).first) == isVisited.end() ){  
                    for( auto filtrIt = (*itr).second.begin(); filtrIt != (*itr).second.end(); filtrIt++ ){
                      if( filtr( (*filtrIt) ) ){
                       
                        queue.push_back((*itr).first);
                        isVisited.insert(make_pair((*itr).first, node));
                   
                        if( (*itr).first == to )
                   	       get_out = true;

                   	      break;
                        }
                    }

                 }

                 if(get_out)
                    break;  
           }
           

    }


    if( !get_out ){
    	throw NoRouteException();
    }

    auto path = isVisited.find(to); 
    listResult.insert( listResult.begin(), (*path).first );
    _T tmp = (*path).second;
    while( true ){
        
        auto path = isVisited.find( tmp );
        tmp = (*path).second;
        listResult.insert( listResult.begin(), (*path).first );
        

        if((*path).first == from)
        	break;

    }
    

    


    return listResult;




}

#ifndef __PROGTEST__
//=================================================================================================
class CTrain
{
  public:
                             CTrain                        ( const string    & company, 
                                                             int               speed )
                             : m_Company ( company ), 
                               m_Speed ( speed ) 
    { 
    }
    //---------------------------------------------------------------------------------------------
    string                   m_Company;
    int                      m_Speed; 
};
//=================================================================================================
class TrainFilterCompany
{
  public:
                             TrainFilterCompany            ( const set<string> & companies ) 
                             : m_Companies ( companies ) 
    { 
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain & train ) const
    { 
      return m_Companies . find ( train . m_Company ) != m_Companies . end (); 
    }
    //---------------------------------------------------------------------------------------------
  private:  
    set <string>             m_Companies;    
};
//=================================================================================================
class TrainFilterSpeed
{
  public:
                             TrainFilterSpeed              ( int               min, 
                                                             int               max )
                             : m_Min ( min ), 
                               m_Max ( max ) 
    { 
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain    & train ) const
    { 
      return train . m_Speed >= m_Min && train . m_Speed <= m_Max; 
    }
    //---------------------------------------------------------------------------------------------
  private:  
    int                      m_Min;
    int                      m_Max; 
};
//=================================================================================================
bool               NurSchnellzug                           ( const CTrain    & zug )
{
  return ( zug . m_Company == "OBB" || zug . m_Company == "DB" ) && zug . m_Speed > 100;
}
//=================================================================================================
static string      toText                                  ( const list<string> & l )
{
  ostringstream oss;
  
  auto it = l . cbegin();
  oss << *it;
  for ( ++it; it != l . cend (); ++it )
    oss << " > " << *it;
  return oss . str ();
}
//=================================================================================================
int main ( void )
{
  CRoute<string,CTrain> lines;
  
  lines . Add ( "Berlin", "Prague", CTrain ( "DB", 120 ) )
        . Add ( "Berlin", "Prague", CTrain ( "CD",  80 ) )
        . Add ( "Berlin", "Dresden", CTrain ( "DB", 160 ) )
        . Add ( "Dresden", "Munchen", CTrain ( "DB", 160 ) )
        . Add ( "Munchen", "Prague", CTrain ( "CD",  90 ) )
        . Add ( "Munchen", "Linz", CTrain ( "DB", 200 ) )
        . Add ( "Munchen", "Linz", CTrain ( "OBB", 90 ) )
        . Add ( "Linz", "Prague", CTrain ( "CD", 50 ) )
        . Add ( "Prague", "Wien", CTrain ( "CD", 100 ) )
        . Add ( "Linz", "Wien", CTrain ( "OBB", 160 ) )
        . Add ( "Paris", "Marseille", CTrain ( "SNCF", 300 ))
        . Add ( "Paris", "Dresden",  CTrain ( "SNCF", 250 ) );

   lines.show();     
        
  list<string> r1 = lines . Find ( "Berlin", "Linz" );
  assert ( toText ( r1 ) == "Berlin > Prague > Linz" );
  
  list<string> r2 = lines . Find ( "Linz", "Berlin" );
  assert ( toText ( r2 ) == "Linz > Prague > Berlin" );

  list<string> r3 = lines . Find ( "Wien", "Berlin" );
  assert ( toText ( r3 ) == "Wien > Prague > Berlin" );

  list<string> r4 = lines . Find ( "Wien", "Berlin", NurSchnellzug );
  cout << toText( r4 ) << endl;
  assert ( toText ( r4 ) == "Wien > Linz > Munchen > Dresden > Berlin" );

  list<string> r5 = lines . Find ( "Wien", "Munchen", TrainFilterCompany ( set<string> { "CD", "DB" } ) );
  cout << toText( r5 ) << endl;
  assert ( toText ( r5 ) == "Wien > Prague > Munchen" );

  list<string> r6 = lines . Find ( "Wien", "Munchen", TrainFilterSpeed ( 120, 200 ) );
  cout << toText( r6 ) << endl;
  assert ( toText ( r6 ) == "Wien > Linz > Munchen" );

  list<string> r7 = lines . Find ( "Wien", "Munchen", [] ( const CTrain & x ) { return x . m_Company == "CD"; } );
  cout << toText( r7 ) << endl;
  assert ( toText ( r7 ) == "Wien > Prague > Munchen" );
  
  list<string> r8 = lines . Find ( "Munchen", "Munchen" );
  cout << toText( r8 ) << endl;
  assert ( toText ( r8 ) == "Munchen" );
  
  list<string> r9 = lines . Find ( "Marseille", "Prague" );
  cout << toText( r9 ) << endl;
  assert ( toText ( r9 ) == "Marseille > Paris > Dresden > Berlin > Prague" 
           || toText ( r9 ) == "Marseille > Paris > Dresden > Munchen > Prague" );
  
  try 
  { 
    list<string> r10 = lines . Find ( "Marseille", "Prague", NurSchnellzug ); 
    assert ( "Marseille > Prague connection does not exist!!" == NULL );
  }
  catch ( const NoRouteException & e ) 
  { 
  }
  
  list<string> r11 = lines . Find ( "Salzburg", "Salzburg" );
  assert ( toText ( r11 ) == "Salzburg" );

  list<string> r12 = lines . Find ( "Salzburg", "Salzburg", [] ( const CTrain & x ) { return x . m_Company == "SNCF"; }  );
  assert ( toText ( r12 ) == "Salzburg" );
 
  try 
  { 
    list<string> r13 = lines . Find ( "London", "Oxford" ); 
    assert ( "London > Oxford connection does not exist!!" == NULL );
  }
  catch ( const NoRouteException & e ) 
  { 
  }
  
  return 0;
}
#endif  /* __PROGTEST__ */
