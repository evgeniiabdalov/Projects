

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>

using namespace std;

// Node is an element of decoding tree,
// each node represents a translation from coded word to decoded word
// it keeps indexes of codedWord and decodedWord
// it has a reference to previous Node, as we go from the bottom to the top of the tree 
struct Node{

    pair<int, int> codedDecodedIndex;
    struct Node *  previousNode;

};

// CodeRule class keeps rule about codedWord -> decodedWord translation
class CodeRule{
          
    public:
        
       bool   setCodeRule(string codedWord, string decodedWord);        

       bool   checkWord(string codedWord, string decodedWord);

       void   showCodeRule();

       int     getWordSize(){return this->wordSize;};

    private:
 
        map<char, char> codeRuleMap; // ngk -> dog: {key:d, value:n}, {key:o, value:g}, {key:g, value:k} 
        set<char>       codeRuleSet; // values of codeRuleMap, check if there is no duplicities in translation
        int             wordSize;

};



// setting code rule between codedWord and decodedWord 
bool CodeRule::setCodeRule(string codedWord, string decodedWord){

            
            this->wordSize = codedWord.size();   

            for(int i = 0; i < codedWord.size() && decodedWord.size(); i++){

                  if( codeRuleSet.find(codedWord[i]) == codeRuleSet.end() ){
                       codeRuleMap.insert({decodedWord[i], codedWord[i]}); // inserting new rule           
                       codeRuleSet.insert(codedWord[i]); // inserting word to check for duplicities                       
                  }
                  else{
                       
                       // word is already in set

                       char previousCoded = codeRuleMap.find(decodedWord[i])->second;
                      
                       if(codedWord[i] == previousCoded){
                         // correct decoding 
                         continue;
                       }
                     
                      // incorrect decoding
                      return false;

                  }

            }

            return true;
                  
               
}


// displaying code rule between codedWord and decodedWord
void  CodeRule::showCodeRule(){

            map<char, char>::iterator codeRuleMapIt;

            for( codeRuleMapIt = codeRuleMap.begin(); codeRuleMapIt != codeRuleMap.end(); codeRuleMapIt++ ){

                  cout << "decoded " << codeRuleMapIt->first << " -> coded " << codeRuleMapIt->second << endl; 

            }    


}

// check if translation codedWord -> decodedWord doesn't contradict the translation of given CodeRule
bool CodeRule::checkWord(string codedWord, string decodedWord){

     int word = codedWord.size(); 
     
     for( int i = 0; i < word; i++ ){
   
          char key = decodedWord[i];  

          
          map<char, char>::iterator codeRuleMapIt = codeRuleMap.find(key);

          if( codeRuleMapIt != codeRuleMap.end() ){
             
             char coded = codeRuleMapIt->second;
              
             
             if( codedWord[i] != coded )
                    return false;   
                                 
          }

     }

     return true;  
    
    
}

// Decoder class is responsible for decoding process
class Decoder{

   public:

      Decoder(){
           globalCounter = 0;
      };

      void addWord(string word);

      void createWordsVector(string inputLine);

      void showDictionary();

      void showCodeRulesWords();

      void showCodeRulesMap();

      void createOutputMessage(struct Node * node);

      void showResults();

      void decode(struct Node * currentNode, int codedWordIndex);

   private:
   
       bool                          isWordPossible(struct Node * currentNode, string codedWord, string decodedWord);

       int                           globalCounter; // counter of possible translations

       map<int, vector<string>>      dictionary; // key int is words length, value vector is a vector of words woth correspoding length 

       map<string, int>              dictionaryIndexes; // each word has an index in order to simplify word search       
  
       map<int, int>                 searchIndexes;  // key is a unique identifier 

       vector<string>                codedWords; // coded message is kept as a vector of coded words

       
       map<pair<int, int>, CodeRule> codeRulesMap; // key is a pair of coded word index and decoded word( word from Dictionary) index,
                                                  // value is an istance of CodeRule object 

       vector<string>                outputMessage; // represents decoded message which is a result of decoding process


};

// adds word to the dictionary
void Decoder::addWord(string word){

           int searchIndex = 0; 
 
           //---------------------------------------------------------------------------
 
           int dictionaryIndex = dictionaryIndexes.size();
           
           dictionaryIndexes.insert({word, dictionaryIndex});

           //---------------------------------------------------------------------------

           int wordSize = word.size();

           searchIndex = wordSize;

           map<int, vector<string>>::iterator dictionaryIt = dictionary.find(wordSize); 

           if( dictionaryIt != dictionary.end() ){
  
                  searchIndex += (dictionaryIt->second).size();

                  (dictionaryIt->second).push_back(word);

           } 
           else{

               vector<string> tmpVector;
               tmpVector.push_back(word);

               dictionary.insert({word.size(), tmpVector});

           }          
  

           searchIndexes.insert({dictionaryIndex, searchIndex});


}

// creates vector of coded words by parsing input coded message
void Decoder::createWordsVector(string inputLine){

    int startPosition = 0; 

    int inputSize = inputLine.size();
    int endPosition = inputSize - 1;

    for(int i = 0; i < inputSize; i++){

        if( inputLine[i] == ' ' ){
              
            string codedWord = inputLine.substr(startPosition, i - startPosition);

            codedWords.push_back(codedWord);  
            
            startPosition = i+1;
        
        }
        else if( i == endPosition ){

           string codedWord = inputLine.substr(startPosition, i - startPosition + 1);

           codedWords.push_back(codedWord);  
      
        }

    }

    
}

// check if translation codedWord -> decodedWord is possible, i.e.
// translation codedWord -> decodedWord doesn't contradict previous translations in the tree 
bool Decoder::isWordPossible(struct Node * currentNode, string codedWord, string decodedWord){

        
       if(currentNode == NULL)
           return true;  

       
       while(currentNode != NULL){

            //check if word is possible

            pair<int, int> codedDecodedIndex = currentNode->codedDecodedIndex;

             
            if( codedDecodedIndex.first < codedWords.size() ){

 
               CodeRule codeRule = codeRulesMap.find(codedDecodedIndex)->second;
            
               
               if( !codeRule.checkWord( codedWord, decodedWord )){
                     return false;
               }


            }

            currentNode = currentNode->previousNode;

         
        }
    
        
       return true;   

}


// runs the overall decoding process
void Decoder::decode(struct Node * currentNode, int codedWordIndex){
    
    
    if(codedWordIndex >= codedWords.size()){
        
         globalCounter++;

         
        if( globalCounter == 1 ) 
          createOutputMessage(currentNode);

         return;
    }
        

    string codedWord = codedWords[codedWordIndex];
    
    int wordSize = codedWord.size(); 

    
    map<int, vector<string>>::iterator dictionaryIt = dictionary.find(wordSize);

    vector<string> dictionaryVector;

    if( dictionaryIt != dictionary.end() ){

      dictionaryVector = dictionary.find(wordSize)->second;

    }
                      
  
    int dictionarySize = dictionaryVector.size(); 

    
      for( int i = 0; i < dictionarySize; i++ ){
               
    
            //chars in dictionary      
         
            if( isWordPossible(currentNode, codedWord, dictionaryVector[i]) ){
  
                  int codeRuleCounter = 0;

                  int decodedWordIndex = dictionaryIndexes.find(dictionaryVector[i])->second;

                  //-------------------------------------------------------------------- 

                  pair<int, int> codeRuleKey = pair<int, int>(codedWordIndex, decodedWordIndex);

                  //---------------------------------------------------------------------

                  map<pair<int, int>, CodeRule>::iterator codeRulesMapIt = codeRulesMap.find(codeRuleKey);
                  

                  if( codeRulesMapIt == codeRulesMap.end() ){

                      CodeRule codeRule;

                      if( !codeRule.setCodeRule(codedWord, dictionaryVector[i]) ){
                          continue;
                      }
                  
                            codeRulesMap.insert({codeRuleKey, codeRule});

                  }        

                  //----------------------------------------------------------

                  Node * tmpNode = new Node;

                  tmpNode->codedDecodedIndex = codeRuleKey;
            
                  tmpNode->previousNode = currentNode; 

                  //---------------------------------------------------------
  
                  int nextCodedWordIndex = codedWordIndex + 1;

                  decode(tmpNode, nextCodedWordIndex);
 
                  //check all possible decodings
                  //call decode() function for each variant   
                                                                         
            } 
            
      }

     

}

// displays Dictionary content
void Decoder::showDictionary(){

    map<int, vector<string>>::iterator  dictionaryIt;
    
    for( dictionaryIt = dictionary.begin(); dictionaryIt != dictionary.end(); dictionaryIt++ ){

          vector<string> dictionaryVector = dictionaryIt->second;

          for( int i = 0; i < dictionaryVector.size(); i++ ){

               cout << "dictionary word: " << dictionaryVector[i] << endl;

          }

          cout << "----------------------------" << endl;

    }


    map<string, int>::iterator  dictionaryIndexesIt; 

    for( dictionaryIndexesIt = dictionaryIndexes.begin(); dictionaryIndexesIt != dictionaryIndexes.end(); dictionaryIndexesIt++ ){

         cout << "dictionary word " << dictionaryIndexesIt->first << " index " << dictionaryIndexesIt->second << endl;  

    }

}  

// displays words in CodeRule
void Decoder::showCodeRulesWords(){

    map<pair<int, int>, CodeRule>::iterator codeRulesMapIt;

    for( codeRulesMapIt = codeRulesMap.begin(); codeRulesMapIt != codeRulesMap.end(); codeRulesMapIt++ ){

         pair<int, int> codeRuleKey = codeRulesMapIt->first;
         CodeRule codeRule = codeRulesMapIt->second;          

    }

    cout << endl; 


}

// displays CodeRule
void Decoder::showCodeRulesMap(){

    map<pair<int, int>, CodeRule>::iterator codeRulesMapIt;

    for( codeRulesMapIt = codeRulesMap.begin(); codeRulesMapIt != codeRulesMap.end(); codeRulesMapIt++ ){

         pair<int, int> codeRuleKey = codeRulesMapIt->first;
         CodeRule codeRule = codeRulesMapIt->second;

         cout << "codeRulesKey codedIndex: " << codeRuleKey.first << " decodedIndex: " << codeRuleKey.second << endl;
         codeRule.showCodeRule(); 

    }

    cout << endl; 

}

// puts decoded words in vector which functions as an output message
void Decoder::createOutputMessage(struct Node * node){

        if(node == NULL)
           return;  

        while(node != NULL){

             pair<int, int> keyPair = node->codedDecodedIndex;

             CodeRule codeRule = codeRulesMap.find(keyPair)->second; 

             int wordSize = codeRule.getWordSize(); 

             int decodedIndex = keyPair.second;

             int searchIndex = searchIndexes.find(decodedIndex)->second;
 
  
             vector<string> wordsVector = dictionary.find(wordSize)->second;

             string decodedWord = wordsVector[searchIndex - wordSize];

                      
             outputMessage.push_back(decodedWord);             
 
             
             node = node->previousNode;
  
        }

       cout << endl;
}


void Decoder::showResults(){

       cout << "--------------------------------------" << endl;

       if(globalCounter == 1){

            for(int i = outputMessage.size() - 1; i >= 0 ; i--){
              cout << outputMessage[i] << " ";
            }
          
         cout << endl;  
       }
       else if(globalCounter == 0){
          cout << "There is no solutions." << endl;
       }
       else{
          cout << "Number of solutions: " << globalCounter << endl; 
       } 
  

}

int main()
{
  
    Decoder decoder; 

    
    decoder.addWord("a");
    decoder.addWord("brown");
    decoder.addWord("dog");
    decoder.addWord("fox");
    decoder.addWord("jumps");
    decoder.addWord("lazy");
    decoder.addWord("over");
    decoder.addWord("quick");
    decoder.addWord("the");
    decoder.addWord("unambiguous");

    decoder.showDictionary();

    cout << "-------------------------------------------------------------------" << endl;
    

    decoder.createWordsVector("ngk qwpl doxcy"); 

    decoder.decode(NULL, 0);
    
    
    /*
    
    decoder.createWordsVector("jlxkzmvjpjb ghi qjmsu zrpwl fpt ojkyb pdir x exac npv"); 

    decoder.decode(NULL, 0);
    
    */

    /*
    
    decoder.createWordsVector("iby zfgld ksahc qam jftxn avys e weuo par"); 

    decoder.decode(NULL, 0);

    */

    /* 
    decoder.createWordsVector("iby zfgld ksahc qam jftxn avys x weuo par"); 

    decoder.decode(NULL, 0);

    */

    /*

    decoder.createWordsVector("jlxkzmvjpjb ghi qjmsu zrpwl fpt ojkyb pdir x exac npv"); 

    decoder.decode(NULL, 0);
  
    */
 
    decoder.showResults();    

    return 0;
}
