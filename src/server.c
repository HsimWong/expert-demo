#include "server.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#define MAX_NODES 64
#define MAX_JOBS 256


/* Code lines below pasted here from linux kernal
 * which gets struct object address from its member variables
 * hoping to drop `Server *self` in parameters in the future
 */
// #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  
// #define container_of(ptr, type, member) ({          \  
//         const typeof( ((type *)0)->member ) *__mptr = (ptr); \  
//         (type *)( (char *)__mptr - offsetof(type,member) );})  




typedef struct json_object Json;

// void *get_self_pointer(void *v_pointer, )

void add_node(Server *self, Node *node) {
    self->nodes[self->nodes_num] = node;
    self->nodes_num++;
}
// schedule_jobs(self);


void add_job(Server *self, Job *job) {
    self->new_jobs[self->new_jobs_num] = job;
    self->new_jobs_num++;
}

void schedule_jobs(Server *self) {
    // if (self->new_jobs_num <= 0) {
    //     return;
    // }
}

void migrate(Server *self, Job *job) {

}

int get_failed_jobs(Server * self, int * indeces) {
    int failed_job_num = 0;
    for (int i = 0; i < self->scheduled_jobs_num; i++) {
        self->scheduled_jobs[i];
    }
}



void run(Server *self) {
    //shcedule new jobs
    //iterate all nodes and list ones that fail
    //iterate all jobs and list jobs running on such nodes,
    //migrate jobs that fails to its backup node
    //clear registration info of the job on the failed node
    //re-register job info to the back-up node 
    while (1) {
        if (self->new_jobs_num >= 0) {
            schedule_jobs(self);
        }
        
        int failed_jobs_indeces[MAX_JOBS];
        int failed_nodes_indeces[MAX_NODES];
        int failed_jobs_num = 0;
        int failed_nodes_num = 0;





        

    }

}
Server * InitServer(char *server_ip) {
    Server * self = (Server *)malloc(sizeof(Server));
    self->server_ip = server_ip,
    self->nodes = (Node **)malloc(MAX_NODES * sizeof(Node *));
    self->new_jobs = (Job **)malloc(MAX_JOBS * sizeof(Job *));
    self->scheduled_jobs = (Job **)malloc(MAX_JOBS * sizeof(Job *));
    self->nodes_num = 0;
    self->new_jobs_num = 0;
    self->scheduled_jobs_num = 0;
    self->add_node = add_node;
    self->run = run;
    return self;
}


// char *get_field_value_string(Json *json, char *field) {
//     char *ret;
//     Json *field_object;
//     json_object_object_get_ex(json, field, &field_object);
//     return (char *)json_object_get_string(field_object);
// }

// Json *get_field_object(Json *json, char *field) {
//     Json *field_object;
//     json_object_object_get_ex(json, field, &field_object);
//     return field_object;
// }



int main(int argc, char const *argv[])
{
    // enum json_type type;
    Json *config = json_object_from_file("./conf.json");
    Json *server_config = get_field_object(config, "server");
    Json *nodes_config = get_field_object(config, "nodes");
    Json *jobs_config = get_field_object(config, "jobs");
    Server* server = InitServer(get_field_value_string(server_config, "ip_addr"));
    printf("%s\n", server->server_ip);
    json_object_object_foreach(nodes_config, key, val) {
        Node * node = InitNode(key, get_field_value_string(val,"display"));
        server->add_node(server, node);
    }
    json_object_object_foreach(jobs_config, key, val) {
        Job * job = InitJob(
            get_field_value_string(jobs_config, "master_node"),
            get_field_value_string(jobs_config, "backup_node"),
            get_field_value_string(jobs_config, "entrypoint")
        );
        server->add_job(server, job);
    }

    server->run(server);
    
    return 0;
}
