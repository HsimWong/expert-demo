#ifndef CONF_H
#define CONF_H

typedef struct {
    char * main_node_addr;
    char * back_node_addr;
    char * job_entrypoint;
    char * display;
}Conf;

extern Conf conf;

#endif