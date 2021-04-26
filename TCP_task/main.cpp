
#include <iostream>
using namespace std;

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // htons(), htonl()
#include <strings.h> // bzero()
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <sstream>

#define BUFFER_SIZE          10240
#define TIMEOUT              1
#define TIMEOUT_RECHARGING   5
#define SERVER_HASH          54621
#define CLIENT_HASH          45328



struct server_messages{
    const string SERVER_PASSWORD      = "101 PASSWORD\a\b";
    const string SERVER_MOVE          = "102 MOVE\a\b";
    const string SERVER_TURN_LEFT     = "103 TURN LEFT\a\b"; 
    const string SERVER_TURN_RIGHT    = "104 TURN RIGHT\a\b";
    const string SERVER_PICK_UP       = "105 GET MESSAGE\a\b";
    const string SERVER_LOGOUT        = "106 LOGOUT\a\b";
    const string SERVER_OK            = "200 OK\a\b";
    const string SERVER_LOGIN_FAILED  = "300 LOGIN FAILED\a\b";
    const string SERVER_SYNTAX_ERROR  = "301 SYNTAX ERROR\a\b";
    const string SERVER_LOGIC_ERROR   = "302 LOGIC ERROR\a\b";         
};

struct Result{
    string Command;
    bool   State;
    bool   Timeout_flag;
};

struct Message{
    string message="";
    bool   isMessage=false;
    bool   syntaxError=false;
    bool   specialSigns=false;
};

class Message_processor{
    
public:   
    
    Message_processor();
   ~Message_processor();     
    
    
    
    void            addMessage(vector<char> message);
    struct Message  getMessage();
    
private:
    
    string          current_message;
    vector<char>    general_vector;
    int             position;
    bool            syntaxError;
    bool            specialSigns;
    bool            hasUser;
    vector<string>  messagesList;
    void            processGeneralMessage();
    
};
 


  Message_processor::Message_processor(){
    
    this->current_message="";
    this->syntaxError=false;
    this->specialSigns=false;
    this->hasUser=false;
    this->position=0;
    

}
  
  Message_processor::~Message_processor(){
  
    vector<string>().swap(messagesList);
      
}

void  Message_processor::addMessage(vector<char> message){
    
    
    for(int i=0;i<message.size();i++)
        general_vector.push_back(message.at(i));
        
}


void Message_processor::processGeneralMessage(){
    
    vector<char> tmp_vector;
    
    specialSigns=false;
    
    cout<<"size "<<general_vector.size()<<endl;
    for(int i=0; i<general_vector.size();i++){
        
       if(i>0){   
          if(general_vector.at(i-1)=='\a' && general_vector.at(i)!='\b'){
               specialSigns=true;
               break;            
          }
         
         if(general_vector.at(i-1)!='\a' && general_vector.at(i)=='\b'){
               specialSigns=true;
               break;  
         }
       
        }
       
         if(general_vector.at(i)=='\0'){
                specialSigns=true;
                break;  
         }
           
          
    }
      
       
    
    for(int i=0; i<general_vector.size();i++){
         
        if(i>0)     
         if(general_vector.at(i-1)=='\a' && general_vector.at(i)=='\b'){
             
             if(i>11 && !hasUser){
                cout<<"oops"<<endl;   
                syntaxError=true;
             }
    
             
             for(int j=position;j<i; j++){
                current_message+=general_vector.at(j);
             }
             
             for(int k=i+1;k<general_vector.size();k++){
               tmp_vector.push_back(general_vector.at(k));                     
             }
              
              general_vector=tmp_vector;
              
              vector<char>().swap(tmp_vector);                      
             
              cout<<"current message "<<current_message<<endl;
                 
              messagesList.push_back(current_message);
             
              current_message="";
              
              if(!hasUser)
                  hasUser=true;
              
              break;
             
                           
        }
        
        if(!hasUser && i>=11){
             cout<<"oops"<<endl;   
             syntaxError=true;
         }  
        
         
         
        
      }
    
    cout<<"general vector size "<<general_vector.size()<<endl;
    
    
    
     
     if(general_vector.size()==100){
         cout<<"oops"<<endl;   
         syntaxError=true;
     }
    
    
    
    cout<<"current_message "<<current_message<<endl;

    
    
}

struct Message Message_processor::getMessage(){
   
    struct Message msgStruct;
    
    this->processGeneralMessage();
    
    
    if(specialSigns){
        msgStruct.specialSigns=true;
        cout<<"special signs"<<endl;
    }
    
    if(syntaxError){
        
        msgStruct.isMessage=true;
        msgStruct.message="";
        msgStruct.syntaxError=true;
        cout<<"syntax error "<<endl; 
        syntaxError=false;
        specialSigns=false;
        return msgStruct;
    }
        
    
    if(messagesList.size()>0){
        msgStruct.message=messagesList.front().substr(0,messagesList.front().length()-1);
        msgStruct.isMessage=true;
        msgStruct.syntaxError=false;
        messagesList.erase(messagesList.begin());
    }
    else if(specialSigns){
        msgStruct.message="";
        msgStruct.isMessage=true;
        msgStruct.syntaxError=false;
    }
    else{
        msgStruct.message="";
        msgStruct.isMessage=false;
        msgStruct.syntaxError=false;
    }
    
    
    
    return msgStruct;
}


class Authentication{

public:
    void    setUser(string user);
    bool    passwordCheck(string password);
    bool    isAuthenticated(){return authenticated;}
    bool    hasUser(){return has_user;}
    void    backUp();
    string  getHashServer();
    
private:
    string user;
    bool   authenticated=false;
    bool   has_user=false;
    int    hash_server = 0;
    int    hash_klient = 0;
    
};

void Authentication::setUser(string user){
    cout<<"User: "<<user<<endl;
    
    this->user=user;
    this->has_user=true;
    
    int  user_number=0;
    int* index=new int[this->user.length()];   
    int  hash_value = 0;    
    
    for(int i=0;i<this->user.length();i++){
        index[i]=(int)this->user[i];
        user_number+=user[i];
    }    
    
    hash_value = ( user_number * 1000 ) % 65536;
    
    hash_server = ( hash_value + SERVER_HASH ) % 65536;
    hash_klient = ( hash_value + CLIENT_HASH ) % 65536;
    cout<<"hash server "<<hash_server<<endl;
    cout<<"hash_klient "<<hash_klient<<endl;
    
}

bool Authentication::passwordCheck(string password){
    
    int password_value = std::atoi(password.c_str());
    cout<<"password value "<<password_value<<endl;
    if( password_value == hash_klient ){
        authenticated = true;
        return true;
    }
    return false;      
    
}

string Authentication::getHashServer(){
    
    ostringstream oss;
    
    oss<<hash_server<<"\a\b";
    
    return oss.str();
    
}

void Authentication::backUp(){
    this->authenticated=false;
    this->has_user=false;
}


class Controller{
    
public:
    
    struct Result process(string client_message);
    bool          hasUser(){authentication.hasUser();}
    void          backUp();
    
private:
 
    Authentication            authentication;
    struct  server_messages   commands;
    string                    current_message="";
    string                    general_message="";
    
    int                       position=0;
    int                       x_current=0;      
    int                       y_current=0;
    int                       x_previous=0;
    int                       y_previous=0;
    int                       point_counter=0;
    bool                      initialized=false;
    bool                      searchingMessage=false;
    bool                      waitingMessage=false;
    bool                      waitingPassword=false; 
    bool                      incorrectCoordinates=false;
    bool                      recharging=false;
    bool                      up=false;
    bool                      down=false;
    bool                      step=false;
    void                      getCoordinates(string klient_confirm);
    string                    direction_manager();
    string                    search_manager();
    
     
};

void Controller::backUp(){
    authentication.backUp();
}

struct Result Controller::process(string client_message){
   
    struct Result result_set;
    
    current_message=client_message;  
    
    
    while(true){
              
     
            
    if(current_message.length()+2>100 || (current_message[current_message.length()-1]==' ') && !waitingPassword){
        result_set.State=false;
        result_set.Command=commands.SERVER_SYNTAX_ERROR;
        authentication.backUp();
        return result_set;
    }   
    
    
    if(current_message.find("RECHARGING")==0){
        result_set.Timeout_flag=true;
        result_set.Command="";
        result_set.State=true;
        recharging=true;
        return result_set;
    }    
   
            
    
    if(recharging && current_message.find("FULL POWER")!=0){
        cout<<"recharging "<<recharging<<endl; 
        result_set.Command=commands.SERVER_LOGIC_ERROR;
        result_set.State=false;
          
        authentication.backUp(); 
        return result_set;
    } 
    else if(recharging && current_message.find("FULL POWER")==0){
        result_set.Command="";
        result_set.State=true;
        result_set.Timeout_flag=false;
        recharging=false;
        return result_set;
    }
    

    
    if(authentication.isAuthenticated()){
      
        
       if(waitingMessage){
           if(current_message!=""){
             result_set.State=false;
             result_set.Command=commands.SERVER_LOGOUT;
             return result_set;
           }
           else
               waitingMessage = false;          
           
       }
       
       if(searchingMessage){
           
            if(current_message.find("OK ")>=0 && current_message!=""){  
              cout<<"SEARCHING MESSAGE "<<current_message<<endl;
              getCoordinates(current_message);
              
              if(incorrectCoordinates){
                 result_set.Command=commands.SERVER_SYNTAX_ERROR;
                 result_set.State=false;
                 
                 authentication.backUp();
                 return result_set;        
              } 
            }
           cout<<"x "<<x_current<<" y "<<y_current<<endl; 
           result_set.Command=search_manager();
           result_set.State=true;
           return result_set;
        
            
            
       }
       
      
       
       if(current_message.find("OK ")>=0 && !searchingMessage){  
        
        getCoordinates(current_message);
   
        
            if(incorrectCoordinates){
                result_set.Command=commands.SERVER_SYNTAX_ERROR;
                result_set.State=false;
                
                authentication.backUp();
                return result_set;        
            } 
        
        
          result_set.Command=direction_manager();
          result_set.State=true;
          return result_set;
      
     } 
    }
    else if(!(authentication.hasUser())){
         
        if(current_message.length()>99){
         
          result_set.State=false;
          result_set.Command=commands.SERVER_SYNTAX_ERROR;
          
          return result_set;
                      
        }
        else {
          cout<<"user message "<<current_message<<endl;
          authentication.setUser(current_message);
          result_set.State=true;
          result_set.Command=authentication.getHashServer();
          waitingPassword=true;
        
         
          return result_set;
          
        }  
    }
    else if(current_message!="" && authentication.hasUser()){
       
        waitingPassword=false;
       
        
        if(current_message.length()<6 && current_message[0]!=' ' && current_message[current_message.length()-1]!=' '){        
              
          if(authentication.passwordCheck(current_message)){
            cout<<"server ok"<<endl; 
            result_set.State=true;
            result_set.Command=commands.SERVER_OK+commands.SERVER_MOVE;
            return result_set;
          }
          else{
            result_set.State=false;
            result_set.Command=commands.SERVER_LOGIN_FAILED;
            
            authentication.backUp();
            return result_set; 
          }
            
        
        }
        else{
            result_set.State=false;
            result_set.Command=commands.SERVER_SYNTAX_ERROR;
            
            authentication.backUp();
            return result_set;
        }
        
        
     }
        
    }
}

void Controller::getCoordinates(string klient_confirm){
    
    int i,end_position,position,data_length=0;
    string data_string="",x_string="",y_string="";
    
    
    end_position=klient_confirm.find("\a\b",klient_confirm.find("OK "));
    data_length=end_position-3;
    data_string=klient_confirm.substr(klient_confirm.find("OK ")+3,data_length);
    position=data_string.find(" ");
    
    for(i=0;i<data_string.length();i++){
       if(i<position) 
        x_string+=data_string[i];
       
       if(i>position) 
        y_string+=data_string[i];
         
    }
    
    //cout<<"data string"<<data_string<<endl;
    //cout<<"data length"<<data_length<<endl;
    double n;
    if(modf(stod(x_string.c_str()),&n)!=0 || modf(stod(y_string.c_str()),&n)!=0)
          incorrectCoordinates=true;   
        
    if(x_previous==0 && y_previous==0 && !searchingMessage){
        x_previous=atoi(x_string.c_str());
        y_previous=atoi(y_string.c_str());
    }    
    else if(x_previous==0 && y_previous==0 && searchingMessage){
        x_current=atoi(x_string.c_str());
        y_current=atoi(y_string.c_str());
        initialized=true;    
    }    
    else {
        x_current=atoi(x_string.c_str());
        y_current=atoi(y_string.c_str());
        initialized=true;
    }
    
    //cout<<"Previous x: "<<x_previous<<endl;
    //cout<<"Previous y: "<<y_previous<<endl;
    
    //cout<<"Current x: "<<x_current<<endl;
    //cout<<"Current y: "<<y_current<<endl;
}

string Controller::direction_manager(){
 
   struct server_messages direction_command;  
    
     
      
   
    if(up){
       x_previous=x_current;
       y_previous=y_current;
       up=false;
       return direction_command.SERVER_TURN_RIGHT;
    }  
   
    if(down){
       x_previous=x_current;
       y_previous=y_current;
       down=false;
       return direction_command.SERVER_TURN_LEFT;
    } 
   
   
    if(x_previous==0 && y_previous==0 && !initialized){
        waitingMessage=true;
        searchingMessage=true;
        return direction_command.SERVER_PICK_UP;
    }    
        
    if(initialized && x_current==0 && y_current==0 && (x_previous!=0 || y_previous!=0)){
        waitingMessage=true;
        searchingMessage=true;
        return direction_command.SERVER_PICK_UP;
    }
   
        
    if((x_current==0 && y_current==0 && !initialized) || (x_current==x_previous && y_current==y_previous))            
        return direction_command.SERVER_MOVE;
       
    if(x_current>x_previous && x_current>0 && y_current<0 ){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_LEFT;
    }
    
    if(x_current>x_previous && x_current>0 && y_current>0 ){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_RIGHT;
    }
    
    if(x_current<x_previous && x_current<0 && y_current<0){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_RIGHT;
    }
    
    if(x_current<x_previous && x_current<0 && y_current>0){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_LEFT;
    }
        
    
    if(y_current>y_previous && x_current>0 ){ 
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_LEFT;
    }  

    if(y_current>y_previous && x_current<0 ){ 
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_RIGHT;
    }  
  
    if(y_current<y_previous && x_current>0 ){ 
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_RIGHT;
    }

    if(y_current<y_previous && x_current<0 ){ 
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_TURN_LEFT;
    } 
  
   if(x_current<x_previous && x_current>0 ){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_MOVE;
   }
  
   if(x_current>x_previous && x_current<0){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_MOVE;
   } 
  
   if(x_current==0 && y_current<y_previous && y_current>0 && x_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_MOVE;
   }  
    
   if(x_current==0 && y_current<y_previous && y_current<0 && x_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       down=true;
       return direction_command.SERVER_TURN_LEFT;
   }
   
      
    if(x_current==0 && y_current>y_previous && y_current<0 && x_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_MOVE;
    }  
    
    if(x_current==0 && y_current>y_previous && y_current>0 && x_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       up=true;
       return direction_command.SERVER_TURN_RIGHT;
    }
   
      
    if(x_current==0 && y_current>0 && x_previous>0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }  
    
     
    if(x_current==0 && y_current>0 && x_previous<0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
    
    if(x_current==0 && y_current<0 && x_previous>0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }  
    
     
    if(x_current==0 && y_current<0 && x_previous<0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    } 
   
    //------------------------------------------
   
    if(y_current==0 && x_current>0 && y_previous>0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }  
    
     
    if(y_current==0 && x_current>0 && y_previous<0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
    
    if(y_current==0 && x_current<0 && y_previous>0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }  
    
     
    if(y_current==0 && x_current<0 && y_previous<0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    } 
    
    //----------------------------------------------
   
     if(y_current==0 && x_current<x_previous && x_current>0 && y_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_MOVE;
     }  
    
   if(y_current==0 && x_current<x_previous && x_current<0 && y_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       down=true;
       return direction_command.SERVER_TURN_LEFT;
   }
   
      
    if(y_current==0 && x_current>x_previous && x_current<0 && y_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       return direction_command.SERVER_MOVE;
    }  
    
    if(y_current==0 && x_current>x_previous && x_current>0 && y_previous==0){
       x_previous=x_current;
       y_previous=y_current;
       up=true;
       return direction_command.SERVER_TURN_RIGHT;
    }
   
   
   
    
}

string Controller::search_manager(){
    cout<<"counter "<<point_counter<<endl;
    struct server_messages direction_command;  
    cout<<"in search messenger"<<endl;
    if(step){
        step=false;
        waitingMessage=true;
        return direction_command.SERVER_PICK_UP;
    }
    
    if(x_current==0 && y_current==0){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }  
    
     
    if(x_current==x_previous && y_current==y_previous){            
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }    
    
    
    
    
    //---------------------------------------------------
    
    if(point_counter==4 && x_current==-1 && y_current==2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
    
    if(point_counter==4 && x_current==-2 && y_current==-1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
        
    if(point_counter==4 && x_current==2 && y_current==1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
        
    if(point_counter==4 && x_current==1 && y_current==-2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
    
     
    //-------------------------------------------------
    
    if(point_counter==4 && x_current==1 && y_current==1 && x_previous==2 && y_previous==1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
    
    if(point_counter==4 && x_current==1 && y_current==-1 && x_previous==1 && y_previous==-2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
        
    if(point_counter==4 && x_current==-1 && y_current==-1 && x_previous==-2 && y_previous==-1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
        
    if(point_counter==4 && x_current==-1 && y_current==1 && x_previous==-1 && y_previous==2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }   
    
    
    //-------------------------------------------------
    
    if(point_counter==4 && x_current==1 && y_current==1 && x_previous==0 && y_previous==1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
    
    if(point_counter==4 && x_current==1 && y_current==-1 && x_previous==1 && y_previous==0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
        
    if(point_counter==4 && x_current==-1 && y_current==-1 && x_previous==0 && y_previous==-1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
        
    if(point_counter==4 && x_current==-1 && y_current==1 && x_previous==-1 && y_previous==0){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
     
      
    
    //-------------------------------------------------
    
    
    if(x_current==0 && (y_current>0 && y_current<2)){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }     
        
    if(x_current==0 && (y_current<0 && y_current>-2)){            
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }  
        
    if((x_current>0 && x_current<2) && y_current==0){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }  
        
    if((x_current<0 && x_current>-2) && y_current==0){            
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }      
    
    //--------------------------------------
    
    if(x_current==0 && y_current==2 && x_previous==0 && y_previous==1){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        return direction_command.SERVER_TURN_RIGHT;      
    }            
    
    if(x_current==0 && y_current==2 && x_previous!=0 && y_previous!=1){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        step=true;
        return direction_command.SERVER_MOVE;
    }
   
    //---------------------------------------
   
    if(x_current==0 && y_current==-2 && x_previous==0 && y_previous==-1){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        return direction_command.SERVER_TURN_RIGHT;
    }        
        
    
    if(x_current==0 && y_current==-2 && x_previous!=0 && y_previous!=-1){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        step=true;
        return direction_command.SERVER_MOVE;        
    }        
        
        
    //---------------------------------------    
   
    if(x_current==2 && y_current==0 && x_previous==1 && y_previous==0){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        return direction_command.SERVER_TURN_RIGHT;
    }
        
    if(x_current==2 && y_current==0 && x_previous!=1 && y_previous!=0){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        step=true;
        return direction_command.SERVER_MOVE;
    }    
    
    //--------------------------------------    
        
    if(x_current==-2 && y_current==0 && x_previous==-1 && y_current==0){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        return direction_command.SERVER_TURN_RIGHT;
    }            
       
    if(x_current==-2 && y_current==0 && x_previous!=-1 && y_current!=0){
        x_previous=x_current;
        y_previous=y_current;
        point_counter++;
        step=true;
        return direction_command.SERVER_MOVE;
    }            
        
    //-------------------------------------
    
    if(x_current==2 && y_current==2 && x_previous==1 && y_previous==2 ){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }

    if(x_current==2 && y_current==-2 && x_previous==2 && y_previous==-1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
    
    if(x_current==-2 && y_current==-2 && x_previous==-1 && y_previous==-2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
            
    if(x_current==-2 && y_current==2 && x_previous==-2 && y_previous==1){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_RIGHT;
    }
    
    if(x_current==2 && y_current==2 && x_previous==2 && y_previous>2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }

    if(x_current==2 && y_current==-2 && x_previous==2 && y_previous<-2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
    
    if(x_current==-2 && y_current==-2 && x_previous==-2 && y_previous<-2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }
            
    if(x_current==-2 && y_current==2 && x_previous==-2 && y_previous>2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
    
    if(x_current==2 && y_current==2 && x_previous>2 && y_previous==2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }

    if(x_current==2 && y_current==-2 && x_previous>2 && y_previous==-2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;      
    }
    
    if(x_current==-2 && y_current==-2 && x_previous<-2 && y_previous==-2){
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
            
    if(x_current==-2 && y_current==2 && x_previous<-2 && y_previous==2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }
    
    //----------------------------------------
      
    if(x_current==2 && y_current<2 && y_current>-2 && x_previous>2){
        if(y_current==0)
            point_counter++;
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }

    if(x_current==-2 && y_current<2 && y_current>-2 && x_previous<-2){
        if(y_current==0)
            point_counter++;
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;      
    }
    
    if(x_current<2 &&x_current>-2  && y_current==-2 && y_previous<-2){
        if(x_current==0)
            point_counter++;
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
            
    if(x_current<2 && x_current>-2 && y_current==2 && y_previous>2){
        if(x_current==0)
            point_counter++;
        x_previous=x_current;
        y_previous=y_current;
        return direction_command.SERVER_TURN_LEFT;
    }
       
    //----------------------------------------
    
    if(x_current<2 || y_current<2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }

    if(x_current<2 || y_current==-2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }
    
    if(x_current<-2 && y_current<-2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }
            
    if(x_current<-2 && y_current<2){
        x_previous=x_current;
        y_previous=y_current;
        step=true;
        return direction_command.SERVER_MOVE;
    }
    
}


int main(int argc, char** argv) {
    
        
                
    if(argc<2){
        cerr<<"Usage:server port"<<endl;
        return -1;
    } 
     
     //Vytvoreni koncoveho bodu
     
     int l=socket(AF_INET,SOCK_STREAM,0);
     if(l<0){
         perror("Nemohu vytvorit socket!");
         return -1;
     }
     
     int port=atoi(argv[1]);
     if(port==0){
         cerr<<"Usage:server port"<<endl;
         close(l);
         return -1;         
     }
     
     struct sockaddr_in adresa;
     bzero(&adresa,sizeof(adresa));
     adresa.sin_family=AF_INET;
     adresa.sin_port=htons(port);
     adresa.sin_addr.s_addr=htonl(INADDR_ANY);
     
         
     //Prirazeni socketu k rozhranim
     if(bind(l,(struct sockaddr *) &adresa,sizeof(adresa))<0){
         perror("Problem s bind()!");
         close(l);
         return -1;
     }
     
     //Oznacim socket jako pasivni
     if(listen(l,10)<0){
         perror("Problem s listen()!");
         close(l);
         return -1;
     }
     
     struct sockaddr_in vzdalena_adresa;
     socklen_t velikost;
     
     
     
     while(true){
         
         //Cekam na prichozi spojeni
         int c=accept(l,(struct sockaddr *) &vzdalena_adresa,&velikost);
         if(c<0){
             perror("Problem s accept()");
             close(l);
             return -1;
         }
         
         
                      
         pid_t pid=fork();
         
         if(pid==0){
            
            
             struct timeval timeout;
             timeout.tv_sec=TIMEOUT;
             timeout.tv_usec=0;
             fd_set sockets;
             int retval;
             char buffer[BUFFER_SIZE];
             
             Message_processor  msgProcessor;
             struct Message     outputStruct;
             vector<char>       tmp_vector;
             string             client_message;
             struct Result      result;
             Controller         controller;
             bool               first_message=true;
             int                bytesRead=0;
             bool               getFirstMessage=false;       
                      
             
             
            while(true){
                
                FD_ZERO(&sockets);
                FD_SET(c,&sockets);
                       
               
               
                
               client_message="";
               
                
                while(true){
                
                                       
                   client_message="";  
                   
                     
                   outputStruct=msgProcessor.getMessage(); 
                  
                   client_message=outputStruct.message;
                    
                  
                   
                   cout<<bytesRead<<endl;
                    cout<<"client message "<<client_message<<endl;
                     
                    if(outputStruct.isMessage && !getFirstMessage){
                        getFirstMessage = true;
                    }
                   
                    if(!outputStruct.isMessage && getFirstMessage){
                        break;
                    }
                    
                    if((outputStruct.specialSigns && controller.hasUser()) || outputStruct.syntaxError)
                    {
                       cout<<"special signs error"<<endl; 
                       result.Command="301 SYNTAX ERROR\a\b";  
                       result.State=false;
                    }
                    else if(getFirstMessage){ 
                      result=controller.process(client_message);
                    }
                    
                    
                     cout<<"result "<<result.Command<<endl;
               
                  if(getFirstMessage)
                  if(send(c,result.Command.c_str(),result.Command.length(),0)<0){
                     perror("Nemohu odeslat data!");
                     close(c);
                     return -3;
                    }
                   
                     //first_message=false;
                  
                    
                   if(outputStruct.syntaxError || !result.State){ 
                        break;
                   }
                   
                                      
                   
                   
                } 
                 
               
              
                
               
                if(!result.State && getFirstMessage){
                       
                    controller.backUp();
                    getFirstMessage=false;
                    first_message=true;
                    result.Command="";
                    result.State=true;
                    result.Timeout_flag=false;
                    
                    msgProcessor.~Message_processor();
                                        
                    for(int i=0;i<string(buffer).length();i++)
                      buffer[i]=NULL;
                               
                
                    close(c);
                       
                                       
                    return -3;
                }
                 
              
                if(result.Timeout_flag)
                  timeout.tv_sec=TIMEOUT_RECHARGING;
                else
                  timeout.tv_sec=TIMEOUT;
               
               
               
               retval=select(c+1,&sockets,NULL,NULL,&timeout);
          
               if(retval<0){
                 perror("Chyba v select()!"); 
                 close(c);
                 return -1;
               }
               
               if(!FD_ISSET(c,&sockets)){
                 first_message=true;
                 getFirstMessage=false;
                 controller.backUp();
                    
                 cout<<"Connection timeout!"<<endl;
                 close(c);
                 return 0;
               } 
                   
                   
            bytesRead=recv(c,buffer,BUFFER_SIZE,0);
            
            if(bytesRead<=0){
                perror("Chyba pri cteni ze socketu");
                close(c);
                return -3;
            }
                  
            
            for(int i=0;i<bytesRead;i++)
                tmp_vector.push_back(buffer[i]);
                      
           
            msgProcessor.addMessage(tmp_vector);
            
            tmp_vector.clear();           
            
       }
              
                
                  
               
               
               
               
               
                 
                
                             
                              
                                                     
               
               
            
                                      
                
             close(c);
             return 0;
      
         }
         close(c);
                
     }
     
     close(l);     
    return 0;
}


