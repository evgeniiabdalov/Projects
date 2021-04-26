#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <algorithm>
#include <pthread.h>
#include <semaphore.h>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <atomic>
using namespace std;


class CFITCoin;
class CCVUTCoin;
class CCustomer;

typedef struct shared_ptr<CFITCoin>                        AFITCoin;
typedef struct shared_ptr<CCVUTCoin>                       ACVUTCoin;
typedef struct shared_ptr<CCustomer>                       ACustomer;
//=================================================================================================
class CFITCoin
{
public:
    CFITCoin                      ( const vector<uint32_t> & vectors,
                                    int               distMax )
            : m_Vectors ( vectors ),
              m_DistMax ( distMax ),
              m_Count ( 0 )
    {
    }
    virtual                  ~CFITCoin                     ( void ) = default;
    vector<uint32_t>         m_Vectors;
    int                      m_DistMax;
    uint64_t                 m_Count;
};
//=================================================================================================
class CCVUTCoin
{
public:
    CCVUTCoin                     ( const vector<uint8_t> & data,
                                    int               distMin,
                                    int               distMax )
            : m_Data ( data ),
              m_DistMin ( distMin ),
              m_DistMax ( distMax ),
              m_Count ( 0 )
    {
    }
    virtual                  ~CCVUTCoin                    ( void ) = default;
    vector<uint8_t>          m_Data;
    int                      m_DistMin;
    int                      m_DistMax;
    uint64_t                 m_Count;
};
//=================================================================================================
class CCustomer
{
public:
    virtual                  ~CCustomer                    ( void ) = default;
    virtual AFITCoin         FITCoinGen                    ( void ) = 0;
    virtual ACVUTCoin        CVUTCoinGen                   ( void ) = 0;

    virtual void             FITCoinAccept                 ( AFITCoin          x ) = 0;
    virtual void             CVUTCoinAccept                ( ACVUTCoin         x ) = 0;
};
//=================================================================================================
#endif /* __PROGTEST__ */


class fitCoinSolver{

public:

    fitCoinSolver(){
        bit_position = 32;       
    };
    ~fitCoinSolver(){};

    uint64_t       mining(vector<uint32_t>& input,int dist);

private:

    uint64_t       getKoefficient(int diffMax,int fixBit);
    void           generateBits(vector<uint32_t>& input);
    bool           getBit(uint32_t x,int position) const;
    void           editBit(uint32_t& x,int position) const;
    uint32_t       generateMax();
    uint64_t       combination(int x,int y);
    void           setCombinations(void);

    int            bit_position;
    uint64_t       combinations[33][33];



};

void fitCoinSolver::setCombinations(){
    
    for(int i=0;i<33;i++){
        for(int j=0;j<33;j++)
            combinations[i][j] = combination(i,j);
    }
    
    
}

uint64_t fitCoinSolver::combination(int x, int y){
    
    if (y>x/2) y=x-y;
    if (y==1)  return (uint64_t)x;
    if (y==0)  return 1;
    
    uint64_t r=1;
    for (int i=1; i<=y;++i) {
        r*=x-y+i;
        r/=i;
    }
   
    return r;
    
}


bool fitCoinSolver::getBit(uint32_t x,int position) const{

    return ((x) & (1<<position));

}

void fitCoinSolver::editBit(uint32_t& x, int position) const{

    int max = 32;

    for(int i=position;i<max;i++){


        if(getBit(x,i)){
            if(!getBit(x,i-1))
                x = x | (1<<(i-1));
        }
        else{
            if(getBit(x,i-1))
                x = x ^ (1<<(i-1));
        }

        if(i==max-1){
            if(getBit(x,i))
                x = x ^ (1<<i);
        }

    }


}

uint64_t fitCoinSolver::getKoefficient(int diffMax, int fixBit){

    uint64_t koefficient = 1;
    
    if(fixBit == 0)
        return koefficient;
    
    if(fixBit<diffMax)
        diffMax = fixBit;
    
    for(int d=1;d<=diffMax;d++){

        
        koefficient += combinations[fixBit][d];
        
        
        
    }
    
    return koefficient;
}

uint32_t fitCoinSolver::generateMax(){

    uint32_t number = 0x00000000;

    for(int i=0;i<bit_position;i++)
        number = number | (1<<i);

    return number;

}

void fitCoinSolver::generateBits(vector<uint32_t>& input){

    bool stable = true;
    bool bit_val = false;

    int i = 0;

    while(i<bit_position){

        for(unsigned int j=0;j<input.size();j++){

            if(j == 0)
                bit_val = getBit(input[j],i);
            else{

                if(bit_val != getBit(input[j],i))
                    stable = false;

                bit_val = getBit(input[j],i);

            }
        }

        if(stable){

            bit_position--;

            if(i!=(bit_position-1)){
                for(unsigned int l=0;l<input.size();l++){
                    editBit(input[l],i+1);

                }
            }

        }
        else{
            stable = true;
            i++;
        }
    }


}

uint64_t  fitCoinSolver::mining(vector<uint32_t>& main_input,int dist){

    vector<uint32_t> input = main_input;

    setCombinations();
    
    generateBits(input);

    uint64_t counter = 0;
    uint32_t number = 0x00000000;

    uint32_t max_number = generateMax();

    while(number<=max_number){

        int diff = 0;

        for(unsigned int i=0;i<input.size();i++){

            int tmp_diff = 0;

            for(int bit=0;bit<bit_position;bit++){

                if( getBit(input[i],bit) != getBit(number,bit) )
                    tmp_diff++;

            }

            if(tmp_diff>diff)
                diff = tmp_diff;

        }

        if(diff==dist)
            counter++;
        else if(diff<dist)
            counter += getKoefficient(dist-diff,32-bit_position);


        number++;

    }

    //cout<<"fit Coin "<<counter<<endl;
    return counter;

}


class cvutCoinSolver{

public:

    cvutCoinSolver(){};
    ~cvutCoinSolver(){};

    uint64_t         mining(vector<uint8_t>& input,int distMin,int distMax);


private:

    bool             getBit(uint8_t x,int position) const;
    int              editDistanceDP(string line1, string line2, int m, int n);

    vector<uint8_t>  input;

};


bool cvutCoinSolver::getBit(uint8_t x,int position) const{
    return ((x) & (1<<position));
}

//https://www.geeksforgeeks.org/dynamic-programming-set-5-edit-distance/
int cvutCoinSolver::editDistanceDP(string line1, string line2, int m, int n){

    

    vector<vector<int>> dp; //(m+1)*sizeof(int));
    dp.resize(m+1);
    
    for(int i=0; i<m+1; i++)
        dp[i].resize(n+1); //((n+1)*sizeof(int));

    for (int i=0; i<=m; i++)
    {
        for (int j=0; j<=n; j++)
        {
            if (i==0)
                dp[i][j] = j;

            else if (j==0)
                dp[i][j] = i;

            else if (line1[i-1] == line2[j-1])
                dp[i][j] = dp[i-1][j-1];

            else
                dp[i][j] = 1 + min(dp[i][j-1],
                                   min(dp[i-1][j],
                                       dp[i-1][j-1]));
        }
    }

    return dp[m][n];

}


uint64_t cvutCoinSolver::mining(vector<uint8_t>& input, int distMin, int distMax){

    this->input = input;

    int size = input.size();

    uint64_t counter = 0;

    string str1 = "";

    stringstream lineStream1;


    for(int i=0;i<8*size;i++){

        lineStream1<<getBit(input[size-1-i/8],7-(i-8*(i/8)));
        str1 = lineStream1.str();

        string str2 = "";

        for(int j=0;j<8*size;j++){

            stringstream lineStream2;

            lineStream2<<getBit(input[j/8],j-8*(j/8));

            str2 = lineStream2.str() + str2;

            if(str1.length() - str2.length() <= (unsigned int)distMax || str2.length() - str1.length() <= (unsigned int)distMax  ){

                int diff = editDistanceDP(str1,str2,str1.length(),str2.length());

                if(diff <= distMax && diff >=distMin)
                    counter++;

            }
        }
    }
    //cout<<"cvut Coin "<<counter<<endl;
    return counter;

}


//-----------------------parallelismus-----------------------------------

//data fitCoinProblem
class fitCoinData{

public:
    fitCoinData(){};
    fitCoinData(AFITCoin problem, ACustomer customer): fitCoinProblem(problem),customer(customer){};

    AFITCoin   fitCoinProblem;
    ACustomer  customer;

};

//data cvutCoinProblem
class cvutCoinData{

public:

    cvutCoinData(){};
    cvutCoinData(ACVUTCoin problem, ACustomer customer): cvutCoinProblem(problem),customer(customer){};

    ACVUTCoin   cvutCoinProblem;
    ACustomer   customer;

};


//object could contain fitCoinProblem,cvutCoinProblem, signal stop and delete
class QueueObject{

public:

    QueueObject(){
       customerId = 0;
       type = "";
    };
    QueueObject(string type):type(type){
        customerId = 0;
    };
    QueueObject(AFITCoin problem, ACustomer customer,int cid):type("fitCoin"),fitCoin(problem,customer),customerId(cid){};
    QueueObject(ACVUTCoin problem, ACustomer customer,int cid):type("cvutCoin"), cvutCoin(problem,customer),customerId(cid){};


    string        type;
    fitCoinData   fitCoin;
    cvutCoinData  cvutCoin;
    int           customerId;    

};

//polymorph thread safety queue
class ThreadSafeQueue{

public:

    QueueObject pop();
    void        push(const QueueObject & item);
    void        push(const vector<QueueObject> & items);

private:

    queue<QueueObject>    queue_;
    mutex                 queue_mutex;
    condition_variable    cond_;

};

QueueObject ThreadSafeQueue::pop(){

    unique_lock<mutex> mlock(queue_mutex);

    while(queue_.empty()){

        cond_.wait(mlock);

    }

    QueueObject item = queue_.front();
    queue_.pop();
    return item;

}

void ThreadSafeQueue::push(const QueueObject & item){


    unique_lock<mutex> mlock(queue_mutex);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();


}

void ThreadSafeQueue::push(const vector<QueueObject> & items){


    unique_lock<mutex> mlock(queue_mutex);
    for(auto & item : items) {
        queue_.push(item);
    }
    mlock.unlock();
    cond_.notify_all();


}

//customer and his queue of solved problems
class CustomerObject{

public:

    CustomerObject(){
        hasFitCoin = true;
        hasCvutCoin = true;
        solved_queue_size = 0;
        activeProblems = 0;
    };
    CustomerObject( ACustomer c):customer(c){
        hasFitCoin = true;
        hasCvutCoin = true;
        solved_queue_size = 0;
        activeProblems = 0;
    };

    ACustomer customer;
    queue<QueueObject>   solved_queue;

    int  solved_queue_size;
    int  activeProblems;

    bool hasFitCoin;
    bool hasCvutCoin;


};


class CRig
{
public:
    static void              Solve                         ( ACVUTCoin         x );
    static void              Solve                         ( AFITCoin          x );

    CRig                          ( void ){
        customers = 0;
        customersAmount = 0;
        isStopped = false;
    };
    ~CRig                         ( void ){};
    void                     Start                         ( int               thrCnt );
    void                     Stop                          ( void );
    void                     AddCustomer                   ( ACustomer         c );

private:

    bool                     isStopped;
    vector<thread*>          threads;

    mutex                    endMutex;


    mutex                    customersMutex;
    int                      customers;
    int                      customersAmount;
    condition_variable       isEnd;


    static void              WorkingThreadFunction(CRig * instance);

    static void              FitCoinFunction(CRig* instance, ACustomer c,int customerIndex);
    static void              CvutCoinFunction(CRig* instance, ACustomer c,int customerIndex);
    static void              SolvedProblemFunction(CRig* instance, ACustomer c,int customerIndex);

   
    
    map<int,CustomerObject>        mapCustomers;
    mutex                          solvedMutex;
    mutex                          solvedMutex2;
    condition_variable             isSolved;
    
    

    ThreadSafeQueue                queue;

};

void CRig::Start(int thrCnt){

    threads.resize(thrCnt);
    //cout<<"start"<<endl;
    for(int i=0;i<thrCnt;i++)
        threads[i] = new thread(WorkingThreadFunction,this);

}

void CRig::Stop(){

    isStopped = true;

    {
        unique_lock<mutex> lockCustomer(endMutex);

        while (customers != 0)
            isEnd.wait(lockCustomer, [&]{ return customers == 0; });
    }

    vector<QueueObject> stopObjects;
    for( size_t i = 0, l = this->threads.size(); i < l; i++ )
        stopObjects.push_back(QueueObject("stop"));


    this->queue.push( stopObjects );

    //cout << "pushed" << endl;

    for(unsigned int i=0;i<threads.size();i++)
        threads[i]->join();

    //cout << "joined" << endl;

    

    isStopped = false;
}

void CRig::AddCustomer(ACustomer c){

    CustomerObject customerObject(c);
    int tmp = customersAmount;      
    
    customersMutex.lock();
    customers++;
    mapCustomers.insert(std::pair<int,CustomerObject>(tmp,customerObject));
    customersAmount++;
    customersMutex.unlock();
     
    
    thread fitCoinThread = thread(&FitCoinFunction,this,c,tmp);
    fitCoinThread.detach();

    thread cvutCoinThread = thread(&CvutCoinFunction,this,c,tmp);
    cvutCoinThread.detach();

    thread solvedProblemThread = thread(&SolvedProblemFunction,this,c,tmp);
    solvedProblemThread.detach();
       
}

void CRig::WorkingThreadFunction(CRig* instance){

    QueueObject queueObject;

    while(true){


        queueObject = instance->queue.pop();

        if(queueObject.type == "stop")
            break;
        else if(queueObject.type == "fitCoin"){
            
            
            Solve(queueObject.fitCoin.fitCoinProblem);

            //push solved problem to customer and increment active problems
              unique_lock<mutex> mlock(instance->solvedMutex);
              instance->mapCustomers.find(queueObject.customerId)->second.solved_queue.push(queueObject);
              instance->mapCustomers.find(queueObject.customerId)->second.solved_queue_size++;
              mlock.unlock();
              instance->isSolved.notify_all();
                    
            
            


        }
        else if(queueObject.type == "cvutCoin"){

            
            Solve(queueObject.cvutCoin.cvutCoinProblem);

            //push solved problem to customer and increment active problems
              unique_lock<mutex> mlock(instance->solvedMutex);
              instance->mapCustomers.find(queueObject.customerId)->second.solved_queue.push(queueObject);
              instance->mapCustomers.find(queueObject.customerId)->second.solved_queue_size++;
              mlock.unlock();
              instance->isSolved.notify_all();
            
              
              
              
              
        }


    }

}

void CRig::FitCoinFunction(CRig* instance, ACustomer c,int customerIndex){

    AFITCoin problem;

    while((problem = c->FITCoinGen()) != NULL){

        instance->queue.push(QueueObject(problem,c,customerIndex));
        unique_lock<mutex> mlock(instance->solvedMutex);
        instance->mapCustomers.find(customerIndex)->second.activeProblems++;
        mlock.unlock();
    }

    //customer doesnt have fitCoin problems
    unique_lock<mutex> mlock(instance->solvedMutex);
    instance->mapCustomers.find(customerIndex)->second.hasFitCoin = false;
    mlock.unlock();

    
    
    



    pthread_exit(0);

}

void CRig::CvutCoinFunction(CRig* instance, ACustomer c,int customerIndex){

    ACVUTCoin problem;

    while((problem = c->CVUTCoinGen()) != NULL){

        instance->queue.push(QueueObject(problem,c,customerIndex));
        unique_lock<mutex> mlock(instance->solvedMutex);
        instance->mapCustomers.find(customerIndex)->second.activeProblems++;
        mlock.unlock();
    }


    //customer doesnt have cvutCoin problems
    unique_lock<mutex> mlock(instance->solvedMutex);
    instance->mapCustomers.find(customerIndex)->second.hasCvutCoin = false;
    mlock.unlock();

    
    





    pthread_exit(0);



}

void CRig::SolvedProblemFunction(CRig* instance, ACustomer c,int customerIndex){

    QueueObject      queueObject;
    CustomerObject*  customerObject;

    // size() => data race
    customerObject = &(instance->mapCustomers.find(customerIndex)->second);

    
    
    
    

    while(true){
        //cout << "customerObject->activeProblems = " << customerObject->activeProblems << endl;
        //cout << "customerObject->solved_queue_size = " << customerObject->solved_queue_size << endl;
        //cout << "customerObject->hasFitCoin = " << (customerObject->hasFitCoin ? "True" : "False") << endl;
        //cout << "customerObject->hasCvutCoin = " << (customerObject->hasCvutCoin ? "True" : "False") << endl;

        //check customer problems
        if(customerObject->activeProblems == 0)
            if(!customerObject->hasFitCoin && !customerObject->hasCvutCoin)
                if(customerObject->solved_queue_size==0) {
                    break;
                }

        //wait for problems to be solved
        while(customerObject->solved_queue_size==0){
            unique_lock<mutex> mlock(instance->solvedMutex2);

            
            instance->isSolved.wait(mlock, [&]{ return customerObject->solved_queue_size > 0; });

        }

        unique_lock<mutex> mlock(instance->solvedMutex);
        queueObject =  customerObject->solved_queue.front();
        customerObject->solved_queue.pop();
        customerObject->solved_queue_size--;
        mlock.unlock();


        if(queueObject.type == "fitCoin"){
            //passing fitCoin

            queueObject.fitCoin.customer->FITCoinAccept(queueObject.fitCoin.fitCoinProblem);
            unique_lock<mutex> mlock(instance->solvedMutex);
            customerObject->activeProblems--;//decrement active problems
            mlock.unlock();
        }
        else if(queueObject.type == "cvutCoin"){
            //passing cvutCoin

            queueObject.cvutCoin.customer->CVUTCoinAccept(queueObject.cvutCoin.cvutCoinProblem);
            unique_lock<mutex> mlock(instance->solvedMutex);
            customerObject->activeProblems--;//decrement active problems
            mlock.unlock();
        }

    }



    instance->customersMutex.lock();
    instance->customers--;
    instance->mapCustomers.erase(customerIndex);
    instance->customersMutex.unlock();

    if(instance->customers == 0 && instance->isStopped)
        instance->isEnd.notify_one();

    //cout << "Return thread finished" << endl;

    pthread_exit(0);

}

void CRig::Solve(AFITCoin x){

    fitCoinSolver solver;

    x->m_Count = solver.mining(x->m_Vectors,x->m_DistMax);

}


void CRig::Solve(ACVUTCoin x){

    cvutCoinSolver solver;

    x->m_Count = solver.mining(x->m_Data,x->m_DistMin,x->m_DistMax);

}

#ifndef __PROGTEST__
#include "test.inc"
#endif /* __PROGTEST__ */