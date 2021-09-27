#ifndef NODE_H
#define NODE_H


typedef struct  {
    char * entrypoint;
} Job;

typedef struct  {
    Job ** jobs;
    int jobs_num;
    char * ip_addr;
    char * display;
    void (*excecute)(void *self, const char * command);
    void (*addJob)(Node * self, Job * job);
}Node;

void InitNode(Node *self, Job ** jobs, 
              char * ip_addr, char * display);



// extern Node;
// extern Job;

#endif