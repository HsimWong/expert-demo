#ifndef SERVER_H
#define SERVER_H
#include "node.h"

// typedef struct Server Server;
// typedef struct Node Node;
typedef struct Server{
    char * server_ip;
    Job ** scheduled_jobs;
    Job ** new_jobs;
    Node ** nodes;
    int new_jobs_num;
    int scheduled_jobs_num;
    int nodes_num;
    void (*add_node)(struct Server * self, Node * node);
    void (*add_job)(struct Server * self, Job * job);
    void (*run)(struct Server * self);
}Server;

Server *InitServer(char *server_ip);


#endif