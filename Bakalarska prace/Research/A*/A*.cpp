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


#define ROW 9 
#define COL 10 

typedef pair<int, int> Pair;

typedef pair<double, pair<int, int>> pPair;

// a structure to hold the neccesary parameters
struct cell{         
       //row and column index of its parent
       int parent_i, parent_j;
       // f = g + h  
       double f, g, h;   
};

// check if given cell(row, col) is valid 
bool isValid( int row, int col){

     return ( row >= 0) && (row < ROW) &&
            (col >= 0) && (col < COL);

}


// check if given cell is blocked or not
bool isUnBlocked( int grid[][COL], int row, int col){

     if( grid[row][col] == 1)
           return (true);
     else
           return (false);	
}

// check if destintion has been reached
bool isDestination(int row, int col, Pair dest){

     if( row == dest.first && col == dest.second)
            return (true);
     else
     	    return (false);

}

// calculate H heuristics
double calculateHValue(int row, int col, Pair dest){

	return ((double)sqrt ( (row - dest.first) * (row - dest.first) +
		                   (col - dest.second) * (col - dest.second) )
	);
}

// trace the path from the source to destination
void tracePath(cell cellDetails[][COL], Pair dest){

     cout << "The Path is" << endl;
     int row = dest.first;
     int col = dest.second;

     stack<Pair> Path;

     while ( !(cellDetails[row][col].parent_i == row
               && cellDetails[row][col].parent_j == col  
     	      ) 
     	   )
     {
     	Path.push(make_pair(row, col));
     	int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
     }

     Path.push(make_pair(row, col));
     while(!Path.empty())
     {
     	pair<int, int> p = Path.top();
     	Path.pop();
     	cout << "-> " << p.first << " " << p.second << endl;
     }

     return;
}


// function to find the shortest path between a given source cell to a destination cell according to
// A* search algorithm
void aStarSearch(int grid[][COL], Pair src, Pair dest){

      // check if source is out of the range
	  if(isValid(src.first, src.second) == false)
	  {
	  	cout << "source is invalid" << endl;
	  	return;
	  }
      

      // check if destination is out of the range
	  if(isValid(dest.first, dest.second) == false)
	  {
        cout << "destination is invalid" << endl;
	  	return;
	  }

	  if(isUnBlocked(grid, src.first, src.second) == false || 
         isUnBlocked(grid, dest.first, dest.second) == false  
	  	)
	  {
	  	cout << "source or destination is blocked" << endl;
	  	return;
	  }

      if(isDestination(src.first, src.second, dest) == true)
      {
      	cout << "we are already at the destination" << endl;
        return;
      }

      // create a closed list and initialise it to false -> no cell has been included yet 
      bool closedList[ROW][COL];
      memset(closedList, false, sizeof(closedList));
  
      // declare a 2d array of structure to hold the details
      cell cellDetails[ROW][COL];   
 
      int i, j;

      for (i = 0; i < ROW; i++)
      {
          for ( j = 0; j < COL; j++ )
          {  
              cellDetails[i][j].f = FLT_MAX;
              cellDetails[i][j].g = FLT_MAX;
              cellDetails[i][j].h = FLT_MAX;
              cellDetails[i][j].parent_i = -1;
              cellDetails[i][j].parent_j = -1;    
          }

      }


      //initialising the parameters of the starting node
      i = src.first, j = src.second;
      cellDetails[i][j].f = 0.0;
      cellDetails[i][j].g = 0.0;
      cellDetails[i][j].h = 0.0;
      cellDetails[i][j].parent_i = i;
      cellDetails[i][j].parent_j = j; 
 
      /* 
     Create an open list having information as- 
     <f, <i, j>> 
     where f = g + h, 
     and i, j are the row and column index of that cell 
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
     This open list is implenented as a set of pair of pair.*/
     set<pPair> openList;

     openList.insert(make_pair(0.0, make_pair(i,j)));

     bool foundDest = false;

     while(!openList.empty())
     {
     	pPair p = *openList.begin();
 
         
        //remove this vertex from the open list  
     	openList.erase(openList.begin());
  
        //add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        /* 
        Generating all the 8 successor of this cell 
  
            N.W   N   N.E 
              \   |   / 
               \  |  / 
            W----Cell----E 
                 / | \ 
               /   |  \ 
            S.W    S   S.E 
  
        Cell-->Popped Cell (i, j) 
        N -->  North       (i-1, j) 
        S -->  South       (i+1, j) 
        E -->  East        (i, j+1) 
        W -->  West           (i, j-1) 
        N.E--> North-East  (i-1, j+1) 
        N.W--> North-West  (i-1, j-1) 
        S.E--> South-East  (i+1, j+1) 
        S.W--> South-West  (i+1, j-1)*/

        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------ 
        if(isValid(i-1, j) == true)
        {
            // If the destination cell is the same as the 
            // current successor
            if (isDestination(i-1, j, dest) == true)
            {
               cellDetails[i-1][j].parent_i = i;
               cellDetails[i-1][j].parent_j = j;
               cout << "the destination has been found" << endl;
               tracePath(cellDetails, dest);
               return;
            }
            else if( closedList[i-1][j] == false 
            	     &&
            	     isUnBlocked(grid, i-1, j) == true;
            	   )
            {
            	gNew = cellDetails[i][j].g + 1.0;
            	hNew = calculateHValue(i-1, j, dest);
            	fNew = gNew + hNew;
                 
            	if(cellDetails[i-1][j].f == FLT_MAX 
            		     || 
            	   cellDetails[i-1][j].f > fNew)
                {
                	openList.insert( make_pair(fNew, make_pair(i-1, j)));
                    
  
                    //update the details of this cell
                    cellDetails[i-1][j].f = fNew;
                    cellDetails[i-1][j].g = gNew;
                    cellDetails[i-1][j].h = hNew;
                    cellDetails[i-1][j].parent_i = i;
                    cellDetails[i-1][j].parent_j = j;            


                }
            }
        }

        //----------- 2nd Successor (South) ------------
        if(isValid(i+1, j) == true)
        {
            // If the destination cell is the same as the 
            // current successor 
            if(isDestination(i+1, j, dest) == true)
            {
                cellDetails[i+1][j].parent_i = i;
                cellDetails[i+1][j].parent_j = j;
                cout << "the destination has been found" << endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;    
            }
            else if(closedList[i+1][j] == false &&
                    isUnBlocked(grid, i+1, j) == true
                    )
            {
                 gNew = cellDetails[i][j].g + 1.0;
                 hNew = calculateHValue(i+1, j, dest);
                 fNew = gNew + hNew;

                 if(cellDetails[i+1][j].f == FLR_MAX ||
                    cellDetails[i+1][j].f > fNew)
                 {
                 	openList.insert(make_pair(fNew, make_pair(i+1,j)));

                    //update the details of this cell
                    cellDetails[i+1][j].f = fNew;
                    cellDetails[i+1][j].g = gNew;
                    cellDetails[i+1][j].h = hNew;
                    cellDetails[i+1][j].parent_i = i;
                    cellDetails[i+1][j].parent_j = j;            


                 }
            }
        }


        //----------- 3rd Successor (East) ------------   
        if(isValid(i, j+1) == true){
                  
            // If the destination cell is the same as the 
            // current successor 
            if(isDestination(i, j+1, dest) == true){ 
                cellDetails[i][j+1].parent_i = i;
                cellDetails[i][j+1].parent_j = j;
            	cout << "the destination has been found" << endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if(closedList[i][j+1] == false &&
                    isUnBlocked(grid, i, j+1) == true     
            	   )
            {
            	gNew = cellDetails[i][j].g + 1.0;
            	hNew = calculateHValue(i, j+1, dest);
            	fNew = gNew + hNew;

                if( cellDetails[i][j+1].f == FLT_MAX ||
                    cellDetails[i][j+1].f > fNew
                  )
                {
                   openList.insert(make_pair(fNew, make_pair(i, j+1)));

                   //update the details of this cell
                   cellDetails[i][j+1].g = gNew;
                   cellDetails[i][j+1].h = hNew;
                   cellDetails[i][j+1].f = fNew;
                   cellDetails[i][j+1].parent_i = i;
                   cellDetails[i][j+1].parent_j = j;
                
                }
            }
        }

        //----------- 4th Successor (West) ------------ 
        if(isValid(i, j-1) == true){
            
            // If the destination cell is the same as the 
            // current successor 
            if(isDestination(i, j-1, dest) == true)
            {
               cellDetails[i][j-1].parent_i = i;
               cellDetails[i][j-1].parent_j = j;
               cout << "the destination has been found" << endl;
               tracePath(cellDetails, dest); 
               foundDest = true;
               return;
            }
            else if(closedList[i][j-1] == false &&
                    isUnBlocked(grid, i, j-1) == true; 
            	   )
            {
            	gNew = cellDetails[i][j].g + 1.0;
            	hNew = calculateHValue(i, j-1, dest);
            	fNew = gNew + hNew;

            	if(cellDetails[i][j-1].f = FLT_MAX ||
                   cellDetails[i][j-1].f > fNew
            	  )
                {
                	openList.insert(make_pair(fNew, make_pair(i, j-1)));

                	//update the details of this cell
                    cellDetails[i][j-1].g = gNew;
                    cellDetails[i][j-1].h = hNew;
                    cellDetails[i][j-1].f = fNew;
                    cellDetails[i][j-1].parent_i = i;
                    cellDetails[i][j-1].parent_j = j;
                }
            }
        }


        //----------- 5th Successor (North-East) ------------ 
        if(isValid(i-1, j+1) == true){

          
              if(isDestination(i-1, j+1, dest) == true){
                    cellDetails[i-1][j+1].parent_i = i;
                    cellDetails[i-1][j+1].parent_j = j;
                    cout << "the destination has been found" << endl;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
              }
              else if( closedList[i-1][j+1] == false
                       && isUnBlocked(grid, i-1, j+1) == true
              	     )
              {
                  gNew = cellDetails[i][j].g + 1.0;
                  hNew = calculateHValue(i-1, j+1, dest);
                  fNew = gNew + hNew;

                  if(cellDetails[i-1][j+1].f == FLT_MAX ||
                     cellDetails[i-1][j+1].f > fNew
                  	)
                  {

                    openList.insert(make_pair(fNew, make_pair(i-1, j+1)));

                    
                    //update the details of this cell
                    cellDetails[i-1][j+1].g = nNew;
                    cellDetails[i-1][j+1].h = hNew;
                    cellDetails[i-1][j+1].f = fNew;
                    cellDetails[i-1][j+1].parent_i = i;
                    cellDetails[i-1][j+1].parent_j = j;

                } 
            }
        }


        //----------- 6th Successor (North-West) ------------ 
        if(isValid(i-1, j-1) == true){

          
              if(isDestination(i-1, j-1, dest) == true){
                    cellDetails[i-1][j-1].parent_i = i;
                    cellDetails[i-1][j-1].parent_j = j;
                    cout << "the destination has been found" << endl;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
              }
              else if( closedList[i-1][j-1] == false
                       && isUnBlocked(grid, i-1, j-1) == true
              	     )
              {
                  gNew = cellDetails[i][j].g + 1.0;
                  hNew = calculateHValue(i-1, j-1, dest);
                  fNew = gNew + hNew;

                  if(cellDetails[i-1][j-1].f == FLT_MAX ||
                     cellDetails[i-1][j-1].f > fNew
                  	)
                  {

                    openList.insert(make_pair(fNew, make_pair(i-1, j-1)));

                    
                    //update the details of this cell
                    cellDetails[i-1][j-1].g = nNew;
                    cellDetails[i-1][j-1].h = hNew;
                    cellDetails[i-1][j-1].f = fNew;
                    cellDetails[i-1][j-1].parent_i = i;
                    cellDetails[i-1][j-1].parent_j = j;

                } 
            }
        }
   

        //----------- 7th Successor (South-East) ------------ 
        if(isValid(i+1, j+1) == true){

          
              if(isDestination(i+1, j+1, dest) == true){
                    cellDetails[i+1][j+1].parent_i = i;
                    cellDetails[i+1][j+1].parent_j = j;
                    cout << "the destination has been found" << endl;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
              }
              else if( closedList[i+1][j+1] == false
                       && isUnBlocked(grid, i+1, j+1) == true
              	     )
              {
                  gNew = cellDetails[i][j].g + 1.0;
                  hNew = calculateHValue(i+1, j+1, dest);
                  fNew = gNew + hNew;

                  if(cellDetails[i+1][j+1].f == FLT_MAX ||
                     cellDetails[i+1][j+1].f > fNew
                  	)
                  {

                    openList.insert(make_pair(fNew, make_pair(i+1, j+1)));

                    
                    //update the details of this cell
                    cellDetails[i+1][j+1].g = nNew;
                    cellDetails[i+1][j+1].h = hNew;
                    cellDetails[i+1][j+1].f = fNew;
                    cellDetails[i+1][j+1].parent_i = i;
                    cellDetails[i+1][j+1].parent_j = j;

                } 
            }
        }

        
        //----------- 8th Successor (South-West) ------------ 
        if(isValid(i+1, j-1) == true){

          
              if(isDestination(i+1, j-1, dest) == true){
                    cellDetails[i+1][j-1].parent_i = i;
                    cellDetails[i+1][j-1].parent_j = j;
                    cout << "the destination has been found" << endl;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
              }
              else if( closedList[i+1][j-1] == false
                       && isUnBlocked(grid, i+1, j-1) == true
              	     )
              {
                  gNew = cellDetails[i][j].g + 1.0;
                  hNew = calculateHValue(i+1, j-1, dest);
                  fNew = gNew + hNew;

                  if(cellDetails[i+1][j-1].f == FLT_MAX ||
                     cellDetails[i+1][j-1].f > fNew
                  	)
                  {

                    openList.insert(make_pair(fNew, make_pair(i+1, j-1)));

                    
                    //update the details of this cell
                    cellDetails[i+1][j-1].g = nNew;
                    cellDetails[i+1][j-1].h = hNew;
                    cellDetails[i+1][j-1].f = fNew;
                    cellDetails[i+1][j-1].parent_i = i;
                    cellDetails[i+1][j-1].parent_j = j;

                } 
            }
        }  
         
    }

    if(foundDest == false){
    	cout << "failed to find path" << endl;
    } 

    return;

} 








// Driver program to test above function 
int main() 
{ 
    /* Description of the Grid- 
     1--> The cell is not blocked 
     0--> The cell is blocked    */
    int grid[ROW][COL] = 
    { 
        { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 }, 
        { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 }, 
        { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 }, 
        { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 }, 
        { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 }, 
        { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 }, 
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 }, 
        { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 }, 
        { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } 
    }; 
  
    // Source is the left-most bottom-most corner 
    Pair src = make_pair(8, 0); 
  
    // Destination is the left-most top-most corner 
    Pair dest = make_pair(0, 0); 
  
    aStarSearch(grid, src, dest); 
  
    return(0); 
}