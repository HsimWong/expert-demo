
#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include <stdlib.h>
#include "utils.h"
#include <netinet/in.h>
#include <json-c/json.h>
#include <sys/socket.h>
#define BUFFER_SIZE 1024



// typedef struct json_object json_object;

char *get_field_value_string(json_object *json, char *field) {
    char *ret;
    json_object *field_object;
    json_object_object_get_ex(json, field, &field_object);
    return (char *)json_object_get_string(field_object);
}

json_object *get_field_object(json_object *json, char *field) {
    json_object *field_object;
    json_object_object_get_ex(json, field, &field_object);
    return field_object;
}

// int check_str_if_identical(char *str1, char *str2) {
//     int i = 0;
//     while (1) {
//         if (str1[i] == 0 || str2[i] == 0) {
//             return true;
//         } 
//         i++;
//     }
// }


char * message_send(char *target, char *message) {
    int sock;
	struct sockaddr_in server;
	char server_reply[BUFFER_SIZE];

    sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr(target);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("connect failed. Error");
		// return 1;
	}
	
	puts("Connected\n");
	
    // while (1) {
    

    if(send(sock , message , strlen(message) , 0) < 0) {
        puts("Send failed");
        // return 1;
    }
    
    //Receive a reply from the server
    if(recv(sock , server_reply , BUFFER_SIZE , 0) < 0) {
        puts("recv failed");
        // return;
    }
    
    puts("Server reply :");
    puts(server_reply);
    // }
}

 

void message_receive(
    char **target_table,
    void (**target_func_list)(char *, char *),
    int targets_func_num) {

    int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[BUFFER_SIZE];
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("bind failed. Error");
        // return;
	}
	puts("bind done");
	//Listen
	listen(socket_desc , 3);
	c = sizeof(struct sockaddr_in);
    while (1) {
        puts("Waiting for incoming connections...");        
        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("accept failed");
            // return 1;
        }
        puts("Connection accepted");
        
        read_size = recv(client_sock , client_message , BUFFER_SIZE , 0);
        if (read_size == 0) {
            puts("Client disconnected");
            fflush(stdout);
            continue;
        } else if (read_size == -1) {
            perror("recv failed");
            continue;
        } else {
            printf("Message received: %s\n", client_message);
            // write(client_sock , client_message , strlen(client_message));
            json_object *config = json_tokener_parse(client_message);
            char *target_str = get_field_value_string(config, "function");
            for (int i = 0; i < targets_func_num; i++) {
                printf("Target string: %s\n", target_str);
                printf("cmpstr: %s\n", target_table[i]);
                // target_str = "***";    
                printf("%d\n", strcmp(target_str, target_table[i]));
                
                if (strcmp(target_str, target_table[i]) == 0) {
                    printf("Function matched");
                    char * parameters = get_field_value_string(config, "parameters");
                    char return_str[BUFFER_SIZE];
                    printf("Start going!\n");
                    (target_func_list[i])("parameters", return_str);
                    printf("Return string fetched, \n");
                    // printf("%s\n",parameters);
                    write(client_sock, return_str, strlen(return_str));
                    continue;
                } else {
                    printf("token not matched\n");
                }
            }
        }
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
    char exec_command[512] = "ssh ";
    strcat(exec_command, remote_addr);
    strcat(exec_command, " ; \"");
    strcat(exec_command, command);
    strcat(exec_command, "\"");
    return exec_local(exec_command);        
}


