#include "node.h"
#include "utils.h"
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>



void execute(void * self, const char * command) {
    fprintf(stdout, "%s", command);
}

void add(Node * self, Job * job) {
    self->jobs[self->jobs_num] = job;
    self->jobs_num++;
}

int execute_remote(char * remote_addr, char *command) {
    char *exec_command[256] = "ssh ";
    strcat(exec_command, remote_addr);
    strcat(exec_command, " ; ");
    
    
}

void terminate(Node *self, Job *job) {
    if (job->pid <= 0) {
        fprintf(stderr, "Error: Terminating a pending job\n");
        return;
    }


}

JobStatus check_job_status(Node * self, Job * job) {
    pid_t pid = job->pid;
    char pidfile[32] = "/proc/";
    strcat(pidfile, itoa(pid));
    strcat(pidfile, "/cmdline");
    return access(pidfile, F_OK) ? Working : Failed;
}




Node *InitNode(char * ip_addr, char * display) {
    Node *self = (Node *)malloc(sizeof(Node));
    Job **jobs = (Job **)malloc(sizeof(Job *));
    self->jobs = jobs;
    self->ip_addr = ip_addr;
    self->jobs_num = 0;
    self->display = display;
    // self->excecute = execute;
    self->add = add;
    self->terminate = terminate;
    return self;
}

Job *InitJob(char *master_node, 
             char *backup_node,
             char *entrypoint) {
    Job *self = (Job *)malloc(sizeof(Job));
    self->running_node = "";
    self->backup_node = backup_node;
    self->master_node = master_node;
    self->entrypoint = entrypoint;
    self->pid = -1;
    self->status = Pending;
    return self;
}