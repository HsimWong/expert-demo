#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <unistd.h>
#define PORT 23333

int check_connectivity(char * target);
// pid_t getPID(char * command);
pid_t exec_local(char * command);
pid_t execute_remote(char * remote_addr, char *command);


#endif