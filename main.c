#include <stdio.h>
#include <string.h>
#include "binomial.h"
#include "fatal.h"

#define MAX 30

struct process{
    int id;
    int e; //execution time
    int t; //arrival time
    struct process *nextProcess;
}typedef process;

process* insertToList(process *p, int id, int e, int t);
process* splitString(char *str, process *p);
process* readFile(char *file, process *p);

int main() {
    printf("Hello, World!\n");
    process *root = NULL;

    char buf[MAX];

    root = readFile("input.txt", root);

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
        //fputs(chunk, stdout);
        //fputs("|*\n", stdout);  // marker string used to show where the content of the chunk array has ended
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
            printf("%s\n", ptr);
            counter++;
            ptr = strtok(NULL, delim);
        }else if(counter == 1){
            e = atoi(ptr);
            printf("%s\n", ptr);
            counter++;
            ptr = strtok(NULL, delim);
        }else if(counter == 2){
            t = atoi(ptr);
            printf("%s\n", ptr);
            counter++;
            ptr = strtok(NULL, delim);
        }
    }
    //create node and add to linked list.
    p = insertToList(p, id, e, t);
    return p;
}