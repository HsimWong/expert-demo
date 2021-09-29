#ifndef NODE_H
#define NODE_H
#include <stdlib.h>

// typedef struct Node Node;

typedef enum {
    Pending = 0,
    Working = 1,
    Finished = 2,
    Failed = 3
}JobStatus;

typedef struct {
    char *running_node;
    char *backup_node;
    char *master_node;
    JobStatus status;
    pid_t pid;
    char *entrypoint;
} Job;

typedef struct Node {
    Job **jobs;
    int jobs_num;
    char *ip_addr;
    char *display;
    void (*run)(struct Node *self);
    void (*terminate)(struct Node *self, Job *job);
    void (*add)(struct Node *self, Job *job);

    // void (*excecute)(struct Node *self, int job_index);
    // void (*addJob)(struct Node *self, Job *job);
    // void (*)
    // void (*check_job_status)(struct Node *self, Job *job);
    // void (*terminate_job)(struct Node *self, Job *job);
}Node;

Node *InitNode(char *ip_addr, char *display);
Job *InitJob(char *master_node, 
             char *backup_node,
             char *entrypoint);


// extern Node;
// extern Job;

#endif