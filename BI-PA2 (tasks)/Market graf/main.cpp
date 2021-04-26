
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <vector>




using namespace std;

class Graf{

 public:  
    
    Graf(){};
    ~Graf(){};
    void BFS                (int start);
     
    void getResults         (void);
    bool initiateGraf       (int size);
    void addEdge            (int v, int w)                  { graf[v].push_back(w); };
    void setResourse        (int i, int resource)           { resources_vector[i].push_back(resource); };
    void setResourcesAmount (int amount, int minimal)       { this->resources_amount = amount;
                                                              this->minimal_amount = minimal; };
    
  private:
 
       list<int>           *graf;
       vector<int>         *resources_vector;
       int                 *distances;
       
       
       int        size;
       int        resources_amount;
       int        minimal_amount;
    
       int        general_distance;
       
       
};


bool Graf::initiateGraf(int size){
    
    this->size = size;
    
    graf = new list<int>[this->size];
    resources_vector = new vector<int>[this->size];
    distances = new int[this->size];
    general_distance = 0;
    
    return true;     
    
}

void Graf::BFS(int start){
    
    vector<int>  current_resources;
    
    vector<bool> resource_present;
    resource_present.resize(resources_amount, false);  
    resource_present[resources_vector[start][0]] = true;
    
    vector<bool> visited;
    visited.resize(size, false);  
    
    bool  get_out = false;
        
    
    list<int> queue;
    list<int>::iterator it;
    
    
    visited[start] = true;
    queue.push_back(start);
    
    
    int step = 1;
    int distance = 0;    
    int current_time = 0;
    int next_time = 0;
    
    
       
    
    while(!queue.empty()){       
        
        
        if(get_out)
            break;
        
        int current = queue.front();
        int current_size = graf[current].size();
        queue.pop_front();
        
                
        for( it = graf[current].begin(); it != graf[current].end(); ++it ){
                    
            if(!visited[*it]){
               
                
                visited[*it] = true;
                queue.push_back(*it);
               
                next_time++;
                
                if((int)resources_vector[start].size() + current_size < minimal_amount){ 
                    
                    if( !resource_present[resources_vector[*it][0]] ){
                      
                        resources_vector[start].push_back(resources_vector[*it][0]);                       
                        resource_present[resources_vector[*it][0]] = true;
                        
                        distance += step;                        
                        
                    }
                    
                                        
                }    
                else{
                    
                                   
                    //----------------sort resource-------------------------//
                    
                    if( !resource_present[resources_vector[*it][0]] ){   
                    
                        const vector<int>::iterator tmp_it = lower_bound(current_resources.begin(), current_resources.end(), this->resources_vector[*it][0]);
                        current_resources.insert(tmp_it, resources_vector[*it][0]);
                        resource_present[resources_vector[*it][0]] = true;
                                         
                    }                    
                    
                      
                      
                }    
                               
                
            }
            
        }
        
        for(unsigned int i = 0; i < current_resources.size(); i++){
            
            if((int)resources_vector[start].size() >= minimal_amount){
                get_out = true;
                break;
            }            
            
            resources_vector[start].push_back(current_resources[i]);
            
            distance += step;
            
                
        }
        
        
        if(step != 1)
            current_time--;
        
        if(current_time <= 0){
           step++; 
           current_time = next_time;
           next_time = 0;        
        }        
        
        
        
        vector<int> tmp_vector;
        current_resources.swap(tmp_vector);
        
              
    }
    
    
    distances[start] = distance;
    general_distance += distance;
    
    
    
    
    vector<bool> tmp_vector1;
    resource_present.swap(tmp_vector1);
    
    vector<int> tmp_vector2;
    current_resources.swap(tmp_vector2);
    
    vector<bool> tmp_vector3;
    visited.swap(tmp_vector3);
    
    
    
    
}

void Graf::getResults(void){
    
    
    for(int i = 0; i < this->size; i++)
        this->BFS(i);
    
    cout<<general_distance<<endl;
    
    for(int i = 0; i < this->size; i++){

        cout<<distances[i]<<' ';
        
        for(int j = 0; j < minimal_amount; j++){
            cout<<resources_vector[i][j]<<' ';
        }
        cout<<endl;
            
    }
       
    
    
}


int main(int argc, char** argv) {

    
    Graf graf;
        
    int size_graf = 0;
    int size_connections = 0;
    
    int resource_amount = 0;
    int minimal_amount = 0;
    
    int resource = 0;
    
    int from = 0;
    int to = 0;
    
    cin>>size_graf;
    cin>>size_connections;
    
    graf.initiateGraf(size_graf);
    
    
    cin>>resource_amount;
    cin>>minimal_amount;
    
    graf.setResourcesAmount(resource_amount, minimal_amount);
    
    
    for(int i = 0; i < size_graf; i++){
    
        cin>>resource;        
        graf.setResourse(i, resource);   
    }
    
    for(int i = 0; i < size_connections; i++){
        
        cin>>from;
        cin>>to;
     
        graf.addEdge(from, to);
        graf.addEdge(to, from);
         
        
    }
    
    graf.getResults();
    
    
    
    
    Graf graf;
    
    graf.initiateGraf(3);
    
    graf.setResourcesAmount(1,1);
    
    
    graf.setResourse(0,0);
    graf.setResourse(1,0);
    graf.setResourse(2,0);
    graf.setResourse(3,4);
    graf.setResourse(4,2);
    
    
    graf.addEdge(0,1);
    graf.addEdge(1,0);
    
    graf.addEdge(2,1);
    graf.addEdge(1,2);
       
    graf.addEdge(1,3);
    graf.addEdge(3,1);
    
   
    graf.addEdge(4,1);
    graf.addEdge(1,4);
    
    
    
    
    
    
   
    graf.getResults();
    graf.BFS(0);
    
    
    
    return 0;
}

