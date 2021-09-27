#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "utils.h"

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

// pid_t getPID(char *command) {
//     // char check_pid_command[1024] = "ps aux | grep ";
//     // strcat(check_pid_command, command);
//     // strcat(check_pid_command, " | awk '{print $2}'");
//     char check_pid_command[] = "echo \"$!\"";
    
// }

pid_t exec_local(char * command) {
    putenv("DISPLAY=:0");
    int command_length = prune_command(command);
    char *exec_command = malloc(command_length * sizeof(char));
    strcat (exec_command, command);
    strcat(exec_command, " & echo \"$!\"");
    // printf("%s",exec_command);
    FILE *fp;
    char result[8] = {0};
    fp = popen(exec_command, "r");
    fgets(result, 8, fp);
    return atoi(result);
}

