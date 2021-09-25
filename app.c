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
#define SERVER 0
#define CLIENT 1

// const char * server_ip = "10.2.25.125";
// const char * client_ip = "10.2.25.61";
const char * server_ip = "ping -c 1 -w 1 10.2.25.5 2> /dev/null | grep received";
const char * client_ip = "ping -c 1 -w 1 10.2.25.125 2> /dev/null | grep received";
const char * ping_public = "ping -c 1 -w 1 baidu.com 2> /dev/null | grep received";

char * demo_command = "/home/dji/Onboard-SDK-master/build/bin/camera-stream-target-tracking-sample";



void go_client();
void go_server();
void app_control(const char * ping_command, 
                   int requester);

//void getPID();



int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <role>\n", argv[0]);
        return 1;
    }
    printf("App entered, %s\n", argv[1]);
    const char * target;
    
    int requester;


    switch (argv[1][0]) {
    case 's':
        printf("Entering Server Mode\n");
        target = client_ip;
        requester = SERVER;
        break;
    case 'c':
        target = server_ip;
        requester = CLIENT;
        break;
    default:
        fprintf(stderr, "option error, exiting");
        exit(1);
    }
    
    printf("FUCK\n");
    // printf("%d", requester);
    app_control(target, requester);
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
    system("/home/dji/Onboard-SDK-master/build/bin/camera-stream-target-tracking-sample /home/dji/Onboard-SDK-master/build/bin/UserConfig.txt &");
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
        printf("Application started");
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
                    printf("Application terminated");
                    app_disabled = 1;
                }
            } else {
                if (app_disabled) {
                    run_app();
                    pid = getPID();
                    //pthread_create(&app_thread, NULL, __start_routine, NULL);
                    printf("Application started");
                    app_disabled = 0;
                }
            }
        } else {
            if (reachable(private_conn)) {
                //if (!app_disabled) {
                    //pthread_cancel(app_thread);
            terminate(pid);
                    printf("Application terminated");
                    app_disabled = 1;
                //}
            } else if (reachable(public_conn) && !reachable(private_conn)) {
                if (app_disabled) {
                    //pthread_create(&app_thread, NULL, __start_routine, NULL);
                    run_app();
                    pid = getPID();
                    printf("Application started");
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

