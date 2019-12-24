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
void printList(process *root){
    process *temp = root;
    while(temp != NULL){
        printf("%d ", temp->id);
        temp = temp->nextProcess;
    }
}
double calculateC(process *node, int eI, int eMax);
double calculatePV(process *node, int eI, int t, int eMax);

int main() {
    process *root = NULL;
    int q = 1;
    int eMax = 0;
    char buf[MAX];

    root = readFile("input.txt", root);
    eMax = findEMax(root);

    printList(root);
    removeFirstProcess(&root);
    printf("\n*****\n");
    printList(root);

    BinQueue H1, H2;
    BinTree p, r[20]={NULL};
    ElementType Item;
    char ch;
    int i;
    H1 = Initialize( );
    Item = 13;
    Insert(Item, H1);
    Item = 4;
    Insert(Item, H1);
    Item = 10;
    Insert(Item, H1);
    Item = 2;
    Insert(Item, H1);
    Item = 7;
    Insert(Item, H1);
    Item = 9;
    Insert(Item, H1);
    Item = 15;
    Insert(Item, H1);
    Item = 1;
    Insert(Item, H1);
    Item = 5;
    Insert(Item, H1);
    Item = 11;
    Insert(Item, H1);

    return 0;
}

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

