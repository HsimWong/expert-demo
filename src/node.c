#include "node.h"
#include <stdio.h>

void execute(void * self, const char * command) {
    fprintf(stdout, "%s", command);
}

void addJob(Node * self, Job * job) {
    self->jobs[self->jobs_num] = job;
    self->jobs_num++;
}



void InitNode(Node *self, Job ** jobs, 
        char * ip_addr, char * display) {
    self->jobs = jobs;
    self->ip_addr = ip_addr;
    self->jobs_num = 0;
    self->display = display;
    self->excecute = execute;
    self->addJob = addJob;
}