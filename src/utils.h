#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <unistd.h>

int check_connectivity(char * target);
// pid_t getPID(char * command);
pid_t exec_local(char * command);

#endif