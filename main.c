#include <stdio.h>
#include <string.h>
#include "binomial.h"
#include "fatal.h"
#include <math.h>


#define MAX 30

struct process{
    int id;
    int e; //execution time
    int t; //arrival time
    int wt; //waiting time
    double pV;
    int bhCounter;
    struct process *nextProcess;
}typedef process;

process* insertToList(process *p, int id, int e, int t);
process* splitString(char *str, process *p);
process* readFile(char *file, process *p);
int findEMax(process *p);
void removeFirstProcess(process **root){
    process* temp = *root;
    temp = temp->nextProcess;
    free(*root);
    *root = temp;
}
void printList(process *root);
double calculateC(process *node, int eI, int eMax);
double calculatePV(process *node, int eI, int t, int eMax);
ElementType* createElementType(int id, int e, int t, double pValue);
int findMaxArrivingTime(process *root);
process* findProcessByTime(process *root, int t);
void deleteNode(process **root, int id);
int numberInputList(process *root);

int main() {
    process *inputRoot = NULL;
    process *uP = NULL;
    process *completedRoot = NULL;
    int q = 1;
    int eMax = 0;
    inputRoot = readFile("input.txt", inputRoot);
    eMax = findEMax(inputRoot);
    int timeMax = findMaxArrivingTime(inputRoot);
    int uPFlag = 0;

    //printList(inputRoot);
    deleteNode(&inputRoot, 2);
    deleteNode(&inputRoot, 1);

    BinQueue H1, H2;
    BinTree p, r[20]={NULL};
    ElementType *Item;

    H1 = Initialize( );
   Item = createElementType(1,3,0,7);
    Insert(*Item, H1);
    Item = createElementType(2,5,1,2);
    Insert(*Item, H1);
    Item = createElementType(3,7,2,4);
    Insert(*Item, H1);
    Item = createElementType(4,3,3,12);
    Insert(*Item, H1);

    BinTree p2, r2[20]={NULL};
    p2=H1->TheTrees[2];
    //printTree(p2, r2, 0);


    //initialize parameters; q, eMax
    //while there exist processes in the input list
    //      put the next process i arrived in uP
    //      while uP is allocated
    //          enqueue incoming processes by their priority
    //          if e>q
    //              preempt current process
    //              reassign new priority
    //              re-insert process into BH
    //          Else release uP
    //          for each process i in BH
    //              Update WTi
    //          DeleteMin
    //          Assign uP to this process
    //      end of While
    //end of While
    double newPriority = 0;
    int numberOfInput = 0;
    int timeCounter = 0;
    int maxArrivingTime = 0;


    while(inputRoot != NULL){
        //put the next process i arrived in uP
        //Find the next process
        process *nextProcess = NULL;
        maxArrivingTime = findMaxArrivingTime(inputRoot);
        while(timeCounter <= maxArrivingTime){
            nextProcess = findProcessByTime(inputRoot, timeCounter);
            if(nextProcess != NULL){
                break;
            }else{
                timeCounter++;
            }
        }
        //Put the process to uP
        uP = insertToList(uP, nextProcess->id, nextProcess->e, nextProcess->t);
        //delete process from input list
        deleteNode(inputRoot, nextProcess->id);
        //while uP is allocated
        while(uP != NULL){
            //enqueue incoming processes by their priority
            process *iterInput = inputRoot;
            while(iterInput != NULL){
                if(iterInput->t < timeCounter){
                    double newPValue = 0;
                    newPValue = calculatePV(iterInput, iterInput->e, iterInput->t, eMax);
                    ElementType *Item = createElementType(iterInput->id, iterInput->e, iterInput->t, newPValue);
                    Insert(*Item, H1);
                    deleteNode(&inputRoot, iterInput->id);
                }
                iterInput = iterInput->nextProcess;
            }
            //if e>q
            if(uP->e > q){
                //reassign new priority
                timeCounter = timeCounter + q;
                newPriority = calculatePV(uP, uP->e, uP->t, eMax);
                uP->pV = newPriority;
                //re-insert process into BH
                Item = createElementType(uP->id,uP->e,timeCounter,newPriority);
                Insert(*Item, H1);
                //preempt current process
                uP = NULL;
                //for each process i in BH
                //Update WTi
                increaseWaitingTime(H1, 0, q);
            }else{
                timeCounter = timeCounter + uP->e;
                //for each process i in BH
                //Update WTi
                increaseWaitingTime(H1, 0, uP->e);
                completedRoot = insertToList(completedRoot, uP->id, uP->e, uP->t);
                //release uP
                uP = NULL;
            }
            //DeleteMin
            ElementType minItem = DeleteMin(H1);
            //Assigns min process on BH to uP
            uP = insertToList(uP, minItem.id, minItem.e, minItem.t);
        }
    }

    return 0;
}

ElementType* createElementType(int id, int e, int t, double pValue){
    ElementType *node = malloc(sizeof(ElementType));
    node->id = id;
    node->e = e;
    node->t = t;
    node->pValue = pValue;
    return node;
}

process* findProcessByTime(process *root, int t){
    process *iter = root;
    while(iter != NULL){
        if(iter->t == t){
            return iter;
        }
        iter = iter->nextProcess;
    }
    return NULL;
}

int findTMax(process *root){
    process *iter = root;
    int tMax = 0;
    while(iter != NULL){
        if(iter->t > tMax){
            tMax = iter->t;
        }
        iter = iter->nextProcess;
    }
    return tMax;
}

//TODO eksik parametleri ekle. Örne?in: waiting time.
process* insertToList(process *p, int id, int e, int t){
    process *node = malloc(sizeof(process));
    node->id = id;
    node->e = e;
    node->t = t;
    node->nextProcess = NULL;

    if(p == NULL){
        return node;
    }else{
        process *iter = p;
        while(iter->nextProcess != NULL){
            iter = iter->nextProcess;
        }
        iter->nextProcess = node;
        return p;
    }
}

void deleteNode(process **root, int id){
    process *temp = *root, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->id == id)
    {
        *root = temp->nextProcess;   // Changed head
        free(temp);               // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->id != id)
    {
        prev = temp;
        temp = temp->nextProcess;
    }

    // If key was not present in linked list
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->nextProcess = temp->nextProcess;

    free(temp);  // Free memory

}

void printList(process *root){
    process *temp = root;
    while(temp != NULL){
        printf("%d ", temp->id);
        temp = temp->nextProcess;
    }
}

int findMaxArrivingTime(process *root){
    int maxT = root->t;
    process *iter = root;
    while(iter != NULL){
        if(iter->t > maxT){
            maxT = iter->t;
        }
        iter = iter->nextProcess;
    }
    return maxT;
}

process* readFile(char *file, process *p){
    FILE *fp = fopen("input.txt", "r");
    if(fp == NULL) {
        perror("Unable to open file!");
        exit(1);
    }

    char chunk[128];

    while(fgets(chunk, sizeof(chunk), fp) != NULL) {
        p = splitString(chunk, p);
    }

    fclose(fp);
    return p;
}

process* splitString(char *str, process *p){
    int counter = 0;
    int init_size = strlen(str);
    char delim[] = " ";
    int id = 0;
    int e = 0;
    int t = 0;
    char *ptr = strtok(str, delim);

    while(ptr != NULL)
    {
        if(counter == 0){
            ptr++;
            id = atoi(ptr);
            counter++;
            ptr = strtok(NULL, delim);
        }else if(counter == 1){
            e = atoi(ptr);
            counter++;
            ptr = strtok(NULL, delim);
        }else if(counter == 2){
            t = atoi(ptr);
            counter++;
            ptr = strtok(NULL, delim);
        }
    }
    //create node and add to linked list.
    p = insertToList(p, id, e, t);
    return p;
}

//finding eMax value for beginning of program.
int findEMax(process *p){
    int temp = 0;
    process *iter = p;
    while(iter != NULL){
        if(iter->e > temp){
            temp = iter->e;
        }
        iter = iter->nextProcess;
    }
    return temp;
}

double calculateC(process *node, int eI, int eMax){
    if(node->bhCounter == 0){
        return 1;
    }else{
        double calculation = -pow((2*eI)/(3*eMax), 3);
        return exp(calculation);
    }
}

double calculatePV(process *node, int eI, int t, int eMax){
    if(node->e == eI){
        return t;
    }else{
        return calculateC(node, eI, eMax);
    }
}

int numberInputList(process *root){
    process *iter = root;
    int counter = 0;
    while(iter != NULL){
        counter++;
        iter = iter->nextProcess;
    }
    return counter;
}