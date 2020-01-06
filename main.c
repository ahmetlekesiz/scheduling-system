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

struct qValues{
    int qId;
    double awt;
    struct qValues *nextQ;
}typedef qValues;

process* insertToList(process *p, int id, int e, int t, int wT, int bhCounter);
process* splitString(char *str, process *p);
process* readFile(process *p);
int findEMax(process *p);
void printList(process *root);
ElementType* createElementType(int id, int e, int t, double pValue, int wt, int eProcess, int bhCounter);
int findMaxArrivingTime(process *root);
process* findProcessByTime(process *root, int t);
void deleteNode(process **root, int id);
double cValue(process *node, int eI, int eMax);
double calculatePriority(process *node, int e, int t, int eMax);
double sumWT(process *completed);
process* callForQ(int q, BinQueue H, ElementType *Item);
double printAll(process* completedRoot, BinQueue H1, ElementType *Item, int q);
double sumProcess(process *completed);
qValues* insertToQ(qValues *q, int qId, double awt);
void findQMin(qValues *q);

int main() {
    process *completedRoot = NULL;
    BinQueue H1 = Initialize();
    ElementType *Item;
    qValues *qRoot = NULL;
    double awt;

    int qValue;
    printf("Please enter the last value for q that you want to calculate(For example: 10). \nThe program will calculate the waiting time for all process and show you the most optimize value of q.\n");
    scanf("%d", &qValue);

    for (int i = 1; i <= qValue; ++i) {
        awt = printAll(completedRoot, H1, Item, i);
        qRoot = insertToQ(qRoot, i, awt);
    }

    printf("\n*** Result ***\n");
    findQMin(qRoot);

    return 0;
}

void findQMin(qValues *q){
    if(q != NULL){
        int counter = 1;
        int qValue = 1;
        double awtMin = q->awt;
        qValues *iter = q;
        while(iter != NULL){
            if(iter->awt < awtMin){
                awtMin = iter->awt;
                qValue = counter;
            }
            iter = iter->nextQ;
            counter++;
        }
        printf("Most optimize q value is q=%d with AWT=%0.2f\n", qValue, awtMin);
    }else{
        printf("List is null!");
    }
};

double printAll(process* completedRoot, BinQueue H1, ElementType *Item, int q){
    printf("\nq=%d\n", q);
    completedRoot = callForQ(q, H1, Item);
    printList(completedRoot);
    double sum = sumWT(completedRoot);
    double processCounter = sumProcess(completedRoot);
    double awt = sum / processCounter;
    printf("AWT = %.0f/%.0f = %.2f\n", sum, processCounter, awt);
    return awt;
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

    //if linked list is empty
    if(p == NULL){
        return node;
    }
    //if node lower than root, change root
    else if(node->id < p->id)
    {
        node->nextProcess = p;
        return node;
    }
    else{
        process *iter = p;
        while(iter->nextProcess != NULL && iter->nextProcess->id < node->id){
            iter = iter->nextProcess;
        }
        node->nextProcess = iter->nextProcess;
        iter->nextProcess = node;
        return p;
    }
}

qValues* insertToQ(qValues *q, int qId, double awt){
    qValues *node = malloc(sizeof(process));
    node->qId = qId;
    node->awt = awt;
    node->nextQ = NULL;

    if(q == NULL){
        return node;
    }else{
        qValues *iter = q;
        while(iter->nextQ != NULL){
            iter = iter->nextQ;
        }
        iter->nextQ = node;
        return q;
    }
}

void deleteNode(process **root, int id){
    process *iter = *root, *prev;

    if (iter != NULL && iter->id == id)
    {
        *root = iter->nextProcess;
        free(iter);
        return;
    }

    while (iter != NULL && iter->id != id)
    {
        prev = iter;
        iter = iter->nextProcess;
    }

    if (iter == NULL){
        return;
    }

    prev->nextProcess = iter->nextProcess;

    free(iter);
}

void printList(process *root){
    process *temp = root;
    printf("PID   Waiting Time\n");
    printf("------------------\n");
    while(temp != NULL){
        printf("P%d    %d\n", temp->id, temp->wt);
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

process* readFile(process *p){
    FILE *fp = fopen("input.txt", "r");
    if(fp == NULL) {
        perror("I could not open the file!");
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

int findEMax(process *p){
    //finding eMax value for beginning of program.
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
        return cValue(node, e, eMax)*e;
}

double sumWT(process *completed){
    double sum = 0;
    process *iter = completed;
    while(iter != NULL){
        sum = sum + iter->wt;
        iter = iter->nextProcess;
    }
    return sum;
}

double sumProcess(process *completed){
    double processCounter = 0;
    process *iter = completed;
    while(iter != NULL){
        processCounter++;
        iter = iter->nextProcess;
    }
    return processCounter;
}

process* callForQ(int q, BinQueue H1, ElementType *Item){
    process *inputRoot = NULL, *uP = NULL, *completedRoot = NULL;
    inputRoot = readFile(inputRoot);
    int eMax = findEMax(inputRoot);
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
            if(H1->CurrentSize != 0){
                ElementType minItem = DeleteMin(H1);
                //Assigns min process on BH to uP
                uP = insertToList(uP, minItem.id, minItem.e, minItem.t, minItem.wt, minItem.bhCounter);
                uP->eProcess = minItem.eProcess;
            }
        }
    }
    return completedRoot;
}
