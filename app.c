#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define SERVER 0
#define CLIENT 1

// const char * server_ip = "10.2.25.125";
// const char * client_ip = "10.2.25.61";
const char * server_ip = "ping -c 1 -w 1 192.168.122.1 2> /dev/null | grep received";
const char * client_ip = "ping -c 1 -w 1 192.168.122.82 2> /dev/null | grep received";
const char * ping_public = "ping -c 1 -w 1 baidu.com 2> /dev/null | grep received";
void go_client();
void go_server();
void * app_control(const char * ping_command, 
                   pthread_t app_thread, 
                   int requester);



int main(int argc, char const *argv[]) {
    printf("App entered, %s\n", argv[1]);
    pthread_t thread_application;
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
    // printf("%d", requester);
    app_control(target, thread_application, requester);
    pthread_join(thread_application, NULL);
    return 0;
}



int reachable(char buffer[]) {
    for (int i = 0; i < 1024; i++) {
        if (buffer[i] == '\%') {
            return buffer[i - 2] == ' ';
            break;
        }
    }
}


void * app_control(const char * ping_private, 
    pthread_t app_thread, int requester) {
    
    int if_connected = 0;
    // char * ping_command = "";
    int app_disabled = 0;
    // strcat(ping_command, target);
    FILE * fp_private;
    FILE * fp_public;
    void *(*__start_routine)(void *) = (requester == 0) ? (void *)go_server : (void * )go_client;
    printf("FUCK\n");
    if (requester == SERVER) {
        pthread_create(&app_thread, NULL, __start_routine, NULL);

    }
    while (1) {
        char private_conn[128] = {0};
        char public_conn[128] = {0};
        fp_private = popen(ping_private, "r");
        fp_public = popen(ping_public, "r");
        fgets(private_conn, 1024, fp_private);
        fgets(public_conn, 1024, fp_public);
        // printf("%s", );
        printf("public: %s\n", reachable(public_conn) ? "OK":"Failed");
        printf("private: %s\n", reachable(private_conn) ? "OK":"Failed");
        
        if (requester == SERVER) {
            printf("SERVER!\n"); 
            
            if (!reachable(private_conn) && !reachable(public_conn)) {
                if (!app_disabled) {
                    pthread_cancel(app_thread);
                    printf("Application terminated");
                    app_disabled = 1;
                }
            } else {
                if (app_disabled) {
                    pthread_create(&app_thread, NULL, __start_routine, NULL);
                    printf("Application started");
                    app_disabled = 0;
                }
            }
        } else {
            if (reachable(private_conn)) {
                if (!app_disabled) {
                    pthread_cancel(app_thread);
                    printf("Application terminated");
                    app_disabled = 1;
                }
            } else if (reachable(public_conn) && !reachable(private_conn)) {
                if (app_disabled) {
                    pthread_create(&app_thread, NULL, __start_routine, NULL);
                    printf("Application started");
                    app_disabled = 0;
                }
            } 
        }
        close(fp_private);
        close(fp_public);
        sleep(1);
    }
}

void go_client() {
    printf("Detected master offline\n");
    while (1) {
        printf("Going on client\n");
        printf("I dont want to run");
        sleep(1);
    }
}

void go_server() {
    printf("Master back online\n");
    while (1) {
        printf("Going on server\n");
        printf("I am running");
        sleep(1);
    }
}

