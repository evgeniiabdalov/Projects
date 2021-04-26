
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>


using namespace std;

struct node{
    
    long long int key;
    struct node* right;
    struct node* left;
    
};


struct response{
    string strResponse;
    long long int    intResponse;
};

class BVS{
    
    
 public:
 
     BVS(){ 
            bvs = NULL;
            responseCount = 0;
            responseSize = 10;
            responses = (struct response*)malloc(responseSize*sizeof(struct response));            
          }
     
     void         console(int command, long long int key, int mode);   

     void         printResponse();
     
     void         inorder();
     
     void         inorder(struct node* root);
     
     
private:
    
     struct response* responses;
     
     long long int              responseCount;       
    
     long long int              responseSize;
     
     struct node* bvs;
       
     struct node* newNode(long long int value);
      
     struct node* minValueNode(struct node* node);
 
     void         addResponse(string response);
     
     void         addResponse(int response);
     
     struct node* insert(struct node* node, long long int key);
     
     struct node* searchParent(struct node* root, long long int key);
     
     struct node* searchSuccessor(struct node* root, struct node* prevNode, long long int key);
     
     struct node* rotate(struct node* root, struct node* prevNode, long long int key, int mode, int side);
     
     struct node* deleteNode(struct node* root, long long int key);
     
     
};

void BVS::inorder() 
{   cout<<"inorder"<<endl;
    if (this->bvs != NULL) 
    { 
        inorder(this->bvs->left); 
        cout<<this->bvs->key<<' '; 
        inorder(this->bvs->right); 
    }
    cout<<endl;
} 

void BVS::inorder(struct node* root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->left); 
        cout<<root->key<<' '; 
        inorder(root->right); 
    } 
} 

void BVS::addResponse(string response){
    
    struct response structResponse;
    
    structResponse.strResponse = response;
    structResponse.intResponse = -1;   
    
    
    this->responses[responseCount] = structResponse;
    
    this->responseCount++;
    
    if(this->responseCount >= this->responseSize){
        
        this->responseSize += 10;

        responses = (struct response*)realloc(responses, responseSize*sizeof(struct response));  
 
    
    }
    
    
}

void BVS::addResponse(int response){
    
    struct response structResponse;
    
    structResponse.intResponse = response;
    structResponse.strResponse = "null";
    
    this->responses[responseCount] = structResponse;

    this->responseCount++;
    
    if(this->responseCount >= this->responseSize){
        
        this->responseSize += 10;
        
        responses = (struct response*)realloc(responses, responseSize*sizeof(struct response));    
    
    }
    
    
}


struct node* BVS::newNode(long long int value){
    
    struct node* node = (struct node*)malloc(sizeof(struct node));
    
    node->key = value;
    node->right = NULL;
    node->left = NULL;
    
    return node;    
}

struct node* BVS::insert(struct node* node,long long  int key){
    
    if(node == NULL)
        return newNode(key);
    
    if(key < node->key)
        node->left = insert(node->left, key);
    else if(key > node->key)
        node->right = insert(node->right, key);
    
    return node;   
    
}

struct node* BVS::minValueNode(struct node* node){
    
    
    struct node* current = node;
    
    while(current->left != NULL)
        current = current->left;
    
    return current;
    
    
}

struct node* BVS::searchSuccessor(struct node* root, struct node* prevNode,long long  int key){
        
    if(root == NULL){
        this->addResponse("notfound");
        return root;
    }
    
    
    if(root->key == key){
        
        if(root->right != NULL){
            struct node* nextNode = minValueNode(root->right);
            this->addResponse(nextNode->key);
            return root;
        }
        else if(prevNode != NULL){
            this->addResponse(prevNode->key);
            return root;
        }
        else{
            this->addResponse("nosuccessor");
            return root;
        }
        
    }
    
    
    
    if(root->key < key)
        return searchSuccessor(root->right, prevNode, key);
    
    if(root->key > key)
        return searchSuccessor(root->left, root, key); 
    
    return root;
       
}

struct node* BVS::searchParent(struct node* root, long long int key){
    
    if(root == NULL){
        this->addResponse("notfound");
        return root;
    }
    
    
    if(root->key == key){        
        this->addResponse("noparent");
        return root;
    }   
    
    if(root->right != NULL)
     if(root->right->key == key){
        this->addResponse(root->key);
        return root;
     }
    
    if(root->left != NULL)
      if(root->left->key == key){
        this->addResponse(root->key);
        return root;
      }  
    
    
    if(root->key < key){
        return searchParent(root->right, key);
    }
    if(root->key > key){
        return searchParent(root->left, key);
    }
    
    return root;
}  

struct node* BVS::rotate(struct node* root, struct node* prevNode, long long int key, int mode, int side){
    
    if(root == NULL){
        this->addResponse("notfound");
        return root;
    }
    
    
    if(root->key == key){
        
        if(mode == 1){
            
            if(root->right == NULL){
                this->addResponse("norotate");
                return root;
            }
            
            struct node* tmpNode = this->newNode(root->key);
            struct node* mainNode = this->newNode(root->right->key);
            
            tmpNode->left = root->left;
            tmpNode->right = root->right->left;            
            
            
            mainNode->right = root->right->right;
            mainNode->left = tmpNode;
            
            if(side == 1)
                prevNode->left = mainNode;
            else if(side == 2)
                prevNode->right = mainNode;
            
            return mainNode;            
            
        }
        else if(mode == 2){
            
            if(root->left == NULL){
                this->addResponse("norotate");
                return root;
            }
            
            struct node* tmpNode = this->newNode(root->key);
            struct node* mainNode = this->newNode(root->left->key);
                        
            
            tmpNode->right = root->right;
            tmpNode->left = root->left->right;
            
            mainNode->left = root->left->left;
            mainNode->right = tmpNode;
            
            
            if(side == 1)
                prevNode->left = mainNode;
            else if(side == 2)
                prevNode->right = mainNode;
            
            return mainNode;          
                        
            
        }
    
        
    }
        
    
    if(root->key < key)
        root->right = rotate(root->right, root, key, mode, 2);
    
    if(root->key > key)
        root->left = rotate(root->left, root, key, mode, 1); 
    
    return root;
       
}

struct node* BVS::deleteNode(struct node* root, long long int key){
    
    if(root == NULL)
        return root;
       
    
    if(key < root->key)
        root->left = deleteNode(root->left, key);
    else if(key > root->key)
        root->right = deleteNode(root->right, key);
    else{
        
        if(root->left == NULL){
            
            struct node* temp = root->right;
            free(root);
            return temp;
            
        }
        else if(root->right == NULL){
            
            struct node* temp = root->left;
            free(root);
            return temp;
            
        }
        
        struct node* temp = minValueNode(root->right);
        
        root->key = temp->key;
        
        root->right = deleteNode(root->right, temp->key);
        
        
    }
    
    return root;
    
        
    
}

void BVS::console(int command, long long int key, int mode){
    
    
    if(command == 1){
       this->bvs = this->insert(this->bvs, key);
    }
    else if(command == 2){
       this->bvs = this->deleteNode(this->bvs, key);
    }    
    else if(command == 3){
        this->searchParent(this->bvs, key);
    }
    else if(command == 4){
        this->searchSuccessor(this->bvs, NULL, key);
    }
    else if(command == 5){
        this->bvs = this->rotate(this->bvs, NULL, key, mode, 0);
    }
    
    
}


void BVS::printResponse(){
    
    
    for(int i = 0; i < this->responseCount; i++){
        if(this->responses[i].intResponse == -1)
          cout<<this->responses[i].strResponse<<endl;
        else
          cout<<this->responses[i].intResponse<<endl;  
    }

}


int main(int argc, char** argv) {

    
    BVS bvs;
    
    int command = 0;
    int key = 0;
    int mode = 0;
    
    
    while(true){
        
        cin>>command;
        
        if(command == 6)
            break;        
        else if(command == 5){
            cin>>key>>mode;
            bvs.console(command,key,mode);
        }
        else{
            cin>>key;
            bvs.console(command,key,0);                    
        }
       
        
    }
    
    bvs.printResponse();
    bvs.inorder();      
    
    return 0;
}

