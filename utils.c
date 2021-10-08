#include "utils.h"
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// gcc sql-test.c -o test $(mysql_config --libs --cflags) && ./test

void msg_send(struct Sender *self, char *message, int length) {
    send(self->socket , message , length, 0);
}

void sender_close(struct Sender *self) {
    sleep(1);
    close(self->socket);
}

Sender *InitSender(char *addr, int port) {
    Sender *self = (Sender *)malloc(sizeof(Sender));
    // struct sockaddr_in *server = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    struct sockaddr_in server;
    self->socket = socket(AF_INET , SOCK_STREAM , 0);
    server.sin_addr.s_addr = inet_addr(addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
    self->server = &server;
    self->msg_send = msg_send;
    self->sender_close = sender_close;
    printf("%d\n", port);
    if (connect(self->socket , (struct sockaddr *)&(server) , sizeof(server)) < 0) {
		printf("connect failed. Error");
	}
	printf("Connected\n");
    return self;
}

void update_status(DBUpdate *self, char *node_ip, char *node_status) {
    char sql_statement[1024];
    sprintf(sql_statement, 
        "update node set node_status=\"%s\" where node_ip=\"%s\"", 
        node_status, node_ip);
    
    if(mysql_query(self->connection, sql_statement) !=0) {
        printf("Query failed  with the following message:\n");
        printf("%s\n", mysql_error(self->connection));
        exit(1);
    }
}

void db_close(struct DBUpdate *self) {
    mysql_close(self->connection);
}

DBUpdate *InitDBUpdate() {
    DBUpdate *self = (DBUpdate *)malloc(sizeof(DBUpdate));
    self->connection = mysql_init(NULL);
    if (mysql_real_connect(self->connection, 
            "hongkong.chn.ryan1202.wang", 
            "node", "password", "node", 
            3306, NULL, 0) == NULL) {

        printf("The authentication failed with the following message:\n");
        printf("%s\n", mysql_error(self->connection));
        exit(1);
    } else {
        printf("The connection is open\n");
    }

    self->update_status = update_status;
    self->db_close = db_close;
}


int reachable(char buffer[]) {
    for (int i = 0; i < 1024; i++) {
        if (buffer[i] == '\%') {
            return buffer[i - 2] == ' ';
            break;
        }
    }
    return 0;
}

int check_connectivity(char *target) {
    char command[1024];
    sprintf(command, "ping -c 1 -w 1 %s 2> /dev/null | grep received", target);
    char conn[128];
    FILE *fp;
    fp = popen(command, "r");
    fgets(conn, 128, fp);
    return reachable(conn);
}