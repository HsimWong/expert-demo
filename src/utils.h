#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <unistd.h>
#include <json-c/json.h>
#define PORT 23334

// 
// typedef struct json_object Json;
int check_connectivity(char * target);

pid_t exec_local(char * command);
pid_t execute_remote(char * remote_addr, char *command);
char *get_field_value_string(json_object *json, char *field);
json_object *get_field_object(json_object *json, char *field);
char * message_send(char *target, char *message);

void message_receive(
    char **target_table,
    void (**target_func_list)(char *, char *),
    int targets_func_num);

#endif