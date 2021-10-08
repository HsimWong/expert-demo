/*
* This sniplet is coded in thread fashion for further extension.
* I understand there is no need to code with pthread.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include "utils.h"
#define SERVER 0
#define CLIENT 1
#define FAIL 0
#define SUCCESS 1

char * server_ip = "192.168.122.82";
char * client_ip = "192.168.122.1";
// const char * server_ping_command = "ping -c 1 -w 1 10.2.25.5 2> /dev/null | grep received";
// const char * client_ping_command = "ping -c 1 -w 1 10.2.25.125 2> /dev/null | grep received";
const char * server_ping_command = "ping -c 1 -w 1 192.168.122.82 2> /dev/null | grep received";
const char * client_ping_command = "ping -c 1 -w 1 192.168.122.1 > /dev/null | grep received";

const char * ping_public = "ping -c 1 -w 1 baidu.com 2> /dev/null | grep received";
const char * visu_server = "localhost";
int port = 10000;

char * demo_command = "/home/dji/Onboard-SDK-master/build/bin/camera-stream-target-tracking-sample";
Sender *sender;
DBUpdate *dbhandler;


void go_client();
void go_server();
void report(int if_success, int requester);
void app_control(const char * ping_command, 
                   int requester);



void report(int if_success, int requester) {
    // char *requester_ip = requester ? client_ip : server_ip;
    // char *suffix = if_success ? "1" : "0";
    // char *message = (char *)malloc(32 * sizeof(char));
    // strcat(message, requester_ip);
    // strcat(message, ",");
    // strcat(message, suffix);
    // sender->msg_send(sender, message, 32);
    char *request_ip = requester ? client_ip : server_ip;
    dbhandler->update_status(dbhandler, request_ip, if_success ? "SUCCESS" : "FAILED");
}
//void getPID();

// char * get_self_ip() {

// }


int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <role>\n", argv[0]);
        return 1;
    }
    printf("App entered, %s\n", argv[1]);
    const char * target;
    
    int requester;
    sender = InitSender(visu_server, port);
    dbhandler = InitDBUpdate();
    // dbhandler->register_node(dbhandler, )

    switch (argv[1][0]) {
    case 's':
        printf("Entering Server Mode\n");
        target = client_ping_command;
        requester = SERVER;
        dbhandler->register_node(dbhandler, server_ip, "node1");
        break;
    case 'c':
        target = server_ping_command;
        requester = CLIENT;
        dbhandler->register_node(dbhandler, client_ip, "node1");
        break;
    default:
        fprintf(stderr, "option error, exiting");
        exit(1);
    }
    
    printf("FUCK\n");
    // printf("%d", requester);
    app_control(target, requester);
    sender->sender_close(sender);
    // pthread_join(thread_application, NULL);
    return 0;
}

//int getPID(const char * process_name) {
//    FILE * pid_fp;
//    pid_fp = popen(demo_command, "r");
//    char buffer[]
    
//}

int reachable(char buffer[]) {
    for (int i = 0; i < 1024; i++) {
        if (buffer[i] == '\%') {
            return buffer[i - 2] == ' ';
            break;
        }
    }
    return 0;
}

void run_app() {
    // system("/home/dji/Onboard-SDK-master/build/bin/camera-stream-target-tracking-sample /home/dji/Onboard-SDK-master/build/bin/UserConfig.txt &");
    // report(SUCCESS)
    system("firefox");
}

int getPID() {
    char check_pid_command[256] = "ps aux | grep $RUNAPP | awk '{print $2}'";
    FILE *fp;
    char result[8] = {0};
    fp = popen(check_pid_command, "r");
    fgets(result, 8, fp);
    return atoi(result);
}

void terminate(int pid) {
    kill(pid, SIGKILL);
}

void app_control(const char * ping_private, 
    int requester) {
    char run_path[128] = "RUNAPP=";
    // printf("Try adding $RUNPATH");
    putenv(strcat(run_path, demo_command));
    // system("echo $RUNAPP");
    int if_connected = 0;
    // char * ping_command = "";
    int app_disabled = 0;
    // strcat(ping_command, target);
    FILE * fp_private;
    FILE * fp_public;
    //void *(*__start_routine)(void *) = (requester == 0) ? (void *)go_server : (void * )go_client;
    //printf("FUCK\n");
    int pid = 0;
    if (requester == SERVER) {
        //pthread_create(&app_thread, NULL, __start_routine, NULL);
        run_app();
        printf("Application started\n");
        pid = getPID();
    } else {
        pid = 65536;
        app_disabled = 1;
    }
    while (1) {
        char private_conn[128] = {0};
        char public_conn[128] = {0};
        fp_private = popen(ping_private, "r");
        fp_public = popen(ping_public, "r");
        fgets(private_conn, 1024, fp_private);
        fgets(public_conn, 1024, fp_public);
        // printf("%s", private_conn);
        // printf("public: %s\n", reachable(public_conn) ? "OK":"Failed");
        // printf("private: %s\n", reachable(private_conn) ? "OK":"Failed");
        if (requester == SERVER) {
            // printf("SERVER!\n"); 
            if (!reachable(private_conn) && !reachable(public_conn)) {
                if (!app_disabled) {
                    //pthread_cancel(app_thread);
                    if (!pid) {
                        continue;
                    }
                    terminate(pid);    
                    printf("Application terminated\n");
                    report(FAIL, requester);
                    app_disabled = 1;
                }
            } else {
                if (app_disabled) {
                    run_app();
                    report(SUCCESS, requester);
                    pid = getPID();
                    //pthread_create(&app_thread, NULL, __start_routine, NULL);
                    printf("Application started\n");
                    app_disabled = 0;
                }
            }
        } else {
            if (reachable(private_conn)) {
                //if (!app_disabled) {
                    //pthread_cancel(app_thread);
            if (!app_disabled) {
                terminate(pid);
                report(FAIL, requester);
                printf("Application terminated\n");
                app_disabled = 1;
            }
                //}
            } else if (reachable(public_conn) && !reachable(private_conn)) {
                if (app_disabled) {
                    //pthread_create(&app_thread, NULL, __start_routine, NULL);
                    run_app();
                    report(SUCCESS, requester);
                    pid = getPID();
                    printf("Application started\n");
                    app_disabled = 0;
                }
            } 
        }
        pclose(fp_private);
        pclose(fp_public);
        sleep(1);
    }
}


//void go_client() {
//    printf("Detected master offline\n");
//    while (1) {
//        printf("Going on client\n");
//        printf("I dont want to run");
//        sleep(1);
//    }
//}

//void go_server() {
//    printf("Master back online\n");
//    while (1) {
//        printf("Going on server\n");
//        printf("I am running");
//        sleep(1);
//    }
//}

