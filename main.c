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
    int eProcess;
    double pV;
    int bhCounter;
    struct process *nextProcess;
}typedef process;

process* insertToList(process *p, int id, int e, int t, int wT, int bhCounter);
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
ElementType* createElementType(int id, int e, int t, double pValue, int wt, int eProcess, int bhCounter);
int findMaxArrivingTime(process *root);
process* findProcessByTime(process *root, int t);
void deleteNode(process **root, int id);
int numberInputList(process *root);
double cValue(process *node, int eI, int eMax);
double calculatePriority(process *node, int e, int t, int eMax);

int main() {
    process *inputRoot = NULL, *uP = NULL, *completedRoot = NULL;
    int q = 3;
    int eMax = 0;
    inputRoot = readFile("input.txt", inputRoot);
    eMax = findEMax(inputRoot);

    BinQueue H1 = Initialize();
    ElementType *Item;

    int timeCounter = 0;
    int maxArrivingTime = 0;
    double priority = 0;

    while(inputRoot != NULL){
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
        if(nextProcess != NULL){
            uP = insertToList(uP, nextProcess->id, nextProcess->e, nextProcess->t, nextProcess->wt, nextProcess->bhCounter);
            deleteNode(&inputRoot, nextProcess->id);
        }
        //while uP is allocated
        while(uP != NULL){
            //if e>q
            if(uP->e > q){
                timeCounter = timeCounter + q;
                //for each process i in BH, Update WTi
                increaseWaitingTime(H1, 0, q);
                uP->e = uP->e - q;
                //re-insert process into BH
                uP->bhCounter++;
                priority = calculatePriority(uP, uP->e, uP->t, eMax);
                Item = createElementType(uP->id,uP->e,timeCounter,priority, uP->wt, uP->eProcess, uP->bhCounter);
                Insert(*Item, H1);
                //preempt current process
                uP = NULL;
            //if e<=q
            }else{
                timeCounter = timeCounter + uP->e;
                //for each process i in BH, Update WTi
                increaseWaitingTime(H1, 0, uP->e);
                uP->e = 0;
                completedRoot = insertToList(completedRoot, uP->id, uP->eProcess, uP->t, uP->wt, uP->bhCounter);
                uP = NULL;
            }
            //enqueue incoming processes by their priority
            process *iterInput = inputRoot;
            while(iterInput != NULL){
                if(iterInput->t <= timeCounter){
                    priority = calculatePriority(iterInput, iterInput->e, iterInput->t, eMax);
                    ElementType *Item = createElementType(iterInput->id, iterInput->e, iterInput->t, priority, iterInput->wt, iterInput->eProcess, iterInput->bhCounter);
                    if(Item->t < timeCounter){
                        Item->wt = timeCounter - Item->t;
                    }
                    Insert(*Item, H1);
                    deleteNode(&iterInput, iterInput->id);
                    inputRoot = iterInput;
                }else{
                    iterInput = iterInput->nextProcess;
                }
            }
            //calculate priority values
           // calculatePVForAll(H1, 0, eMax);
            //DeleteMin
            ElementType minItem = DeleteMin(H1);
            //Assigns min process on BH to uP
            uP = insertToList(uP, minItem.id, minItem.e, minItem.t, minItem.wt, minItem.bhCounter);
            uP->eProcess = minItem.eProcess;
        }
        printf("kjk");
    }
    return 0;
}

ElementType* createElementType(int id, int e, int t, double pValue, int wt, int eProcess, int bhCounter){
    ElementType *node = malloc(sizeof(ElementType));
    node->id = id;
    node->e = e;
    node->t = t;
    node->wt = wt;
    node->eProcess = eProcess;
    node->bhCounter = bhCounter;
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
process* insertToList(process *p, int id, int e, int t, int wT, int bhCounter){
    process *node = malloc(sizeof(process));
    node->id = id;
    node->e = e;
    node->t = t;
    node->wt = wT;
    node->bhCounter = bhCounter;
    node->pV = 0;
    node->eProcess = e;
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
    p = insertToList(p, id, e, t, 0, 0);
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

double cValue(process *node, int eI, int eMax){
    if(node->bhCounter == 0){
        return 1;
    }else{
        double top = 2*eI;
        double bottom = 3*eMax;
        double division = top/bottom;
        double calculation = -pow(division, 3);
        return 1/exp(calculation);
    }
}

double calculatePriority(process *node, int e, int t, int eMax){
    //calculate the priority value
    //  check if there is a process that has the same e value
    //      if exist take tArrival
    //      else c(eI)*eI (Implement the c(eI) for first insertion and further insertions).

    //check if there are any same eI
        return cValue(node, e, eMax)*e;
        //TODO if eI == eJ return tArrival!
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