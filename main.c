/**
This program aims to create a miniature dictionary by implementing two important data structures, the AVL tree and the HASH. Of course, what's a
dictionary without tools that optimize its functionality? This program aims to provide as much diverse tools as possible to make it as efficient as
possible**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<conio.h>
#include <windows.h>

///Defining necessary types that facilitate the process performed latterly.
typedef struct wordNode *wordPtr;
typedef wordPtr pos;
typedef struct meaningList *meaningPtr;
typedef struct hashNode* hashPtr;

///A linked list structure for the meanings of the words.
struct meaningList{
    char meaning[15];
    struct meaningList *next;
};
/// An AVL tree structure for the dictionary.
struct wordNode{
  ///The contents of each node in the AVL tree.
  char word[15];
  char wordSyn[15];
  char wordAnt[15];
  ///A pointer to the meanings linked list.
  meaningPtr wordMeaning;

  ///AVL tree necessary equipment.
  int height;
  pos left;
  pos right;
};

///This is the struct node for the hash table.
struct hashNode{
  ///The contents of each node in the Hash Table.
  char wordHash[15];
  char synHash[15];
  char antHash[15];
  ///A pointer to the meanings linked list.
  meaningPtr hashMeaning;
  int flag;


};
///A local variable used in the printLexicoFirstChar function.
wordPtr firstChar = NULL;
///More local variables
int numRecords = 0;
int reHashFlag = 0;

///Declaring the prototypes of functions used in this program.
wordPtr insert( wordPtr w, char inputWord[15], char syn[15], char ant[15], meaningPtr listMeaning);
meaningPtr insertMeaning(meaningPtr meaningRoot , char  inputMeaning[15]);
wordPtr singleRotateLeft(wordPtr w2);
wordPtr singleRotateRight(wordPtr w1);
wordPtr doubleRotateLeft(wordPtr w3);
wordPtr doubleRotateRight(wordPtr w3);
void printMenuAVL();
wordPtr findWord(wordPtr w , char key[15]);
wordPtr updateWord(wordPtr w , char key[15]);
void printLexico(wordPtr w);
void printLexicoFirstChar(wordPtr w, char charKey);
wordPtr deleteNode(wordPtr w , char wordDel[15]);
wordPtr deleteNodeFirstChar(wordPtr w , char delKey);
wordPtr minValueNode(wordPtr w);
void saveToFile(wordPtr w, FILE* pFile);
void printMenuHash();
void switchToHash();
int computeSize();
int isPrime(int x);
void insertHash(hashPtr hash[] , char word[15] , char syn[15] , char ant[15] , meaningPtr meaning , int size);
void createHashTable(hashPtr hash[],int size);
void reHash(hashPtr oldHash[], hashPtr newHash[], unsigned int oldSize, unsigned int newSize);




int main(){

    int choice = 0;
    char fileLine[100];
    char* word;
    char* arrOfWords[10];
    int numOfMeanings,j,k;

    ///dictionaryRoot is the root for the dictionary AVL tree.
    wordPtr dictionaryRoot = NULL;
    FILE* pFile = fopen("inputFile.txt", "r");



    if (pFile == NULL){
        printf("The file you're wishing to access is not found!");
    }

    ///The process of extracting the word and its associated information.
    while (fgets(fileLine, 100, pFile) != NULL){
        int i=0;
        meaningPtr meaningRoot = NULL;
        word = strtok(fileLine,":,/*");
        while(word != NULL){
            arrOfWords[i]=word;
            word = strtok(NULL, ":,/*");
            i++;
        }

    numOfMeanings= i-3;

    if(numOfMeanings>1){
        for(j=1; j<=numOfMeanings; j++){
            meaningRoot = insertMeaning(meaningRoot , arrOfWords[j]);
        }
    }
    else{
        meaningRoot = insertMeaning(meaningRoot , arrOfWords[1]);
    }

///Inserting each word in the file as a node with its information.

    dictionaryRoot = insert(dictionaryRoot , arrOfWords[0], arrOfWords[i - 2] , arrOfWords[i - 1] , meaningRoot);



}
    printf("\t\t\t\t**************************************************");
    printf("                                                          ");
    printf("\t\t*Hello user, welcome to this miniature dictionary!*");
    printf("                                                  ");
    printf("\t\t\t**************************************************\n\n");
    printf("This program provides you with various operations that aim to optimize the functionality of this dictionary.\n\n");
    printMenuAVL();

    while(choice != 10){
        ///Get the user's choice of operations.
        choice = getchar() - '0';
        char userWord[15], userAnt[15], userSyn[15];
        meaningPtr userMeaning = NULL;
        char userMean[15];
        int choiceMean;
        switch(choice){
            case 1: {
                printf("Please enter the word you'd like to add to the dictionary: \n");
                scanf("%s", &userWord);
                printf("Please enter the meaning of the word: \n");
                scanf("%s", &userMean);
                userMeaning = insertMeaning(userMeaning, userMean);
                printf("Do you have another meaning? \n1.Yes\n2.No\n");
                scanf("%d", &choiceMean);
                if(choiceMean == 1){
                printf("Please enter the next meaning: \n");
                scanf("%s", &userMean);
                userMeaning = insertMeaning(userMeaning, userMean);
                }

                printf("Please enter the antonym of the word: \n");
                scanf("%s", &userAnt);
                printf("Please enter the synonym of the word: \n");
                scanf("%s", &userSyn);
                dictionaryRoot = insert(dictionaryRoot, userWord, userSyn, userAnt, userMeaning);
                userMeaning = NULL;
                printMenuAVL();
            }
            break;

            case 2:{
                char updateKey[15];
                printf("Please enter the word you wish to update: \n");
                scanf("%s", &updateKey);
                wordPtr temp = updateWord(dictionaryRoot , updateKey);
                if(temp != NULL)
                    printf("Updating successful!\n\n");
                else
                    printf("The word you want to update is not found!\n\n");
                printMenuAVL();
            }
            break;

            case 3: {
                printLexico(dictionaryRoot);
                printMenuAVL();
            }
            break;

            case 4:{
                char key[15];
                printf("Please enter the word you'd like to list the synonym and the antonym for: \n");
                scanf("%s", &key);
                wordPtr treasure = findWord(dictionaryRoot , key);
                if(treasure != NULL){
                    printf("The synonym of the word is: %s\n", treasure->wordSyn);///If the top node is the word we want then return it.
                    printf("The antonym of the word is: %s\n", treasure->wordAnt);
                }
                else{
                    printf("Sorry! Word not found!\n\n");
                }
                printMenuAVL();
            }
            break;

            case 5:{
                char k[15];
                printf("Please enter the character in which you'd like to list the words that start with it lexicographically: \n");
                scanf("%s", &k);

                printLexicoFirstChar(dictionaryRoot , k[0]);
                if(firstChar != NULL)
                    printLexico(firstChar);
                else
                    printf("\nThe dictionary doesn't have words that start with the character you entered\n\n");
                printMenuAVL();
            }
            break;

            case 6:{
                char wordDelete[15];
                printf("Please enter the word you want to delete from the dictionary: \n");
                scanf("%s", &wordDelete);
                dictionaryRoot = deleteNode(dictionaryRoot , wordDelete);

                printMenuAVL();
            }
            break;

            case 7:{
                char charDelete[15];
                printf("Please enter the character you want to delete the words that start with that character from the dictionary: \n");
                scanf("%s", &charDelete);
                dictionaryRoot = deleteNodeFirstChar(dictionaryRoot , charDelete[0]);

                printMenuAVL();
            }
            break;
            case 8:{
                FILE* pFile;
                pFile = fopen("out.txt", "w");
                saveToFile(dictionaryRoot , pFile );

                fclose(pFile);
                printMenuAVL();
            }
            break;

            case 9:{
            switchToHash();
            }
            break;

            case 10: exit(-1);
            break;

        }



    }

    return 0;
}
///Utility function that returns the height of a certain node.
 int getHeight(wordPtr w){
    if(w==NULL)
        return -1;
    else
        return w->height;
}

///Utility function that calculates the maximum number between two numbers.
int maxNum(int x, int y){
    if(x>y)
        return x;
    else
        return y;
}
///This function inserts a node in the AVL tree by passing to it the word, its meanings, its synonym and its antonym.
wordPtr insert( wordPtr w, char inputWord[15], char syn[15], char ant[15], meaningPtr listMeaning){

///If the dictionary pointer to node passed as an argument to this function points to NULL, create a new node.
    if(w==NULL){
        w = malloc(sizeof (struct wordNode));
        strcpy(w->word , inputWord);
        strcpy(w->wordSyn , syn);
        strcpy(w->wordAnt , ant);
        w->wordMeaning = listMeaning;

        w->height = 0;
        w->left = NULL;
        w->right = NULL;
    }

///Else, compare the ASCII value of the word to be inserted and the the word already there.
///If the word to be inserted is less, then move insert it onto the left of the tree.
     else if(strcmp(inputWord , w->word) < 0 ){
        w->left=insert(w->left , inputWord , syn , ant , listMeaning);
        if(getHeight(w->left) - getHeight(w->right) == 2){
            if(strcmp(inputWord , w->left->word) < 0)
                w=singleRotateLeft(w);
            else
                w=doubleRotateLeft(w);
        }
    }
///If the word to be inserted is greater, then move insert it onto the right of the tree.
    else if(strcmp(inputWord , w->word) > 0){
        w->right = insert(w->right , inputWord , syn , ant , listMeaning);
        if(getHeight(w->right) - getHeight(w->left) == 2){

            if(strcmp(inputWord , w->right->word) > 0)
                w=singleRotateRight(w);
            else
                w=doubleRotateRight(w);
        }
    }

    w->height = 1 + maxNum(getHeight(w->right) , getHeight(w->left));

    return w;
}

///Insertion function for the meanings linked list that returns the root of the linked list.
meaningPtr insertMeaning(meaningPtr meaningRoot , char  inputMeaning[10]){

    meaningPtr movePtr = meaningRoot;
    if (movePtr == NULL)
    {
        meaningPtr curr = (meaningPtr)malloc(sizeof (struct meaningList));
        meaningRoot = curr;
        strcpy(curr->meaning , inputMeaning);
        curr->next = NULL;


    }
    else
    {
        while( movePtr->next != NULL)
        {
            movePtr = movePtr->next;
        }
        meaningPtr curr = (meaningPtr)malloc(sizeof (struct meaningList));
        movePtr->next = curr;
        strcpy(curr->meaning , inputMeaning);
        curr->next = NULL;

    }
return meaningRoot;

}

///The following are the utility functions for maintaining the balance for the AVL tree.
wordPtr singleRotateLeft(wordPtr w2) {
    wordPtr w1;
    w1        = w2->left;
    w2->left  = w1->right;
    w1->right = w2;

    w2->height = 1 + maxNum(getHeight(w2->right) , getHeight(w2->left));
    w1->height = 1 + maxNum(getHeight(w2) , getHeight(w1->left));

    return w1;
}
wordPtr singleRotateRight(wordPtr w1) {
    wordPtr w2;
    w2        = w1->right;
    w1->right  = w2->left;
    w2->left = w1;

    w1->height = 1 + maxNum(getHeight(w1->left) , getHeight(w1->right));
    w2->height = 1 + maxNum(getHeight(w1) , getHeight(w2->right));

    return w2;
}
wordPtr doubleRotateLeft(wordPtr w3) {
    w3->left  = singleRotateRight(w3->left);
    return singleRotateLeft(w3);
}
wordPtr doubleRotateRight(wordPtr w3) {
    w3->right  = singleRotateLeft(w3->right);
    return singleRotateRight(w3);
}

///The menu presented to the user as a guide.
void printMenuAVL(){

            printf("\n");
            printf("1]Insert a new word into the dictionary.\n");
            printf("2]Search for a word and in case it's present, update its information.\n");
            printf("3]List words and their associated information lexicographically.\n");
            printf("4]List synonyms and antonyms of a word.\n");
            printf("5]List all words lexicographically that have the same first character.\n");
            printf("6]Delete a word from the dictionary.\n");
            printf("7]Delete all words that start with a specific letter.\n");
            printf("8]Save the updated words to a file.\n");
            printf("9]Switch to the HASHING mode.\n");
            printf("10]Quit the program, peacefully.\n\n\n");
            printf("So, which operation from the list above would you like to perform?\n\n");

}

///This function searches for a specific word in the dictionary and if it finds it, it returns the word. If not, it returns NULL.
wordPtr findWord(wordPtr w, char key[15]){
 wordPtr wordTemp = w;
 if(wordTemp != NULL)
        {
            if(strcmp(wordTemp->word, key) == 0){
                return wordTemp;
            }
            else if(strcmp(wordTemp->word, key)>0)
                wordTemp = wordTemp->left;///If the top is not the word we want and the word big than the top then go left.
            else
                wordTemp = wordTemp->right;///Else go right.
        }
return NULL;
}

///This function is used to update a given word in the dictionary/
wordPtr updateWord(wordPtr w , char key[15]){
    wordPtr temp = w;
    meaningPtr tempMean = w->wordMeaning;
    int option;
    char upMean[15], upSyn[15], upAnt[15];
while(temp != NULL)
        {
            if(strcmp(temp->word, key) == 0){

               printf("Would you like to enter a new meaning?\n1.Yes\n2.No\n");
               scanf("%d", &option);
               if(option == 1){

                printf("Please enter the meaning you would like to add: ");
                scanf("%s", &upMean);
                tempMean = insertMeaning(temp->wordMeaning, upMean);
               }
               printf("Would you like to replace the synonym?\n1.Yes\n2.No\n");
               scanf("%d", &option);
               if(option == 1){
                printf("Please enter the new synonym you'd like to replace the original one with:\n");
                scanf("%s", &upSyn);
                strcpy(temp->wordSyn , upSyn);
               }
                printf("Would you like to replace the antonym?\n1.Yes\n2.No\n");
               scanf("%d", &option);
               if(option == 1){
                printf("Please enter the new antonym you'd like to replace the original one with:\n");
                scanf("%s", &upAnt);
                strcpy(temp->wordAnt , upAnt);
               }
               return temp;
            }
            ///If the value of the word we want to update is less than the word pointed at, go left.
            else if(strcmp(temp->word, key)>0)
                temp = temp->left;
            else
                ///Else, go right.
                temp = temp->right;
        }



}

///This function prints all the words that start with the specified letter, by traversing through the AVL tree and checking the first character of
///each word and if it met the conditions then it is added to the local variable firstChar.
void printLexicoFirstChar(wordPtr w, char charKey){

    wordPtr temp = w;
    if(temp != NULL){
        printLexicoFirstChar(temp->left , charKey);
        if(temp->word[0] == charKey){
        firstChar = insert(firstChar , temp->word , temp->wordSyn , temp->wordAnt , temp->wordMeaning);
        }
        printLexicoFirstChar(temp->right , charKey);
    }
}
///This function is an INNORDER traversal that prints the words and their associated data lexicographically.
void printLexico(wordPtr w){

    if(w!= NULL){
       printLexico(w->left);
       printf("The word is: %s\n",w->word);
       printf("Its meaning/meanings:");
       meaningPtr temp = w->wordMeaning;
       while (temp != NULL){
        printf("%s ", temp->meaning);
        temp = temp->next;
       }
       printf("\nThe synonym of the word is: %s\n", w->wordSyn);
       printf("The antonym of the word is: %s\n", w->wordAnt);
       printLexico(w->right);

    }
}

///Utility function used in the deleteNode function to find the node with the minimum value.
wordPtr minValueNode(wordPtr w)
{
    if( w == NULL )
            return NULL;
        else if( w->left == NULL )
            return w;
        else
            return minValueNode( w->left );
}
///This function deletes a word from the dictionary by traversing through the dictionary and comparing the value of the word to delete and the words
///in the dictionary, if found, it frees the node and adjusts the tree.
wordPtr deleteNode(wordPtr w, char wordDel[15]){
           wordPtr temp;

           if( w == NULL ){
                printf("Element not found!\n");
                return NULL;
            }

           else if (strcmp(wordDel , w->word ) < 0){
                w->left = deleteNode( w->left , wordDel );
                if(getHeight(w->left) - getHeight(w->right )>=2 ){
                    if(strcmp(wordDel,w->left->word) < 0)
                        w=singleRotateRight(w);
                    else
                        w=doubleRotateRight(w);
                }
            }
            ///Go right.
            else if(strcmp(wordDel , w->word ) > 0){
                w->right = deleteNode(w->right , wordDel );
                if(getHeight(w->right) - getHeight(w->left) >= 2)
                {
                    if(strcmp(wordDel , w->right->word) > 0)
                    w = singleRotateLeft(w);
                    else
                    w = doubleRotateLeft(w);
                }
            }
            else
            { ///The element to be deleted is found.
                if( w->left && w->right )
            {
                /// Replace with smallest in right subtree
                temp = minValueNode( w->right );
                strcpy(w->word , temp->word);
                strcpy(w->wordSyn , temp->wordSyn);
                strcpy(w->wordAnt, temp->wordAnt);

                w->wordMeaning = temp->wordMeaning;
                w->right = deleteNode(  w->right , w->word);
            }

            else
            {
                temp = w;
                if( w->left == NULL )
                    w = w->right;
                else if( w->right == NULL )
                    w = w->left;
                free( temp);
            }
            }
            return w;
        }

///This function saves the data to an output file.
 void saveToFile(wordPtr w , FILE* pFile){

    if(w!= NULL){
         saveToFile(w->left , pFile);
         meaningPtr temp = w->wordMeaning;
         fprintf(pFile , "%s:", w->word);
       while (temp != NULL){

        fprintf(pFile , "%s", temp->meaning);
        if(temp->next != NULL)
            fprintf(pFile , ",");
        temp = temp->next;
       }

       fprintf(pFile , "/%s", w->wordSyn);
       fprintf(pFile , "*%s", w->wordAnt);


       saveToFile(w->right , pFile);

    }

 }

///This function aims to delete all words that start with a specific character in an AVL tree. It compares the first character of each word
///and when it finds it, it calls the deleteNode function.
wordPtr deleteNodeFirstChar(wordPtr w , char delKey){
    wordPtr temp = NULL;
    if(w == NULL)
        return w;

    else{
        if( delKey == w->word[0]){
          w = deleteNode(w , w->word);
        }
        else if (delKey < w->word[0]){
          w = deleteNodeFirstChar(w->left , delKey);
        }
        else if (delKey < w->word[0]){
          w = deleteNodeFirstChar(w->right , delKey);
        }
    }

return w;
}



void switchToHash(){
    char fileLine[100];
    char* word;
    char* arrOfWords[10];
    int numOfMeanings,j,k;
    int size = computeSize();
    ///dictionaryRoot is the root for the dictionary AVL tree.
    hashPtr dictionaryHash[size];
    createHashTable(dictionaryHash , size);
    FILE* pFile = fopen("out.txt", "r");

    if (pFile == NULL){
        printf("The file you're wishing to access is not found!");
    }

    ///The process of extracting the word and its associated information.
    while (fgets(fileLine, 100, pFile) != NULL){
        int i=0;
        meaningPtr meaningRoot = NULL;
        word = strtok(fileLine,":,/*");
        while(word != NULL){
            arrOfWords[i]=word;
            word = strtok(NULL, ":,/*");
            i++;
        }

    numOfMeanings= i-3;

    if(numOfMeanings>1){
        for(j=1; j<=numOfMeanings; j++){
            meaningRoot = insertMeaning(meaningRoot , arrOfWords[j]);
        }
    }
    else{
        meaningRoot = insertMeaning(meaningRoot , arrOfWords[1]);
    }

///Inserting each word in the file as a node in the hash table with its information.

    insertHash(dictionaryHash , arrOfWords[0], arrOfWords[i - 2] , arrOfWords[i - 1] , meaningRoot , size);

    }
    int numOfRecords = 0;
    int option = 0;
    char userWord[15], userAnt[15], userSyn[15];
    meaningPtr userMeaning = NULL;
    char userMean[15];
    int choiceMean;
    printMenuHash();
    while(option != 8){

        option = getchar() - '0';

        switch(option){
        case 1:{
                printHash(dictionaryHash , size);
                printMenuHash();
        }
        break;
        case 2:{
                printf("The size of the hash table is: %d\n", size);
                printMenuHash();
        }
        break;
        case 3: {
                printf("The hash function used is as follows:\n");
                printf("((while(key[j]) hashValue = (hashValue << 5) + key[j++])%% sizeOfTable)\n");
                printMenuHash();
        }
        break;
        case 4:{
                ///We have to check for rehashing by checking if the number of records has reached half of the size.
                if((numRecords + 1) > (size / 2)) {
                    int newSize = computeSize(size);
                    hashPtr newHash[newSize];
                    createHashTable(newHash , newSize);
                    reHash( dictionaryHash , newHash , size , newSize);
                    printf("Please enter the word you'd like to add to the hash table: \n");
                    scanf("%s", &userWord);
                    printf("Please enter the meaning of the word: \n");
                    scanf("%s", &userMean);
                    userMeaning = insertMeaning(userMeaning, userMean);
                    printf("Do you have another meaning? \n1.Yes\n2.No\n");
                    scanf("%d", &choiceMean);
                    if(choiceMean == 1){
                    printf("Please enter the next meaning: \n");
                    scanf("%s", &userMean);
                    userMeaning = insertMeaning(userMeaning, userMean);
                    }

                    printf("Please enter the antonym of the word: \n");
                    scanf("%s", &userAnt);
                    printf("Please enter the synonym of the word: \n");
                    scanf("%s", &userSyn);

                    insertHash(newHash , userWord, userSyn, userAnt, userMeaning, newSize);

                    userMeaning = NULL;




                }
                else{
                    printf("Please enter the word you'd like to add to the hash table: \n");
                    scanf("%s", &userWord);
                    printf("Please enter the meaning of the word: \n");
                    scanf("%s", &userMean);
                    userMeaning = insertMeaning(userMeaning, userMean);
                    printf("Do you have another meaning? \n1.Yes\n2.No\n");
                    scanf("%d", &choiceMean);
                    if(choiceMean == 1){
                        printf("Please enter the next meaning: \n");
                    scanf("%s", &userMean);
                    userMeaning = insertMeaning(userMeaning, userMean);
                    }

                    printf("Please enter the antonym of the word: \n");
                    scanf("%s", &userAnt);
                    printf("Please enter the synonym of the word: \n");
                    scanf("%s", &userSyn);

                    insertHash(dictionaryHash , userWord, userSyn, userAnt, userMeaning, size);

                userMeaning = NULL;


              }
              printMenuHash();
            }
            break;
        case 5: {
                char wordSearch[15];
                printf("Enter the name of the word you want to search for in the hash table: \n");
                scanf(" %s", &wordSearch);
                int k = searchForRecord(dictionaryHash , size , wordSearch);
                if(k!=-1){
                    printf("The record has been found!\n");
                    printf("The word's associated information is:\n");

                    printf("The meaning/meanings of the word: ");
                    meaningPtr temp = dictionaryHash[k]->hashMeaning;
                while (temp != NULL){
                    printf("%s ", temp->meaning);
                    temp = temp->next;
               }
                printf("\nThe synonym of the word is: %s\n", dictionaryHash[k]->synHash);
                printf("The antonym of the word is: %s\n", dictionaryHash[k]->antHash);
                }
                else
                    printf("The record was not found in the hash table!\n");
                printMenuHash();
        }
        break;

        case 6:{
                char wordDelete[15];
                printf("Enter the word you want to delete from the hash table: \n");
                scanf(" %s", &wordDelete);
                int delIndex = searchForRecord(dictionaryHash , size , wordDelete);
                if (delIndex != -1){
                    hashDeleteRecord(dictionaryHash , size , wordDelete);
                    printf("Record was found and removed!");

                }
                else
                    printf("Record was not found!");
                printMenuHash();
        }
        break;

        case 7:{
                FILE* pF = fopen("out.txt", "w");
                saveHashToFile(dictionaryHash , size , pF);
                fclose(pF);
                printMenuHash();
        }
        break;

        case 8:{
            printf("Thank you for using this program, we hope your needs were satisfied!");
            exit(-1);

        }
        break;


        }

    }

}

///This prints the hash menu.
void printMenuHash(){
    printf("\nPlease select one of the following operations for the application to perform:\n\n");
    printf("1]Print the hashed table.\n");
    printf("2]Print the size of the hash table.\n");
    printf("3]Print used hash function.\n");
    printf("4]Insert a new record to hash table.\n");
    printf("5]Search for a specified record.\n");
    printf("6]Delete the specified record.\n");
    printf("7]Save hash table back to file.\n");
    printf("8]Exit program.\n");
}


///This function will return the required table size for a specified number of data which we will get by counting the lines
///of the output file.

int computeSize(){
    int numOfWords = 0;
    char line[100];
    FILE* pFile = fopen("out.txt" , "r");

    while(fgets(line , 100 , pFile) != NULL){
        numOfWords++;
    }

    int requiredSize = 2*numOfWords + 1;

    while(!isPrime(requiredSize))
        requiredSize++;

    return requiredSize;
}
///This function returns true if the number is prime and false if it's not.
int isPrime(int x)
{
    int i;

    for(i = 2 ; i <= x/2 ; i++)
        if (x % i == 0)
            return 0;

    return 1;
}

///This is the hash function used to give the data a guide into which index to dwell in.
int hashFunction(char key[], int size)
{
    unsigned int hashValue = 0;
    int i = 0;

    while(key[i] != '\0'){
        hashValue = (((hashValue << 5)) +  key[i++]);
    }
    return (hashValue % size);
}

///This function goes through a loop to dynamically allocate a pointer to the hash structure for each index until it reaches the size of the array.
void createHashTable(hashPtr hash[],int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        hash[i]=(hashPtr) malloc (sizeof(struct hashNode));
        hash[i]->flag=0;

    }
}


void insertHash(hashPtr hash[] , char word[15] , char syn[15] , char ant[15] , meaningPtr meaning , int size){

    unsigned int index = hashFunction(word , size);
    numRecords++;
    int i=1;
    while( hash[index]->flag ==1 )
    {
        index = ((index + i) % size);
        i++;
    }
    strcpy((hash[index]->wordHash) , word);
    strcpy((hash[index]->synHash), syn);
    strcpy((hash[index]->antHash), ant);
    hash[index]->hashMeaning = meaning;
    hash[index]->flag = 1;

   /// printf("%s %d\n", (hash[index]->wordHash), index);

}


///This function takes in as argument a word that the user wants to delete, it courses through the array, if it finds it, it returns the index, if not
///then it returns -1.
int searchForRecord(hashPtr hash[], int size, char keyWord[])
{

    int i = 1;

    int hashIndex = hashFunction(keyWord , size);


    while( (i < size) && strcmp(hash[hashIndex]->wordHash, keyWord) != 0 )
    {
        hashIndex = ((hashIndex + i) % size);
        i++;
    }

    if(i<size)
        return hashIndex;
    else
        return -1;
}



void saveHashToFile(hashPtr h[], int size, FILE* pFile)
{
    int i;
    for(i=0; i<size; i++)
    {
        if(h[i]->flag==1){

            fprintf(pFile , "%s:", h[i]->wordHash);
            meaningPtr temp = h[i]->hashMeaning;
            while (temp != NULL){
                fprintf(pFile,"%s", temp->meaning);
                if(temp->next != NULL)
                    fprintf(pFile , ",");
                temp = temp->next;
            }
            fprintf(pFile , "/%s", h[i]->synHash);
            fprintf(pFile , "*%s", h[i]->antHash);
        }
    }



}

///This function first checks if a specified record is in the hash table. In case it is, the flag is set to 0.
void hashDeleteRecord(hashPtr hash[], int size , char key[])
{
    ///Find the position of the record that we want to delete

    int index = searchForRecord(hash , size , key);
    if (index != -1)
        hash[index]->flag = 0;
}

///This function prints the
void printHash(hashPtr hash[] , int size)
{


    int i=0;
    for(i=0; i<size; i++)
    {
        printf("\n************************************************\n");

        if(hash[i]->flag==1){

            printf("The index of the word is: %d\n", i);
            printf("The word is: %s\n", hash[i]->wordHash);
            printf("The meaning/meanings of the word: ");
            meaningPtr temp = hash[i]->hashMeaning;
            while (temp != NULL){
                printf("%s ", temp->meaning);
                temp = temp->next;
            }
            printf("\nThe synonym of the word is: %s\n", hash[i]->synHash);
            printf("The antonym of the word is: %s\n", hash[i]->antHash);
        }
        else
            printf("\nWAIT! %d is an empty cell",i);

    }

}
///This function is called when the hash table needs to be hashed due to a
void reHash(hashPtr oldHash[], hashPtr newHash[], unsigned int oldSize, unsigned int newSize)
{
    reHashFlag = 1;
    int i;
    for(i=0; i<oldSize; i++)
    {
        if(oldHash[i]->flag == 1)
            insertHash(newHash , oldHash[i]->wordHash , oldHash[i]->synHash , oldHash[i]->antHash , oldHash[i]->hashMeaning , newSize);

    }
}














