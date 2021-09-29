#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "utils.h"
#include <netinet/in.h>
#include <sys/socket.h>

char * send(char *target, char *messsge) {
    
}

void receive(
    char **target_table,
    void **target_func(char *input_mesg, char *output_mesg)) 
{
    int server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    // server_address = AF_INET;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    bind(server_socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address));
    while (1) {
        listen(server_socket_descriptor, 5);
        int client_socket = accept(server_socket_descriptor, NULL, NULL);

    }
}


int check_connectivity(char * target) {
    const char * target_addr = target;
    char command[128] = "ping -c 1 -w 1 ";
    strcat(command, target_addr);
    strcat(command, " 2> /dev/null | grep received");

    FILE * terminal;
    char ping_return_line[256];
    terminal = popen(command, "r");
    fgets(ping_return_line, 256, terminal);
    for (int i = 0; i < 1024; i++) {
        if (ping_return_line[i] == '\%') {
            return ping_return_line[i - 2] == ' ';
            break;
        }
    }
    return 0;
}

int prune_command(char *origin) {
    int length = 0;
    while (origin[length] != 0) {
        length++;
    }
    for (int i = length - 1; i >= 0; i--) {
        if (origin[i] == ' ') {
            origin[i] = 0;
            length --;
        } else {
            break;
        }
    }

    if (origin[length - 1] == '&' 
        && origin[length - 2] != '&') {
        // length--;
        origin[length - 1] = 0;
        length--;
    }
    return length;
}

void get_command_result(char * command, char * result, int length) {
    prune_command(command);
    FILE *fp;
    fp = popen(command, "r");
    fgets(result, length, fp);
}

pid_t exec_local(char * command) {
    putenv("DISPLAY=:0");
    int command_length = prune_command(command);
    char *exec_command = malloc(command_length * sizeof(char));
    strcat (exec_command, command);
    strcat(exec_command, " & echo \"$!\"");
    FILE *fp;
    char result[8] = {0};
    fp = popen(exec_command, "r");
    fgets(result, 8, fp);
    return atoi(result);
}

pid_t execute_remote(char * remote_addr, char *command) {
    char *exec_command[512] = "ssh ";
    strcat(exec_command, remote_addr);
    strcat(exec_command, " ; \"");
    strcat(exec_command, command);
    strcat(exec_command, "\"");
    return exec_local(exec_command);        
}


