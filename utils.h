#ifndef UTILS_H
#define UTILS_H
#include <my_global.h>
#include <mysql.h>

typedef struct Sender {
    struct sockaddr_in *server;
    int socket;
    void (*msg_send)(struct Sender *self, char *message, int length);
    void (*sender_close)(struct Sender *self);
} Sender;

Sender *InitSender(char *addr, int port);


typedef struct DBUpdate{
    MYSQL *connection;
    void (*register_node)(struct DBUpdate *self, char *node_ip, char *node_name);
    void (*update_status)(struct DBUpdate *self, char *node_ip, char *node_status);
    void (*db_close)(struct DBUpdate *self);
} DBUpdate;

DBUpdate *InitDBUpdate();

int check_connectivity(char *target);


#endif